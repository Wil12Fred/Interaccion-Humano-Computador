//#include <stdarg.h>
#include <GLUT/glut.h>
#include "Camara.h"
#include "primitivas.h"
#include "Vector_tools.h"
#include "light.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static light **LOCAL_MyLights;
static int current_mode = 0;
static int current_light = -1;
static int spot_move=0;

bool command = false; /* command mode */
char strCommand[256];

static Camara *MiCamara;
static int old_x, old_y;

/*
void display(void) {
    float At[3];
    float Direction[3];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    MiCamara->SetGLCamera();
    
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    drawSphereTurtle();
    glPopMatrix();
    
    glutSwapBuffers();
}
*/

void display(void) {
	float At[3];
	float Direction[3];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);  
	//SetGLCamera( MiCamara );
	MiCamara->SetGLCamera();
	SetLight( LOCAL_MyLights[0] );
	SetLight( LOCAL_MyLights[1] );
	SetLight( LOCAL_MyLights[2] );
	glPushMatrix();
	glColor3f(1.0,1.0,0.0);
	drawSphereTurtle();
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

void parseCommand(char* strCommandParse) {
    char *strToken0;
    char *strToken1;
    double val;
    strToken0 = strtok(strCommandParse, " ");
    while ((strToken1 = strtok(NULL, " ")) != NULL) {
        val = atof(strToken1);
        if (!strcmp("fd", strToken0)) { // FORWARD
            glTranslatef(0.0, 0.0, val);
        } else if (!strcmp("bk", strToken0)) { // BACK
            glTranslatef(0.0, 0.0, -val);
        } else if (!strcmp("rt", strToken0)) { // RIGHT
            glRotatef(-val, 0., 1., 0.);
        } else if (!strcmp("lt", strToken0)) { // LEFT
            glRotatef(val, 0., 1., 0.);
        } else if (!strcmp("up", strToken0)) { // UP
            glRotatef(val, 1., 0., 0.);
        } else if (!strcmp("dn", strToken0)) { // DOWN
            glRotatef(-val, 1., 0., 0.);
        }
        strToken0 = strtok(NULL, " ");
        display();
    }
    // EXIT COMMAND MODE
    if (strToken0 != NULL && strncmp(strToken0, "exit", 4) == 0) {
        command = false;
        // HOME
    } else if (strToken0 != NULL && !strcmp("home", strToken0)) {
        glLoadIdentity();
    }
}

/*void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    MiCamara->SetGLAspectRatioCamera();
}*/

void MouseMotion(int x, int y) {
    old_y = y;
    old_x = x;
}

void Zoom(int x, int y) {
    float zoom;
    zoom = (float) ((y - old_y) * DEGREE_TO_RAD);
    old_y = y;
    switch (MiCamara->camMovimiento) {
        case CAM_EXAMINAR:
            if (MiCamara->camAperture + zoom > (5 * DEGREE_TO_RAD) && MiCamara->camAperture + zoom < 175 * DEGREE_TO_RAD)
                MiCamara->camAperture = MiCamara->camAperture + zoom;
            break;
    }
    glutPostRedisplay();
}

