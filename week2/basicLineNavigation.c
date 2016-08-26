#include<stdio.h>
#include<wiringPi.h>

#define FRONT 1
#define LEFT 1
#define RIGHT 1
static int state = 1;
 
int main(){
while(1){
switch(state)
{
case 1:
	if(digitalRead(FRONT)==1)
	{	goUntilLROn();
		turnDirection(right);
	}
	if(digitalRead(RIGHT)==1)
	{
		driveForward();
	}
	else 
		turnRight(10);
default : break;
}
}
}

void goUntilLROn()
{
	
}

void turnDirection(){}

void turnRight(){}
void driveForward(){}

