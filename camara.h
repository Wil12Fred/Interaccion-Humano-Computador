#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//#include "Vector_tools.h"

#define CAM_PARALLEL 1
#define CAM_CONIC 2

#define CAM_STOP 0
#define CAM_EXAMINAR 1
#define CAM_PASEAR 2
#define CAM_TRIPODE 3
#define CAM_PAN 4

class Camara {
public:
    // we consider a rigth handed reference system for the camera
    // V point where the camera is placed (world coordinates)
    // A point the camera is looking at   (world coordinates)
    // up vector : unit vector, perpendicular to AV (world componnents)
    // origin camera reference system : at V
    // Z camera : defined by vector from A to V (penetrates the viewer's eye)
    // Y camera : defined by up vector
    // X camera : looking from V towards A goes rigthwards
	objl::Vector3 camView;
	objl::Vector3 camAt;
	objl::Vector3 camUp;
    float camAperture; // field of view radians
    // NOTE : OpenGL uses degrees
    // defined as they are used by OpenGL
    // always => positive ; Far > Near (distance from plane to camera origin)
    float camNear;
    float camFar;
    int camProjection; // PARALLEL or CONIC
    int camMovimiento; // EXAMINAR, ANDAR, TRIPODE or PAN	
    // ****** dependent values ******
    // window system dependent
    float aspectRatio;
	float rotate_c;
    // for ortho projection
    float x1, x2, y1, y2, z1, z2;
    // camera i j k vectors in world coordinates
	objl::Vector3 camI;
	objl::Vector3 camJ;
	objl::Vector3 camK;
    Camara();
    Camara(float positionX, float positionY, float positionZ);
    Camara(const Camara& orig);
    virtual ~Camara();
    void SetCamera(
            float viewX, float viewY, float viewZ,
            float atX, float atY, float atZ,
            float upX, float upY, float upZ);
    void SetDependentParametersCamera();
    void SetGLCamera();
    void SetGLAspectRatioCamera();
    // step admits negative values
    // Free camera advances "step" following vector VA
    void AvanceFreeCamera( float step);
    // ROTATION
    void YawCamera( float angle); // local axis Y camera
    void Rotar_Latitud( float inc);
    void Rotar_Longitud(float inc);
	objl::Vector3 getPos(objl::Vector3 point);
	objl::Vector3 getLookAt();
	void ZoomIn();
	void ZoomOut();
};

Camara::Camara() {
	camAt=objl::Vector3(0,0,0);
	camUp=objl::Vector3(0,1,0);
    camProjection = CAM_CONIC;
    aspectRatio = 1.0f;
	rotate_c=0;
}

Camara::Camara(float positionX, float positionY, float positionZ) {
    int ierr = 0;
	camView=objl::Vector3(positionX,positionY,positionZ);
	camAt=objl::Vector3(0,0,0);
	camUp=objl::Vector3(0,1,0);
    camAperture = 60.0f * DEGREE_TO_RAD;
    camNear = 0.5f;
    camFar = 200.0f;
    camProjection = CAM_CONIC;
    aspectRatio = 1.0f;
    SetDependentParametersCamera();
	rotate_c=0;
}

void Camara::SetDependentParametersCamera() {
    // camera i j k vectors in world coordinates
    // camIX, camIY, camIZ;
    // camJX, camJY, camJZ;
    // camKX, camKY, camKZ;
    float ix, iy, iz;
    float jx, jy, jz;
    float kx, ky, kz;
    float atX, atY, atZ;
    float upX, upY, upZ;
    float viewX, viewY, viewZ;
    int ierr = 0;
	viewX = camView.X;
	viewY = camView.Y;
	viewZ = camView.Z;
	atX = camAt.X;
	atY = camAt.Y;
	atZ = camAt.Z;
    upX = camUp.X;//0.0f;
    upY = camUp.Y;//1.0f;
    upZ = camUp.Z;//0.0f;
    // i, j, k, up	must be unit vectors
    // k = normalizar(  AV  )
    // i = normalizar( up ^ k )
    // j = k ^ i
    UnitVectorPP(&ierr, &kx, &ky, &kz,
            atX, atY, atZ,
            viewX, viewY, viewZ);
    UnitVectorVV(&ierr, &ix, &iy, &iz,
            upX, upY, upZ,
            kx, ky, kz);
    UnitVectorVV(&ierr, &jx, &jy, &jz,
            kx, ky, kz,
            ix, iy, iz);
	camK=objl::Vector3(kx,ky,kz);
	camI=objl::Vector3(ix,iy,iz);
	camJ=objl::Vector3(jx,jy,jz);
	//std::cout << "up-> " << jx << " " << jy << " " << jz << std::endl;
	camUp=camJ;//objl::Vector3(jx, jy, jz);
}

