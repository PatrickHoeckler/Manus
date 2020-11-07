#include "pch.h"
#include "getHandGesture.h"

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
int getHandGesture(handFeat& hand)
{
	int nFinger = (int)hand.fingertips.size();
	int nDefects = (int)hand.defects.size();
	int distance;
	bool thumb = false;
	for (int i = 0; i < nFinger; i++)
	{
		if (hand.fingertips.at(i).y > hand.center.y - 1.5 * hand.radius)
		{
			thumb = true;
			break;
		}
	}

	if (nFinger == 0)
	{
		return 6; //closed fist
	}
	if (nFinger == 1)
	{
		if (thumb) return 7; //only thumb
		return 1; //1 finger
	}
	if (nFinger == 2)
	{
		if (thumb)
		{
			distance = (int)abs(hand.fingertips.front().x - hand.fingertips.back().x);
			if (distance > 3 * hand.radius) return 8; //surf
			return 9; //gun sign
		}
		distance = (int)cv::norm(hand.fingertips.front() - hand.fingertips.back());
		if (distance > 2.5 * hand.radius) return 10; //horns
		return 2; //2 fingers
	}
	if (nFinger == 3)
	{
		return 3; //3 fingers
	}
	if (nFinger == 4)
	{
		return 4; //4 fingers
	}
	if (nFinger == 5)
	{
		return 5; //5 fingers
	}
	return 0;
}