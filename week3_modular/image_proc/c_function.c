#include <stdio.h>
#include <wiringPi.h>

#define COLOR_DET0 0
#define COLOR_DET1 1

int colour_detected = 0;
int ObjectDetect();

void main() {

	ObjectDetect();
	
	if (wiringPiSetup () == -1) 
	exit (1);	

	pinMode (COLOR_DET0, OUTPUT);
	pinMode (COLOR_DET1, OUTPUT);

	while(1) {
		digitalWrite(COLOR_DET0, colour_detected%2);	
		digitalWrite(COLOR_DET1, colour_detected/2);
		usleep(1000);

		if (colour_detected != 0)
		printf("colour_detected is %d", colour_detected);	
	}

}
