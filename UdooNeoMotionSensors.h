#ifndef __UDOONEO_MOTION_SENSORS_H__
#define __UDOONEO_MOTION_SENSORS_H__


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct vector3D{
	float x;
	float y;
	float z;
} VECTOR_3D;


typedef struct Orientation{
	float heading;
	float pitch;
	float roll;
} ORIENTATION;


int enableAccelerometer(void);
int enableMagnetometer(void);
int enableGyroscope(void);

int setAccelerometerRange(int range);
int setMagnetometerRange(int range);
int setGyroscopeRange(int range);

int setAccelerometerFreq(int freq);
int setMagnetometerFreq(int freq);
int setGyroscopeFreq(int freq);

int getAccelerometer(VECTOR_3D *acc);
int getMagnetometer(VECTOR_3D *mag);
int getGyroscope(VECTOR_3D *gyr);

int getOrientation(ORIENTATION *orientation);

#endif
