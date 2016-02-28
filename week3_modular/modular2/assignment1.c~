#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
	sv->bProgramExit = 0;
	sv->drive_state = STOP_WAIT;
	turn = 1;	
}

void init_sensors(SharedVariable* sv) {
	pinMode(LEFT_LED, INPUT);
	pinMode(RIGHT_LED, INPUT);
	pinMode(FRONT_LED, INPUT);
/*	pinMode(US_1_ECHO, INPUT);
	pinMode(US_1_TRIG, OUTPUT);
	pinMode(US_2_ECHO, INPUT);
	pinMode(US_2_TRIG, OUTPUT);
	pinMode(US_3_ECHO, INPUT);
	pinMode(US_3_TRIG, OUTPUT);
	pinMode(US_4_ECHO, INPUT);
	pinMode(US_4_TRIG, OUTPUT);
	pinMode(US_5_ECHO, INPUT);
	pinMode(US_5_TRIG, OUTPUT);
*/
}
void setup_US(int TRIG, int ECHO){
	pinMode(TRIG, INPUT);
        pinMode(ECHO, OUTPUT);
        digitalWrite(ECHO, LOW);

        //TRIG pin must start LOW
        delay(30);
        pinMode(TRIG, OUTPUT);
        pinMode(ECHO, INPUT);
 
        //TRIG pin must start LOW
        digitalWrite(TRIG, LOW);
        delay(30);
}
int getCM(int TRIG, int ECHO) {
        //Send trig pulse
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(20);
        digitalWrite(TRIG, LOW);
 
        //Wait for echo start
        while(digitalRead(ECHO) == LOW);
 
        //Wait for echo end
        long startTime = micros();
        while(digitalRead(ECHO) == HIGH);
        long travelTime = micros() - startTime;
 
        //Get distance in cm
        int distance = travelTime / 58;
 
        return distance;
}
void body_ultrasound(SharedVariable* sv) {
	int dist_2,dist_3,dist_4;
	while(1){
	//setup(US_1_TRIG,US_1_ECHO);
	setup(US_2_TRIG,US_2_ECHO);
	setup(US_3_TRIG,US_3_ECHO);
	setup(US_4_TRIG,US_4_ECHO);
	//setup(US_5_TRIG,US_5_ECHO);

	dist_2 = getCM(US_2_TRIG,US_2_ECHO);
	dist_3 = getCM(US_3_TRIG,US_3_ECHO);
	dist_4 = getCM(US_4_TRIG,US_4_ECHO); 
	if((dist_2>5 && dist_2<30)||(dist_3>5 && dist_3<30)||(dist_4>5 && dist_4<30))
	{
		sv->drive_state = OBSTACLE_DETECTED;	
	}	
	}
}

void body_irled(SharedVariable* sv) {
	while(1){
	if(!digitalRead(LEFT_LED))
		sv->left_led = 1;
	else
		sv->left_led = 0;
	if(!digitalRead(RIGHT_LED))
		sv->right_led = 1;
	else
		sv->right_led = 0;
	if(!digitalRead(FRONT_LED))
		sv->front_led = 1;
	else
		sv->front_led = 0;
	}
}


void body_linefollow(SharedVariable* sv){
	while(!sv->init_start);
	while(1)
	{
	  if((!sv->front_led))
		sv->drive_state = DRIVE_FORWARD;
	   
	  else{	
		if(turn == 1)
		sv->drive_state = TURN_RIGHT;
		else if(turn == 2)
		sv->drive_state = TURN_LEFT;
		
		//turn = 0;		
		}		  	
	  switch(sv->drive_state){
		case DRIVE_FORWARD:
			drive_forward(sv);
			break;
		case TURN_RIGHT:	
			turn_right(sv);
			break;
		case TURN_LEFT:	
			turn_left(sv);
			break;
		case OBSTACLE_DETECTED:	
			lane_change(sv);
			break;
		case STOP_WAIT:
			stop();

		default : break;	
		  }	
	}
}

void drive_forward(SharedVariable* sv)
{
	if(!sv->right_led)
	{
		right();
		stop();
		while(!sv->right_led)
		forward();
		stop();
		left();
	}	
	while(sv->drive_state == DRIVE_FORWARD)
		forward();
			
}
void turn_right(SharedVariable* sv)
{
		
}
void turn_left(SharedVariable* sv)
{}
void lane_change(SharedVariable* sv)
{}
