#include <stdio.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "create.h"

#define FRONT 2
#define LEFT  1
#define RIGHT 3

void body_irled(SharedVariable* sv)
{

if (wiringPiSetup () == -1)
exit (1) ;

if(digitalRead(FRONT))
{	
	printf("Front sensor detected lane\n");
}
if(digitalRead(LEFT))
{	
	printf("Left sensor detected lane\n");
}
if(digitalRead(RIGHT))
{	
	printf("Right sensor detected lane\n");
}
return 0 ;
}
