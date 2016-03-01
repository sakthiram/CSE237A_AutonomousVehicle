#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#define IR_LEFT 27
#define IR_CENTER 28
#define IR_RIGHT 29

int main (void)
{
struct timeval tv,tve;
struct timezone tz;
int flag=0;

if (wiringPiSetup () == -1)
exit(1);

pinMode(IR_LEFT, INPUT) ;
pinMode(IR_CENTER, INPUT) ;
pinMode(IR_RIGHT, INPUT) ;

for(;;) {

if (digitalRead(IR_RIGHT) == 0) 
{
	usleep(1000); //1 ms
	printf("Read 0 value from IR sensor");
}

//delay(100);
}

return 0;
}
