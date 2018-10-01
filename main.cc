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
#include "leapmotion.h"
//#include "obj.h"
#include "vector2.h"
#include "vector3.h"
#include "vertex.h"
#include "material.h"
#include "mesh.h"
#include "png_texture.h"
#include "ParseObj/obj.h"
#include "primitivas.h"
#include "objetoModel.h"
#include "listcube.h"
#include "Vector_tools.h"
#include "light.h"
#include "utility.h"

static light **LOCAL_MyLights;
static int current_mode = 0;
static int current_light = -1;
static int spot_move=0;

#include "mouse.h"

double rotate_x=0;
double rotate_y=0; 
double rotate_z=0;
//double rotate_c=0;
GLUquadric* qobj;

bool game=false;
bool gameBackup;
bool topoo=true;
bool topooBackup;
bool inmove=false;

SampleListener listener;
Controller controller;

#include "camara.h"

static Camara *MiCamara;

objl::Vector3 getPos(objl::Vector3 point){
	return MiCamara->getPos(point);
}

objl::Vector3 getPos(double x, double y, double z){
	objl::Vector3 point(x,y,z);
	return getPos(point);
}

objl::Vector3 getLookAt(){
	return MiCamara->getLookAt();
}

#include "hand_primitivas.h"

void initQuadric(){
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
}

Model* Bottom;
Model* Topo;

void initDiglet(){
	Topo=new Model("Diglett.obj",50);
	Topo->addMesh(0);Topo->addColor(0,0.0,1.0,0.0);
	Topo->addMesh(1);Topo->addColor(1,1.0,1.0,0.0);
	Topo->moveToCenter();
	Topo->useColor=true;
	Topo->solid();
}

void initTopo(){
	if(topoo){
		Topo=new Model("Topo.obj",120);
		Topo->addMesh(0);Topo->addColor(0,0.0,1.0,0.0);
		Topo->addMesh(1);Topo->addColor(1,1.0,1.0,0.0);
		Topo->moveToCenter();
		Topo->useColor=true;
		Topo->solid();
	} else {
		initDiglet();
	}
}

void initBottom(){
	Bottom=new Model("Bottom.obj",1.5);
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
	MiCamara->SetCamera(0, 300, 250,
				0, -300, -250,
				0, 1, 0);
	MiCamara->camAperture=130 * DEGREE_TO_RAD;
	MiCamara->camNear = 1;
	MiCamara->camFar = 2000;
}

void initGlVariables(){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_COLOR_MATERIAL);
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
		MiCamara->rotate_c-=5;
	} else if (key == GLUT_KEY_F5){
		MiCamara->rotate_c+=5;
	} else if (key == GLUT_KEY_F6){
		MiCamara->ZoomIn();
	} else if (key == GLUT_KEY_F7){
		MiCamara->ZoomOut();
	} else if (key == GLUT_KEY_F8){
		if (current_mode != 0 && current_mode != 7){
		} else {
			current_mode = 7;
			if (current_light == -1) glutPassiveMotionFunc(Mouse_Luces);
			if (current_light != 2) current_light++;                    
			else current_light = 0;
			printf("Luz actual = %d\n",current_light);                  
		}
		//break;
	} else if(key==GLUT_KEY_F9){
		if (current_light != -1) {
			if ( LOCAL_MyLights[current_light]->switched ){
				SwitchLight( LOCAL_MyLights[current_light], FALSE);
			} else {
				SwitchLight( LOCAL_MyLights[current_light], TRUE);
			}
		}
		//break;
	} else if (key==GLUT_KEY_F10){
		if (current_light == 2){
			if ( spot_move == 0 ){
				glutPassiveMotionFunc(Mouse_Spot);
				spot_move = 1;
			}else{
				glutPassiveMotionFunc(Mouse_Luces);
				spot_move = 0;
			}
		}
		//break;
	}
	glutPostRedisplay();
}

std::vector<objl::Vector3> Articulation_Points;
std::vector<Objeto> Topos, Bottoms;

