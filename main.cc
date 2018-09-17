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
double rotate_c=0;
GLUquadric* qobj;

bool game=false;
bool gameBackup;
bool topoo=true;
bool topooBackup;
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

void initDiglet(){
	Topo=new Model("Diglett.obj",40);
	//Topo->setToCenter(0, -70, -15);
	std::cout << Topo->formatObj.LoadedMeshes.size() << std::endl;
	Topo->addMesh(0);Topo->addColor(0,0.0,1.0,0.0);
	Topo->addMesh(1);Topo->addColor(1,1.0,1.0,0.0);
	Topo->moveToCenter();
	Topo->useColor=true;
	Topo->solid();
}

void initTopo(){
	if(topoo){
		Topo=new Model("Topo.obj",120);
		//Topo->setToCenter(0, -70, -15);
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
	//glClearColor(0.3, 0.8, 0.8, 0.5);
	glColor3f(1.0,1.0,0.0);
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

void createTopos(int n_topos=4,double amp=400, double dist=0){
	int scens=6;
	double angle;
	if(n_topos>1){
		angle = 360.0/(scens*(n_topos-1));
	} else {
		angle = 0;
	}
	Topos.clear();
	objl::Vector3 Center=Camara, Position;
	Center.Y/=5;
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
	if(Bottoms[0].intersected && Bottoms[1].maxY<=0){
		topoo=!topoo;
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
	draw_topos();
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
	if( topoo!=topooBackup|| gameBackup!=game || NEWHAND || inmove){
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
	Cube leftC(Camara.X-600,Camara.Y/2,0,400);
	Cube rightC(Camara.X+200,Camara.Y/2,0,400);
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

void display(void){
	float At[3];
	float Direction[3];
	switch( current_light ){
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
                         glutSolidSphere(0.05,28,28);                         
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
                        glutSolidSphere(0.05,28,28);                                   
                        break;
		default:
			break;
	}
	glPopMatrix();
	glutSwapBuffers();
}

void myDisplay(void){
	inmove=false;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//| GL_BLENT_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLoadIdentity();
	//dibujar escenario Menu o Game
	glLoadIdentity();
	SetLight( LOCAL_MyLights[0] );
	SetLight( LOCAL_MyLights[1] );
	SetLight( LOCAL_MyLights[2] );
	glPushMatrix();
	loadCamera();
	draw_hands();
	//glEnable(GL_BLEND);
	if(game){
		//initTopo();
		if(gameBackup!=game){
			initTopo();
			//topoo=!topoo;
			createTopos();
		}
		gameBackup=game;
		draw_sceneGame();
	} else {
		if(topooBackup!=topoo){
			createTopos(1,400,200);
			createBottoms();
		}
		gameBackup=game;
		draw_sceneMenu();
	}
	//Escenario
	//glDisable(GL_DEPTH_TEST);
	draw_scene();
	display();
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glFlush();
	glutSwapBuffers();
}

void lightVariables(){
	//DIRECCIONAL
	LOCAL_MyLights = (light **) malloc( 3 * sizeof(light *));
	LOCAL_MyLights[0] = CreateDefaultLight();
	LOCAL_MyLights[0]->type = AGA_DIRECTIONAL;
	LOCAL_MyLights[0]->id = GL_LIGHT0;
	LOCAL_MyLights[0]->position[0] = 1.0f;
	LOCAL_MyLights[0]->position[1] = 1.0f;
	LOCAL_MyLights[0]->position[2] = 1.0f;
	LOCAL_MyLights[0]->position[3] = 0.0f;
	LOCAL_MyLights[0]->pointAtInfinity[0] = LOCAL_MyLights[0]->position[0];
	LOCAL_MyLights[0]->pointAtInfinity[1] = LOCAL_MyLights[0]->position[1];
	LOCAL_MyLights[0]->pointAtInfinity[2] = LOCAL_MyLights[0]->position[2];
	//POSICIONAL
	LOCAL_MyLights[1] = CreateDefaultLight();
	LOCAL_MyLights[1]->type = AGA_POSITIONAL;
	LOCAL_MyLights[1]->id = GL_LIGHT1;
	LOCAL_MyLights[1]->position[0] = 1.0f;
	LOCAL_MyLights[1]->position[1] = 1.0f;
	LOCAL_MyLights[1]->position[2] = -1.0f;
	LOCAL_MyLights[1]->position[3] = 1.0f;
	//SPOT
	LOCAL_MyLights[2] = CreateDefaultLight();
	LOCAL_MyLights[2]->type = AGA_SPOT;
	LOCAL_MyLights[2]->id = GL_LIGHT2;
	LOCAL_MyLights[2]->position[0] = -1.0f;     
	LOCAL_MyLights[2]->position[1] = 1.0f;
	LOCAL_MyLights[2]->position[2] = 1.0f;
	LOCAL_MyLights[2]->spotDirection[0] = 1.0f; 
	LOCAL_MyLights[2]->spotDirection[1] = -1.0f;
	LOCAL_MyLights[2]->spotDirection[2] = -1.0f;
}

void reshape(int width, int height) {
    //glViewport(0, 0, width, height);
    //MiCamara->SetGLAspectRatioCamera();
}

int main(int argc, char **argv){
	controller.addListener(listener);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Interacción Humano Computador");
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glutMotionFunc(NULL);
	glutPassiveMotionFunc(MouseMotion);
	myInit();
	glutMotionFunc(NULL);
	lightVariables();
	glEnable(GL_COLOR_MATERIAL);
	glutMainLoop();
}
