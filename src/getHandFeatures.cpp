#include "pch.h"
#include "getHandFeatures.h"

#include "keyboarInput.h"
#include "manusUtilities.h"

void getHandFeatures(std::vector<cv::Point>& contour, handFeat& hand)
{
	hand.contour = contour;

	//Max inscribed circle
	maxInscribedCircle(contour, hand.center, hand.radius);

	//Convex Hull
	std::vector<int> hull;
	cv::convexHull(contour, hull, false, false);

	//Convexity defects
	std::vector<cv::Vec4i> def;
	cv::convexityDefects(contour, hull, def);

	//Detecting defects between fingers
	int ylim = hand.center.y + hand.radius;
	for (int i = 0; i < def.size(); i++)
	{
		cv::Point p0 = contour.at(def.at(i)[0]);
		cv::Point p1 = contour.at(def.at(i)[1]);
		cv::Point p2 = contour.at(def.at(i)[2]);
		int angle = calculateAngle(p2, p1, p0);
		int distance = (int)cv::norm(p2 - hand.center);
		if (angle < 100 && distance < hand.radius + 30 && p2.y < ylim)
			hand.defects.push_back(p2);
	}

	//Detecting clusters of fingertips
	std::vector<std::vector<int> > cluster(5, std::vector<int>());
	int k = 0;
	int contSize = (int)contour.size();
	for (int i = 0; i < def.size(); i++)
	{
		int curIdx = def.at(i)[0];
		cv::Point cur = contour.at(curIdx);
		int distance = (int)cv::norm(cur - hand.center);
		if (ylim < cur.y || distance < alpha * hand.radius) continue;

		for (int k = 0; k < 5; k++)
		{
			if (cluster.at(k).size() == 0) cluster.at(k).push_back(curIdx);
			for (int j = 0; j < cluster.at(k).size(); j++)
			{
				int idxDistance = (int)abs(cluster.at(k).at(j) - curIdx);
				if (idxDistance < 30 || contSize - idxDistance < 30)
				{
					cluster.at(k).push_back(curIdx);
					k = 5;
					break;
				}
			}
		}
	}

	//Calculating the clusters average - this will be the fingertips
	for (int i = 0; i < cluster.size(); i++)
	{
		int tipX, tipY;
		tipX = tipY = 0;
		if (cluster.at(i).size() == 0) continue;
		for (int j = 0; j < cluster.at(i).size(); j++)
		{
			tipX += contour.at(cluster.at(i).at(j)).x;
			tipY += contour.at(cluster.at(i).at(j)).y;
		}
		hand.fingertips.push_back(cv::Point((int)(tipX / cluster.at(i).size()), (int)(tipY / cluster.at(i).size())));
	}

	return;
}