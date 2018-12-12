#include <stdio.h>
#include <string.h>
#include <time.h>
#include <set>
#include <map>
#include <ctime>
//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <vector>
#include <errno.h>
//
//#include "conneccion.h"
#include "modelocs.h"
//#include <conio.h>
//#include <stlib.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//
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
#include "utilidad.h"

unsigned t0, t1;

MainConnection* MC;//("192.168.8.108",port);

std::string readServer(int SocketFD){
	int n;
	char buffer[5]; //
	bzero(buffer, 5);
	n = read(SocketFD, buffer, 4); // Reading first 4 bytes
	if(n==0){//interrupted connection
		std::cout << "Server: The connection was interrupted" << std::endl;
		return "";
	}
	int size_mns=stoi(std::string(buffer));
	char mns[size_mns+1];
	mns[size_mns]=0;
	n = read(SocketFD, mns, size_mns);
	return std::string(mns);
}

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
//bool inmove=false;

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
Model* Trofeo;

void initDiglet(){
	Topo=new Model("Diglett.obj",50);
	Topo->addMesh(0);Topo->addColor(0,0.0,1.0,0.0);
	Topo->addMesh(1);Topo->addColor(1,0.0,0.0,0.0);
	Topo->moveToCenter();
	Topo->useColor=true;
	Topo->solid();
}

void initTopo(){
	if(topoo){
		Topo=new Model("Topo.obj",120);
		Topo->addMesh(0);Topo->addColor(0,0.0,1.0,0.0);
		Topo->addMesh(1);Topo->addColor(1,0.0,0.0,0.0);
		Topo->moveToCenter();
		Topo->useColor=true;
		Topo->solid();
	} else {
		initDiglet();
	}
}

void initTrofeo(){
	Trofeo=new Model("estrellica.obj",20);
	Trofeo->addMesh(0);Topo->addColor(0,1.0,1.0,0.0);
	//Topo->addMesh(1);Topo->addColor(1,1.0,1.0,0.0);
	//Topo->addMesh(2);Topo->addColor(2,1.0,1.0,0.0);
	Trofeo->moveToCenter();
	Trofeo->useColor=true;
	Trofeo->solid();
}

void initBottom(){
	Bottom=new Model("Bottom.obj", 1.5);
	Bottom->addMesh(2);Bottom->addColor(2,1.0,0.0,0.0);
	Bottom->addMesh(3);Bottom->addColor(3,0.0,0.0,1.0);
	Bottom->moveToCenter();
	Bottom->useColor=true;
	Bottom->solid();
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
	/*LOCAL_MyLights[1] = CreateDefaultLight();
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
	LOCAL_MyLights[2]->spotDirection[2] = -200.0f;*/
}

void initVariables(){
	initQuadric();
	initTopo();
	initTrofeo();
	initBottom();
	gameBackup=true;
	MiCamara->SetCamera(0, 300, 250,
				0, -300, -250,
				0, 1, 0);
	MiCamara->camAperture=130 * DEGREE_TO_RAD;
	MiCamara->camNear = 1;
	MiCamara->camFar = 2000;
	SwitchLight( LOCAL_MyLights[0], TRUE);
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
	initLightVariables();
	initVariables();
	initGlVariables();
}

bool colaborator=true;
std::thread readT;

void getCantToposServer(){
	write(MC->socketFD, "0002",4);
	//std::string mns=readServer(MC->socketFD);
	//return stoi(mns);
}


void initGameConnection(){
	game=!game;
	if(game){
		write(MC->socketFD, "0001",4);
		getCantToposServer();
		/*std::string mns = readServer(MC->socketFD);
		if(mns=="0"){
			colaborator=true;
		} else {
			colaborator=false;
		}*/
	} else {
		write(MC->socketFD, "0005",4);
		//readT.detach();
		//readT.std::thread::~thread();
	}
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
		initGameConnection();
		glutPostRedisplay();
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
		topoo=!topoo;
	} else if(key==GLUT_KEY_F9){
	} else if (key==GLUT_KEY_F10){
	}
	glutPostRedisplay();
}

