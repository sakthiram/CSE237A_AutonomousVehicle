#include "create.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>
#include <strings.h>

int uturn_done;

void init_shared_variable(SharedVariable* sv) {
	sv->bProgramExit = 0;
	sv->drive_state = STOP_WAIT;
	sv->init_start = 0;
	sv->manual_stop = 1;
	sv->current_direction = STOP;
	sv->obstacle_detected = 0;
	sv->next_lane_obstacle_detected =0;
	sv->current_lane = 1;
	bzero(sv->directions,10);
	sv->turn = 0;
	sv->turn_count = 0;
	uturn_done = 0;
		
}

void init_sensors(SharedVariable* sv) {
	pinMode(LEFT_LED, INPUT);
	pinMode(RIGHT_LED, INPUT);
	pinMode(FRONT_LED, INPUT);
	pinMode(COLOR_DET0, INPUT);
	pinMode(COLOR_DET1, INPUT);
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
	int dist_1,dist_2,dist_3,dist_4,dist_5;
	while(1){
	setup_US(US_1_TRIG,US_1_ECHO);
	setup_US(US_2_TRIG,US_2_ECHO);
	setup_US(US_3_TRIG,US_3_ECHO);
	setup_US(US_4_TRIG,US_4_ECHO);
	setup_US(US_5_TRIG,US_5_ECHO);

	dist_1 = getCM(US_1_TRIG,US_1_ECHO);
	dist_2 = getCM(US_2_TRIG,US_2_ECHO);
	dist_3 = getCM(US_3_TRIG,US_3_ECHO);
	dist_4 = getCM(US_4_TRIG,US_4_ECHO); 
	dist_5 = getCM(US_5_TRIG,US_5_ECHO); 


	if(sv->current_lane == 0) {
	if((dist_4>5 && dist_4<60) || (dist_5>5 && dist_5<30))
	{
			sv->next_lane_obstacle_detected = 1;
			printf("rightlane obstacle\n");
	} else {
		sv->next_lane_obstacle_detected = 0;
	}
	}

	if(sv->current_lane == 1){
	if((dist_2>5 && dist_2<60) || (dist_1>5 && dist_1<30))
	{	
			sv->next_lane_obstacle_detected = 1;
			printf("leftlane obstacle\n");
	} else {
		sv->next_lane_obstacle_detected = 0;
	}
	}

	if((dist_3>2 && dist_3<15))
	{
		printf("Obstacle detected\n");
		sv->obstacle_detected = 1;	
	
	} else {
		sv->obstacle_detected = 0;	
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
	 
	 if (sv->obstacle_detected == 1 && sv->next_lane_obstacle_detected == 0) {
		sv->drive_state = OBSTACLE_DETECTED;	
	 } else if (sv->obstacle_detected == 1 && sv->next_lane_obstacle_detected == 1) {
		sv->drive_state = STOP_WAIT;	
	 } else if((!sv->front_led)){
	        sv->drive_state = DRIVE_FORWARD;
	        }
	  else{
		// RED
		if (digitalRead(COLOR_DET0) == 1 && digitalRead(COLOR_DET1) == 0) {
			printf("RED\n");
			//Wait till GREEN
			while (!(digitalRead(COLOR_DET0) == 0 && digitalRead(COLOR_DET1) == 1)) {
			stop(sv);
			delay(100);
			}
		}
		
	        if(sv->turn == 1){
	        sv->drive_state = TURN_RIGHT;
	        printf("sv->turn right set\n");
	        stop(sv);
	        }
	        else if(sv->turn == 2)
	        sv->drive_state = TURN_LEFT;
	        
	        //sv->turn = 0;		
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
			stop_wait(sv);
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
/*	printf("press aaa\n");
 while (!(x == 'a' && y == 'a' && z == 'a')) { // 3 times ESC
	sv->init_start = 0;
	
	x = getch();
	y = getch();
	z = getch(); 
	}
	printf("pressed aaa\n");

 sv->init_start =1;
*/
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
		
			left(sv);
			delay(200);
			forward(sv);
			delay(300);
		right(sv);
		delay(130);
	}
	if(sv->left_led) 
	{
			right(sv);
			delay(200);
			forward(sv);
			delay(300);
		left(sv);
		delay(80);
			
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
				forward(sv);
		else if (left_flag == 1){
		while (sv->left_led == 1)
			forward(sv);
		} else if (right_flag == 1) {
		while (sv->right_led == 1)
			forward(sv);
		}


			printf("After Forward0\n");
		forward(sv);
		delay_custom(sv,3000);
			
			printf("After Forward2\n");
			right(sv);
			delay_custom(sv,1400);
			printf("After right\n");
			forward(sv);

			if (sv->current_lane == 1) {
				delay_custom(sv,4000);
			} else {
				delay_custom(sv,7000);
			}

			sv->turn_count++;
			turn_update(sv, (sv->turn_count - 1), (sv->turn_count));

}

void turn_left(SharedVariable* sv)
{
	printf("Eneterd Turn Left");
	int left_flag = 0; 
	int right_flag = 0; 
			printf("After Forward0\n");

		int i;

		for (i=0; i<3; i++) {
		forward(sv);
		delay_custom(sv,2000);
		right(sv);
		delay_custom(sv,105);
		} 
		forward(sv);
		delay_custom(sv,2000);
			
			printf("After Forward2\n");
			left(sv);
			delay_custom(sv,1200);
			printf("After left\n");
			forward(sv);
			
			if (sv->current_lane == 0) {
				delay_custom(sv,3000);
			} else {
				delay_custom(sv,6000);
			}

			sv->turn_count++;
			turn_update(sv, (sv->turn_count - 1), (sv->turn_count));
}

void lane_change(SharedVariable* sv)
{

	printf("Eneterd Lane Change");
		create_init(sv);
		if (sv->current_lane == 0) {
		right(sv);
		delay(1500);
		} else { 
		left(sv);
		delay(1000);
		}

		while (sv->front_led != 1) {
			forward(sv);
			delay(50);
		}

		delay(4000);

		if (sv->current_lane == 0) {
		left(sv);
		delay(1000);
		sv->current_lane = 1;
		} else { 
		right(sv);
		delay(1500);
		sv->current_lane = 0;
		}
		sv->obstacle_detected = 0;
		sv->next_lane_obstacle_detected =0;
}

void stop_wait(SharedVariable* sv)
{
	while(sv->obstacle_detected == 1 && sv->next_lane_obstacle_detected == 1)
		stop(sv);
}

void turn_update(SharedVariable* sv, int first, int second)
{
	if(sv->directions[first]==0 || sv->directions[second] == 0)
	        sv->turn = 0;

	else if(sv->directions[first] == 'B' && sv->directions[second] =='C')
		sv->turn = 0;
	else if(sv->directions[first] == 'B'&& sv->directions[second] == 'D')
		sv->turn = 2;

	else if(sv->directions[first] == 'C'&& sv->directions[second] == 'B')
		sv->turn = 0;
	else if(sv->directions[first] == 'C'&& sv->directions[second] == 'D')
		sv->turn = 1;
	else if(sv->directions[first] == 'D'&& sv->directions[second] == 'C')
		sv->turn = 2;
	else if(sv->directions[first] == 'D'&& sv->directions[second] == 'B')
		sv->turn = 1;
}

void delay_custom(SharedVariable* sv, int delay_val) {

	int current_dir = sv->current_direction;
	if (delay_val <= 300) {
		delay(delay_val);
	} else {
		while (delay_val > 0) {
		while (sv->obstacle_detected == 1 || sv->next_lane_obstacle_detected == 1) 
			stop(sv);
		create_init(sv);
		switch(current_dir) {
		case STOP: stop(sv); break; 
		case FORWARD: forward(sv); break; 
		case REVERSE: reverse(sv); break; 
		case LEFT: left(sv); break; 
		case RIGHT: right(sv); break; 
		}
		printf("Before Delay");
		delay(300);
		printf("After Delay");
		delay_val -= 300;
		}			
	}	
}

void uturn(SharedVariable* sv) {
printf("Starting UTURN\n");

	if (sv->current_lane == 1) {
		left(sv);
		delay(1300);
		forward(sv);
		delay(2500);	
		left(sv);
		delay(1300);
	} else {
		right(sv);
		delay(1300);
		forward(sv);
		delay(2500);	
		right(sv);
		delay(1300);
	}
uturn_done = 1;
}
