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
bool game=false;
bool newhand=false;
#include "leapmotion.h"
#include "listcube.h"

double rotate_y=0; 
double rotate_x=0;

double hand_trans_z=-510;
double hand_trans_y=-200;

double topo_trans_x=-219;
double topo_trans_y=-100;
double topo_trans_z=-445;
double topo_dif_x=119;
double topo_dif_z=35;
double topo_esc=58;
double topo_cant=4;

double bottom_trans_x=-185;
double bottom_trans_y=-180;
double bottom_trans_z=-500;
double bottom_dif_x=135;
double bottom_dif_z=75;
double bottom_esc=80;
double bottom_cant=3;

SampleListener listener;
Controller controller;

void myInit() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	/*if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);*/
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
	else if (key == GLUT_KEY_F1)
		game=!game;
	glutPostRedisplay();
 
}

void draw_pixel(objl::Vertex v){
	glBegin(GL_POINTS);
	glVertex3f(v.Position.X, v.Position.Y, v.Position.Z);
	glEnd();
}

void draw_line(objl::Vertex v1, objl::Vertex v2, double width=2.5) {//Corregir
	glLineWidth(width); 
	glBegin(GL_LINES);
		glVertex3f(v1.Position.X, v1.Position.Y, v1.Position.Z);
		glVertex3f(v2.Position.X, v2.Position.Y, v2.Position.Z);
	glEnd();
}

void draw_triangle(objl::Vertex v1, objl::Vertex v2, objl::Vertex v3){
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

void Cube::draw(){
	//glScalef(3,3,1.0);
	glBegin(GL_POLYGON);
		glVertex3f(trans_x,trans_y,trans_z);
		glVertex3f(trans_x+esc,trans_y,trans_z);
		glVertex3f(trans_x+esc,trans_y+esc,trans_z);
		glVertex3f(trans_x,trans_y+esc,trans_z);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex3f(trans_x,trans_y+esc,trans_z);
		glVertex3f(trans_x,trans_y+esc,trans_z-esc);
		glVertex3f(trans_x+esc,trans_y+esc,trans_z-esc);
		glVertex3f(trans_x+esc,trans_y+esc,trans_z);
	glEnd();
	//glScalef(1.0/3,1.0/3,1.0);
}

void ListCube::draw(){
	for (int i=0;i<cubes.size();i++)
		cubes[i].draw();
}

ListCube list_cube;

std::vector<Vector> Articulation_Points;

void draw_finger(Finger& finger){
	double trans_z=hand_trans_z;
	double trans_y=hand_trans_y;
	Bone::Type boneType = static_cast<Bone::Type>(3);
	Vector boneEndPast=Vector(finger.bone(boneType).nextJoint().x,
				finger.bone(boneType).nextJoint().y,
				finger.bone(boneType).nextJoint().z);
	boneEndPast.z+=trans_z;
	boneEndPast.y+=trans_y;
	Articulation_Points.push_back(boneEndPast);
	for (int b = 2; b >-1; --b) { 
		boneType = static_cast<Bone::Type>(b);
		Bone bone = finger.bone(boneType);
		//Vector boneStart = bone.prevJoint();
		Vector boneEnd = bone.nextJoint();
		boneEnd.z+=trans_z;
		boneEnd.y+=trans_y;
		//std::cout << boneStart.x << " " << boneStart.y << " " << boneStart.z << std::endl; 
		//std::cout << boneEnd.x << " " << boneEnd.y << " " << boneEnd.z << std::endl; 
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
			glVertex3f(boneEndPast.x, boneEndPast.y, boneEndPast.z);
			glVertex3f(boneEnd.x, boneEnd.y,boneEnd.z);
		glEnd();
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_POINTS);
			glVertex3f(boneEnd.x,
					boneEnd.y,
					boneEnd.z);
		glEnd();
		boneEndPast=boneEnd;
		Articulation_Points.push_back(boneEndPast);
	}
	//std::cout << std::endl;
}

void draw_hand(Hand& hand){
	FingerList fingers = hand.fingers();
	for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
		Finger finger = *fl;
		draw_finger(finger);
	}
}

