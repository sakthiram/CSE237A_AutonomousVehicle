#include <stdio.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ECHOPIN 0
#define TRIGPIN 1

int main (void)
{
struct timeval tv,tve;
struct timezone tz;

printf (“RasPi Distance with HC-SR04 \n”) ;

if (wiringPiSetup () == -1)
exit (1) ;

pinMode (ECHOPIN, INPUT) ;
pinMode (TRIGPIN, OUTPUT) ;

digitalWrite (TRIGPIN,0) ; //LOW
delayMicroseconds(2);
digitalWrite (TRIGPIN,1) ; //HIGH
delayMicroseconds(10);
digitalWrite (TRIGPIN,0) ; //LOW
for(;;) {
if (digitalRead (ECHOPIN) == 1) {
gettimeofday(&tv,&tz);
break;
}
}
for(;;) {
if (digitalRead (ECHOPIN) == 0) {
gettimeofday(&tve,&tz);
break;
}
}

float usec;
usec = tve.tv_usec – tv.tv_usec;
usec = usec/58; //Distance = ((Duration of high level)*(Sonic :340m/s))/2
printf(“%f\n”,usec);
return 0 ;
}
