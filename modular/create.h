#ifndef _ASSIGNMENT_BODY_
#define _ASSIGNMENT_BODY_


#define CAR_STATE_ON 1
#define CAR_STATE_OFF 0

// B. Shared structure
// All thread fuctions get a shared variable of the structure
// as the function parameter.
// If needed, you can add anything in this structure.
typedef struct shared_variable {
	int bProgramExit; // Once it is set to 1, the program will be terminated.
	int button, pin_temp,track,shock;
	int car_state,prev_button,curr_button;
} SharedVariable;


// C. Functions
// You need to implement following functions.
// Do not change any function name here.
void init_ultrasound(SharedVariable* sv);
void init_irled(SharedVariable* sv);

void body_ultrasound(SharedVariable* sv);
void body_irled(SharedVariable* sv);

#endif
