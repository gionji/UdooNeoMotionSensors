#include "UdooNeoMotionSensors.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


int main(){

	ORIENTATION ori;

printf("bau");

	while(1){
		getOrientation(&ori);

		printf("ori %f %f %f \n", ori.heading, ori.pitch, ori.roll);
		usleep(50000);
	}

	return 0;

}
