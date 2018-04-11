/***********************************************************

	Starter code for Assignment 3

***********************************************************/

#include <cstdlib>
#include "raytracer.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	// Build your scene and setup your camera here, by calling
	// functions from Raytracer.  The code here sets up an example
	// scene and renders it from two different view points, DO NOT
	// change this if you're just implementing part one of the
	// assignment.
	Raytracer raytracer;
	LightList light_list;
	Scene scene;

	int width = 320;
	int height = 240;

	if (argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
	}

	// Define materials for shading.
	Material gold(Color(0.1, 0.1, 0.1), Color(0.75164,0.60648,0.22648),
		Color(0.628281, 0.555802, 0.366065),
		51.2);
	Material jade(Color(0, 0, 0), Color(0.54,0.89,0.63),
		Color(0.316228,0.316228,0.316228),
		12.8);
	Material steel(Color(0.5, 0.5, 0.5), Color(0.7, 0.7, 0.7),
		Color(0.85, 0.85, 0.85),
		25);
	Material ruby(Color(0, 0, 0), Color(0.9,0,0),
		Color(0.6, 0.6, 0.6),
		25);
	Material roughruby(Color(0, 0, 0), Color(0.9,0,0),
		Color(0.6, 0.6, 0.6),
		25);
	Material mirror(Color(0, 0, 0), Color(0.15,0.15,0.15),
		Color(1,1,1),
		999, 0.15);

	// Defines a point light source.
	PointLight* pLight = new PointLight(Point3D(1,1,1), Color(0.75,0.75,0.75));
	AreaLight* aLight = new AreaLight(Point3D(1,1,1), Color(0.75,0.75,0.75));
	light_list.push_back(pLight);
	// light_list.push_back(aLight);

	// Add a unit square into the scene with material mat.

	double factor1[3] = { 0.5, 0.5, 0.5 };
	SceneNode* s1 = new SceneNode(new UnitSphere(), &gold);
	scene.push_back(s1);
	s1->translate(Vector3D(0, -1, -3));
	s1->scale(Point3D(0, 0, 0), factor1);

	SceneNode* s2 = new SceneNode(new UnitSphere(), &ruby);
	scene.push_back(s2);
	s2->translate(Vector3D(-1, -2, 5));
	s2->scale(Point3D(0, 0, 0), factor1);

	SceneNode* roughrubysphere = new SceneNode(new UnitSphere(), &roughruby);
	scene.push_back(roughrubysphere);
	roughrubysphere->translate(Vector3D(1, -2, 5));
	roughrubysphere->scale(Point3D(0, 0, 0), factor1);

	SceneNode* s3 = new SceneNode(new UnitSphere(), &steel);
	scene.push_back(s3);
	s3->translate(Vector3D(6, 1, 0));
	s3->scale(Point3D(0, 0, 0), factor1);

	SceneNode* s4 = new SceneNode(new UnitSphere(), &jade);
	scene.push_back(s4);
	s4->translate(Vector3D(-2, 2, 4));
	s4->scale(Point3D(0, 0, 0), factor1);

	SceneNode* s5 = new SceneNode(new UnitSphere(), &gold);
	scene.push_back(s5);
	s5->translate(Vector3D(-7, 5, 5));
	s5->scale(Point3D(0, 0, 0), factor1);

	SceneNode* s6 = new SceneNode(new UnitSphere(), &mirror);
	scene.push_back(s6);
	s6->translate(Vector3D(4, 6, 6));
	s6->scale(Point3D(0, 0, 0), factor1);

	double factor2[3] = { 16, 16, 16 };

	SceneNode* p1 = new SceneNode(new UnitSquare(), &mirror);
	scene.push_back(p1);
	p1->translate(Vector3D(0, 0, -8.1));
	p1->scale(Point3D(0, 0, 0), factor2);

	SceneNode* p2 = new SceneNode(new UnitSquare(), &mirror);
	scene.push_back(p2);
	p2->translate(Vector3D(0, 0, 8.1));
	p2->scale(Point3D(0, 0, 0), factor2);

	SceneNode* p3 = new SceneNode(new UnitSquare(), &mirror);
	scene.push_back(p3);
	p3->translate(Vector3D(0, -8.1, 0));
	p3->scale(Point3D(0, 0, 0), factor2);
	p3->rotate('x', 90);

	SceneNode* p4 = new SceneNode(new UnitSquare(), &mirror);
	scene.push_back(p4);
	p4->translate(Vector3D(0, 8.1, 0));
	p4->scale(Point3D(0, 0, 0), factor2);
	p4->rotate('x', 90);

	SceneNode* p5 = new SceneNode(new UnitSquare(), &mirror);
	scene.push_back(p5);
	p5->translate(Vector3D(-8.1, 0, 0));
	p5->scale(Point3D(0, 0, 0), factor2);
	p5->rotate('y', 90);

	SceneNode* p6 = new SceneNode(new UnitSquare(), &mirror);
	scene.push_back(p6);
	p6->translate(Vector3D(8.1, 0, 0));
	p6->scale(Point3D(0, 0, 0), factor2);
	p6->rotate('y', 90);

	int deg = 0;
	// Render the scene, feel free to make the image smaller for
	// testing purposes.
	Camera camera1(Point3D(0, 0, 7.99), Vector3D(0, 0, -1), Vector3D(0, 1, 0), 90.0);
	Image image1(width, height);
	raytracer.render(camera1, scene, light_list, image1, deg); //render 3D scene to image
	image1.flushPixelBuffer("view1.bmp"); //save rendered image to file
	printf("%s\n", "image 1 done");
	// Render it from a different point of view.
	Camera camera2(Point3D(7.7, 7.7, 7.7), Vector3D(-0.5, -1, -1), Vector3D(0, 1, 0), 60.0);
	Image image2(width, height);
	raytracer.render(camera2, scene, light_list, image2, deg);
	image2.flushPixelBuffer("view2.bmp");
	printf("%s\n", "image 2 done");

	// Free memory
	for (size_t i = 0; i < scene.size(); ++i) {
		delete scene[i];
	}

	for (size_t i = 0; i < light_list.size(); ++i) {
		delete light_list[i];
	}

	return 0;
}
