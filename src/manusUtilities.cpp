#include "pch.h"
#include "manusUtilities.h"

int calculateAngle(cv::Point p0, cv::Point p1, cv::Point p2)
{
	//Angulo calculado pela relacao: 
	//				 <p0-p1, p0-p2>
	//  cos(angle) = ---------------
	//               |p0-p1|*|p0-p2|
	cv::Point p0p1, p0p2;
	p0p1 = p0 - p1;
	p0p2 = p0 - p2;
	double a = p0p1.dot(p0p2); // <p0p1, p0p2>  produto escalar
	double b = p0p1.dot(p0p1); // |p0p1|^2 magnitude do vetor ao quadrado
	double c = p0p2.dot(p0p2); // |p0p2|^2 magnitude do vetor ao quadrado
	double angle = (57.295779)*acos(a / sqrt(b*c)); // 57.295779 ~ 180/pi, usado para conversao de radianos para graus
	return (int)round(angle);
}

void maxInscribedCircle(std::vector<cv::Point>& contour, cv::Point& center, int& radius)
{
	cv::Rect bRect = cv::boundingRect(contour);
	int k = 0;
	int jMax = 8;
	while (true)
	{
		float dx = (float)bRect.width / 10;
		float dy = (float)bRect.height / 10;
		radius = 0;
		for (int i = 0; i < 11; i++)
		{
			for (int j = 0; j < jMax; j++)
			{
				cv::Point cur((int)(bRect.x + dx * i), (int)(bRect.y + dy * j));
				int distance = (int)cv::pointPolygonTest(contour, cur, true);
				if (distance > radius)
				{
					radius = distance;
					center = cur;
				}
			}
		}
		if (++k < 2)
		{
			bRect = cv::Rect(center.x - radius, center.y - radius, 2 * radius, 2 * radius);
			jMax = 11;
		}
		else break;
	}
}