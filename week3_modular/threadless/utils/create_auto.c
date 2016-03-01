#include <stdio.h> // standard input / output functions
#include <string.h> // string function definitions
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h>
#include <wiringPi.h>
#include <termios.h>
#include <time.h>   // time calls
//#include <ncurses.h> // use -lncurses along with gcc cmd

/* Define the serial port path */
#define CREATE_SERIAL_PORT "/dev/ttyUSB0"
/* Define baudrate */
#define CREATE_SERIAL_BRATE B115200

#define LEFT_LED 27
#define RIGHT_LED 29
#define FRONT_LED 28

#define US_1_ECHO 7
#define US_1_TRIG 21
#define US_2_ECHO 1
#define US_2_TRIG 22
#define US_3_ECHO 0
#define US_3_TRIG 23
#define US_4_ECHO 2
#define US_4_TRIG 24
#define US_5_ECHO 3
#define US_5_TRIG 25

static int fd;
static int state = 0;

void main()
{

// Initialize Irobot
 create_init();
	pinMode(LEFT_LED, INPUT);
	pinMode(RIGHT_LED, INPUT);
	pinMode(FRONT_LED, INPUT);

while(1) {

	  delay (1000);	  
	  if(digitalRead(FRONT_LED) == 1) {
		if(digitalRead(RIGHT_LED) == 1 && digitalRead(LEFT_LED) == 1)
		{
			forward();
			//delay(100);
		}else {
			stop();
		}	
	  }
	  else{	
		printf("ENtered turn right\n");
		while(digitalRead(LEFT_LED) == 1 || digitalRead(RIGHT_LED) == 1)
		{	forward();			
			delay(50);		
		}	
		if (digitalRead(RIGHT_LED) == 0) {
			while (digitalRead(LEFT_LED) == 1)
				{right();
				 delay(300);}		
		}	
		printf("before while\n");
		while(digitalRead(LEFT_LED) == 0)
		{	right();
			delay(50);	
		}		
		printf("after while\n");
		while(digitalRead(LEFT_LED) == 1 && digitalRead(FRONT_LED) == 1) 
		{	forward();
			delay(50);		
		}		
		printf("after forward\n");
		if (digitalRead(FRONT_LED) == 0) {
			while (digitalRead(LEFT_LED) == 1) 
				{right();
				  delay(50);}
			printf("left led on\n");
		}

			while (digitalRead(LEFT_LED) == 0)
				{right();
				  delay(50);}	
			printf("left led off\n");
			forward();
			delay(1000);
			printf("turning left");
			left();
			printf("left turned");			
			delay(1000);


	  }


}
reverse();
delay(1000);

right();
left();
forward();
delay(1000);
stop();

}

int getch(void)
{
 int ch;
 struct termios oldt;
 struct termios newt;
 tcgetattr(STDIN_FILENO, &oldt); /*store old settings */
 newt = oldt; /* copy old settings to new settings */
 newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */
 tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */
 ch = getchar(); /* standard getchar call */
 tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */
 return ch; /*return received char */
}


int create_init() {

////////////////
// Initialize //
////////////////

fd = open(CREATE_SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY );


// can't open port
if (fd == -1) {
  printf("Error opening port\n");
  return -1;
}
// open successful
else {
  printf("Serial port opened with status: %d\n", fd);
  fcntl(fd, F_SETFL, 0);
}
// configure port
struct termios portSettings;
tcgetattr(fd, &portSettings);


if(cfsetispeed(&portSettings, CREATE_SERIAL_BRATE) != 0)
  printf("Failed setting baud rate for input");
if(cfsetospeed(&portSettings, CREATE_SERIAL_BRATE) != 0)
  printf("Failed setting baud rate for output");


//set parity bits
portSettings.c_cflag &= ~PARENB;
portSettings.c_cflag &= ~CSTOPB;
portSettings.c_cflag &= ~CSIZE;
portSettings.c_cflag |= CS8;
cfmakeraw(&portSettings);


if(tcsetattr(fd, TCSANOW, &portSettings) != 0) {
  printf("Failed pushing port settings.\n");
  return fd;
}

// Running iRobot create

char initsafe[] = {128, 131};
char initfull[] = {128, 132};  

int wd;
wd = write(fd, initfull, 8);
wd = write(fd, initfull, 8);
wd = write(fd, initfull, 8);
}

void forward() {
char forward[] = {137, 0, 100, 128, 0 };
int wd;
if (state != 1) {
printf("FORWARD Entered");
wd =  write(fd, forward, 8);
printf("FORWARD Written");
}
state = 1;
}

void reverse() {
char reverse[] = {137, 255, 100, 128, 0 };
int wd;
if (state != 2) {
printf("REVERSE Entered");
wd =  write(fd, reverse, 8);
printf("REVERSE Written");
}
state = 2;
}

void left() {
char left[] = {137, 0, 100, 0, 1 };
int wd;
if (state != 3) {
printf("LEFT Entered");
wd =  write(fd, left, 8);
printf("LEFT Written");
}
state = 3;
}

void right() {
char right[] = {137, 0, 100, 255, 255 }; 
int wd;
if (state != 4) {
printf("RIGHT Entered");
wd =  write(fd, right, 8);
printf("RIGHT Written");
}
state = 4;
}

void stop() {
char stop[] = {137, 0, 0, 0, 0 }; 
int wd;
if (state != 0) {
printf("STOP Entered");
wd =  write(fd, stop, 8);
printf("STOP Written");
}
state = 0;
}


