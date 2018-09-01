#include <stdio.h>
#include <string.h>
#include <time.h>
//#include <conio.h>
//#include <stlib.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "obj.h"
bool game=true;
#include "leapmotion.h"

double rotate_y=0; 
double rotate_x=0;

SampleListener listener;
Controller controller;


void myInit() {
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void specialKeys( int key, int x, int y ) {
	/*if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);*/
	
	//  Flecha derecha: aumentar rotación 5 grados
	if (key == GLUT_KEY_RIGHT)
		rotate_y += 5;
	//  Flecha izquierda: disminuir rotación 5 grados
	else if (key == GLUT_KEY_LEFT)
		rotate_y -= 5;
 
	else if (key == GLUT_KEY_UP)
		rotate_x += 5;
 
	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 5;
	//  Solicitar actualización de visualización
	else if (key == GLUT_KEY_F1)
		game=!game;
	glutPostRedisplay();
 
}

void draw_pixel(int x, int y) {//Corregir
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void draw_line(objl::Vertex v1, objl::Vertex v2, double width=2.5) {//Corregir
	glLineWidth(width); 
	glBegin(GL_LINES);
		glVertex3f(v1.Position.X, v1.Position.Y, v1.Position.Z);
		glVertex3f(v2.Position.X, v2.Position.Y, v2.Position.Z);
	glEnd();
}

void draw_hands(){
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		const Hand hand = *hl;
		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			for (int b = 0; b < 4; ++b) { 
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				Vector boneStart = bone.prevJoint();
				Vector boneEnd = bone.nextJoint();
				std::cout << boneStart.x << " " << boneStart.y << " " << boneStart.z << std::endl; 
				glBegin(GL_LINES);
					glVertex3f(boneStart.x, boneStart.y, boneStart.z);
					glVertex3f(boneEnd.x, boneEnd.y,boneEnd.z);
				glEnd();
			}
		}
	}
	std::cout << std::endl;
}

void draw_triangle(objl::Vertex v1, objl::Vertex v2, objl::Vertex v3){
	/*printf("(%.3f, %.3f, %.3f); ",v1.Position.X, v1.Position.Y, v1.Position.Z);
	printf("(%.3f, %.3f, %.3f); ",v2.Position.X, v2.Position.Y, v2.Position.Z);
	printf("(%.3f, %.3f, %.3f)\n",v3.Position.X, v3.Position.Y, v3.Position.Z);*/
	glBegin(GL_TRIANGLES);
		glVertex3f(v1.Position.X, v1.Position.Y, v1.Position.Z);
		glVertex3f(v2.Position.X, v2.Position.Y, v2.Position.Z);
		glVertex3f(v3.Position.X, v3.Position.Y, v3.Position.Z);
	glEnd();
}

void draw_loadedMesh(objl::Loader& Loader,int i){
	objl::Mesh curMesh = Loader.LoadedMeshes[i];
	for (int j=0;j<curMesh.Indices.size();j+=3){
		draw_triangle(curMesh.Vertices[curMesh.Indices[j]],
				curMesh.Vertices[curMesh.Indices[j+1]],
				curMesh.Vertices[curMesh.Indices[j+2]]);
	}
}

objl::Loader BottomLoader;
objl::Loader TopoLoader;
bool isBottomLoaded=false;
bool isTopoLoaded=false;

void draw_bottom(){
	if(!isBottomLoaded){
		bool loadout = isBottomLoaded = BottomLoader.LoadFile("model.obj");
		if(!loadout){
			printf("Error\n");
			return;
		}
	}
	glColor3f(1.0, 0.0, 0.0);
	draw_loadedMesh(BottomLoader,2);
	glColor3f(0.0, 0.0, 1.0);
	draw_loadedMesh(BottomLoader,3);
}

void draw_topo(){
	if(!isTopoLoaded){
		bool loadout = isTopoLoaded = TopoLoader.LoadFile("Topoo.obj");
		if(!loadout){
			printf("Error\n");
			return;
		}
	}
	draw_loadedMesh(TopoLoader,0);
}

void draw_bottoms(){
	//glTranslatef(0,-200,-100);
	draw_bottom();
	glTranslatef(-150,0,0);
	draw_bottom();
	glTranslatef(-150,0,0);
	draw_bottom();
}

void draw_topos(){
	glColor3f(1.0,0.0,0.0);
	glTranslatef(2.9,0,0); draw_topo();
	glTranslatef(-2.0,0,-2); draw_topo();
	glTranslatef(-2.0,0,0); draw_topo();
	glTranslatef(-2.0,0,2); draw_topo();
}

/*void draw_hands(){
	(-17.9619, 54.2626, 62.3615);
	(14.6468, 76.7065, 13.6628);
}*/

void draw_sceneMenu(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(110,1.0,1,500);
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0,-200,-100);
	glRotatef(rotate_x,1.0,0.0,0.0);
	glRotatef(rotate_y,0.0,1.0,0.0);
	draw_bottoms();
}

void draw_sceneGame(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,1.0,0.1,20.0);
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0,-3.0,-8);
	glRotatef(rotate_x,1.0,0.0,0.0);
	glRotatef(rotate_y,0.0,1.0,0.0);
	draw_topos();
}

bool gameBackup;

void idle(void){
	if(gameBackup!=game){
		draw_hands();
		glutPostRedisplay();
	}
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//dibujar escenario Menu o Game
	if(game){
		draw_sceneGame();
	} else {
		draw_sceneMenu();
	}
	gameBackup=game;
	glFlush();
	glutSwapBuffers();
	//if(gameBackup!=game)
	//Delay(20);
	//sleep(20);
	//glutPostRedisplay();
}

int main(int argc, char **argv) {
	controller.addListener(listener);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Interacción Humano Computador");
	myInit();
	glutIdleFunc(idle);
	glutDisplayFunc(myDisplay);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
}
