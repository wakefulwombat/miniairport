#pragma once
#include "DxLib.h"

const static double M_PI = 3.14159265358979;

enum class Direction4 {
	RIGHT,
	DOWN,
	LEFT,
	UP,
	NEUTRAL
};

enum class Direction8 {
	RIGHT,
	RIGHT_DOWN,
	DOWN,
	LEFT_DOWN,
	LEFT,
	LEFT_UP,
	UP,
	RIGHT_UP,
	NEUTRAL
};

class Vec2D {
public:
	double x, y;
	Vec2D();
	Vec2D(double x, double y);
	Vec2D operator+(const Vec2D& obj);
	Vec2D& operator+=(const Vec2D& obj);
	Vec2D operator-(const Vec2D& obj);
	Vec2D& operator-=(const Vec2D& obj);
	Vec2D operator*(const double& d);
	double operator*(const Vec2D& obj);
	Vec2D operator/(const double& d);
	bool operator==(const Vec2D& obj);
	bool operator!=(const Vec2D& obj);

	double getDistance(Vec2D p2);
	bool isInSquare(Vec2D leftup, Vec2D rightdown);
	double toPower();
	double toNorm();
	double toAngle();
	Direction4 toDirection4(double neutral_min = 0.01);
	Direction8 toDirection8(double neutral_min = 0.01);
	double getPowerFrom(Vec2D origin);
	double getNorm(Vec2D origin);
	double getAngle(Vec2D origin);
	double dot(Vec2D vec);
	double cross(Vec2D vec);
	static Vec2D fromXY(double x, double y);
	static Vec2D fromPowAng(double pow, double ang);
	static Vec2D getRandomOnCircle(double r);
	static Vec2D getRandomInCircle(double r);
	static Vec2D getRandomOnSquare(int width_half, int height_half);
	static Vec2D getRandomInSquare(int width_half, int height_half);
	static bool isCrossLine(Vec2D p1_start, Vec2D p1_end, Vec2D p2_start, Vec2D p2_end);
	static bool isInSquare(Vec2D leftup, Vec2D rightdown, Vec2D p);
	static bool isPointWholeInSquare(Vec2D p1_lu, Vec2D p1_ru, Vec2D p1_rd, Vec2D p1_ld, Vec2D p2);
};

class Size {
public:
	int width, height;
	Size() {};
	Size(int width, int height);
	Size operator/(const double& i);
	Size operator*(const double& d);
	Size operator*(const double& d) const;
	Size operator+(const Size& obj);

	Vec2D toVec();
};


class Mat2D {
public:
	double matrix[2][2];
	Mat2D();
	Mat2D(int m00, int m10, int m01, int m11);
	Vec2D operator*(const Vec2D& obj);
	Mat2D operator*(const Mat2D& obj);
	Mat2D operator+(const Mat2D& obj);
	Mat2D operator-(const Mat2D& obj);

	Mat2D init_rotation(double angle);
	Mat2D init_scaling(double x, double y);
	Mat2D init_stretch(double x, double y);

	Vec2D trans(Vec2D vec, Vec2D origin);

	static Vec2D rotation(double angle, Vec2D vec);
	static Vec2D rotation(double angle, Vec2D vec, Vec2D origin);
	static Vec2D scaling(double x, double y, Vec2D vec);
	static Vec2D scaling(double x, double y, Vec2D vec, Vec2D origin);
	static Vec2D stretch(double x, double y, Vec2D vec);
	static Vec2D stretch(double x, double y, Vec2D vec, Vec2D origin);
};

class Color_HSV;

class Color_RGB {
public:
	int r, g, b;

	Color_RGB();
	Color_RGB(int r, int g, int b);
	Color_RGB operator*(const double &d);

	Color_HSV toHSV();
	unsigned int toColor() { return GetColor(this->r, this->g, this->b); }
};

class Color_HSV {
public:
	int h, s, v;

	Color_HSV();
	Color_HSV(int h, int s, int v);

	Color_RGB toRGB();
	Color_HSV rotate(int addAngle);
};

int Round(double x);
bool fileExistT(char* filepath);//テキスト
bool fileExistB(char* filepath);//バイナリ

class RequiredFunc {
public:
	virtual void initialize() = 0;
	virtual void update() = 0;
	virtual void finalize() {};
};