void createBottoms(){
	int scens=6;
	double angle=360.0/(scens*(3-1));
	Bottoms.clear();
	objl::Vector3 Center=MiCamara->camView, Position;
	Center.Y/=4;
	double beg=-(360.0/(scens*2));
	for (int i=0;i<3;i++){
		Position=getCircleCoordinate(Center,350,beg+i*angle);
		Bottoms.push_back(Objeto(Bottom,Position));
	}
	Bottoms[0].useColor=true;
	Bottoms[0].Colores[2]=objl::Vector3(1,1.0,0.0);
	Bottoms[0].Colores[3]=objl::Vector3(1,0.0,0.0);
	Bottoms[1].useColor=true;
	Bottoms[1].Colores[2]=objl::Vector3(0,1.0,0.0);
	Bottoms[1].Colores[3]=objl::Vector3(1,0.0,0.0);
	Bottoms[2].useColor=true;
	Bottoms[2].Colores[2]=objl::Vector3(0.0,0.0,1.0);
	Bottoms[2].Colores[3]=objl::Vector3(1,0.0,0.0);
}

void createTopos(int n_topos=4,double amp=400, double dist=0, double CY=MiCamara->camView.Y/4){
	int scens=6;
	double angle;
	if(n_topos>1){
		angle = 360.0/(scens*(n_topos-1));
	} else {
		angle = 0;
	}
	Topos.clear();
	objl::Vector3 Center=MiCamara->camView, Position;
	Center.Y=CY;
	Center.Z+=amp;
	double begin;
	if(n_topos>1){
		begin=-(360.0/(scens*2));
	} else {
		begin=0;
	}
	for (int i=0;i<n_topos;i++){
		Position=getCircleCoordinate(Center,Center.Z+dist,begin+i*angle);
		Topos.push_back(Objeto(Topo,Position));
	}
}

void draw_bottoms(){
	for (int i=0;i<Bottoms.size();i++){
		Bottoms[i].draw();
	}
	if(Bottoms[0].intersected && Bottoms[0].maxY<=0){
		topoo=!topoo;
	}
	if(Bottoms[1].intersected && Bottoms[1].maxY<=0){
		game=!game;
	}
	if(Bottoms[2].intersected && Bottoms[2].maxY<=0){
		delete MiCamara;
		exit(0);
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
	//std::cout << "***pase5" << std::endl;
	getArticulationPoints(Articulation_Points);
	//std::cout << "**/pase5" << std::endl;
	for (int i=0;i<Bottoms.size();i++){
		Bottoms[i].intersecta(Articulation_Points);
	}
	draw_bottoms();
	draw_topos();
	//std::cout << "**-pase5" << std::endl;
}

void draw_sceneGame(){
	if(hands.isEmpty()){
		getArticulationPoints(Articulation_Points);
		for (int i=0;i<Topos.size();i++){
			Topos[i].intersecta(Articulation_Points);
			if(Topos[i].intersected){
				if (Topos[i].maxY<=0){ 	
					Topos[i].invisible=true;
				}
			}
		}
	} else {
		for (int i=0;i<Topos.size();i++){
			for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
				Hand hand = *hl;
				if(isPuno(hand)){
					getLeapArticulationPoints(Articulation_Points,hand.isRight());
					Topos[i].intersecta(Articulation_Points);
					if(Topos[i].intersected){
						if (Topos[i].maxY<=0){ 	
							Topos[i].invisible=true;
						}
					}
				}
			}
				
		}
	}
	if(!draw_topos()){
		game=false;
	}
}

void idle(void){
	//std::cout << "repase1" << std::endl;
	if( topoo!=topooBackup || gameBackup!=game/*  || inmove*/){
		//std::cout << "repase2" << std::endl;
		glutPostRedisplay();
	} else if (NEWHAND) {
		glutPostRedisplay();
	}
}

void loadCamera(){
	if(gameBackup!=game){
		MiCamara->rotate_c=0;
	}
	MiCamara->SetGLCamera();
}

