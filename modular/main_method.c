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

	pthread_t t_ultrasound,t_irled;

	while (v.bProgramExit != 1) {
		// Create sensing threads
		thread_create(ultrasound);
		thread_create(irled);
		
		// Wait until all threads finish
		thread_join(ultrasound);
		thread_join(irled);
		delay(1);
	}

	printf("end of file \n");

	return 0;
}
