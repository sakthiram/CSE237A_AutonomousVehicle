#include <sstream>
#include <string>
#include <iostream>
#include <highgui.h>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <wiringPi.h>

#define COLOR_DET0 0
#define COLOR_DET1 1

     //extern "C" {
     //   //int ObjectDetect();
     //   extern int colour_detected;
     //}

int colour_detected = 0, prev_colour = 0;

using namespace cv;
//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN[3] = {120, 31, 3};
int H_MAX[3] = {186, 93, 14};
int S_MIN[3] = {72, 4, 151};
int S_MAX[3] = {256, 121, 187};
int V_MIN[3] = {154, 254, 109};
int V_MAX[3] = {256, 256, 140};
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=250;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const std::string windowName = "Original Image";
const std::string windowName1 = "HSV Image";
const std::string windowName2 = "Thresholded Image";
const std::string windowName3 = "After Morphological Operations";
const std::string trackbarWindowName = "Trackbars";
void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed





}
std::string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}
void createTrackbars(){
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN[0]);
	sprintf( TrackbarName, "H_MAX", H_MAX[0]);
	sprintf( TrackbarName, "S_MIN", S_MIN[0]);
	sprintf( TrackbarName, "S_MAX", S_MAX[0]);
	sprintf( TrackbarName, "V_MIN", V_MIN[0]);
	sprintf( TrackbarName, "V_MAX", V_MAX[0]);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN[0], H_MAX[0], on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX[0], H_MAX[0], on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN[0], S_MAX[0], on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX[0], S_MAX[0], on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN[0], V_MAX[0], on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX[0], V_MAX[0], on_trackbar );


}
void drawObject(int x, int y,Mat &frame){

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!


	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

	putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}
void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
	


}
void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed, int i){

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	std::vector< std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	std::string colour = "BLACK";
	
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
					refArea = area;
				}else objectFound = false;


			}

			switch(i) {
			case 0 : colour = "Tracking Object - RED_circle"; 
				 colour_detected = 1; // RED
				 break; 
			case 1 : colour = "Tracking Object - GREEN_circle"; 
				 colour_detected = 2; // GREEN
				 break; 
			//case 2 : colour = "Tracking Object - RED"; 
			//	 colour_detected = 1; // RED
			//	 break; 
			//case 3 : colour = "Tracking Object - GREEN";  
			//	 colour_detected = 2; // GREEN
			//	 break; 
			//case 4 : colour = "Tracking Object - GREEN"; 
			//	 colour_detected = 2; // GREEN
			//	 break; 
			case 2 : colour = "Tracking Object - PINK";
				 colour_detected = 3; // PINK
				 break; 
				
			}  


			//let user know you found an object
			if(objectFound ==true){
				putText(cameraFeed,colour,Point(0,50),2,1,Scalar(0,255,0),2);
				//putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,cameraFeed);
			 } else 
			colour_detected = 0;
		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}
int main()
{
	//some boolean variables for different functionality within this
	//program
    bool trackObjects = true;
    bool useMorphOps = false;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//x and y values for the location of the object
	int x=0, y=0, i=0;
	long start_time;

	if (wiringPiSetup () == -1) 
	exit (1);	

	pinMode (COLOR_DET0, OUTPUT);
	pinMode (COLOR_DET1, OUTPUT);

	digitalWrite(COLOR_DET0, 0);	
	digitalWrite(COLOR_DET1, 0);

	//create slider bars for HSV filtering
	createTrackbars();
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while(1){
		//store image to matrix
		capture.read(cameraFeed);

		for (i=0; i<3; i++) {	
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		//filter HSV image between values and store filtered image to
		//threshold matrix
		inRange(HSV,Scalar(H_MIN[i],S_MIN[i],V_MIN[i]),Scalar(H_MAX[i],S_MAX[i],V_MAX[i]),threshold);
		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		if(useMorphOps)
		morphOps(threshold);
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if(trackObjects)
			trackFilteredObject(x,y,threshold,cameraFeed,i);

		//show frames 
		imshow(windowName2,threshold);
		imshow(windowName,cameraFeed);
		imshow(windowName1,HSV);
		
	
		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
			
		if (colour_detected != 0 && colour_detected != prev_colour) {
		digitalWrite(COLOR_DET0, colour_detected%2);	
		digitalWrite(COLOR_DET1, colour_detected/2);
		//start_time = micros();
		}

		//if (colour_detected != 0) {
		//if (micros()-start_time > 2000000) {
		//digitalWrite(COLOR_DET0, 0);	
		//digitalWrite(COLOR_DET1, 0);
		//}
		//}

		prev_colour = colour_detected;

		}
	}






	return 0;
}

