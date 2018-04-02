/***********************************************************

	Starter code for Assignment 3

	Implements scene_object.h

***********************************************************/

#include <cmath>
#include "scene_object.h"
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

bool UnitSquare::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0),
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point,
	// intersection.normal, intersection.none, intersection.t_value.
	//
	// HINT: Remember to first transform the ray into object space
	// to simplify the intersection test.
	Point3D e = worldToModel * ray.origin;
	Vector3D d = worldToModel * ray.dir;
	Vector3D ec = Vector3D(e[0], e[1], e[2]);
	Vector3D n = Vector3D(0.0, 0.0, 1.0); // already a unit normal
	double t = (-ec).dot(n) / d.dot(n);
	Point3D p = e + t * d;
	if (p[0] <= 0.5 && p[0] >= -0.5 && p[1] <= 0.5 && p[1] >= -0.5) {
		if ((ray.intersection.none || t < ray.intersection.t_value) && t > 0.001) {
			ray.intersection.none = false;
			ray.intersection.point = modelToWorld * p;
			ray.intersection.t_value = t;
			ray.intersection.normal = transNorm(worldToModel, n);
			return true;
		}
	}
	//ray.intersection.none = true;
	return false;
}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSphere, which is centred
	// on the origin.
	//
	// Your goal here is to fill ray.intersection with correct valuess
	// should an intersection occur.  This includes intersection.point,
	// intersection.normal, intersection.none, intersection.t_value.
	//
	// HINT: Remember to first transform the ray into object space
	// to simplify the intersection test.
	Point3D e = worldToModel * ray.origin;
	Vector3D d = worldToModel * ray.dir;
	Vector3D eminusc = Vector3D(e[0], e[1], e[2]);
	double A = d.dot(d);
	double B = 2 * d.dot(eminusc);
	double C = eminusc.dot(eminusc) - 1;
	double det = pow(B, 2) - (4 * A * C);
	if (det < 0) {
		//ray.intersection.none = true;
		return false;
	} else {
		double t = (-B - sqrt(det)) / (2 * A);
		if ((ray.intersection.none || (t < ray.intersection.t_value && t > 0)) && t > 0.001) {
			Point3D p = e + (t * d);
			Vector3D n = Vector3D(p[0], p[1], p[2]);
			ray.intersection.none = false;
			ray.intersection.point = modelToWorld * p;
			ray.intersection.t_value = t;
			ray.intersection.normal = transNorm(worldToModel, n);
			return true;
		}
	}
	//ray.intersection.none = true;
	return false;
}

void SceneNode::rotate(char axis, double angle) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;

	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
			this->trans = this->trans*rotation;
			angle = -angle;
		}
		else {
			this->invtrans = rotation*this->invtrans;
		}
	}
}

void SceneNode::translate(Vector3D trans) {
	Matrix4x4 translation;

	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	this->trans = this->trans*translation;
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	this->invtrans = translation*this->invtrans;
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
	Matrix4x4 scale;

	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	this->trans = this->trans*scale;
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	this->invtrans = scale*this->invtrans;
}
