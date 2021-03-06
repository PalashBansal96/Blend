//cylinder.cpp

#include "cylinder.h"
#include "lightsource.h"

void Cylinder::drawShape(Material* mat) {
	const int nVertices = 3*2*180, exnVertices = 1*180*2*3 + 3*178 + 3*178;
	GLfloat cyl_vertices[nVertices];
	GLfloat cyl_colors[nVertices];

	for (int u = 0, ctr = 0; u < 10; u+=9) {
		for (int v = 0; v < 360; v += 2) {
			float x = radius*cos(degreeToRadians(v)); float z = radius*sin(degreeToRadians(v)); float y = -5+u;
			cyl_vertices[ctr++] = position[0] + x;	
			cyl_vertices[ctr++] = position[1] + y;
			cyl_vertices[ctr++] = position[2] + z;
		}
	}
	for (int u = 0, ctr = 0; u < 2; u += 1) {
		for (int v = 0; v < 180; v += 1) { 
			cyl_colors[ctr++] = mat->color.r; cyl_colors[ctr++] = mat->color.g; cyl_colors[ctr++] = mat->color.b;
		}
	}

	GLushort cyl_indices[exnVertices];
	int idx = 0;
	for (int u = 0; u < 1; u += 1) {
		for (int v = 0; v < 180; v +=1) {
			cyl_indices[idx++] = v + u*180;	cyl_indices[idx++] = v + (u+1)*180;	cyl_indices[idx++] = v + (u+1)*180 + 1;
			if (v == 179)	cyl_indices[idx-1] = cyl_indices[idx-1] - v - 1;

			cyl_indices[idx++] = v + u*180;
			cyl_indices[idx++] = v + u*180 + 1;
			if (v == 179)	cyl_indices[idx-1] = cyl_indices[idx-1] - v;

			cyl_indices[idx++] = v + (u+1)*180 + 1;
			if (v == 179)	cyl_indices[idx-1] = cyl_indices[idx-1] - v - 1;
		}
	}

	cout << "Index : " << idx << endl;
	for (int v = 1; v < 179; v++) {
		cyl_indices[idx++] = 0;
		cyl_indices[idx++] = v;
		cyl_indices[idx++] = v+1;
	}
	cout << "Index : " << idx << endl;
	for (int v = 1; v < 179; v++) {
		cyl_indices[idx++] = 2*180-1;
		cyl_indices[idx++] = 2*180-1-v;
		cyl_indices[idx++] = 2*180-1-v-1;
	}

	expandedVertices.clear();
	expandedColors.clear();
	expandedNormals.clear();
	for (int i = 0; i < exnVertices - 3*178 - 3*178; i++) {
		expandedVertices.push_back(cyl_vertices[cyl_indices[i]*3]);	expandedVertices.push_back(cyl_vertices[cyl_indices[i]*3+1]);	expandedVertices.push_back(cyl_vertices[cyl_indices[i]*3+2]);
		expandedColors.push_back(cyl_colors[cyl_indices[i]*3]);	expandedColors.push_back(cyl_colors[cyl_indices[i]*3+1]);	expandedColors.push_back(cyl_colors[cyl_indices[i]*3+2]);
		expandedNormals.push_back(cyl_vertices[cyl_indices[i]*3] - position[0]);	expandedNormals.push_back(0);	expandedNormals.push_back(cyl_vertices[cyl_indices[i]*3+2] - position[2]);
	}
	for (int i = exnVertices - 3*178 - 3*178; i < exnVertices - 3*178; i++) {
		expandedVertices.push_back(cyl_vertices[cyl_indices[i]*3]);	expandedVertices.push_back(cyl_vertices[cyl_indices[i]*3+1]);	expandedVertices.push_back(cyl_vertices[cyl_indices[i]*3+2]);
		expandedColors.push_back(cyl_colors[cyl_indices[i]*3]);	expandedColors.push_back(cyl_colors[cyl_indices[i]*3+1]);	expandedColors.push_back(cyl_colors[cyl_indices[i]*3+2]);
		expandedNormals.push_back(0); expandedNormals.push_back(5); expandedNormals.push_back(0);
	}
	for (int i = exnVertices - 3*178; i < exnVertices; i++) {
		expandedVertices.push_back(cyl_vertices[cyl_indices[i]*3]);	expandedVertices.push_back(cyl_vertices[cyl_indices[i]*3+1]);	expandedVertices.push_back(cyl_vertices[cyl_indices[i]*3+2]);
		expandedColors.push_back(cyl_colors[cyl_indices[i]*3]);	expandedColors.push_back(cyl_colors[cyl_indices[i]*3+1]);	expandedColors.push_back(cyl_colors[cyl_indices[i]*3+2]);
		expandedNormals.push_back(0); expandedNormals.push_back(5); expandedNormals.push_back(0);
	}
}

bool Cylinder::intersect(Ray& r) const
{
	return false;
}


