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

	// vec3 N = normalize(normalInterp);
    // vec3 L = normalize(lightPos - vertPos);
    // float diffuse = Kd * max(0.0, dot(N, L));
	//
    // vec3 R = normalize(-L + 2.0 * dot(N, L) * N);
    // vec3 B = normalize(viewVec);
    // float specular = Ks * pow(max(0.0, dot(R, B)), shininessVal);
    // gl_FragColor = vec4(Ka * ambientColor + diffuseColor * diffuse + specular * specularColor, 1.0);

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
	c.clamp();
	ray.col = c;
}