std::vector<objl::Vector3> Articulation_Points;
std::vector<Objeto> Topos;
std::vector<Objeto> Bottoms;

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
	int scens=1;
	double angle;
	if(n_topos>1){
		angle = 360.0/(scens*(n_topos/*-1*/));
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
		//if(!Bottoms[i].invisible){
			Bottoms[i].draw(true);
		//} else {
		//	Topos[i].draw(true);
		//}
	}
	//double max=(Bottoms[0].model->max.Y+Bottoms[0].model->min.Y)/2;
	if(Bottoms[0].intersected){
		//if( Bottoms[0].maxY<=max/* && WasIntersected[0]==0*/){
			topoo=!topoo;
			//std::cout << "00000000" << std::endl;
		//} else {
			//Bottoms[0].intersected=0;
		//}
	}
	if(Bottoms[1].intersected){// && Bottoms[1].maxY<=max/* && WasIntersected[1]==0*/){
		//initGameConnection();
		//std::cout << "1111y000" << std::endl;
	}
	if(Bottoms[2].intersected){// && Bottoms[2].maxY<=max/* && WasIntersected[2]==0*/){
		//std::cout << "2r12:000" << std::endl;
		current_light=0;
		//delete MiCamara;
		//exit(0);
	}
}

std::vector<bool> Block;

bool draw_topos(){
	bool existTopos=false;
	for (int i=0;i<Topos.size();i++){
		if(!colaborator){
			if(!Topos[i].invisible){
				existTopos=true;
				if(Block[i]){
					Topo->addColor(0,1.0,0.0,0.0);
					Topos[i].draw(false, false, false);
				} else {
					Topos[i].draw();
				}
			} else {
				//Topos[i].maxY=(Topos[i].model->min.Y+Topos[i].model->max.Y)/2;
				if(Block[i]){
					Topo->addColor(0,0.0,1.0,0.0);
					Topos[i].draw(true, false, true);
				} else {
					Topos[i].draw(true, true, true);
				}
			}
		} else {//COLABORADOR
			if(game){
				//Topos[i].draw(true,false);
				if(!Topos[i].invisible){
					if(Block[i]){
						Topo->addColor(0,1.0,0.0,0.0);
						Topos[i].draw(false, false, false);//true, false);
					} else {
						Topos[i].draw(false, true, false);
					}
				} else {
					Topos[i].maxY=Topos[i].model->max.Y+100;
					if(Block[i]){
						Topo->addColor(0,0.0,1.0,0.0);
						Topos[i].draw(true, false, true, true);
					} else {
						Topos[i].draw(true, true, true, true);
					}
				}
			} else {
				Topos[i].draw();
			}
		}
	}
	return existTopos;
}

void draw_iluminationMode(){
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		Hand hand = *hl;
		if(hand.isRight() ){
			if (isPuno(hand)){
				Vector palmPosition = hand.palmPosition();
				Mouse_Luces(palmPosition.x, palmPosition.y);
			}
		} else {
			if (isPuno(hand)){
				current_light=-1;
			}
		}
	}
}

int draw_selectionMode(){
	//WasIntersected.resize(Bottoms.size());
	int tot=0;
	for (int i=0;i<Bottoms.size();i++){
		bool isInvisible=Bottoms[i].invisible;
		//bool wasInter=Bottoms[i].intersected;
		//WasIntersected[i]=wasInter;
		//bool wasInter=false;
		bool inters=false;
		for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
			Hand hand = *hl;
			if(isPuno(hand)){
				getArticulationPoints(Articulation_Points,hand.isRight());
				Bottoms[i].intersecta(Articulation_Points);
				if(Bottoms[i].intersected){
					inters=true;
					if(!isInvisible){
						if(Bottoms[i].maxY<=(Bottoms[i].model->max.Y+Bottoms[i].model->min.Y)/2){
							//inters=true;
							tot++;
						//} else {
							//Bottoms[i].intersected = false;
						}
					}
				} else {
				}
			}
		}
		if(isInvisible){
			Bottoms[i].intersected=false;
			if(!inters){
				Bottoms[i].invisible=false;
			}
		} else {
			if(inters){
				Bottoms[i].invisible=true;
				Bottoms[i].intersected=true;
			} else {
				Bottoms[i].intersected=false;
			}
		}
		/*if(wasInter){
			if(inters){
				Bottoms[i].invisible=true;
				tot++;
			}
		} else {
			if(isInvisible){
				Bottoms[i].invisible=false;
			}
		}*/
	}
	return tot;
}

bool win=false;

