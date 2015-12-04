#include "UdooNeoMotionSensors.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int enableAccelerometer(){
	FILE * acc_enable = fopen("/sys/class/misc/FreescaleAccelerometer/enable", "w");
	fprintf(acc_enable, "%d", 1);
	fclose(acc_enable);
	return 0;
}

int enableMagnetometer(){
	FILE * mag_enable = fopen("/sys/class/misc/FreescaleMagnetometer/enable", "w");
	fprintf(mag_enable, "%d", 1);
	fclose(mag_enable);
	return 0;
}

int enableGyroscope(){
	FILE * gyr_enable = fopen("/sys/class/misc/FreescaleGyroscope/enable", "w");
	fprintf(gyr_enable, "%d", 1);
	fclose(gyr_enable);
	return 0;
}



int getAccelerometer(VECTOR_3D *acc){
	FILE * acc_data = fopen("/sys/class/misc/FreescaleAccelerometer/data", "r");
  
	char Areaded[32];
	char _Ax[8], _Ay[8], _Az[8];
	fgets(Areaded, 32, acc_data);
	sscanf( Areaded, "%[^,],%[^,],%[^,]", _Ax, _Ay, _Az );
	
	fclose(acc_data);
	
	acc->x = atoi(_Ax);
	acc->y = atoi(_Ay);
	acc->z = atoi(_Az);

	return 0;
}

int getMagnetometer(VECTOR_3D *mag){
	FILE * acc_data = fopen("/sys/class/misc/FreescaleMagnetometer/data", "r");
  
	char Areaded[32];
	char _Ax[8], _Ay[8], _Az[8];
	fgets(Areaded, 32, acc_data);
	sscanf( Areaded, "%[^,],%[^,],%[^,]", _Ax, _Ay, _Az );
		
	fclose(acc_data);
	
	mag->x = atoi(_Ax);
	mag->y = atoi(_Ay);
	mag->z = atoi(_Az);
	return 0;
}

int getGyroscope(VECTOR_3D *gyr){
	FILE * acc_data = fopen("/sys/class/misc/FreescaleGyroscope/data", "r");
  
	char Areaded[32];
	char _Ax[8], _Ay[8], _Az[8];
	fgets(Areaded, 32, acc_data);
	sscanf( Areaded, "%[^,],%[^,],%[^,]", _Ax, _Ay, _Az );

	fclose(acc_data);
	
	gyr->x = atoi(_Ax);
	gyr->y = atoi(_Ay);
	gyr->z = atoi(_Az);
	return 0;
}

// Compute orientation based on accelerometer and magnetometer data.
//
// 
// pitch: siedo sull eth - sx da 0 a 90, dx da 0 a -90
//
//
int getOrientation(ORIENTATION* orientation) {
	
	// gestire meglio
	enableAccelerometer();
	enableMagnetometer();

  // Grab an acceleromter and magnetometer reading.
	VECTOR_3D acc;
	VECTOR_3D mag;

  getAccelerometer(&acc);

  getMagnetometer(&mag);

  float const PI_F = 3.14159265F;

  // roll: Rotation around the X-axis. -180 <= roll <= 180                                          
  // a positive roll angle is defined to be a clockwise rotation about the positive X-axis          
  //                                                                                                
  //                    y                                                                           
  //      roll = atan2(---)                                                                         
  //                    z                                                                           
  //                                                                                                
  // where:  y, z are returned value from accelerometer sensor                                      
  orientation->roll = (float)atan2(acc.y, acc.z);

  // pitch: Rotation around the Y-axis. -180 <= roll <= 180                                         
  // a positive pitch angle is defined to be a clockwise rotation about the positive Y-axis         
  //                                                                                                
  //                                 -x                                                             
  //      pitch = atan(-------------------------------)                                             
  //                    y * sin(roll) + z * cos(roll)                                               
  //                                                                                                
  // where:  x, y, z are returned value from accelerometer sensor                                   
  if (acc.y * sin(orientation->roll) + acc.z * cos(orientation->roll) == 0)
    orientation->pitch = acc.x > 0 ? (PI_F / 2) : (-PI_F / 2);
  else
    orientation->pitch = (float)atan(-acc.x / (acc.y * sin(orientation->roll) + acc.z * cos(orientation->roll)));

  // heading: Rotation around the Z-axis. -180 <= roll <= 180                                       
  // a positive heading angle is defined to be a clockwise rotation about the positive Z-axis       
  //                                                                                                
  //                                       z * sin(roll) - y * cos(roll)                            
  //   heading = atan2(--------------------------------------------------------------------------)  
  //                    x * cos(pitch) + y * sin(pitch) * sin(roll) + z * sin(pitch) * cos(roll))   
  //                                                                                                
  // where:  x, y, z are returned value from magnetometer sensor                                    
  orientation->heading = (float)atan2(mag.z * sin(orientation->roll) - mag.y * cos(orientation->roll), \
                                      mag.x * cos(orientation->pitch) + mag.y * sin(orientation->pitch) * sin(orientation->roll) + \
                                      mag.z * sin(orientation->pitch) * cos(orientation->roll));

  // Convert angular data to degree 
  orientation->roll = orientation->roll * 180 / PI_F;
  orientation->pitch = orientation->pitch * 180 / PI_F;
  orientation->heading = orientation->heading * 180 / PI_F;

  return 0;
}

