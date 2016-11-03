/*References
  Trackball: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glui.h>
#include <GL/gl.h>
#endif

#include <IL/il.h> //TODO
#include <IL/ilu.h>

#include "shader_utils.h"
#include "gl_utils.h"
#include "camera.h"
#include "renderengine.h"
#include "world.h"
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "lightsource.h"
#include "pointlightsource.h"
#include "triangle.h"
#include "quadric.h"

//Globals
GLuint program;
GLint attribute_coord2d;
int screen_width = 800, screen_height = 600; //Both even numbers
float quadVertices[] = {-1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1}; //2D screen space coordinates
GLuint texImage;
GLint uniform_texImage;

Camera *camera;
RenderEngine *engine;

int init_resources(void)
{
	//Create program
	program = createProgram("vshader.vs", "fshader.fs");
	attribute_coord2d = glGetAttribLocation(program, "coord2d");
	if(attribute_coord2d == -1)
	{
		fprintf(stderr, "Could not bind location: coord2d\n");
		return 0;
	}

	//Initialize raytracer objects
	Vector3D camera_position(0, 0, 10);
	Vector3D camera_target(0, 0, 0); //Looking down -Z axis
	Vector3D camera_up(0, 1, 0);
	float camera_fovy =  45;
	camera = new Camera(camera_position, camera_target, camera_up, camera_fovy, screen_width, screen_height);
	//Create a world
	World *world = new World;
	world->setAmbient(Color(1));
	world->setBackground(Color(0.5, 0.8, 1));

	Material *m = new Material(world);
	m->color = Color(1, 1, 1); m->ka = 0.; m->kd = 0.1; m->ks = 0.8; m->katt = 0.005; m->kr = 0.1; m->n = 128; m->eta = 1.4; m->kt = 1;
	Material *m2 = new Material(world);
	m2->color = Color(1, 1, 1); m2->ka = 0.1; m2->kd = 1; m2->ks = 0.; m2->katt = 0.005; m2->kr = 0.8; m2->n = 256; m2->kt = 0.; m2->eta = 1.75;
	Material *m3 = new Material(world);
	m3->color = Color(0.7, 1, 0.2); m3->ka = 0.2; m3->kd = 1; m3->ks = 0.1; m3->katt = 0.005; m3->kr = 0.2; m3->n = 128; m3->kt = 0;

	Material *mPlane = new Material(world);
	mPlane->color = Color(1, 1, 1); mPlane->kd = 0.9; mPlane->kr = 0.6; mPlane->katt = 0.003; mPlane->eta = 2; mPlane->n = 32;
	Material *mPlane2 = new Material(world);
	mPlane2->color = Color(1, .1, .1); mPlane2->kd = 0.9; mPlane2->kr = 0.7; mPlane2->katt = 0.003; mPlane2->eta = 2; mPlane2->n = 64;

	Object *sphere = new Sphere(Vector3D(4, 0, -10), 3, m3);
	world->addObject(sphere);

	Object *sphere2 = new Sphere(Vector3D(-4, 0, -10), 3, m2);
	world->addObject(sphere2);

	Object *sphere3 = new Sphere(Vector3D(1, 1, -5), 2.5, m);
	world->addObject(sphere3);

//	Object *sphere2 = new Sphere(Vector3D(2, 1.5, -5), 1.4, m2);
//	world->addObject(sphere2);

//	Object *quadric = new Quadric(1, 0, 1, 0, 0, 0, 0, 0, 0, -1, m2);
//	world->addObject(quadric);

//	Object *triangle = new Triangle(Vector3D(0, 10, -15), Vector3D(-8, 0, -18), Vector3D(8, 0, -18), m3);
//	world->addObject(triangle);

	Object *planeTriangle1 = new Triangle(Vector3D(-50, -5, 25), Vector3D(50, -5, 25), Vector3D(50, -5, -50), mPlane);
	world->addObject(planeTriangle1);
	Object *planeTriangle2 = new Triangle(Vector3D(-50, -5, 25), Vector3D(50, -5, -50), Vector3D(-50, -5, -50), mPlane);
	world->addObject(planeTriangle2);

	Object *planeTriangle3 = new Triangle(Vector3D(50, 25, -20), Vector3D(-50, 25, -20), Vector3D(50, -6, -20), mPlane2);
	world->addObject(planeTriangle3);
	Object *planeTriangle4 = new Triangle(Vector3D(50, -6, -20), Vector3D(-50, 25, -20), Vector3D(-50, -6, -20), mPlane2);
	world->addObject(planeTriangle4);

//	Object *planeTriangle3 = new Triangle(Vector3D(10, -10, 0), Vector3D(10, 10, 0), Vector3D(10, 10, -15), mPlane);
//	world->addObject(planeTriangle3);
//	Object *planeTriangle4 = new Triangle(Vector3D(10, -10, -15), Vector3D(10, -10, 0), Vector3D(10, 10, -15), mPlane);
//	world->addObject(planeTriangle4);

	LightSource *light = new PointLightSource(world, Vector3D(0, 10, 0), Color(0, 0.4, 0.8));
	world->addLight(light);
	LightSource *light2 = new PointLightSource(world, Vector3D(10, 10, -5), Color(1, 1, 1));
	world->addLight(light2);
//	LightSource *light3 = new PointLightSource(world, Vector3D(-8, 0, 0), Color(0.2, 0, 0));
//	world->addLight(light3);
	for (int i=0;i<0;i++) {
		LightSource *light4 = new PointLightSource(world, Vector3D(0, 7, i-10), Color(.01, .1, .01));
		world->addLight(light4);
	}
	engine = new RenderEngine(world, camera);

	//Initialise texture
	glGenTextures(1, &texImage);
	glBindTexture(GL_TEXTURE_2D, texImage);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //Show pixels when zoomed
	uniform_texImage = glGetUniformLocation(program, "texImage");
	if(uniform_texImage == -1)
	{
		fprintf(stderr, "Could not bind uniform: texImage\n");
		return 0;
	}
	return 1;
}

void onDisplay()
{
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//printOpenGLError();
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_coord2d);
	glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, quadVertices);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texImage);
	glUniform1i(uniform_texImage, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(attribute_coord2d);

	/* Display the result */
	glutSwapBuffers();
}

