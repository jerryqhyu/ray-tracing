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

/*	Color col = ray.col; 

	Color gl_FragColor = col;
	Vector3D directionOfLightSource = normalize(lightPos - vertPos);

	//Diffuse
	float diffuseIntensity = Kd  * max(0.0, dot(directionOfLightSource.xyz,  normalize(normalInterp).xyz));

	vec3 newDiffuseColor = diffuseColor;
	newDiffuseColor = diffuseColor * diffuseIntensity;

	gl_FragColor = gl_FragColor + vec4(newDiffuseColor, 0.0);


	//Specular
	vec3 newSpecular = specularColor;
	vec3 vectorB = normalize(vec3(0.0,0.0,0.0) - vertPos);

	vec3 vectorR = normalize(reflect(-directionOfLightSource, normalInterp));
	float specularIntensity = Ks  * pow(max(0.0, dot(vectorR.xyz, vectorB.xyz)), shininessVal);


	newSpecular = newSpecular * specularIntensity;

	gl_FragColor = gl_FragColor + vec4(newSpecular, 0.0);*/

/*
	Color newAmb = ((ray.intersection.mat)->ambient + this->col_ambient);
	Color newDif = ((ray.intersection.mat)->diffuse + this->col_diffuse);
	//Color newSpe = ((ray.intersection.mat)->specular + this->col_specular).clamp();
	newAmb.clamp();
	newDif.clamp();

	Color newCol =  newAmb + newDif ;
	newCol.clamp();*/

	//Diffuse
	Vector3D directionOfLightSource = Vector3D(this->pos - ray.intersection.point);
	directionOfLightSource.normalize();
	Vector3D norm = ray.intersection.normal;
	norm.normalize();
	float diffuseIntensity =  0.0;
	if (directionOfLightSource.dot(norm) > 0.0) {
		diffuseIntensity = directionOfLightSource.dot(norm);
	}
	Color newDif = diffuseIntensity * (ray.intersection.mat)->diffuse;	
	newDif.clamp();


	//Specular 
	Vector3D vectorB = Vector3D(0.0,0.0,0.0) - Vector3D(ray.intersection.point[0],ray.intersection.point[1],ray.intersection.point[2]);
	vectorB.normalize();
	Vector3D vectorR = -directionOfLightSource - 2.0 * norm.dot(-directionOfLightSource) * norm;
	vectorR.normalize();
	float specularIntensity = 0.0;
	if (vectorR.dot(vectorB) > 0.0) {
		specularIntensity = pow(vectorR.dot(vectorB), (ray.intersection.mat)->specular_exp);
	}
	Color newSpe = specularIntensity * (ray.intersection.mat)->specular;
	newSpe.clamp();


	Color total = newSpe + newDif + (ray.intersection.mat)->ambient;
	total.clamp();
	ray.col = total;

}

