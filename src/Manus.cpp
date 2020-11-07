#include "pch.h"

#include "getHandColor.h"
#include "getHandFeatures.h"
#include "getHandGesture.h"
#include "keyboarInput.h"
#include "manusUtilities.h"
#include "mouseInput.h"
#include "sendKey.h"


//Global variables
	//Conditions
bool endProgram = false;
bool mirrorImage = false; //Changes the order of left/right hands

bool control = false; //used to select between adjusting hand recognition(false) to sending inputs through gestures(true)

	//Input values
double alpha = 2.0;
double beta = 100.0;
double gamma = 0.0;
double delta = 50.0;
mouse_struct mouse;

	//Variables to get the average colors of the hand
bool colorSelection = true;
int colorsDefined = 0;
std::vector<cv::Scalar> handColor(6);
std::vector<cv::Rect> handBox(6);

	//Font to put text on screen
int font = cv::FONT_HERSHEY_PLAIN;

	//Variables to control sending keyboard inputs
char lastKeyLeft = 0;
char lastKeyRight = 0;
bool pressed = false;
int LkeyCodes[10] = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' }; //left hand gesture-to-key code
int RkeyCodes[10] = { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z' }; //right hand gesture-to-key code
/*Gesture codes
	0 - Unknown
	1 - 1 finger / pointing
	2 - 2 fingers
	3 - 3 fingers
	4 - 4 fingers
	5 - 5 fingers / open palm
	6 - closed fist
	7 - only thumb
	8 - surf
	9 - gun sign
	10 - horns
*/


int main()
{
	cv::VideoCapture cam(1);
	if (!cam.isOpened())
	{
		std::cout << "Error opening cam" << std::endl;
		return -1;
	}

	//Opening Windows
	cv::namedWindow("Cam feed", cv::WINDOW_KEEPRATIO);
	cv::namedWindow("Binary", cv::WINDOW_KEEPRATIO);

	//Binding mouse callback to the video window
	cv::setMouseCallback("Cam feed", mouseCallbackFunction, NULL);

	//Declaring variables used to calculate FPS
	std::clock_t now = std::clock();
	std::clock_t then = now;

	//Render Loop
	while (!endProgram)
	{
		cv::Mat frame, frameYCrCb, binary;
		if (!cam.read(frame))
		{
			endProgram = true;
			continue;
		}
		cv::cvtColor(frame, frameYCrCb, cv::COLOR_BGR2YCrCb);

		//Hand Color Selection
		if (colorSelection)
		{
			cv::Rect mouseBox = drawMouseBox(frame, mouse);
			if (mouse.click)
			{
				handColor.at(colorsDefined++) = getHandColor(frameYCrCb, mouseBox);
				mouse.click = false;
			}
			if (colorsDefined == 6)	colorSelection = false;
		}

		//Binarizing image
		std::vector<cv::Mat> avgBinary(6);
		binary = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);
			//Gets one binary image for each one of the average colors and then combines all
		for (int i = 0; i < colorsDefined; i++)
		{
			cv::inRange(frameYCrCb, handColor.at(i) - cv::Scalar(beta, gamma, delta), handColor.at(i) + cv::Scalar(100.0, 50.0, 50.0), avgBinary.at(i));
			//cv::inRange(frameYCrCb, handColor.at(i) - cv::Scalar(100.0, 0.0, 50.0), handColor.at(i) + cv::Scalar(100.0, 50.0, 50.0), avgBinary.at(i));
			binary += avgBinary.at(i);
		}
		cv::medianBlur(binary, binary, 7);
		cv::erode(binary, binary, cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5)));

		//Getting contourns
		std::vector<std::vector<cv::Point> > conts;
		cv::findContours(binary, conts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		//Separating contours based on area - resulting contours may include head
		std::vector<int> handIdx;
		for (int i = 0; i < conts.size(); i++)
		{
			if (cv::contourArea(conts.at(i)) > 5000)
				handIdx.push_back(i);
		}

		//Removing head and labeling left and right hands based on horizontal position
		int left, right, nHands;
		left = right = -1;
		int max, min;
		for (int i = 0; i < handIdx.size(); i++)
		{
			cv::Rect bRect = cv::boundingRect(conts.at(handIdx.at(i)));
			int x = bRect.x;
			if (i == 0)
			{
				max = min = x;
				left = right = handIdx.at(0);
			}
			if (x < min)
			{
				min = x;
				right = handIdx.at(i);
			}
			else if (x > max)
			{
				max = x;
				left = handIdx.at(i);
			}
		}
		if (handIdx.size() > 2) nHands = 2;
		else nHands = (int)handIdx.size();

		//Getting hand features
		std::vector<handFeat> hand(2);
		if (nHands == 1)
			getHandFeatures(conts.at(right), hand.at(0));
		else if (nHands == 2)
		{
			getHandFeatures(conts.at(right), hand.at(0));
			getHandFeatures(conts.at(left), hand.at(1));
		}

		//Getting gesture based on hand features - uses gesture code to send inputs
		for (int i = 0; i < nHands; i++)
		{
			hand.at(i).gesture = getHandGesture(hand.at(i));
			if (control)
			{
				if ((bool)i == mirrorImage) //Left hand
				{
					char keyCode = LkeyCodes[hand.at(i).gesture - 1];
					sendKey(keyCode, lastKeyLeft);
					lastKeyLeft = keyCode;
				}
				else //Right hand
				{
					char keyCode = RkeyCodes[hand.at(i).gesture - 1];
					sendKey(keyCode, lastKeyRight);
					lastKeyRight = keyCode;
				}
			}
		}
			
		//Drawing contours and their features in the main frame
		for (int i = 0; i < nHands; i++)
		{
			std::vector<cv::Point> hull;
			cv::convexHull(hand.at(i).contour, hull);
			cv::polylines(frame, hand.at(i).contour, true, cv::Scalar(255, 0, 0), 2); //Drawing contour
			cv::polylines(frame, hull, true, cv::Scalar(0, 255, 255), 2); //Drawing convex hull
			cv::circle(frame, hand.at(i).center, 6, cv::Scalar(255, 0, 255), cv::FILLED); //Center of hand
			for (int j = 0; j < hand.at(i).fingertips.size(); j++)
				cv::circle(frame, hand.at(i).fingertips.at(j), 6, cv::Scalar(0, 0, 0), 2); //Fingertips
			for (int j = 0; j < hand.at(i).defects.size(); j++)
				cv::circle(frame, hand.at(i).defects.at(j), 6, cv::Scalar(255, 255, 255), 2); //Defects
			cv::circle(frame, hand.at(i).center, hand.at(i).radius, cv::Scalar(0, 255, 255), 2); //Max inscribed circle
			cv::circle(frame, hand.at(i).center, (int)(alpha * hand.at(i).radius), cv::Scalar(100, 100, 255), 2); //Circle limiting fingertips
							//Putting text - Gesture
			std::stringstream s;
			s << hand.at(i).gesture;
			cv::putText(frame, s.str(), hand.at(i).center, font, 2, cv::Scalar(255, 255, 255), 2); //Gesture
		}

		//Calculating FPS
		now = std::clock();
		float fps = 1.0f * (CLOCKS_PER_SEC / (now - then));
		then = now;

		//Putting text - FPS
		std::stringstream s;
		s << fps;
		std::string fpsText = "FPS = " + s.str();
		cv::putText(frame, fpsText, cv::Point(0, 30), font, 2, cv::Scalar(0, 0, 0), 2);

		//Showing images
		cv::imshow("Cam feed", frame);
		cv::imshow("Binary", binary);

		//Processing input
		processInput(cv::waitKey(1));
	}

	cv::destroyAllWindows();
	return 0;
}