/***********************************************************

	Starter code for Assignment 3

	Implementations of functions in raytracer.h,
	and the main function which specifies the scene to be rendered.

***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];
		if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
			ray.intersection.mat = node->mat;
		}
	}
}

void Raytracer::computeTransforms(Scene& scene) {
	// right now this method might seem redundant. But if you decide to implement
	// scene graph this is where you would propagate transformations to child nodes

	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];
		node->modelToWorld = node->trans;
		node->worldToModel = node->invtrans;
	}
}

void Raytracer::computeShading(Ray3D& ray, LightList& light_list) {
	for (size_t i = 0; i < light_list.size(); ++i) {
		LightSource* light = light_list[i];

		// Each lightSource provides its own shading function.
		// Implement shadows here if needed.
		light->shade(ray);
	}
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int depth) {
	Color col(0.0, 0.0, 0.0);
	traverseScene(scene, ray);
	// You'll want to call shadeRay recursively (with a different ray,
	// of course) here to implement reflection/refraction effects.
	if (!ray.intersection.none) {
		computeShading(ray, light_list);
		int numShadowRay = 10;
		int numReflectRay = 1;

		#pragma omp parallel for
		for (size_t i = 0; i < light_list.size(); ++i) {
			LightSource* light = light_list[i];
			#pragma omp parallel for
			for (size_t j = 0; j < numShadowRay; j++) {
				//Code for soft shadow for AreaLight
				Ray3D shadowRay = ConstructShadowRay(light, ray);
				traverseScene(scene, shadowRay);
				if (shadowRay.intersection.none || shadowRay.intersection.t_value > 1) {
					col = col + double(1)/numShadowRay * ray.col;
				}
			}
		}

		if (depth > 0) {
			//Code for glossy reflection
			#pragma omp parallel for
			for (size_t i = 0; i < numReflectRay; i++) {
				Ray3D reflectRay = ConstructGlossyReflectRay(ray);
				col = col + 0.5 * double(1)/numReflectRay * shadeRay(reflectRay, scene, light_list, --depth);
			}
		}

		col.clamp();
	}

	return col;
}

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image, int degree) {
	computeTransforms(scene);

	Matrix4x4 viewToWorld;
	double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

	viewToWorld = camera.initInvViewMatrix();
	// Construct a ray for each pixel.
	PointList ray_origin_offsets = SubPixelPoints(degree);
	Point3D imagePlane;

	int timeTicks = 1;
	int F = 5;
	int dofSamples = 0;
	Vector3D moveDir = Vector3D(0, 0, 0.5);

	#pragma omp parallel for
	for (int i = 0; i < image.height; i++) {
		#pragma omp parallel for
		for (int j = 0; j < image.width; j++) {
			#pragma omp parallel for
			Point3D origin(0, 0, 0);
			Color col(0,0,0);
			//Code for motion blur of camera
			#pragma omp parallel for
			for (size_t t = 0; t < timeTicks; t++) {
				//Code for antialiasing
				#pragma omp parallel for
				for (size_t n = 0; n < ray_origin_offsets.size(); n++) {
					Point3D point = *(ray_origin_offsets[n]);
					imagePlane[0] = (-double(image.width)/2 + point[0] + j)/factor;
					imagePlane[1] = (-double(image.height)/2 + point[1] + i)/factor;
					imagePlane[2] = -1;

					//Code for dof
					#pragma omp parallel for
					if (dofSamples > 0) {
						Vector3D worldSpaceRayDir = imagePlane - origin;
						worldSpaceRayDir.normalize();
						Point3D C = origin + F * worldSpaceRayDir;
						for (int d = 0; d < dofSamples; d++) {
							double r = ((double) rand() / (RAND_MAX));
							Point3D randomOrigin = Point3D(origin[0], origin[1] + r, origin[2]) + double(t)/timeTicks * moveDir;;
							Vector3D secondDir = C - randomOrigin;
							Ray3D secondRay;
							secondRay.origin = viewToWorld * randomOrigin;
							secondRay.dir = viewToWorld * C - viewToWorld * randomOrigin;
							secondRay.dir.normalize();
							col = col + (double(1)/dofSamples) * (double(1)/timeTicks) * (double(1)/ray_origin_offsets.size()) * shadeRay(secondRay, scene, light_list, 0);
						}
					} else {
						Ray3D ray;
						Point3D e = viewToWorld * origin;
						Vector3D d = viewToWorld * imagePlane - viewToWorld * origin;
						d.normalize();
						ray.origin = e + double(t)/timeTicks * moveDir;
						ray.dir = d;
						col = col + (double(1)/timeTicks) * (double(1)/ray_origin_offsets.size()) * shadeRay(ray, scene, light_list, 3);
					}
				}
			}
			image.setColorAtPixel(i, j, col);
		}
	}
}

PointList Raytracer::SubPixelPoints(int degree) {
	PointList pts;
	if (degree <= 1) {
		pts.push_back(new Point3D(0.5, 0.5, 0));
	} else {
		double d = (double)1 / degree;
		for (int i = 0; i < degree; i++) {
			for (int j = 0; j < degree; j++) {
				double randX = ((double) rand() / (RAND_MAX));
				double randY = ((double) rand() / (RAND_MAX));
				pts.push_back(new Point3D((i + randX) * d, (j + randY) * d, 0));
			}
		}
	}

	return pts;
}

Ray3D Raytracer::ConstructShadowRay(LightSource* light, Ray3D ray) {
	Ray3D shadowRay;
	shadowRay.dir = light->get_position() - ray.intersection.point;
	shadowRay.origin = ray.intersection.point + 0.001 * shadowRay.dir;
	return shadowRay;
}

Ray3D Raytracer::ConstructGlossyReflectRay(Ray3D ray) {
	Vector3D raydir = Vector3D(ray.dir);
	raydir.normalize();
	Vector3D R = raydir + 2 * ray.intersection.normal.dot(-1 * raydir) * ray.intersection.normal;
	R.normalize();
	Vector3D u = R.cross(ray.intersection.normal);
	u.normalize();
	Vector3D v = R.cross(u);
	v.normalize();
	double x = -(ray.intersection.mat) -> roughness/2 + ((double) rand() / (RAND_MAX)) * (ray.intersection.mat) -> roughness;
	double y = -(ray.intersection.mat) -> roughness/2 + ((double) rand() / (RAND_MAX)) * (ray.intersection.mat) -> roughness;
	double z = 1;
	Vector3D sampledDir = x * u + y * v + z * R;
	sampledDir.normalize();
	if (sampledDir.dot(R) < 0) {
		sampledDir = x * u + y * v - z * R;
	}
	Ray3D reflectRay;
	reflectRay.dir = Vector3D(sampledDir);
	reflectRay.origin = Point3D(ray.intersection.point) + 0.001 * reflectRay.dir;
	return reflectRay;
}