void draw_sceneMenu(){
	if(current_light==0){
		draw_iluminationMode();
		for (int i=0;i<Bottoms.size();i++){
			Bottoms[i].intersected=false;
		}
	} else {
		int tot=draw_selectionMode();
		if(tot>1){
			for (int i=0;i<Bottoms.size();i++){
				Bottoms[i].intersected=false;
			}
		}
	}
	if(win){
		Objeto Trofe(Trofeo, objl::Vector3(0,80,120));
		Trofe.draw();
	}
	draw_bottoms();
	draw_topos();
}

void setInvisibleTopo(int i){
	Topos[i].invisible=true;
	write(MC->socketFD, "0004",4);
	std::string size_mns = fillZeros(std::to_string(i).size(),4);
	write(MC->socketFD, size_mns.c_str(),4);
	std::string i_topo = std::to_string(i);
	write(MC->socketFD, i_topo.c_str(),i_topo.size());
}

void draw_sceneGame(){
	if(hands.isEmpty()){
		getArticulationPoints(Articulation_Points);
		for (int i=0;i<Topos.size();i++){
			if(!Topos[i].invisible){
				Topos[i].intersecta(Articulation_Points);
				if(Topos[i].intersected){
					if (Topos[i].maxY<=(Topos[i].model->max.Y+Topos[i].model->min.Y)/2){ 	
						//Topos[i].invisible=true;
						setInvisibleTopo(i);
					}
				}
			}
		}
	} else {
		for (int i=0;i<Topos.size();i++){
			if(!colaborator){
				if(!Topos[i].invisible){
					for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
						Hand hand = *hl;
						getArticulationPoints(Articulation_Points,hand.isRight());
						Topos[i].intersecta(Articulation_Points);
						if(Topos[i].intersected){
							if (Topos[i].maxY<=(Topos[i].model->max.Y+Topos[i].model->min.Y)/2){
								//Topos[i].invisible=true;
								setInvisibleTopo(i);
							}
						}
					}
				}
			} else {//COLABORADOR
				if(!Topos[i].invisible){
					for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
						Hand hand = *hl;
						getArticulationPoints(Articulation_Points,hand.isRight());
						Topos[i].intersecta(Articulation_Points);
						if(Topos[i].intersected){
							/*if (Topos[i].maxY<=(Topos[i].model->max.Y+Topos[i].model->min.Y)/2){
								Topos[i].invisible=true;
							}*/
							//Topos[i].invisible=true;
							setInvisibleTopo(i);
						}
					}
				}
			}
		}
	}
	if(!draw_topos()){
		if(!colaborator){
			//game=false;
		}
	}
}

void idle(void){
	//t1=clock();
	if( topoo!=topooBackup || gameBackup!=game/*  || inmove*/){
		glutPostRedisplay();
	} else if (NEWHAND) {
		glutPostRedisplay();
	} else {
		//double time=(double(t1-t0)/CLOCKS_PER_SEC);
		//if(time>0.016){
			//glutPostRedisplay();
		//}
	}
}

void loadCamera(){
	if(gameBackup!=game){
		MiCamara->rotate_c=0;
	}
	MiCamara->SetGLCamera();
}

bool draw_scene(){
	Cube baseC(/*MiCamara->camView.X*/-800,/*MiCamara->camView.Y/5*/60,/*MiCamara->camView.Z*/+800,1600);
	glColor4f(0,0.2,0,0.8);
	baseC.draw2();
	//
	Cube leftC(/*MiCamara->camView.X*/-400,100/*MiCamara->camView.Y/2*/,200,400);
	Cube rightC(/*MiCamara->camView.X+*/0,100/*MiCamara->camView.Y/2*/,200,400);
	std::vector<objl::Vector3> HandPoints;
	bool imove=false;
	if(current_light!=0){
		if  (!hands.isEmpty()){
			for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
				Hand hand = *hl;
				getLeapArticulationPoints(HandPoints,hand.isRight());
				Vector palmPosition = hand.palmPosition();
				Vector palmNormal = hand.palmNormal();
				//objl::Vector3 Palm = (objl::Vector3(palmPosition.x, palmPosition.y, palmPosition.z));
				objl::Vector3 PalmNormal= objl::Vector3(palmNormal.x,palmNormal.y,palmNormal.z);
				if(isPalm(hand)){
					if(hand.isLeft()){// && isPalm(hand)){
						getLeapArticulationPoints(HandPoints,0);
						if(leftC.check_intersection(HandPoints)){
							imove=true;
							//glColor4f(1.0,0.1,0.1,0.3);
							//leftC.draw(1);
							double opac=std::max(float(0.0),float(PalmNormal.X));
							if(opac>0.5){
								if(!game){
									MiCamara->rotate_c-=opac*0.3;
								} else {
									MiCamara->rotate_c-=opac*0.6;
								}
							}
							glColor4f(1.0,0.1,0.1,0.1+opac*0.9);
							//glColor4f(0.2,0.0,0.0,0.3);//leftC.draw(3);
							leftC.draw(1);
						}
					} else {//if(hand.isRight() && isPalm(hand)){
						getLeapArticulationPoints(HandPoints,1);
						if(rightC.check_intersection(HandPoints)){
							imove=true;
							//glColor4f(1.0,0.1,0.1,0.3);//rightC.draw(1);
						//} else {
							//glColor4f(0.2,0.0,0.0,0.3);//rightC.draw(2);
							double opac=std::max(float(0.0),float(-PalmNormal.X));
							if(opac>0.5){
								if(!game){
									MiCamara->rotate_c+=opac*0.3;
								} else {
									MiCamara->rotate_c+=opac*0.6;
								}
							}
							glColor4f(1.0,0.1,0.1,0.1+opac*0.9);
							rightC.draw(1);
						}
					}
				}
			}
		}
	}
	return imove;
}

