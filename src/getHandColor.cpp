#include "pch.h"
#include "getHandColor.h"

//- frameDraw: frame where the mouse box will be drawn
//- mouse: mouse struct that has the mouse position and click status
cv::Rect drawMouseBox(cv::Mat& frameDraw, mouse_struct& mouse)
{
	//Mouse box position
	int mBoxX, mBoxY;

	//Setting mouse box position
	if (mouse.x < 8) mBoxX = 0;
	else if (mouse.x + 7 > frameDraw.cols) mBoxX = frameDraw.cols - 15;
	else mBoxX = mouse.x - 8;

	if (mouse.y < 8) mBoxY = 0;
	else if (mouse.y + 7 > frameDraw.rows) mBoxY = frameDraw.rows - 15;
	else mBoxY = mouse.y - 8;

	//Drawing box on frame
	cv::Rect mouseBox(mBoxX, mBoxY, 15, 15);
	cv::rectangle(frameDraw, mouseBox, cv::Scalar(0, 0, 255), 2);

	return mouseBox;
}


//- frameGet: frame from where the average color of the mouse box will be calculated
cv::Scalar getHandColor(cv::Mat& frameGet, cv::Rect mouseBox)
{
	int c1, c2, c3, n;
	c1 = c2 = c3 = n = 0;
	cv::Mat box = frameGet(mouseBox);
	cv::Scalar color = cv::Scalar(0, 0, 0);
	for (int i = 0; i < box.cols; i++)
	{
		for (int j = 0; j < box.rows; j++)
		{
			color += cv::Scalar(box.at<cv::Vec3b>(i, j));
			n++;
		}
	}
	color /= n;
	return color;
}