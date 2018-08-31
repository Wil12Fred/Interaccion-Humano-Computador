//
// Archivo:     micubo.c
// Autor:       Mateo Díaz
// Creado:      4/25/2012
// Proyecto:    código fuente para hacer un cubo en OpenGL
// Descripción: crear una ventana OpenGL y dibujar un cubo 3D
//                que el usuario pueda rotar con las teclas de flecha 
// 
// Controles:    Flecha izquierda: rotar a la izquierda
//               Flecha derecha: rotar a la derecha  
//               Flecha hacia arriba: rotar hacia arriba    
//               Flecha hacia abajo: rotar hacia abajo    
 
// ----------------------------------------------------------
// Librerías
// ----------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
 
// ----------------------------------------------------------
// Prototipos de función
// ----------------------------------------------------------
void display();
void specialKeys();
 
// ----------------------------------------------------------
// Variables globales
// ----------------------------------------------------------
double rotate_y=0; 
double rotate_x=0;
 
// ----------------------------------------------------------
// Función de retrollamada “display()”
// ----------------------------------------------------------
void display(){
 
  //  Borrar pantalla y Z-buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
  // Resetear transformaciones
  glLoadIdentity();
 
  // Otras transformaciones
  // glTranslatef( 0.1, 0.0, 0.0 );      // No incluido
  // glRotatef( 180, 0.0, 1.0, 0.0 );    // No incluido
 
  // Rotar cuando el usuario cambie “rotate_x” y “rotate_y”
  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  glRotatef( rotate_y, 0.0, 1.0, 0.0 );
 
  // Otras transformaciones
  // glScalef( 2.0, 2.0, 0.0 );          // No incluido
 
  //LADO FRONTAL: lado multicolor
  glBegin(GL_POLYGON);
 
  glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 es rojo
  glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 es verde
  glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 es azul
  glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 es morado
 
  glEnd();
 
  // LADO TRASERO: lado blanco
  glBegin(GL_POLYGON);
  glColor3f(   1.0,  1.0, 1.0 );
  glVertex3f(  0.5, -0.5, 0.5 );
  glVertex3f(  0.5,  0.5, 0.5 );
  glVertex3f( -0.5,  0.5, 0.5 );
  glVertex3f( -0.5, -0.5, 0.5 );
  glEnd();
 
  // LADO DERECHO: lado morado
  glBegin(GL_POLYGON);
  glColor3f(  1.0,  0.0,  1.0 );
  glVertex3f( 0.5, -0.5, -0.5 );
  glVertex3f( 0.5,  0.5, -0.5 );
  glVertex3f( 0.5,  0.5,  0.5 );
  glVertex3f( 0.5, -0.5,  0.5 );
  glEnd();
 
  // LADO IZQUIERDO: lado verde
  glBegin(GL_POLYGON);
  glColor3f(   0.0,  1.0,  0.0 );
  glVertex3f( -0.5, -0.5,  0.5 );
  glVertex3f( -0.5,  0.5,  0.5 );
  glVertex3f( -0.5,  0.5, -0.5 );
  glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();
 
  // LADO SUPERIOR: lado azul
  glBegin(GL_POLYGON);
  glColor3f(   0.0,  0.0,  1.0 );
  glVertex3f(  0.5,  0.5,  0.5 );
  glVertex3f(  0.5,  0.5, -0.5 );
  glVertex3f( -0.5,  0.5, -0.5 );
  glVertex3f( -0.5,  0.5,  0.5 );
  glEnd();
 
  // LADO INFERIOR: lado rojo
  glBegin(GL_POLYGON);
  glColor3f(   1.0,  0.0,  0.0 );
  glVertex3f(  0.5, -0.5, -0.5 );
  glVertex3f(  0.5, -0.5,  0.5 );
  glVertex3f( -0.5, -0.5,  0.5 );
  glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();
 
  glFlush();
  glutSwapBuffers();
 
}
 
// ----------------------------------------------------------
// Función de retrollamada “specialKeys()” 
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {
 
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
  glutPostRedisplay();
 
}
 
// ----------------------------------------------------------
// Función “main()”
// ----------------------------------------------------------
int main(int argc, char* argv[]){
 
  //  Inicializar los parámetros GLUT y de usuario proceso
  glutInit(&argc,argv);
 
  //  Solicitar ventana con color real y doble buffer con Z-buffer 
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
  // Crear ventana
  glutCreateWindow("Cubo asombroso");
 
  //  Habilitar la prueba de profundidad de Z-buffer
  glEnable(GL_DEPTH_TEST);
 
  // Funciones de retrollamada
  glutDisplayFunc(display);
  glutSpecialFunc(specialKeys);
 
  //  Pasar el control de eventos a GLUT
  glutMainLoop();
 
  //  Regresar al sistema operativo
  return 0;
 
}
