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

objl::Vector3 Camara;
double ratio=250;

objl::Vector3 getLookAt(){
	return getCircleCoordinate(objl::Vector3(Camara.X,Camara.Y/2,Camara.Z),
					ratio,rotate_c);
}

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

#include "hand_primitivas.h"

void initQuadric(){
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
}

void initTopo(){
	Topo=new Model("Topoo.obj",120);
	//Topo->setToCenter(0, -70, -15);
	Topo->addMesh(0);Topo->addColor(0,0.0,1.0,0.0);
	Topo->addMesh(1);Topo->addColor(1,1.0,1.0,0.0);
	Topo->moveToCenter();
	Topo->useColor=true;
	Topo->solid();
}

void initBottom(){
	Bottom=new Model("model.obj",1.5);
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
	Camara=objl::Vector3(0,300,250);
}

void initGlVariables(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
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
	}
	glutPostRedisplay();
}

std::vector<objl::Vector3> Articulation_Points;
std::vector<Objeto> Topos, Bottoms;

void createBottoms(){
	Bottoms.clear();
	objl::Vector3 Center=Camara, Position;
	Center.Y=0;
	double angle=45;
	double beg=-45;
	for (int i=0;i<3;i++){
		Position=getCircleCoordinate(Center,350,beg+i*angle);
		Bottoms.push_back(Objeto(Bottom,Position));
	}
}

void createTopos(){
	int n_topos=4;
	double angle=360/(3*(n_topos-1));
	Topos.clear();
	objl::Vector3 Center=Camara, Position;
	Center.Y=0;
	double beg=-60;
	for (int i=0;i<4;i++){
		Position=getCircleCoordinate(Center,350,beg+i*angle);
		Topos.push_back(Objeto(Topo,Position));
	}
}

void draw_bottoms(){
	for (int i=0;i<Bottoms.size();i++){
		Bottoms[i].draw();
	}
	if(Bottoms[1].intersected && Bottoms[1].maxY<=50){
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
			if (Topos[i].maxY<=50){ 	
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
	gluPerspective(130,1.0,1,2000);
	objl::Vector3 lookAt=getLookAt();
	gluLookAt(Camara.X, Camara.Y, Camara.Z,  //<-- Camara coordinates
			lookAt.X, lookAt.Y, lookAt.Z, //<-- Look at coordinates
			0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
}

void myDisplay(){
	inmove=false;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//| GL_BLENT_BUFFER_BIT);
	glLoadIdentity();
	//dibujar escenario Menu o Game
	loadCamera();
	draw_hands();
	//Escenario
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
	//glDisable(GL_DEPTH_TEST);
	Cube baseC(Camara.X-400,0,Camara.Z+400,800);
	Cube leftC(Camara.X-600,0,0,400);
	Cube rightC(Camara.X+200,0,0,400);

	glColor4f(0,0,0,0.2);
	baseC.draw2();
	std::vector<objl::Vector3> HandPoints;
	getLeapArticulationPoints(HandPoints);
	if(leftC.check_intersection(HandPoints)){
		inmove=true;
		rotate_c-=5;
		glColor4f(1.0,0.1,0.1,0.6);
	} else {
		glColor4f(0.2,0.0,0.0,0.6);
	}
	leftC.draw(3);
	if(rightC.check_intersection(HandPoints)){
		inmove=true;
		rotate_c+=5;
		glColor4f(1.0,0.1,0.1,0.6);
	} else {
		glColor4f(0.2,0.0,0.0,0.6);
	}
	rightC.draw(2);
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
