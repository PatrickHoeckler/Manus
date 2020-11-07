#ifndef GETHANDCOLOR_H
#define GETHANDCOLOR_H

#include "mouseInput.h"

cv::Rect drawMouseBox(cv::Mat& frameDraw, mouse_struct& mouse);
cv::Scalar getHandColor(cv::Mat& frameGet, cv::Rect mouseBox);

#endif // !GETHANDCOLOR_H