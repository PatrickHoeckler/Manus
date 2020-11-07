#include "pch.h"
#include "mouseInput.h"

void mouseCallbackFunction(int mouseEvent, int x, int y, int flags, void* userdata)
{
	mouse.x = x;
	mouse.y = y;
	if (mouseEvent == cv::EVENT_FLAG_LBUTTON) mouse.click = true;
}