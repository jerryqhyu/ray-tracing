/***********************************************************

	Starter code for Assignment 3

	This file contains the interface and datastructures of the raytracer.
	Simple traversal and addition code to the datastructures are given to you.

***********************************************************/
#pragma once

#include "util.h"
#include "scene_object.h"
#include "light_source.h"

typedef std::vector<Point3D*> PointList;

class Raytracer {
public:
	// Renders 3D scene to an image given camera and lights setup.
	void render(Camera& camera, Scene& scene, LightList& light_list, Image& image, int degree = 0);

private:

	// Return the color of the ray after intersection and shading, call
	// this function recursively for reflection and refraction.
	Color shadeRay(Ray3D& ray, Scene& scene, LightList& light_list, int depth);

	// Traversal code for the scene, the ray is transformed into
	// the object space of each node where intersection is performed.
	void traverseScene(Scene& scene, Ray3D& ray);

	// After intersection, calculate the color of the ray by shading it
	// with all light sources in the scene.
	void computeShading(Ray3D& ray, LightList& light_list);

	// Precompute the modelToWorld and worldToModel transformations for each
    // object in the scene.
	void computeTransforms(Scene& scene);

	// Get a list of 9 subpoint increments for antialiasing
	PointList SubPixelPoints(int degree);

	// Get a list of 9 subpoint increments for antialiasing
	Ray3D ConstructShadowRay(LightSource* light, Ray3D ray);

	// Get a list of 9 subpoint increments for antialiasing
	Ray3D ConstructGlossyReflectRay(Ray3D ray);
};