void display(void){
	float At[3];
	float Direction[3];
	/*switch (current_light){
		case 0:*/
	if(current_light==0){
			At[0] = LOCAL_MyLights[current_light]->position[0];
			At[1] = LOCAL_MyLights[current_light]->position[1];
			At[2] = LOCAL_MyLights[current_light]->position[2];         
			Direction[0] = - LOCAL_MyLights[current_light]->position[0];
			Direction[1] = - LOCAL_MyLights[current_light]->position[1];
			Direction[2] = - LOCAL_MyLights[current_light]->position[2];
			Draw_Parallel(At);
			Draw_Meridian(At);
			Draw_Vector(At, Direction);
	}
}

void getTopos(){
	write(MC->socketFD, "0003",4);
	//std::string mns=readServer(MC->socketFD);
	//return mns;
}

bool loadBuffer=false;
int cantTopos=-1;

void updateTopos(){
	char mns[5];
	int n;
	while(1){
		mns[4]=0;
		n = read(MC->socketFD, mns, 4);
		if(std::string(mns)=="0005"){
			mns[2]=0;
			n = read(MC->socketFD, mns, 2);
			if(game){
				int topo_c=atoi(mns);
				Topos[topo_c].invisible=!Topos[topo_c].invisible;
			}
		} else if(std::string(mns)=="0001"){
			mns[1]=0;
			n = read(MC->socketFD, mns, 1);
			if(mns[0]=='0'){
				colaborator=1;
			} else {
				colaborator=0;
			}
		} else if(std::string(mns)=="0002"){
			std::string mns=readServer(MC->socketFD);
			cantTopos=stoi(mns);
			Block.resize(cantTopos);
			for (int i=0; i<cantTopos; i++){
				Block[i]=0;
			}
		} else if(std::string(mns)=="0003"){
			std::string mns=readServer(MC->socketFD);
			for (int i=0;i<mns.size();i++){
				if(mns[i]=='1'){
					Topos[i].invisible=true;
				} else {
					Topos[i].invisible=false;
				}
			}
		} else if(std::string(mns)=="0006"){
			game=!game;
			win=true;
			write(MC->socketFD, "0005",4);//SALIR
		} else if(std::string(mns)=="0007"){
			game=!game;
			win=false;
			write(MC->socketFD, "0005",4);//SALIR
		} else if(std::string(mns)=="0008"){
			std::string mns=readServer(MC->socketFD);
			Block[stoi(mns)]=true;
		}
		glutPostRedisplay();
	}
}

