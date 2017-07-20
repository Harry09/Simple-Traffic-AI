#define _USE_MATH_DEFINES

#include "Common.h"

#include <cmath>
#include <ctime>

float PointDir(sf::Vector2f pos1, sf::Vector2f pos2)
{
	float deg = ToDegree(-atan2(pos2.y - pos1.y, pos2.x - pos1.x));

	if (deg > 0.f)
		return deg;
	else if (deg < 0.f)
		return 360.f + deg;
	else
		return deg;
}

sf::Vector2f LenDir(float len, float dir)
{
	float rad = ToRadian(dir);

	sf::Vector2f vec;
	vec.x = static_cast<float>(cos(rad)) * len;
	vec.y = -static_cast<float>(sin(rad)) * len;
	return vec;
}

float LengthToPoint(sf::Vector2f pos1, sf::Vector2f pos2)
{
	float cx = 0.f, cy = 0.f;

	cx = pos2.x - pos1.x;
	cy = pos2.y - pos1.y;

	return sqrt(pow(cx, 2) + pow(cy, 2));
}


float ToRadian(float deg)
{
	return static_cast<float>(deg * M_PI / 180);
}

float ToDegree(float rad)
{
	return static_cast<float>(rad * 180 / M_PI);
}

float DegreeToSFMLRotate(float degree)
{
	if (degree < 0.f)
		return (-degree);

	return (360.f - degree);
}

float SFMLRotateToDegree(float sfmlRotate)
{
	return 360.f - sfmlRotate;

	/*if (sfmlRotate < 180.f)
		return (180 - 180 - sfmlRotate);
	if (sfmlRotate >= 180.f)
		return (360 - sfmlRotate);*/
}

std::vector<sf::Vector2f> CalcBezierCurve(const std::vector<sf::Vector2f> &points, float perc)
{
	std::vector<sf::Vector2f> step;

	if (points.size() == 1)
		return points;

	for (int i = 0; i < points.size() - 1; ++i)
	{
		step.push_back(points[i] + (points[i + 1] - points[i]) * perc);
	}

	return CalcBezierCurve(step, perc);
}

std::string DecToBin(int number, int len, int index)
{
	index++;

	if (number == 0)
	{
		std::string end;

		for (int i = index; i < len; ++i)
			end += "0";

		return (end + "0");
	}

	if (number == 1)
	{
		std::string end;

		for (int i = index; i < len; ++i)
			end += "0";

		return (end + "1");
	}

	if (number % 2 == 0)
		return DecToBin(number / 2, len, index) + "0";
	else
		return DecToBin(number / 2, len, index) + "1";
}

double ClockToMilliseconds(clock_t ticks)
{
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}

