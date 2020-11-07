#ifndef MANUSUTILITIES_H
#define MANUSUTILITIES_H

int calculateAngle(cv::Point p0, cv::Point p1, cv::Point p2);
void maxInscribedCircle(std::vector<cv::Point>& contour, cv::Point& center, int& radius);

#endif // !MANUSUTILITIES_H
