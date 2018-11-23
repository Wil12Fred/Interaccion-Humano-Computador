#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
//#include "Vector_tools.h"

#define AGA_DIRECTIONAL 1
#define AGA_POSITIONAL 2
#define AGA_SPOT 3

typedef struct _Light {
	// The default values that are listed here
	// are those defined by OpenGL
	// Our Default Light has different values
	// (see SetDefaultLight() )
	int type;
	int id; // GL_LIGHTx ; -1 not binded
	int switched; // TRUE => ON
	int needsUpdate; // TRUE / FALSE
	int white; // TRUE / FALSE
	int attenuation; // TRUE / FALSE
	float ambient[4]; // GL_AMBIENT : default (0.0, 0.0, 0.0, 1.0)
	float diffuse[4]; // GL_DIFFUSE : default (0.0, 0.0, 0.0, 1.0)
	// except for light zero (1.0, 1.0, 1.0, 1.0)
	float specular[4]; // GL_SPECULAR : default (0.0, 0.0, 0.0, 1.0)
	// except for light zero (1.0, 1.0, 1.0, 1.0)
	float position[4]; // GL_POSITION : default (0,0,1,0);
	// directional, in the direction of the -z
	float pointAtInfinity[4]; // these values do not refer to the components of
	// one vector they refer to :
	// the coordinates of one point placed in the infinite
	// ( as the point is in the infinite,
	// its 4th homogeneous coordinate must be 0.0 )
	float spotDirection[4]; // GL_SPOT_DIRECTION : default direction is (0,0,-1)
	// significant only when GL_SPOT_CUTOFF is not 180
	float spotExponent; // GL_SPOT_EXPONENT [0,128], default 0
	// 0 => uniform light distribution
	// higher spot => more focused light source,
	float spotCutOff; // GL_SPOT_CUTOFF [0,90] & 180, default 180
	// 180 => uniform light distribution
	float a; // GL_QUADRATIC_ATTENUATION
	float b; // GL_LINEAR_ATTENUATION
	float c; // GL_CONSTANT_ATTENUATION
	// I = I / ( a*delta*delta + b*delta + c )
	// delta : distance from light position to point
	// default a=0 b=0 c=1, no atenuation
} light;

light *CreateDefaultLight();
void DestroyLight( light *thisLight );

void SetLight( light *thisLight );
void SetDefaultLight( light *thisLight );

void SwitchLight( light *thisLight, int status );

void Rotar_Luces_Longitud( light *thisLight, float inc );
void Rotar_Luces_Latitud( light *thisLight, float inc );

void Acercar_Alejar_Luces( light *thisLight, float step );

void Rotar_Spot_Latitud( light *thisLight, float inc );
void Rotar_Spot_Longitud( light *thisLight, float inc );

light *CreateDefaultLight() {
	light *newLight;
	newLight = (light *) malloc( sizeof(light) * 1 );
	SetDefaultLight( newLight );
	return newLight;
}

void SetDefaultLight( light *thisLight ) {
	int ierr = 0;
	float intensity;
	float vx, vy, vz;
	// directional light
	thisLight->type = AGA_DIRECTIONAL;
	thisLight->id = -1;
	thisLight->switched = FALSE;
	thisLight->white = TRUE;
	thisLight->attenuation = FALSE;
	thisLight->needsUpdate = TRUE;
	intensity = 0.0f;
	thisLight->ambient[0] = intensity;
	thisLight->ambient[1] = intensity;
	thisLight->ambient[2] = intensity;
	thisLight->ambient[3] = 1.0f;
	intensity = 0.8f;
	thisLight->diffuse[0] = intensity;
	thisLight->diffuse[1] = intensity;
	thisLight->diffuse[2] = intensity;
	thisLight->diffuse[3] = 1.0f;
	intensity = 0.0f;
	thisLight->specular[0] = intensity;
	thisLight->specular[1] = intensity;
	thisLight->specular[2] = intensity;
	thisLight->specular[3] = 1.0f;
	thisLight->position[0] = 1.0f;
	thisLight->position[1] = 1.0f;
	thisLight->position[2] = 1.0f;
	thisLight->position[3] = 1.0f;
	vx = 1.0f; vy = 1.0f; vz = 1.0f;	
	VectorNormalize( &ierr, &vx, &vy, &vz );
	thisLight->pointAtInfinity[0] = vx;
	thisLight->pointAtInfinity[1] = vy;
	thisLight->pointAtInfinity[2] = vz; // The light is "placed" at point "V" in the infinite
	thisLight->pointAtInfinity[3] = 0.0f; // So light rays flow in the direction of vector "-v"
	vx = -1.0f; vy = -1.0f; vz = -1.0f;
	VectorNormalize( &ierr, &vx, &vy, &vz );
	thisLight->spotDirection[0] = vx;
	thisLight->spotDirection[1] = vy;
	thisLight->spotDirection[2] = vz;
	thisLight->spotDirection[3] = 0.0f;
	thisLight->spotExponent = 10.0f;
	thisLight->spotCutOff = 30.0f; // must be degrees
	thisLight->a = 0.1f; // GL_QUADRATIC_ATTENUATION
	thisLight->b = 0.0f; // GL_LINEAR_ATTENUATION
	thisLight->c = 1.0f; // GL_CONSTANT_ATTENUATION
}

