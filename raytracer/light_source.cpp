/***********************************************************

	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray
	// is available.  So be sure that traverseScene() is called on the ray
	// before this function.

	//Diffuse
	Vector3D N = Vector3D(ray.intersection.normal);
	Vector3D L = Vector3D(this->get_position() - ray.intersection.point);
	N.normalize();
	L.normalize();
	Color diffuse = 1 * fmax(0, N.dot(L)) * this->col_diffuse;

	Vector3D R = Vector3D(-1 * L + 2 * N.dot(L) * N);
	Vector3D B = Vector3D(ray.origin - ray.intersection.point);
	R.normalize();
	B.normalize();
	Color specular = 1 * pow(fmax(0, R.dot(B)), (ray.intersection.mat)->specular_exp) * this->col_specular;

	Color ambient = 1 * this->col_ambient;

	Color c = diffuse * (ray.intersection.mat)->diffuse + specular * (ray.intersection.mat)->specular + ambient * (ray.intersection.mat)->ambient;

	ray.col = ray.col + c;
	ray.col.clamp();
}

void AreaLight::shade(Ray3D& ray) {
	//Diffuse
	Vector3D N = Vector3D(ray.intersection.normal);
	Vector3D L = Vector3D(this->pos - ray.intersection.point);
	N.normalize();
	L.normalize();
	Color diffuse = 1 * fmax(0, N.dot(L)) * this->col_diffuse;

	Vector3D R = Vector3D(-1 * L + 2 * N.dot(L) * N);
	Vector3D B = Vector3D(ray.origin - ray.intersection.point);
	R.normalize();
	B.normalize();
	Color specular = 0 * pow(fmax(0, R.dot(B)), (ray.intersection.mat)->specular_exp) * this->col_specular;

	Color ambient = 1 * this->col_ambient;

	Color c = diffuse * (ray.intersection.mat)->diffuse + specular * (ray.intersection.mat)->specular + ambient * (ray.intersection.mat)->ambient;

	ray.col = ray.col + c;
	ray.col.clamp();
}