bool draw_scene(){
	Cube baseC(/*MiCamara->camView.X*/-400,/*MiCamara->camView.Y/5*/60,/*MiCamara->camView.Z*/+400,800);
	glColor4f(0,0.2,0,0.8);
	baseC.draw2();
	//
	Cube leftC(/*MiCamara->camView.X*/-400,230/*MiCamara->camView.Y/2*/,200,400);
	Cube rightC(/*MiCamara->camView.X+*/0,230/*MiCamara->camView.Y/2*/,200,400);
	std::vector<objl::Vector3> HandPoints;
	bool imove=false;
	if  (hands.isEmpty()){
		getLeapArticulationPoints(HandPoints,2);
		if(leftC.check_intersection(HandPoints)){
			imove=true;
			MiCamara->rotate_c-=0.3;
			glColor4f(1.0,0.1,0.1,0.3);
			leftC.draw(1);
		} else {
			glColor4f(0.2,0.0,0.0,0.3);
			leftC.draw(3);
		}
		if(rightC.check_intersection(HandPoints)){
			imove=true;
			MiCamara->rotate_c+=0.3;
			glColor4f(1.0,0.1,0.1,0.3);
			rightC.draw(1);
		} else {
			glColor4f(0.2,0.0,0.0,0.3);
			rightC.draw(2);
		}
	} else {
		for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
			Hand hand = *hl;
			if(hand.isLeft() && isPalm(hand)){
				getLeapArticulationPoints(HandPoints,0);
				if(leftC.check_intersection(HandPoints)){
					imove=true;
					MiCamara->rotate_c-=0.3;
					glColor4f(1.0,0.1,0.1,0.3);
					leftC.draw(1);
				} else {
					glColor4f(0.2,0.0,0.0,0.3);
					leftC.draw(3);
				}
			} else if(hand.isRight() && isPalm(hand)){
				getLeapArticulationPoints(HandPoints,1);
				if(rightC.check_intersection(HandPoints)){
					imove=true;
					MiCamara->rotate_c+=0.3;
					glColor4f(1.0,0.1,0.1,0.3);
					rightC.draw(1);
				} else {
					glColor4f(0.2,0.0,0.0,0.3);
					rightC.draw(2);
				}
			}
		}
	}
	return imove;
}

void display(void){
	float At[3];
	float Direction[3];
	switch (current_light){
		case 0:
			At[0] = LOCAL_MyLights[current_light]->position[0];
			At[1] = LOCAL_MyLights[current_light]->position[1];
			At[2] = LOCAL_MyLights[current_light]->position[2];         
			Direction[0] = - LOCAL_MyLights[current_light]->position[0];
			Direction[1] = - LOCAL_MyLights[current_light]->position[1];
			Direction[2] = - LOCAL_MyLights[current_light]->position[2];
			Draw_Parallel(At);
			Draw_Meridian(At);
			Draw_Vector(At, Direction);
			break;
		case 1:
                         At[0] = LOCAL_MyLights[current_light]->position[0];
                         At[1] = LOCAL_MyLights[current_light]->position[1];
                         At[2] = LOCAL_MyLights[current_light]->position[2];
                         Draw_Parallel(At);
                         Draw_Meridian(At);
                         glTranslatef(At[0],At[1],At[2]);
                         glColor3f(1.0,0.0,0.0);
                         glutSolidSphere(10.,28,28);
                         glTranslatef(-At[0],-At[1],-At[2]);
                         break;
		case 2:
                        At[0] = LOCAL_MyLights[current_light]->position[0];
                        At[1] = LOCAL_MyLights[current_light]->position[1];
                        At[2] = LOCAL_MyLights[current_light]->position[2];
                        Direction[0] = LOCAL_MyLights[current_light]->spotDirection[0];
                        Direction[1] = LOCAL_MyLights[current_light]->spotDirection[1];
                        Direction[2] = LOCAL_MyLights[current_light]->spotDirection[2];
                        Draw_Parallel(At);
                        Draw_Meridian(At);
                        glColor3f(1.0,0.0,0.0);
                        Draw_Vector(At, Direction);
                        Draw_Sphere_Spot(At, Direction);
                        glTranslatef(At[0],At[1],At[2]);
                        glutSolidSphere(10,28,28);                                   
                        glTranslatef(-At[0],-At[1],-At[2]);
                        break;
		default:
			break;
	}
}

