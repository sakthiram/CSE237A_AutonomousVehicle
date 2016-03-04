// Important! DO NOT MODIFY this file.
// You will not submit this file.
// This file is provided for your understanding of the program procedure.

// Skeleton code of CSE237A, Week 3: Sensor interaction
// For more details, please see the instructions in the class website


#include <stdio.h>
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "create.h"

#define thread_decl(NAME) \
void* thread_##NAME(void* param) { \
	SharedVariable* pV = (SharedVariable*) param; \
	body_##NAME(pV); \
	return NULL; }

thread_decl(ultrasound)
thread_decl(irled)
thread_decl(linefollow)
thread_decl(keypress)
thread_decl(socket)

#define thread_create(NAME) pthread_create(&t_##NAME, NULL, thread_##NAME, &v);
#define thread_join(NAME) pthread_join(t_##NAME, NULL);

int main(int argc, char* argv[]) {
	// Initialize
	SharedVariable v;

	if (wiringPiSetup() == -1) {
		printf("Failed to setup wiringPi.");
		return 1; 
	}

	init_shared_variable(&v);
	init_sensors(&v);

	pthread_t t_ultrasound, t_irled, t_linefollow,t_keypress,t_socket;
			  
		// Create sensing threads
		thread_create(ultrasound);
		thread_create(irled);
		thread_create(linefollow);
		thread_create(keypress);		
		thread_create(socket);		
		// Wait until all threads finish
		thread_join(socket);
		thread_join(ultrasound);
		thread_join(irled);
		thread_join(linefollow);	
		thread_join(keypress);

	return 0;
}
