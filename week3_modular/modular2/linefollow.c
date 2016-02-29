#include "create.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
	sv->bProgramExit = 0;
	sv->drive_state = STOP_WAIT;
	sv->init_start = 0;
	sv->manual_stop = 0;
	sv->current_direction = STOP;
	sv->obstacle_detected = 0;
	sv->next_lane_obstacle_detected =0;
	sv->current_lane = 1;
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
	setup_US(US_2_TRIG,US_2_ECHO);
	setup_US(US_3_TRIG,US_3_ECHO);
	setup_US(US_4_TRIG,US_4_ECHO);
	//setup(US_5_TRIG,US_5_ECHO);

	dist_2 = getCM(US_2_TRIG,US_2_ECHO);
	dist_3 = getCM(US_3_TRIG,US_3_ECHO);
	dist_4 = getCM(US_4_TRIG,US_4_ECHO); 
	if((dist_3>2 && dist_3<30))
	{
		printf("Obstacle detected\n");
		sv->obstacle_detected = 1;	
	
	if(sv->current_lane == 0 && (dist_4>25 && dist_4<50))
		{sv->next_lane_obstacle_detected = 1;
			printf("rightlane obstacle\n");
		}
	if(sv->current_lane == 1 && (dist_2>25 && dist_2<50))
	{	sv->next_lane_obstacle_detected = 1;
			
			printf("leftlane obstacle\n");
	}
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
			int rotated_left = 0;
			int rotated_right = 1;
	while(!sv->init_start);
	printf("After Init_start\n");
	while(!sv->manual_stop)
	{	
	//	sv->drive_state = DRIVE_FORWARD;
	 
	 if (sv->obstacle_detected == 1 && sv->next_lane_obstacle_detected == 0) {
		sv->drive_state = OBSTACLE_DETECTED;	
	 } else if (sv->obstacle_detected == 1 && sv->next_lane_obstacle_detected == 1) {
		sv->drive_state = STOP_WAIT;	
	 } else if((!sv->front_led)){
	        sv->drive_state = DRIVE_FORWARD;
	        }
	  else{	
	        if(turn == 1){
	        sv->drive_state = TURN_RIGHT;
	        printf("turn right set\n");
	        stop(sv);
	        }
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
			stop(sv);
			break;

		default : break;	
		  }	
	}

	printf("Manual Stop is %d\n",sv->manual_stop);
	stop(sv);

}

void body_keypress(SharedVariable* sv)
{
	int x = ' ';
	int y = ' ';
 	int z = ' ';

// Initialize Irobot
 	create_init(sv);
	printf("press aaa\n");
 while (!(x == 'a' && y == 'a' && z == 'a')) { // 3 times ESC
	sv->init_start = 0;
	
	x = getch();
	y = getch();
	z = getch(); 
	}
	printf("pressed aaa\n");

 sv->init_start =1;

  while (1) {
	x = getch();
	y = getch();
	z = getch(); 

	if (x == 27 && y == 27 && z == 27) {
	sv->manual_stop = 1;
	break;
	}

  }	
	stop(sv);	
	printf("pressed e e e \n");

}

void drive_forward(SharedVariable* sv)
{
	if(!sv->right_led && !sv->left_led)
	{
		//printf("Forward");
		forward(sv);
		//delay(1000);	
	}	
	if(sv->right_led) //right is off implies both scenarios where left and right both are off
	{
		
	//	while (sv->right_led){		
			left(sv);
			delay(200);
			forward(sv);
			delay(500);
	//	}
		right(sv);
		delay(150);
	}
	if(sv->left_led) 
	{
	//	while(sv->left_led){		
			right(sv);
			delay(200);
			forward(sv);
			delay(500);
	//	}
		left(sv);
		delay(150);
			
	}				
}

void turn_right(SharedVariable* sv)
{
		
	printf("Eneterd Turn Right");
	int left_flag = 0; 
	int right_flag = 0; 
		if(sv->left_led == 0)
			left_flag = 1;
		if (sv->right_led == 0) 
			right_flag = 1;

		if (left_flag == 1 && right_flag == 1)
			while (sv->left_led == 1 || sv->right_led == 1) 
				forward();
		else if (left_flag == 1){
		while (sv->left_led == 1)
			forward(sv);
		} else if (right_flag == 1) {
		while (sv->right_led == 1)
			forward(sv);
		}

		//while (left_flag == 1 && sv->left_led != 1) 
		//	forward(sv);
	
		//while (right_flag == 1 && sv->right_led != 1) 
		//	forward(sv);

		//while (sv->right_led == 1)
		//	forward(sv); 

		//while (sv->left_led == 1)
		//	forward(sv);

	
		//printf("Start Drive Forward 1\n");
		//while(sv->front_led != 1)
		//drive_forward(sv);

			printf("After Forward0\n");
		forward(sv);
		delay(3000);
			
			printf("After Forward2\n");
			right(sv);
			delay(1350);
			printf("After right\n");
			forward(sv);
			delay(6000);
	
			//while(sv->right_led != 1 && sv->front_led != 1) 
			//	forward(sv);
			//printf("Affter forward 3\n");
			//if (sv->front_led == 1) {
			//	while (sv->right_led != 1) 
			//		left(sv);
			//}
			//delay(200);
}

void turn_left(SharedVariable* sv)
{
	printf("Eneterd Turn Left");
	int left_flag = 0; 
	int right_flag = 0; 
		if(sv->left_led == 0)
			left_flag = 1;
		if (sv->right_led == 0) 
			right_flag = 1;

		if (left_flag == 1 && right_flag == 1)
			while (sv->left_led == 1 || sv->right_led == 1) 
				forward();
		else if (left_flag == 1){
		while (sv->left_led == 1)
			forward(sv);
		} else if (right_flag == 1) {
		while (sv->right_led == 1)
			forward(sv);
		}

		while (left_flag == 1 && sv->left_led != 1) 
			forward(sv);
	
		while (right_flag == 1 && sv->right_led != 1) 
			forward(sv);

		while (sv->right_led == 1)
			forward(sv); 

		while (sv->left_led == 1)
			forward(sv);

	
		//printf("Start Drive Forward 1\n");
		//while(sv->front_led != 1)
		//drive_forward(sv);

			printf("After Forward0\n");
		forward(sv);
		delay(6000);
			
			printf("After Forward2\n");
			left(sv);
			delay(1000);
			printf("After left\n");
			forward(sv);
			delay(6000);
	
			//while(sv->right_led != 1 && sv->front_led != 1) 
			//	forward(sv);
			//printf("Affter forward 3\n");
			//if (sv->front_led == 1) {
			//	while (sv->right_led != 1) 
			//		left(sv);
			//}
			//delay(200);
}

void lane_change(SharedVariable* sv)
{

	printf("Eneterd Lane Change");
		create_init(sv);
		if (sv->current_lane == 0) {
		right(sv);
		delay(1200);
		} else { 
		left(sv);
		delay(500);
		}

		while (sv->front_led != 1) {
			forward(sv);
			delay(50);
		}

		delay(4000);

		if (sv->current_lane == 0) {
		left(sv);
		delay(700);
		sv->current_lane = 1;
		} else { 
		right(sv);
		delay(1200);
		sv->current_lane = 0;
		}
		sv->obstacle_detected = 0;
		sv->next_lane_obstacle_detected =0;
}
