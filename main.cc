#include <stdio.h>
#include <string.h>
#include <time.h>
#include <set>
#include <map>
//#include <conio.h>
//#include <stlib.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "obj.h"
#include "leapmotion.h"
#include "listcube.h"
#include "png_texture.h"
#include "primitivas.h"
#include "objetoModel.h"

objl::Vector3 getCircleCoordinate(objl::Vector3 Center, double r, double angle){
	Center.X+=sin((angle*pi)/180)*r;
	Center.Z-=cos((angle*pi)/180)*r;
	return Center;
}

objl::Vector3 getCircleCoordinate(objl::Vector3 Center, objl::Vector3 point, double r, double angle){
	objl::Vector3 dif=point-Center;
	angle+=(asin(dif.X/r)*180)/pi;
	Center.X+=sin((angle*pi)/180)*r;
	Center.Z-=cos((angle*pi)/180)*r;
	return Center;
}

double rotate_x=0;
double rotate_y=0; 
double rotate_z=0;
double rotate_c=0;
GLUquadric* qobj;

bool game=false;
bool gameBackup;
bool inmove=false;

SampleListener listener;
Controller controller;

Model* Bottom;
Model* Topo;

objl::Vector3 Camara, LookAt;
double ratio=250;

objl::Vector3 getPos(objl::Vector3 point){
	objl::Vector3 Center=Camara;
	Center.Y=point.Y;
	double r=(point-Center).norm();
	return getCircleCoordinate(Center, point,
					r,rotate_c);
}

objl::Vector3 getPos(double x, double y, double z){
	objl::Vector3 point(x,y,z);
	return getPos(point);
}

objl::Vector3 getLookAt(){
	objl::Vector3 toLook=Camara+LookAt;
	return getPos(toLook);
	//return getCircleCoordinate(objl::Vector3(Camara.X,Camara.Y/2,Camara.Z),
					//ratio,rotate_c);
}

#include "hand_primitivas.h"

void ZoomIn(){
	ratio*=(3.0/4);
	Camara.Y*=(3.0/4);
	Camara.Z=ratio;
}

void ZoomOut(){
	ratio*=(4.0/3);
	Camara.Y*=(4.0/3);
	Camara.Z=ratio;
}

void initQuadric(){
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
}

void initTopo(){
	Topo=new Model("Topo.obj",120);
	//Topo->setToCenter(0, -70, -15);
	Topo->addMesh(0);Topo->addColor(0,0.0,1.0,0.0);
	Topo->addMesh(1);Topo->addColor(1,1.0,1.0,0.0);
	Topo->moveToCenter();
	Topo->useColor=true;
	Topo->solid();
}

void initBottom(){
	Bottom=new Model("Bottom.obj",1.5);
	//Bottom->setToCenter(615-900, -70, 390-25);
	Bottom->addMesh(2);Bottom->addColor(2,1.0,0.0,0.0);
	Bottom->addMesh(3);Bottom->addColor(3,0.0,0.0,1.0);
	Bottom->moveToCenter();
	Bottom->useColor=true;
	Bottom->solid();
}

void initVariables(){
	initQuadric();
	initTopo();
	initBottom();
	gameBackup=true;
	Camara=objl::Vector3(0,300,ratio);
	LookAt=objl::Vector3(-Camara.X,-Camara.Y,-Camara.Z);//getLookAt();
}

void initGlVariables(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.3, 0.8, 0.8, 0.5);
}

void myInit() {
	/*if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);*/
	initGlVariables();
	initVariables();
}

void specialKeys(int key, int x, int y){
	if (key == GLUT_KEY_RIGHT){
		rotate_y -= 5;
	} else if (key == GLUT_KEY_LEFT){
		rotate_y += 5;
	} else if (key == GLUT_KEY_UP){
		rotate_x += 5;
	} else if (key == GLUT_KEY_DOWN){
		rotate_x -= 5;
	} else if (key == GLUT_KEY_F1){//  Solicitar actualización de visualización
		game=!game;
		return;
	} else if (key == GLUT_KEY_F2){
		rotate_z-=5;
	} else if (key == GLUT_KEY_F3){
		rotate_z+=5;
	} else if (key == GLUT_KEY_F4){
		rotate_c-=5;
	} else if (key == GLUT_KEY_F5){
		rotate_c+=5;
	} else if (key == GLUT_KEY_F6){
		ZoomIn();
	} else if (key == GLUT_KEY_F7){
		ZoomOut();
	}
	glutPostRedisplay();
}

