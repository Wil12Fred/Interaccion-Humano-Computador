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
#include "leapmotion.h"

double rotate_y=0; 
double rotate_x=0;

double topo_trans_x=-220;
double topo_trans_y=-300;
double topo_trans_z=-510;
double topo_esc=200;
double bottom_trans_x=-135;
double bottom_trans_y=-520;
double bottom_trans_z=-690;
double bottom_esc=200;

SampleListener listener;
Controller controller;

void myInit() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void specialKeys( int key, int x, int y ) {
	/*if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);*/
	
	//  Flecha derecha: aumentar rotación 5 grados
	if (key == GLUT_KEY_RIGHT)
		rotate_y -= 5;
	//  Flecha izquierda: disminuir rotación 5 grados
	else if (key == GLUT_KEY_LEFT)
		rotate_y += 5;
 
	else if (key == GLUT_KEY_UP)
		rotate_x -= 5;
 
	else if (key == GLUT_KEY_DOWN)
		rotate_x += 5;
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

//(-17.9619, 54.2626, 62.3615);
//(14.6468, 76.7065, 13.6628);
void draw_hands(){
	//glRotatef(180,0.0,1.0,0.0);
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2.5); 
	glBegin(GL_LINES);
		glVertex3f(-41.913,102.057,138.329);glVertex3f(-41.913,102.057,138.329);
		glVertex3f(-41.913,102.057,138.329);glVertex3f(-15.3823,127.651,115.778);
		glVertex3f(-15.3823,127.651,115.778);glVertex3f(-1.37188,144.293,95.827);
		//
		glVertex3f(-48.4583,121.36,135.954);glVertex3f(-32.3174,137.194,76.411);
		glVertex3f(-32.3174,137.194,76.411);glVertex3f(-19.589,140.937,41.664);
		glVertex3f(-19.589,140.937,41.664);glVertex3f(-12.9246,138.229,22.0143);
		//
		glVertex3f(-58.5077,123.748,133.319);glVertex3f(-51.3423,139.198,75.3696);
		glVertex3f(-51.3423,139.198,75.3696);glVertex3f(-53.4176,161.515,40.1717);
		glVertex3f(-53.4176,161.515,40.1717);glVertex3f(-54.0972,171.526,17.691);
		//
		glVertex3f(-68.9242,123.402,131.199);glVertex3f(-70.6707,137.229,78.7907);
		glVertex3f(-70.6707,137.229,78.7907);glVertex3f(-78.777,158.395,47.4467); 
		glVertex3f(-78.777,158.395,47.4467); glVertex3f(-83.4269,169.743,26.837);
		//
		glVertex3f(-79.1953,116.794,129.57);glVertex3f(-87.8555,131.213,82.272);
		glVertex3f(-87.8555,131.213,82.272);glVertex3f(-103.359,147.81,61.7476);
		glVertex3f(-103.359,147.81,61.7476);glVertex3f(-111.675,156.002,49.4818);
	glEnd();
	glColor3f(0.0, 1.0, 0.0);
	glPointSize(2);
	glBegin(GL_POINTS);
		glVertex3f(-41.913,102.057,138.329);
		glVertex3f(-41.913,102.057,138.329);
		glVertex3f(-15.3823,127.651,115.778);
		//
		glVertex3f(-48.4583,121.36,135.954);
		glVertex3f(-32.3174,137.194,76.411);
		glVertex3f(-19.589,140.937,41.664);
		//
		glVertex3f(-58.5077,123.748,133.319);
		glVertex3f(-51.3423,139.198,75.3696);
		glVertex3f(-53.4176,161.515,40.1717);
		//
		glVertex3f(-68.9242,123.402,131.199);
		glVertex3f(-70.6707,137.229,78.7907);
		glVertex3f(-78.777,158.395,47.4467);
		//
		glVertex3f(-79.1953,116.794,129.57);
		glVertex3f(-87.8555,131.213,82.272);
		glVertex3f(-103.359,147.81,61.7476);
	glEnd();
	double trans_x;
	double trans_y;
	double trans_z;
	double esc;
	if(game){
		trans_x=topo_trans_x;
		trans_y=topo_trans_y;
		trans_z=topo_trans_z;
		esc=topo_esc;
	} else {
		trans_x=bottom_trans_x;
		trans_y=bottom_trans_y;
		trans_z=bottom_trans_z;
		esc=bottom_esc;
	}
	glColor3f(0.0, 0.0, 1.0);
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
	/*for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		const Hand hand = *hl;
		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			Bone::Type boneType = static_cast<Bone::Type>(3);
			Vector boneEndPast=Vector(finger.bone(boneType).nextJoint().x,
						finger.bone(boneType).nextJoint().y,
						finger.bone(boneType).nextJoint().z);
			for (int b = 2; b >-1; --b) { 
				boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				Vector boneStart = bone.prevJoint();
				Vector boneEnd = bone.nextJoint();
				//std::cout << boneStart.x << " " << boneStart.y << " " << boneStart.z << std::endl; 
				//std::cout << boneEnd.x << " " << boneEnd.y << " " << boneEnd.z << std::endl; 
				glColor3f(1.0, 0.0, 0.0);
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
			}
		}
	}
	//std::cout << std::endl;*/
	//glRotatef(180,0.0,1.0,0.0);
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
	glTranslatef(-150,0,-50);
	draw_bottom();
	glTranslatef(-150,0,50);
	draw_bottom();
}

void draw_topos(){
	glColor3f(1.0,0.0,0.0);
	glTranslatef(2.9,0,0); draw_topo();
	glTranslatef(-2.0,0,-2); draw_topo();
	glTranslatef(-2.0,0,0); draw_topo();
	glTranslatef(-2.0,0,2); draw_topo();
}

void draw_sceneMenu(){
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120,1.0,1,500);
	glMatrixMode(GL_MODELVIEW);*/
	//glRotatef(rotate_x,1.0,0.0,0.0);
	//glRotatef(rotate_y,0.0,1.0,0.0);
	glTranslatef(0,-500,-150);
	glScalef(3,3,3);
	draw_bottoms();
}

void draw_sceneGame(){
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,1.0,0.1,20.0);
	glMatrixMode(GL_MODELVIEW);*/
	//glRotatef(rotate_x,1.0,0.0,0.0);
	//glRotatef(rotate_y,0.0,1.0,0.0);
	glTranslatef(0,-300,-350);
	glScalef(120,120,120);
	draw_topos();
}

bool gameBackup;

void idle(void){
	if(gameBackup!=game){
		//draw_hands();
		glutPostRedisplay();
	}
}

void loadCamera(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100,1.0,1,2000);
	glMatrixMode(GL_MODELVIEW);
}

void draw_sceneHands(){
	glRotatef(rotate_x,1.0,0.0,0.0);
	glRotatef(rotate_y,0.0,1.0,0.0);
	glTranslatef(0,-100,-200);
	draw_hands();
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//dibujar escenario Menu o Game
	loadCamera();
	if(game){
		draw_sceneHands();
		draw_sceneGame();
		
	} else {
		draw_sceneHands();
		//draw_sceneGame();
		draw_sceneMenu();
	}
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