void draw_hands(){
	glLineWidth(2.5); 
	glPointSize(2);
	glScalef(3,3,1.0);
	//glTranslatef(0,-250,0);
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		Hand hand = *hl;
		//std::cout << std::string(hand.isLeft() ? "Left":"Right") << std::endl;
		draw_hand(hand);
	}
	//glTranslatef(0,250,0);
	glScalef(1.0/3,1.0/3,1.0);
}

objl::Loader BottomLoader;
objl::Loader TopoLoader;
bool isBottomLoaded=false;
bool isTopoLoaded=false;

void draw_bottom(int i=-1){
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
	if(i==1 && list_cube.cubes[list_cube.size()-1-i].intersected){
		game=true;
	}
}

bool draw_topo(int i=-1){
	if(!isTopoLoaded){
		bool loadout = isTopoLoaded = TopoLoader.LoadFile("Topoo.obj");
		if(!loadout){
			printf("Error\n");
			return false;
		}
	}
	if(i==-1){
		glColor3f(1.0,0.0,0.0);
		draw_loadedMesh(TopoLoader,0);
		return true;
	} else {
		if(!list_cube.cubes[list_cube.size()-1-i].intersected){
			glColor3f(1.0,0.0,0.0);
			draw_loadedMesh(TopoLoader,0);
			return true;
		} else {
			//glColor3f(1.0,0.0,1.0);
			//draw_loadedMesh(TopoLoader,0);
		}
	}
}

void draw_bottoms(){
	glTranslatef(0,0,-25);
	draw_bottom(0);
	glTranslatef(-150,0,-25);
	draw_bottom(1);
	glTranslatef(-150,0,25);
	draw_bottom(2);
	glTranslatef(300,0,25);
}

bool draw_topos(){
	bool existTopos=false;
	glTranslatef(4.5,0,-1.25); existTopos=draw_topo(0) || existTopos;
	glTranslatef(-3.2,0,-0.25); existTopos=draw_topo(1) || existTopos;
	glTranslatef(-3.2,0,0); existTopos=draw_topo(2) || existTopos;
	if(!existTopos){
		std::cout << "pase\n";
	}
	glTranslatef(-3.2,0,0.25); existTopos=draw_topo(3) || existTopos;
	glTranslatef(-4.5+3.2*3,0,1.25);
	return existTopos;
}

void draw_sceneMenu(){
	list_cube.set(bottom_trans_x,
			bottom_trans_y,
			bottom_trans_z,
			bottom_esc,
			bottom_dif_x,
			bottom_dif_z,
			bottom_cant);
	glTranslatef(0,-500,5);
	glScalef(3,3,3);
	list_cube.check_intersection(Articulation_Points);
	draw_bottoms();
	glScalef(1.0/3,1.0/3,1.0/3);
	glTranslatef(0,500,-5);
}

bool gameBackup;

void draw_sceneGame(){
	if(gameBackup!=game){
		list_cube.set(topo_trans_x,
				topo_trans_y,
				topo_trans_z,
				topo_esc,
				topo_dif_x,
				topo_dif_z,
				topo_cant);
	}
	glTranslatef(0,-300,-350);
	glScalef(120,120,120);
	//std::cout << Articulation_Points.size() << std::endl;
	list_cube.check_intersection(Articulation_Points);
	if(!draw_topos()){
		game=false;
	}
	glScalef(1.0/120,1.0/120,1.0/120);
	glTranslatef(0,300,350);
}

void idle(void){
	if(gameBackup!=game || newhand){
		newhand=false;
		glutPostRedisplay();
	}
}

void loadCamera(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100,1.0,1,2000);
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0,-100,0);
}

void myDisplay() {
	Articulation_Points.clear();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//dibujar escenario Menu o Game
	loadCamera();
	glRotatef(rotate_x,1.0,0.0,0.0);
	glRotatef(rotate_y,0.0,1.0,0.0);
	glTranslatef(0,-100,-200);
	draw_hands();
	if(game){
		draw_sceneGame();
	} else {
		draw_sceneMenu();
	}
	//glColor3f(0.0,0.0,1.0);list_cube.draw();
	gameBackup=game;
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