void free_resources()
{
	glDeleteProgram(program);
	glDeleteTextures(1, &texImage);
}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
}

void SaveImage()  //TODO:
{
	ILuint imageID = ilGenImage();
	ilBindImage(imageID);
	ilTexImage(camera->getWidth(), camera->getHeight(), 1, 3, IL_RGB, IL_UNSIGNED_BYTE, camera->getBitmap());
	//ilEnable(IL_FILE_OVERWRITE);
	time_t rawtime;
	time (&rawtime);
	char time_str[26];
	ctime_r(&rawtime, time_str);
	time_str[strlen(time_str) - 1] =0;//Remove trailing return character.
	char imageName[256];
	sprintf(imageName, "Lumina %s.png", time_str);
	ilSave(IL_PNG, imageName);
	fprintf(stderr, "Image saved as: %s\n", imageName);
}

void onKey(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27: exit(0);
			break;
		case 's': //Save to image
		case 'S': //Save to image
			SaveImage();
			break;

	}
}

void onIdle(void)
{
	static bool done = false;
	//Generate a pretty picture
	if(!done)
	{
		for(int i=0; i < screen_width/50; i++)
			if(engine->renderLoop())
			{
				done = true;
				fprintf(stderr, "Rendering complete.\n");
			}

		//Update texture on GPU
		glBindTexture(GL_TEXTURE_2D, texImage);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screen_width, screen_height, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());

		glutPostRedisplay();
	}
}

int main(int argc, char* argv[])
{
	srand((unsigned int) time(0));
	if(argc > 1)
	{
		screen_width = atoi(argv[1]);
		screen_height = atoi(argv[2]);
		screen_width -= (screen_width % 2); //Make it even
		screen_height -= (screen_height % 2); //Make it even
	}
	fprintf(stderr, "Welcome to Blend raytracer and editor.\nFull command: %s [width] [height]\nPress 's' to save framebufer to disk.\n", argv[0]);
	/* Glut-related initialising functions */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("Assignment 4: Raytracing");
#ifndef __APPLE__
	GLenum glew_status = glewInit();
	if(glew_status != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}
#endif

	ilInit(); //TODO

	/* When all init functions run without errors,
	   the program can initialise the resources */
	if (1 == init_resources())
	{
		/* We can display it if everything goes OK */
		glutReshapeFunc(onReshape);
		glutDisplayFunc(onDisplay);
		glutKeyboardFunc(onKey);
		glutIdleFunc(onIdle);
		glutMainLoop();
	}

	/* If the program exits in the usual way,
	   free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}