void Examinar(int x, int y) {
    float rot_x, rot_y;
    rot_y = (float) (old_y - y);
    rot_x = (float) (x - old_x);
    MiCamara->Rotar_Latitud( rot_y * DEGREE_TO_RAD);
    MiCamara->Rotar_Longitud(rot_x * DEGREE_TO_RAD);
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

void Mouse_Spot_Abrir_Cerrar(int x, int y) {
    float step;
    step = (float) (y - old_y);
    old_y = y;
    if (LOCAL_MyLights[current_light]->spotCutOff + step < 90 && LOCAL_MyLights[current_light]->spotCutOff + step > 0)
        LOCAL_MyLights[current_light]->spotCutOff += step;
    LOCAL_MyLights[current_light]->needsUpdate = TRUE;
    glutPostRedisplay();
}

void Mouse_Spot(int x, int y) {
    float rot_x, rot_y;
    rot_y = (float) (old_y - y);
    rot_x = (float) (x - old_x);
    Rotar_Spot_Latitud(LOCAL_MyLights[current_light], rot_y * DEGREE_TO_RAD);
    Rotar_Spot_Longitud(LOCAL_MyLights[current_light], rot_x * DEGREE_TO_RAD);
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

void Andar(int x, int y) {
    float rotacion_x, avance_y;
    avance_y = (float) (y - old_y) / 10;
    rotacion_x = (float) (old_x - x) * DEGREE_TO_RAD / 5;
    MiCamara->YawCamera(rotacion_x);
    MiCamara->AvanceFreeCamera(avance_y);
    old_y = y;
    old_x = x;
    glutPostRedisplay();
}

void Mouse_Luces_Acercar_Alejar(int x, int y){
	float step;
	step = (float) (y - old_y) / 20.0f;
	old_y = y;
	Acercar_Alejar_Luces( LOCAL_MyLights[current_light], step );
	glutPostRedisplay();
}

void Mouse_Luces(int x, int y){
	float rot_x, rot_y;
	rot_y = (float)(old_y - y);
	rot_x = (float)(x - old_x);
	Rotar_Luces_Latitud( LOCAL_MyLights[current_light],rot_y*DEGREE_TO_RAD );
	Rotar_Luces_Longitud( LOCAL_MyLights[current_light], rot_x*DEGREE_TO_RAD);
	LOCAL_MyLights[current_light]->pointAtInfinity[0] = LOCAL_MyLights[current_light]->position[0];
	LOCAL_MyLights[current_light]->pointAtInfinity[1] =	LOCAL_MyLights[current_light]->position[1];
	LOCAL_MyLights[current_light]->pointAtInfinity[2] =	LOCAL_MyLights[current_light]->position[2];
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}

/*
void mouse(int button, int state, int x, int y) {
    old_x = x;
    old_y = y;
    switch (button) {
        case GLUT_LEFT_BUTTON:
            switch (MiCamara->camMovimiento) {
                case CAM_EXAMINAR:
                    if (state == GLUT_DOWN)
                        glutMotionFunc(Zoom);
                    if (state == GLUT_UP) {
                        glutPassiveMotionFunc(Examinar);
                        glutMotionFunc(NULL);
                    }
                    break;
                case CAM_PASEAR:
                    if (state == GLUT_DOWN)
                        glutMotionFunc(Andar);
                    if (state == GLUT_UP)
                        glutMotionFunc(NULL);
                    break;
            }
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                //glutMotionFunc(scale);
                break;
        default:
            break;
    }
    glutPostRedisplay();
}
*/

void mouse(int button, int state, int x, int y){
	old_x = x;
	old_y = y;
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(current_light > 0){
				if(current_light == 2 && spot_move == 1){
					if (state == GLUT_DOWN)
						glutMotionFunc(Mouse_Spot_Abrir_Cerrar);
					if (state == GLUT_UP){
						glutPassiveMotionFunc(Mouse_Spot);
						glutMotionFunc(NULL);
					}
				}else{
					if (state == GLUT_DOWN)
						glutMotionFunc(Mouse_Luces_Acercar_Alejar);
					if (state == GLUT_UP){
						glutPassiveMotionFunc(Mouse_Luces);
						glutMotionFunc(NULL);
					}
				}
			}else{
				switch(MiCamara->camMovimiento){
					case CAM_EXAMINAR:
						if (state == GLUT_DOWN) glutMotionFunc(Zoom);
						if (state == GLUT_UP){
							glutPassiveMotionFunc(Examinar);
							glutMotionFunc(NULL);
						}
						break;
					case CAM_PASEAR:
						if (state == GLUT_DOWN) glutMotionFunc(Andar);
						if (state == GLUT_UP) glutMotionFunc(NULL);
						break;
				}
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN){
				printf("Right Buttom\n");
			}
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if (command) {
		if (key == 13) {
			strcat(strCommand, " ");
			if (strlen(strCommand) == 1) command = false;
			parseCommand(strCommand);
			strcpy(strCommand, "");
		} else {
			char strKey[2] = " ";
			strKey[0] = key;
			printf("%s",strKey);
			strcat(strCommand, strKey);
		}
	} else { // not in command mode
		switch (key) {
			case 'h':
				printf("help\n\n");
				printf("c - Toggle culling\n");
				printf("q/escape - Quit\n\n");
				break;
			case 'c':
				if (glIsEnabled(GL_CULL_FACE))
					glDisable(GL_CULL_FACE);
				else
					glEnable(GL_CULL_FACE);
				break;
			case '1':
				glRotatef(1.0, 1., 0., 0.);
				break;
			case '2':
				glRotatef(1.0, 0., 1., 0.);
				break;
			case 'i':
				command = true;
				break;
			case 'q':
		 /*   case 27:
			delete MiCamara;
			exit(0);
			break;
			*/
			case 27:
			//	DestroyCamera(&MiCamara);
				delete MiCamara;
				DestroyLight( LOCAL_MyLights[0] );
				DestroyLight( LOCAL_MyLights[1] );
				DestroyLight( LOCAL_MyLights[2] );
				free (LOCAL_MyLights);
				exit(0);
				break;
        }
    }
    if (current_mode==7){
		switch (key) {
			case 'R':
				if(LOCAL_MyLights[current_light]->diffuse[0]<1){
					LOCAL_MyLights[current_light]->diffuse[0] = LOCAL_MyLights[current_light]->diffuse[0] + 0.1f;
					LOCAL_MyLights[current_light]->needsUpdate = TRUE;
				}
				break;
			case 'G':
				if(LOCAL_MyLights[current_light]->diffuse[1]<1){
					LOCAL_MyLights[current_light]->diffuse[1] = LOCAL_MyLights[current_light]->diffuse[1] + 0.1f;
					LOCAL_MyLights[current_light]->needsUpdate = TRUE;
				}
				break;
			case 'B':
				if(LOCAL_MyLights[current_light]->diffuse[2]<1){
					LOCAL_MyLights[current_light]->diffuse[2] = LOCAL_MyLights[current_light]->diffuse[2] + 0.1f;
					LOCAL_MyLights[current_light]->needsUpdate = TRUE;
				}
				break;
			case 'r':
				if(LOCAL_MyLights[current_light]->diffuse[0]>0){
					LOCAL_MyLights[current_light]->diffuse[0] = LOCAL_MyLights[current_light]->diffuse[0] - 0.1f;
					LOCAL_MyLights[current_light]->needsUpdate = TRUE;
				}
				break;
			case 'g':
				if(LOCAL_MyLights[current_light]->diffuse[1]>0){
					LOCAL_MyLights[current_light]->diffuse[1] = LOCAL_MyLights[current_light]->diffuse[1] - 0.1f;
					LOCAL_MyLights[current_light]->needsUpdate = TRUE;
				}
				break;
			case 'b':
				if(LOCAL_MyLights[current_light]->diffuse[2]>0){
					LOCAL_MyLights[current_light]->diffuse[2] = LOCAL_MyLights[current_light]->diffuse[2] - 0.1f;
					LOCAL_MyLights[current_light]->needsUpdate = TRUE;
				}
				break;
		}
	}
    glutPostRedisplay();
}

/*
static void SpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            glutPassiveMotionFunc(MouseMotion);
            MiCamara->camMovimiento = CAM_STOP;
            break;
        case GLUT_KEY_F2:
            glutPassiveMotionFunc(Examinar);
            MiCamara->camMovimiento = CAM_EXAMINAR;
            break;
        case GLUT_KEY_F3:
            glutPassiveMotionFunc(MouseMotion);
            MiCamara->camMovimiento = CAM_PASEAR;
            MiCamara->camAtY = 0;
            MiCamara->camViewY = 0;
            MiCamara->SetDependentParametersCamera();
            break;
        case GLUT_KEY_PAGE_UP:
            //AvanceFreeCamera( MiCamara, 0.5f );
            if (MiCamara->camAperture < 175 * DEGREE_TO_RAD)
                MiCamara->camAperture = MiCamara->camAperture + 2.5f * DEGREE_TO_RAD;
            break;
        case GLUT_KEY_PAGE_DOWN:
            //AvanceFreeCamera( MiCamara, -0.5f );
            if (MiCamara->camAperture > 5 * DEGREE_TO_RAD)
                MiCamara->camAperture = MiCamara->camAperture - 2.5f * DEGREE_TO_RAD;
            break;
        case GLUT_KEY_UP:
            MiCamara->Rotar_Latitud( 2.5f * DEGREE_TO_RAD);
            //PitchCamera( MiCamara, 3.0f * DEGREE_TO_RAD );
            break;
        case GLUT_KEY_DOWN:
            MiCamara->Rotar_Latitud( -2.5f * DEGREE_TO_RAD);
            //PitchCamera( MiCamara, -3.0f * DEGREE_TO_RAD );
            break;
        case GLUT_KEY_LEFT:
            MiCamara->Rotar_Longitud( -2.5f * DEGREE_TO_RAD);
            //YawCamera( MiCamara, 3.0f * DEGREE_TO_RAD );
            break;
        case GLUT_KEY_RIGHT:
            MiCamara->Rotar_Longitud(2.5f * DEGREE_TO_RAD);
            //YawCamera( MiCamara, -3.0f * DEGREE_TO_RAD );
            break;
        case GLUT_KEY_HOME: //Reset Camera
            MiCamara->camAtX = 0;
            MiCamara->camAtY = 0;
            MiCamara->camAtZ = 0;
            MiCamara->camViewX = 0;
            MiCamara->camViewY = 1;
            MiCamara->camViewZ = -3;
            MiCamara->SetDependentParametersCamera();
            break;
        default:
            printf("key %d %c X %d Y %d\n", key, key, x, y);
    }
    glutPostRedisplay();
}*/

static void SpecialKey ( int key, int x, int y ){
	switch(key) {
		case GLUT_KEY_F1:
			current_mode = 0;
			glutPassiveMotionFunc(MouseMotion);
			MiCamara->camMovimiento = CAM_STOP;
			current_light = -1;
			break;
		case GLUT_KEY_F2:
			if (current_mode != 0) break;
			current_mode = 1;
			glutPassiveMotionFunc(Examinar);
			MiCamara->camMovimiento = CAM_EXAMINAR;
			break;
		case GLUT_KEY_F3:
			if (current_mode != 0) break;
			current_mode = 2;
			glutPassiveMotionFunc(MouseMotion);
			MiCamara->camMovimiento = CAM_PASEAR;
			MiCamara->camAtY = 0;
			MiCamara->camViewY = 0;
			MiCamara->SetDependentParametersCamera();
			break;
		case GLUT_KEY_F8:
			if (current_mode != 0 && current_mode != 7) break;
			current_mode = 7;
			if (current_light == -1) glutPassiveMotionFunc(Mouse_Luces);
			if (current_light != 2) current_light++;
			else current_light = 0;
			printf("Luz actual = %d\n",current_light);
			break;
		case GLUT_KEY_F9:
			if (current_light != -1) {
				if ( LOCAL_MyLights[current_light]->switched ){
					SwitchLight( LOCAL_MyLights[current_light], FALSE);
				} else {
					SwitchLight( LOCAL_MyLights[current_light], TRUE);
				}
			}
			break;
		case GLUT_KEY_F10:
			if (current_light == 2){
			if ( spot_move == 0 ){
			glutPassiveMotionFunc(Mouse_Spot);
			spot_move = 1;
			}else{
			glutPassiveMotionFunc(Mouse_Luces);
			spot_move = 0;
			}
			}
			break;
		case GLUT_KEY_HOME: //Reset Camera
			MiCamara->camAtX =0;
			MiCamara->camAtY =0;
			MiCamara->camAtZ =0;
			MiCamara->camViewX = 0;
			MiCamara->camViewY = 1;
			MiCamara->camViewZ = -3;
			MiCamara->SetDependentParametersCamera();
			break;
		default:
			printf("key %d %c X %d Y %d\n", key, key, x, y );
		}
		glutPostRedisplay();
}

int main(int argc, char** argv) {
	/*bool loadout = Loader.LoadFile("model.obj");
	if(!loadout){
		printf("Error\n");
		return 0;
	}*/
	glutInit(&argc, argv);
	// Colocamos la cámara en (0,1,-3) mirando hacia (0,0,0)
	MiCamara = new Camara(0.0f, 1.0f, -3.0f);
	//MiCamara = new Camara(0.0f, 1.0f, -3.0f);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("tortuga");
	//glEnable(GL_COLOR_MATERIAL);
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKey);
	//Ratón
	glutMouseFunc(mouse); //Pulsado de Botones
	glutMotionFunc(NULL); //Movimiento con Botones pulsados
	glutPassiveMotionFunc(MouseMotion); //Movimientos sin Botones pulsados
	//Creamos las luces y damos a cada una sus características
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
	glEnable(GL_COLOR_MATERIAL);
	glutMainLoop();
	return 0;
}
