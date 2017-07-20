#pragma once
#define _USE_MATH_DEFINES

#include <SFML\System\Vector2.hpp>
#include <vector>
#include <ctime>

float PointDir(sf::Vector2f pos1, sf::Vector2f pos2);
sf::Vector2f LenDir(float len, float dir);
float LengthToPoint(sf::Vector2f pos1, sf::Vector2f pos2);

float ToRadian(float deg);
float ToDegree(float rad);

float DegreeToSFMLRotate(float degree);
float SFMLRotateToDegree(float sfmlRotate);

std::vector<sf::Vector2f> CalcBezierCurve(const std::vector<sf::Vector2f> &points, float perc);

// index must be 0
std::string DecToBin(int number, int len, int index = 0);

template <typename T>
char *DataToBytes(const T& data)
{
	return ((char*)(&data));
}

template <typename T>
char *DataToBytes(T& data)
{
	return ((char*)(&data));
}

double ClockToMilliseconds(clock_t ticks);