void DestroyLight( light *thisLight ) {
	if( ! thisLight ) return;
	free( thisLight );
	thisLight = NULL;
}

void SwitchLight( light *thisLight, int status ) {
	if( ! thisLight ) return;
	if( thisLight->id < GL_LIGHT0 ) return;
	thisLight->switched = status;
	if( status ) {
		glEnable( thisLight->id );
		thisLight->needsUpdate = TRUE;
	}
	else {
		glDisable( thisLight->id );
	}
}

void SetLight( light *thisLight ) {
	int lightId;
	if( !thisLight ) return;
	if( !thisLight->switched ){
		glDisable( thisLight->id );
		 return;
	}
	glEnable( thisLight->id );
	if( thisLight->id < GL_LIGHT0 ) return;
	lightId = thisLight->id;
	//std::cout << lightId << std::endl;
	// Geometric parameters will be always set when the scene is redrawn
	if( thisLight->type == AGA_DIRECTIONAL ) {
		glLightfv( lightId, GL_POSITION, thisLight->pointAtInfinity );
	} else if( thisLight->type == AGA_POSITIONAL ) {
		glLightfv( lightId, GL_POSITION, thisLight->position );
	} else {
		glLightfv( lightId, GL_POSITION, thisLight->position );
		glLightfv( lightId, GL_SPOT_DIRECTION, thisLight->spotDirection );
	}
		//std::cout << thisLight->position[0] << " " << thisLight->position[1] << " " << thisLight->position[2] << std::endl;
	if( thisLight->needsUpdate ) {
		thisLight->needsUpdate = FALSE;
		glLightfv( lightId, GL_AMBIENT, thisLight->ambient );
		glLightfv( lightId, GL_DIFFUSE, thisLight->diffuse );
		glLightfv( lightId, GL_SPECULAR, thisLight->specular );
		if( thisLight->type == AGA_SPOT ) {
			glLightf( lightId, GL_SPOT_EXPONENT, thisLight->spotExponent );
			glLightf( lightId, GL_SPOT_CUTOFF, thisLight->spotCutOff );
		} else {
			glLighti( lightId, GL_SPOT_EXPONENT, 0 );
			glLighti( lightId, GL_SPOT_CUTOFF, 180 );
		}
		if( ! thisLight->attenuation || thisLight->type == AGA_DIRECTIONAL ) {
			glLighti( lightId, GL_CONSTANT_ATTENUATION, 1 );
			glLighti( lightId, GL_LINEAR_ATTENUATION, 0 );
			glLighti( lightId, GL_QUADRATIC_ATTENUATION, 0 );
		} else {
			glLightf( lightId, GL_CONSTANT_ATTENUATION, thisLight->c );
			glLightf( lightId, GL_LINEAR_ATTENUATION, thisLight->b );
			glLightf( lightId, GL_QUADRATIC_ATTENUATION, thisLight->a );
		}
	}
}

void Rotar_Luces_Longitud( light *thisLight, float inc ) {
	float vIn[3];
	vIn[0]= thisLight->position[0];
	vIn[1]= thisLight->position[1];
	vIn[2]= thisLight->position[2];
	VectorRotY( vIn, inc );
	thisLight->position[0] = vIn[0];
	thisLight->position[2] = vIn[2];
}

void Rotar_Luces_Latitud( light *thisLight, float inc ) {
	float vIn[3];
	vIn[0]= thisLight->position[0];
	vIn[1]= thisLight->position[1];
	vIn[2]= thisLight->position[2];
	VectorRotXZ( vIn, inc, TRUE );
	thisLight->position[0] = vIn[0];
	thisLight->position[1] = vIn[1];
	thisLight->position[2] = vIn[2];
}

void Acercar_Alejar_Luces( light *thisLight, float step ) {
	int ierr;
	float vaX, vaY, vaZ;
	float modulo;
	vaX= thisLight->position[0];
	vaY= thisLight->position[1];
	vaZ= thisLight->position[2];
	VectorNormalize( &ierr, &vaX, &vaY, &vaZ );
	vaX= step * vaX;
	vaY= step * vaY;
	vaZ= step * vaZ;
	// Set new position
	modulo = sqrt(pow(thisLight->position[0] + vaX,2) + pow(thisLight->position[1] + vaY,2) +
					pow(thisLight->position[2] + vaZ,2));
	if(modulo < 0.8f) return;
	thisLight->position[0] += vaX;
	thisLight->position[1] += vaY;
	thisLight->position[2] += vaZ;
}

void Rotar_Spot_Latitud( light *thisLight, float inc ) {
	float vIn[3];
	vIn[0]= thisLight->spotDirection[0];
	vIn[1]= thisLight->spotDirection[1];
	vIn[2]= thisLight->spotDirection[2];
	VectorRotXZ( vIn, inc, TRUE );
	thisLight->spotDirection[0] = vIn[0];
	thisLight->spotDirection[1] = vIn[1];
	thisLight->spotDirection[2] = vIn[2];
}

void Rotar_Spot_Longitud( light *thisLight, float inc ) {
	float vIn[3];
	vIn[0]= thisLight->spotDirection[0];
	vIn[1]= thisLight->spotDirection[1];
	vIn[2]= thisLight->spotDirection[2];
	VectorRotY( vIn, inc );
	thisLight->spotDirection[0] = vIn[0];
	thisLight->spotDirection[2] = vIn[2];
}