void myDisplay(void){
	//inmove=false;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//| GL_BLENT_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	/*if(!loadBuffer){
		loadBuffer=true;
		glutSwapBuffers();
		return;
	}*/
	//t0=clock();
	//glLoadIdentity();
	//dibujar escenario Menu o Game
	//glLoadIdentity();
	//SetLight( LOCAL_MyLights[0] );SetLight( LOCAL_MyLights[1] );SetLight( LOCAL_MyLights[2] );glPushMatrix();
	loadCamera();
	//SetLight( LOCAL_MyLights[1] );
	//glEnable(GL_BLEND);
	glPushMatrix();
		draw_hands();
		if(game){
			//initTopo();
			if(gameBackup!=game){
				MiCamara->rotate_c=0;
				initTopo();
				if(colaborator){
					createTopos(cantTopos,0,50, MiCamara->camView.Y/4-100);
				} else {
					createTopos(cantTopos,0,50, MiCamara->camView.Y/4);
					for (int i=0;i<cantTopos;i++){
						Topos[i].invisible=true;
					}
				}
				getTopos();
				/*for (int i=0; i<Topos.size(); i++){
					Topos[i].invisible=true;
				}*/
			}
			/*std::string toposStatus = getTopos();
			for (int i=0;i<Topos.size();i++){
				if(toposStatus[i]=='1'){
					if(!Topos[i].invisible){
						Topos[i].invisible=true;
					}
				} else {
					if(Topos[i].invisible){
						Topos[i].invisible=false;
					}
				}
			}*/
			gameBackup=game;
			draw_sceneGame();
		} else {
			if(gameBackup!=game){// || topooBackup!=topoo){
				initTopo();
				createTopos(1,400,200,250);
				createBottoms();
			} else if(topooBackup!=topoo){
				initTopo();
				createTopos(1,400,200,250);
			}
			gameBackup=game;
			topooBackup=topoo;
			draw_sceneMenu();
		}
		//Escenario
		//glDisable(GL_DEPTH_TEST);
		bool imove=draw_scene();
		display();
	glPopMatrix();
	if(!gameBackup){
		if(Bottoms[1].intersected){// && Bottoms[1].maxY<=max/* && WasIntersected[1]==0*/){
			initGameConnection();
		}
	}
	SetLight( LOCAL_MyLights[0] );
	//glutSwapBuffers();//glDisable(GL_BLEND);//glEnable(GL_DEPTH_TEST);//glPopMatrix();//glFlush();
	NEWHAND=false;
	glutSwapBuffers();
	//glFlush();
	//inmove=imove;
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
	//glutMouseFunc(mouse);glutMotionFunc(NULL);glutPassiveMotionFunc(MouseMotion);
	myInit();
}

void play(){
	controller.addListener(listener);
	glutMainLoop();
}

std::string Cl_ReadMsg(int SocketFD,int size_msg){
	int n;
	char msg[size_msg+1];
	n = read(SocketFD, msg, size_msg);
	msg[size_msg]=0;
	return std::string(msg);
}

/*void read2(int SocketFD){
	int n;
	char buffer[5]; //
	for (;;){
		bzero(buffer, 5);
		do {
			n = read(SocketFD, buffer, 4); // Reading first 4 bytes
			if(n==0){//interrupted connection
				//if(playing)
					//endwin();
				std::cout << "Server: The connection was interrupted" << std::endl;
				return;
			}
			int size_msg=atoi(buffer);
			bzero(buffer, 4); // Zeros for the 4 bytes that was reading   
			//
			n = read(SocketFD, buffer, 1); //reading 1 bytes
			std::string action(buffer);
			bzero(buffer, 1); //equal to the before
			if (action == "G"){ // Responsive when exist movement in Game
				//Cl_game(SocketFD,size_msg);
			} else if(action=="O"){
				//Cl_bullet(SocketFD,size_msg);
			} else if(action=="R"){
				std::string msg=Cl_ReadMsg(SocketFD, size_msg);
				std::cout << msg << std::endl;
				//print_message(msg);
			} else if(action=="D"){
				//if(!playing){
					std::cout << "Downloading" << std::endl;
				//}
				//std::string msg=Cl_Download(SocketFD, size_msg);
				//print_message(msg);
			}
		} while (n == 0);
	}
}

void write2(int SocketFD){
	return;
}*/

int main(int argc, char **argv){
	int port;
	std::cout << "Port: ";
	std::cin >> port;//MainConnection MC("192.168.8.108",port);
	MC=new MainConnection("10.0.1.7"/*"172.20.10.4"*/, port);
	char buffer[100];
	buffer[13]=0;
	read(MC->socketFD,buffer,12);
	readT=std::thread(updateTopos);
	/*ClientServerModel CSM(MC);
	MainConnection MC("172.20.10.4",port);
	CSM.run(read2, write2);*/
	MiCamara = new class Camara();
	initGraphics(argc, argv);
	play();
}
