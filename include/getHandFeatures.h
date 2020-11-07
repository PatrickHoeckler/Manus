#ifndef GETHANDFEATURES_H
#define GETHANDFEATURES_H

struct handFeat
{
	std::vector<cv::Point> contour;
	std::vector<cv::Point> fingertips;
	std::vector<cv::Point> defects;
	cv::Point center;
	int radius;
	int gesture;
};

void getHandFeatures(std::vector<cv::Point>& contour, handFeat& hand);

#endif // !GETHANDFEATURES_H

