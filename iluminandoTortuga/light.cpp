#include <GLUT/glut.h>
#include <stdlib.h>
#include "light.h"
#include "Vector_tools.h"

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
	//
	intensity = 0.0f;
	thisLight->ambient[0] = intensity;
	thisLight->ambient[1] = intensity;
	thisLight->ambient[2] = intensity;
	thisLight->ambient[3] = 1.0f;
	//
	intensity = 0.8f;
	thisLight->diffuse[0] = intensity;
	thisLight->diffuse[1] = intensity;
	thisLight->diffuse[2] = intensity;
	thisLight->diffuse[3] = 1.0f;
	//
	intensity = 0.0f;
	thisLight->specular[0] = intensity;
	thisLight->specular[1] = intensity;
	thisLight->specular[2] = intensity;
	thisLight->specular[3] = 1.0f;
	//
	thisLight->position[0] = 1.0f;
	thisLight->position[1] = 1.0f;
	thisLight->position[2] = 1.0f;
	thisLight->position[3] = 1.0f;
	//
	vx = 1.0f; vy = 1.0f; vz = 1.0f;	
	VectorNormalize( &ierr, &vx, &vy, &vz );
	thisLight->pointAtInfinity[0] = vx;
	thisLight->pointAtInfinity[1] = vy;
	thisLight->pointAtInfinity[2] = vz; // The light is "placed" at point "V" in the infinite
	thisLight->pointAtInfinity[3] = 0.0f; // So light rays flow in the direction of vector "-v"
	//
	vx = -1.0f; vy = -1.0f; vz = -1.0f;
	VectorNormalize( &ierr, &vx, &vy, &vz );
	thisLight->spotDirection[0] = vx;
	thisLight->spotDirection[1] = vy;
	thisLight->spotDirection[2] = vz;
	thisLight->spotDirection[3] = 0.0f;
	//
	thisLight->spotExponent = 10.0f;
	thisLight->spotCutOff = 30.0f; // must be degrees
	//
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
	if( !thisLight->switched ) return;
	if( thisLight->id < GL_LIGHT0 ) return;
	lightId = thisLight->id;
	// Geometric parameters will be always set when the scene is redrawn
	if( thisLight->type == AGA_DIRECTIONAL ) {
		glLightfv( lightId, GL_POSITION, thisLight->pointAtInfinity );
	}
	else if( thisLight->type == AGA_POSITIONAL ) {
		glLightfv( lightId, GL_POSITION, thisLight->position );
	}
	else {
		glLightfv( lightId, GL_POSITION, thisLight->position );
		glLightfv( lightId, GL_SPOT_DIRECTION, thisLight->spotDirection );
	}
	//
	if( thisLight->needsUpdate ) {
		thisLight->needsUpdate = FALSE;
		glLightfv( lightId, GL_AMBIENT, thisLight->ambient );
		glLightfv( lightId, GL_DIFFUSE, thisLight->diffuse );
		glLightfv( lightId, GL_SPECULAR, thisLight->specular );
		//
		if( thisLight->type == AGA_SPOT ) {
			glLightf( lightId, GL_SPOT_EXPONENT, thisLight->spotExponent );
			glLightf( lightId, GL_SPOT_CUTOFF, thisLight->spotCutOff );
		}
		else {
			glLighti( lightId, GL_SPOT_EXPONENT, 0 );
			glLighti( lightId, GL_SPOT_CUTOFF, 180 );
		}
		if( ! thisLight->attenuation || thisLight->type == AGA_DIRECTIONAL ) {
			glLighti( lightId, GL_CONSTANT_ATTENUATION, 1 );
			glLighti( lightId, GL_LINEAR_ATTENUATION, 0 );
			glLighti( lightId, GL_QUADRATIC_ATTENUATION, 0 );
		}
		else {
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
	//
	VectorRotY( vIn, inc );
	//
	thisLight->position[0] = vIn[0];
	thisLight->position[2] = vIn[2];
}

void Rotar_Luces_Latitud( light *thisLight, float inc ) {
	float vIn[3];
	vIn[0]= thisLight->position[0];
	vIn[1]= thisLight->position[1];
	vIn[2]= thisLight->position[2];
	//
	VectorRotXZ( vIn, inc, TRUE );
	//
	thisLight->position[0] = vIn[0];
	thisLight->position[1] = vIn[1];
	thisLight->position[2] = vIn[2];
}

void Acercar_Alejar_Luces( light *thisLight, float step ) {
	int ierr;
	float vaX, vaY, vaZ;
	float modulo;
	//
	vaX= thisLight->position[0];
	vaY= thisLight->position[1];
	vaZ= thisLight->position[2];
	//
	VectorNormalize( &ierr, &vaX, &vaY, &vaZ );
	//
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
	//
	VectorRotXZ( vIn, inc, TRUE );
	//
	thisLight->spotDirection[0] = vIn[0];
	thisLight->spotDirection[1] = vIn[1];
	thisLight->spotDirection[2] = vIn[2];
}

void Rotar_Spot_Longitud( light *thisLight, float inc ) {
	float vIn[3];
	vIn[0]= thisLight->spotDirection[0];
	vIn[1]= thisLight->spotDirection[1];
	vIn[2]= thisLight->spotDirection[2];
	//
	VectorRotY( vIn, inc );
	//
	thisLight->spotDirection[0] = vIn[0];
	thisLight->spotDirection[2] = vIn[2];
}