void myDisplay(void){
	inmove=false;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//| GL_BLENT_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glLoadIdentity();
	//dibujar escenario Menu o Game
	//glLoadIdentity();
	/*SetLight( LOCAL_MyLights[0] );
	SetLight( LOCAL_MyLights[1] );
	SetLight( LOCAL_MyLights[2] );
	glPushMatrix();*/
	loadCamera();
	SetLight( LOCAL_MyLights[0] );
	SetLight( LOCAL_MyLights[1] );
	draw_hands();
	//glEnable(GL_BLEND);
	//std::cout << "pase1" << std::endl;
	if(game){
		//initTopo();
		if(gameBackup!=game){
			//std::cout << "*pase4" << std::endl;
			MiCamara->rotate_c=0;
			initTopo();
			//topoo=!topoo;
			createTopos();
			//std::cout << "/pase4" << std::endl;
		}
		//std::cout << "-pase4" << std::endl;
		gameBackup=game;
		draw_sceneGame();
		//std::cout << "+pase4" << std::endl;
		//std::cout << "pase5" << std::endl;
	} else {
		if(gameBackup!=game || topooBackup!=topoo){
			//std::cout << "-pase5" << std::endl;
			initTopo();
			createTopos(1,400,200,250);
			createBottoms();
			//std::cout << "+pase5" << std::endl;
		}
		gameBackup=game;
		topooBackup=topoo;
		draw_sceneMenu();
		//std::cout << "*-pase5" << std::endl;
	}
	//std::cout << "pase2" << std::endl;
	//Escenario
	//glDisable(GL_DEPTH_TEST);
	bool imove=draw_scene();
	//std::cout << "+pase3" << std::endl;
	glPushMatrix();
	display();
	//std::cout << "+pase6" << std::endl;
	glPopMatrix();
	//glutSwapBuffers();
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	//glPopMatrix();
	glFlush();
	NEWHAND=false;
	//inmove=imove;
}

void initLightVariables(){
	//DIRECCIONAL
	LOCAL_MyLights = (light **) malloc( 3 * sizeof(light *));
	LOCAL_MyLights[0] = CreateDefaultLight();
	LOCAL_MyLights[0]->type = AGA_DIRECTIONAL;
	LOCAL_MyLights[0]->id = GL_LIGHT0;
	LOCAL_MyLights[0]->position[0] = 200.0f;
	LOCAL_MyLights[0]->position[1] = 200.0f;
	LOCAL_MyLights[0]->position[2] = 200.0f;
	LOCAL_MyLights[0]->position[3] = 0.0f;
	LOCAL_MyLights[0]->pointAtInfinity[0] = LOCAL_MyLights[0]->position[0];
	LOCAL_MyLights[0]->pointAtInfinity[1] = LOCAL_MyLights[0]->position[1];
	LOCAL_MyLights[0]->pointAtInfinity[2] = LOCAL_MyLights[0]->position[2];
	//POSICIONAL
	LOCAL_MyLights[1] = CreateDefaultLight();
	LOCAL_MyLights[1]->type = AGA_POSITIONAL;
	LOCAL_MyLights[1]->id = GL_LIGHT1;
	LOCAL_MyLights[1]->position[0] = 200.0f;
	LOCAL_MyLights[1]->position[1] = 200.0f;
	LOCAL_MyLights[1]->position[2] = -200.0f;
	LOCAL_MyLights[1]->position[3] = 1.0f;
	//SPOT
	LOCAL_MyLights[2] = CreateDefaultLight();
	LOCAL_MyLights[2]->type = AGA_SPOT;
	LOCAL_MyLights[2]->id = GL_LIGHT2;
	LOCAL_MyLights[2]->position[0] = -200.0f;     
	LOCAL_MyLights[2]->position[1] = 200.0f;
	LOCAL_MyLights[2]->position[2] = 200.0f;
	LOCAL_MyLights[2]->spotDirection[0] = 200.0f; 
	LOCAL_MyLights[2]->spotDirection[1] = -200.0f;
	LOCAL_MyLights[2]->spotDirection[2] = -200.0f;
}

void reshape(int width, int height) {
    //glViewport(0, 0, width, height);
    //MiCamara->SetGLAspectRatioCamera();
}

void initGraphics(int& argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Interacción Humano Computador");
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(idle);
	//glutMouseFunc(mouse);
	glutMotionFunc(NULL);
	//glutPassiveMotionFunc(MouseMotion);
	myInit();
	initLightVariables();
}

void play(){
	controller.addListener(listener);
	glutMainLoop();
}

int main(int argc, char **argv){
	MiCamara = new class Camara();
	initGraphics(argc, argv);
	play();
}