void Camara::SetGLCamera() {
    float fovy;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (camProjection == CAM_CONIC) {
        fovy = camAperture * RAD_TO_DEGREE;
        gluPerspective(fovy, aspectRatio, camNear, camFar);
    } else { // CAM_PARALLEL		
        glOrtho(x1, x2, y1, y2, z1, z2);
    }
	objl::Vector3 lookAt=this->getLookAt();
    gluLookAt(camView.X, camView.Y, camView.Z,
            lookAt.X, lookAt.Y, lookAt.Z,
            //camAt.X, camAt.Y, camAt.Z,
		0,1,0);
            //camUp.X, camUp.Y, camUp.Z);
	//std::cout << camUp.X << " " << camUp.Y << " " << camUp.Z << std::endl;
    glMatrixMode(GL_MODELVIEW); // *** GL_MODELVIEW ***
}

void Camara::SetCamera(
        float viewX, float viewY, float viewZ,//0, 300, 250
        float atX, float atY, float atZ,// 0, -300, -250
        float upX, float upY, float upZ) {//0 1 0
	camView=objl::Vector3(viewX,viewY,viewZ);
	camAt=objl::Vector3(atX,atY,atZ);
	camUp=objl::Vector3(upX,upY,upZ);
    SetDependentParametersCamera();
}

void Camara::AvanceFreeCamera(float step) {
    float vaX, vaY, vaZ;
    vaX = step * camK.X;
    vaY = step * camK.Y;
    vaZ = step * camK.Z;
    // Set V & A
    camView.X = camView.X + vaX;
    camView.Y = camView.Y + vaY;
    camView.Z = camView.Z + vaZ;
    camAt.X = camAt.X + vaX;
    camAt.Y = camAt.Y + vaY;
    camAt.Z = camAt.Z + vaZ;
    SetDependentParametersCamera();
}

void Camara::YawCamera(float angle) {
    float vIn[3];
    vIn[0] = camAt.X - camView.X;
    vIn[1] = camAt.Y - camView.Y;
    vIn[2] = camAt.Z - camView.Z;
    VectorRotY(vIn, angle);
    camAt.X = camView.X + vIn[0];
    camAt.Y = camView.Y + vIn[1];
    camAt.Z = camView.Z + vIn[2];
    SetDependentParametersCamera();
}

void Camara::Rotar_Longitud(float inc) {
    float vIn[3];
    vIn[0] = camView.X - camAt.X;
    vIn[1] = camView.Y - camAt.Y;
    vIn[2] = camView.Z - camAt.Z;
    VectorRotY(vIn, inc);
    camView.X = camAt.X + vIn[0];
    camView.Z = camAt.Z + vIn[2];
    SetDependentParametersCamera();
}

void Camara::Rotar_Latitud( float inc) {
    float vIn[3];
    vIn[0] = camView.X - camAt.X;
    vIn[1] = camView.Y - camAt.Y;
    vIn[2] = camView.Z - camAt.Z;
    VectorRotXZ(vIn, inc, TRUE);
    camView.X = camAt.X + vIn[0];
    camView.Y = camAt.Y + vIn[1];
    camView.Z = camAt.Z + vIn[2];
    SetDependentParametersCamera();
}

void Camara::SetGLAspectRatioCamera() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    if (viewport[3] > 0)
        aspectRatio = (float) viewport[2] / (float) viewport[3]; // width/height
    else
        aspectRatio = 1.0f;
    SetDependentParametersCamera();
}

objl::Vector3 Camara::getPos(objl::Vector3 point){//R
	objl::Vector3 Center=camView;
	Center.Y=point.Y;
	double r=(point-Center).norm();
	return getCircleCoordinate(Center, point,
		r,rotate_c);
}

objl::Vector3 Camara::getLookAt(){//R
	objl::Vector3 toLook=camView+camAt;
	return getPos(toLook);
}

void Camara::ZoomIn(){//R
        objl::Vector3 toLook=camAt+camView;//getLookAt();         
        double zoom=3.0/4;                               
        camAt=camAt*zoom;     
        camView=toLook-camAt; 
    SetDependentParametersCamera();
}

void Camara::ZoomOut(){//R
        objl::Vector3 toLook=camAt+camView;//getLookAt();       
        double zoom=4.0/3;                             
        camAt=camAt*zoom;   
        camView=toLook-camAt;
    SetDependentParametersCamera();
}

Camara::~Camara() {//R
}