std::vector<objl::Vector3> Articulation_Points;
std::vector<Objeto> Topos, Bottoms;

void createBottoms(){
	Bottoms.clear();
	objl::Vector3 Center=Camara, Position;
	Center.Y/=6;
	double angle=45;
	double beg=-45;
	for (int i=0;i<3;i++){
		Position=getCircleCoordinate(Center,350,beg+i*angle);
		Bottoms.push_back(Objeto(Bottom,Position));
	}
}

void createTopos(){
	int n_topos=4;
	int scens=6;
	double angle;
	if(n_topos>1){
		angle = 360.0/(scens*(n_topos-1));
	} else {
		angle = 0;
	}
	Topos.clear();
	objl::Vector3 Center=Camara, Position;
	Center.Y/=6;
	double amp=400;
	Center.Z+=amp;
	double begin;
	if(n_topos>1){
		begin=-(360.0/(scens*2));
	} else {
		begin=0;
	}
	for (int i=0;i<n_topos;i++){
		Position=getCircleCoordinate(Center,Center.Z,begin+i*angle);
		Topos.push_back(Objeto(Topo,Position));
	}
}

void draw_bottoms(){
	for (int i=0;i<Bottoms.size();i++){
		Bottoms[i].draw();
	}
	if(Bottoms[1].intersected && Bottoms[1].maxY<=0){
		game=!game;
	}
}

bool draw_topos(){
	bool existTopos=false;
	for (int i=0;i<Topos.size();i++){
		if(!Topos[i].invisible){
			existTopos=true;
			Topos[i].draw();
		}
	}
	return existTopos;
}

void draw_sceneMenu(){
	getArticulationPoints(Articulation_Points);
	for (int i=0;i<Bottoms.size();i++){
		Bottoms[i].intersecta(Articulation_Points);
	}
	draw_bottoms();
}

void draw_sceneGame(){
	getArticulationPoints(Articulation_Points);
	for (int i=0;i<Topos.size();i++){
		Topos[i].intersecta(Articulation_Points);
		if(Topos[i].intersected){
			if (Topos[i].maxY<=0){ 	
				Topos[i].invisible=true;
			}
		}
	}
	if(!draw_topos()){
		game=false;
	}
}

void idle(void){
	if(gameBackup!=game || NEWHAND || inmove){
		NEWHAND=false;
		glutPostRedisplay();
	}
}

void loadCamera(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(130,	//<-- fovy
			1.0,	//<-- aspect
			1,	//<-- zNear
			2000);	//<-- zFar
	objl::Vector3 lookAt=getLookAt();
	gluLookAt(Camara.X, Camara.Y, Camara.Z,		//<-- Camara coordinate
			lookAt.X, lookAt.Y, lookAt.Z,	//<-- Look at coordinate
			0, 1, 0);			//<-- Up vector direction
	glMatrixMode(GL_MODELVIEW);
}

void draw_scene(){
	Cube baseC(Camara.X-400,Camara.Y/6,Camara.Z+400,800);
	Cube leftC(Camara.X-700,Camara.Y/2,0,400);
	Cube rightC(Camara.X+300,Camara.Y/2,0,400);
	glColor4f(0,0.2,0,0.3);
	baseC.draw2();
	std::vector<objl::Vector3> HandPoints;
	getLeapArticulationPoints(HandPoints);
	if(leftC.check_intersection(HandPoints)){
		inmove=true;
		rotate_c-=5;
		glColor4f(1.0,0.1,0.1,0.3);
	} else {
		glColor4f(0.2,0.0,0.0,0.3);
	}
	leftC.draw(3);
	if(rightC.check_intersection(HandPoints)){
		inmove=true;
		rotate_c+=5;
		glColor4f(1.0,0.1,0.1,0.3);
	} else {
		glColor4f(0.2,0.0,0.0,0.3);
	}
	rightC.draw(2);
}

void myDisplay(){
	inmove=false;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//| GL_BLENT_BUFFER_BIT);
	glLoadIdentity();
	//dibujar escenario Menu o Game
	loadCamera();
	draw_hands();
	//glEnable(GL_BLEND);
	if(game){
		if(gameBackup!=game){
			createTopos();
		}
		gameBackup=game;
		draw_sceneGame();
	} else {
		if(gameBackup!=game){
			createBottoms();
		}
		gameBackup=game;
		draw_sceneMenu();
	}
	//Escenario
	//glDisable(GL_DEPTH_TEST);
	draw_scene();
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char **argv){
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
