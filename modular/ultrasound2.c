#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include "create.h"

#define ECHOPIN0 0
#define TRIGPIN0 1
#define ECHOPIN1 0
#define ECHOPIN2 0
#define ECHOPIN3 0
#define ECHOPIN4 0
#define TRIGPIN1 1
#define TRIGPIN2 1
#define TRIGPIN3 1
#define TRIGPIN4 1

void init_ultrasound(SharedVariable* sv) {
	// You can initialize the shared variable if needed.
	sv->bProgramExit = 0;
	//code here
}

void body_ultrasound(SharedVariable* sv)
{
struct timeval tv,tve;
struct timezone tz;

float usec;
//printf(“RasPi Distance with HC-SR04 \n”);

if(wiringPiSetup() == -1)
exit(1);

pinMode(ECHOPIN0, INPUT) ;
pinMode(TRIGPIN0, OUTPUT) ;
pinMode(ECHOPIN1, INPUT) ;
pinMode(ECHOPIN2, INPUT) ;
pinMode(ECHOPIN3, INPUT) ;
pinMode(ECHOPIN4, INPUT) ;
pinMode(TRIGPIN1, OUTPUT) ;
pinMode(TRIGPIN2, OUTPUT) ;
pinMode(TRIGPIN3, OUTPUT) ;
pinMode(TRIGPIN4, OUTPUT) ;

digitalWrite (TRIGPIN0,0) ; //LOW
delayMicroseconds(2);
digitalWrite (TRIGPIN0,1) ; //HIGH
delayMicroseconds(10);
digitalWrite (TRIGPIN0,0) ; //LOW

for(;;) {
if (digitalRead (ECHOPIN0) == 1) {
gettimeofday(&tv,&tz);
break;
}
}

for(;;) {
if (digitalRead (ECHOPIN0) == 0) {
gettimeofday(&tve,&tz);
break;
}
}
usec = tve.tv_usec - tv.tv_usec;
usec = usec/58;
printf("%f\n",usec);

digitalWrite (TRIGPIN1,0) ; //LOW
delayMicroseconds(2);
digitalWrite (TRIGPIN1,1) ; //HIGH
delayMicroseconds(10);
digitalWrite (TRIGPIN1,0) ; //LOW

for(;;) {
if (digitalRead (ECHOPIN1) == 1) {
gettimeofday(&tv,&tz);
break;
}
}

for(;;) {
if (digitalRead (ECHOPIN1) == 0) {
gettimeofday(&tve,&tz);
break;
}
}
usec = tve.tv_usec - tv.tv_usec;
usec = usec/58;
printf("%f\n",usec);


digitalWrite (TRIGPIN2,0) ; //LOW
delayMicroseconds(2);
digitalWrite (TRIGPIN2,1) ; //HIGH
delayMicroseconds(10);
digitalWrite (TRIGPIN2,0) ; //LOW

for(;;) {
if (digitalRead (ECHOPIN2) == 1) {
gettimeofday(&tv,&tz);
break;
}
}

for(;;) {
if (digitalRead (ECHOPIN2) == 0) {
gettimeofday(&tve,&tz);
break;
}
}
usec = tve.tv_usec - tv.tv_usec;
usec = usec/58;
printf("%f\n",usec);


digitalWrite (TRIGPIN3,0) ; //LOW
delayMicroseconds(2);
digitalWrite (TRIGPIN3,1) ; //HIGH
delayMicroseconds(10);
digitalWrite (TRIGPIN3,0) ; //LOW

for(;;) {
if (digitalRead (ECHOPIN3) == 1) {
gettimeofday(&tv,&tz);
break;
}
}

for(;;) {
if (digitalRead (ECHOPIN3) == 0) {
gettimeofday(&tve,&tz);
break;
}
}
usec = tve.tv_usec - tv.tv_usec;
usec = usec/58;
printf("%f\n",usec);


digitalWrite (TRIGPIN4,0) ; //LOW
delayMicroseconds(2);
digitalWrite (TRIGPIN4,1) ; //HIGH
delayMicroseconds(10);
digitalWrite (TRIGPIN4,0) ; //LOW

for(;;) {
if (digitalRead (ECHOPIN4) == 1) {
gettimeofday(&tv,&tz);
break;
}
}

for(;;) {
if (digitalRead (ECHOPIN4) == 0) {
gettimeofday(&tve,&tz);
break;
}
}
usec = tve.tv_usec - tv.tv_usec;
usec = usec/58;
printf("%f\n",usec);


return 0;
}
