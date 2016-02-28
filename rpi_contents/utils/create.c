#include <stdio.h> // standard input / output functions
#include <string.h> // string function definitions
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h>
#include <termios.h>
#include <time.h>   // time calls
//#include <ncurses.h> // use -lncurses along with gcc cmd

/* Define the serial port path */
#define CREATE_SERIAL_PORT "/dev/ttyUSB0"
/* Define baudrate */
#define CREATE_SERIAL_BRATE B115200

static int fd;

void main()
{

 int x = ' ';
 int y = ' ';
 int z = ' ';

// Initialize Irobot
 create_init();

 while (!(x == 27 && y == 27 && z == 27)) { // 3 times ESC

 printf("Press any key to continue...\n");
 x = getch();

 if (x == ' ') {
  stop();
 }

 if (x == 27)
 {
  y = getch();
  z = getch();
 }

 if (x == 27 && y == 91)
 {
  switch (z)
  {
   case 65:
   printf("up arrow key pressed\n");
   forward();
   //stop();
   break;

   case 66:
   printf("down arrow key pressed\n");
   reverse();
   //stop();
   break;

   case 67:
   printf("right arrow key pressed\n");
   right();
   //stop();
   break;

   case 68:
   printf("left arrow key pressed\n");
   left();
   //stop();
   break;
  }
 }
 }
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
wd =  write(fd, forward, 8);
}

void reverse() {
char reverse[] = {137, 255, 100, 128, 0 };
int wd;
wd =  write(fd, reverse, 8);
}

void left() {
char left[] = {137, 0, 100, 0, 1 };
int wd;
wd =  write(fd, left, 8);
}

void right() {
char right[] = {137, 0, 100, 255, 255 }; 
int wd;
wd =  write(fd, right, 8);
}

void stop() {
char stop[] = {137, 0, 0, 0, 0 }; 
int wd;
wd =  write(fd, stop, 8);
}


