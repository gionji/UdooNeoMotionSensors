#include "UdooNeoMotionSensors.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int Ax, Ay, Az;
int Mx, My, Mz;
int Gx, Gy, Gz;

int isAccelerometerEnabled = 0;
int isMagnetometerEnabled = 0;
int isGyroscopeEnabled = 0;

int iterationSleepMicros = 100000; // 100 milliseconds - 0.1 sec
int numberOfIterations = 0;


void usage(){}

int main(int argc, char *argv[]){

	printf("Program name: %s\n", argv[0]);

	while ((argc > 1) && (argv[1][0] == '-')){
		switch (argv[1][1]){
			case 'a': // accelerometer
				isAccelerometerEnabled = 1;
				break;

			case 'm': // magnetometer
				isMagnetometerEnabled = 1;
				break;
			case 'g': // gyroscope
				isGyroscopeEnabled = 1;
				break;
			case 'f': // frequency
				iterationSleepMicros = (1.0 / atoi( &argv[1][2] ) ) * 1000000;
				printf("period: %d\n", iterationSleepMicros);
				break;
			case 'i': // iterations
				numberOfIterations = atoi( &argv[1][2] );
				printf("iterations: %d\n", numberOfIterations);
				break;
			default:
				printf("Wrong Argument: %s\n", argv[1]);
				usage();
		}

		++argv;
		--argc;
	}

	if(isAccelerometerEnabled + isMagnetometerEnabled + isGyroscopeEnabled == 0){
		printf("Abort: no sensor type selected.\n");
		usage();
		return(0);
	}

	
	if(isAccelerometerEnabled){
		enableAccelerometer();
	}

	if(isMagnetometerEnabled){
		enableMagnetometer();
	}

	if(isGyroscopeEnabled){
		enableGyroscope();
	}
	
	usleep(10000);

	int i;
	VECTOR_3D vect;
	for(i=0; i < numberOfIterations || numberOfIterations == 0 ; ++i){

		if(isAccelerometerEnabled){
			getAccelerometer(&vect);
			printf("Acceleration: %d  %d  %d\n", vect.x, vect.y, vect.z);
		}
	
		if(isMagnetometerEnabled){
			getMagnetometer(&vect);
			printf("Magnetic Field: %d  %d  %d\n", vect.x, vect.y, vect.z);
		}
	
		if(isGyroscopeEnabled){
			getGyroscope(&vect);
			printf("Gyroscope: %d  %d  %d\n", vect.x, vect.y, vect.z);
		}		

		usleep( iterationSleepMicros );
	}

	return 0;
}
