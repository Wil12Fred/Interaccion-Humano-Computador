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

double rotate_x=0;
double rotate_y=0; 
double rotate_z=0;
GLUquadric* qobj;

#include "hand_primitivas.h"

bool game=false;
bool gameBackup;

SampleListener listener;
Controller controller;

Model* Bottom;
Model* Topo;

void myInit() {
	/*if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);*/
	Bottom=new Model("model.obj",2);
	Topo=new Model("Topoo.obj",120);
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.2, 0.2, 0.1, 0.5);

	Bottom->setToCenter(615-900, -70, 390-25);
	Bottom->addMesh(2);Bottom->addColor(2,1.0,0.0,0.0);
	Bottom->addMesh(3);Bottom->addColor(3,0.0,0.0,1.0);
	Bottom->useColor=true;
	Bottom->solid();

	Topo->setToCenter(0, -70, -15);
	Topo->addMesh(0);Topo->addColor(0,0.0,1.0,0.0);
	Topo->addMesh(1);Topo->addColor(1,1.0,1.0,0.0);
	Topo->useColor=true;
	Topo->solid();

	gameBackup=true;
}

void specialKeys( int key, int x, int y ) {
	//  Flecha derecha: aumentar rotación 5 grados
	if (key == GLUT_KEY_RIGHT)
		rotate_y -= 5;
	//  Flecha izquierda: disminuir rotación 5 grados
	else if (key == GLUT_KEY_LEFT)
		rotate_y += 5;
	else if (key == GLUT_KEY_UP)
		rotate_x += 5;
	else if (key == GLUT_KEY_DOWN)
		rotate_x -= 5;
	//  Solicitar actualización de visualización
	else if (key == GLUT_KEY_F1){
		game=!game;
		return;
	} else if (key == GLUT_KEY_F2)
		rotate_z-=5;
	else if (key == GLUT_KEY_F3)
		rotate_z+=5;
	glutPostRedisplay();
}

std::vector<objl::Vector3> Articulation_Points;
objl::Loader TopoLoader, BottomLoader;
std::vector<Objeto> Topos, Bottoms;

void createBottoms(){
	Bottoms.clear();
	double x,y,z;
	Bottoms.push_back(Objeto(Bottom,x=-250,y=0,z=0));
	Bottoms.push_back(Objeto(Bottom,x+=250,y=0,z-=15));
	Bottoms.push_back(Objeto(Bottom,x+=250,y=0,z+=15));
}

void draw_bottoms(){
	for (int i=0;i<Bottoms.size();i++){
		Bottoms[i].draw();
	}
	if(Bottoms[1].intersected && Bottoms[1].maxY<=0){
		game=!game;
	}
}

void createTopos(){
	Topos.clear();
	double x,y,z;
	Topos.push_back(Objeto(Topo,x=-420,y=0,z=30));
	Topos.push_back(Objeto(Topo,x+=280,y=0,z-=25));
	Topos.push_back(Objeto(Topo,x+=280,y=0,z-=25));
	Topos.push_back(Objeto(Topo,x+=280,y=0,z+=25));
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
		//if(!Bottoms[i].intersected){
			Bottoms[i].intersecta(Articulation_Points);
		//}
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
	if(gameBackup!=game || NEWHAND){
		NEWHAND=false;
		glutPostRedisplay();
	}
}

void loadCamera(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(130,1.0,1,2000);
	gluLookAt(0, 200, 0, 
			0.0, 100.0, -300.0,
			 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	/*glTranslatef(0,0,-100);
	glBegin(GL_LINES);
	glColor3f(0.0,1.0,100);
		glVertex3f(-30,0,-30);glVertex3f(30,0,30);
		glVertex3f(-30,0,30);glVertex3f(30,0,-30);
	glEnd();*/
	glTranslatef(0,0,-250);
}

void myDisplay() {
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
	glTranslatef(0,0,250);
	Cube base(-400,0,245,800);
	glColor4f(1,1,1,0.2);
	base.draw();
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	glFlush();
	glutSwapBuffers();
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
