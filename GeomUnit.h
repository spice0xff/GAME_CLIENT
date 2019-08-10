#ifndef _GeomUnit_
#define _GeomUnit_


#include "stdafx.h"


// Класс точка на плоскости.
class Point2
{
public:
	Point2();										// Конструктор по умолчанию.
	Point2(double dX, double dY);					// Конструктор.

	double	dX;										// Координата X.
	double	dY;										// Координата Y.

	void	Set(double dX, double dY);				// Установка координат.
	int		dXi();									// Возврат координаты X.
	int		dYi();									// Возврат координаты Y.
	
	bool	operator ==(const Point2 &Point2);		// Оперетор сравнения на равентсво.
	bool	operator !=(const Point2 &Point2);		// Оперетор сравнения на неравентсво.
	Point2	operator +(const Point2 &Point2);		// Оператор сложения.
	Point2	operator -(const Point2 &Point2);		// Оператор вычитания.
	void	operator +=(const Point2 &Point2);		// Оператор увелечения на заданную велечину.
	void	operator -=(const Point2 &Point2);		// Оператор уменьшения на заданную велечину.
};
// Класс точка на плоскости.

// Класс вектор в плоскости.
class Vector2: public Point2
{
public:
	Vector2();										// Конструктор по умолчанию.
	Vector2(double dX, double dY);					// Конструктор.

	void	SetDX(double dX);						// Установка координаты X.
	double	GetDX();								// Получение координаты X.
	void	ChangeDX(double dX);					// Изменение координаты X.

	void	SetDY(double dY);						// Установка координаты Y.
	double	GetDY();								// Получение координаты Y.
	void	ChangeDY(double dY);					// Изменение координаты Y.

	void	SetAngle(double angle);					// Установка угла.
	double	GetAngle();								// Получение угла.
	void	Rotate(double angle);					// Поворот на угол.

	void	SetModule(double module);				// Установка модуля.
	double	GetModule();							// Получения модуля.
	void	Resize(double module);					// Маштабирование модуля.

	bool	operator ==(Vector2 vector2);			// Оперетор сравнения на равентсво.
	bool	operator !=(Vector2 vector2);			// Оперетор сравнения на неравентсво.
	Vector2	operator +(Vector2 vector2);			// Оператор сложения.
	Vector2	operator -(Vector2 vector2);			// Оператор вычитания.
	void	operator +=(Vector2 vector2);			// Оператор увелечения на заданную велечину.
	void	operator -=(Vector2 vector2);			// Оператор уменьшения на заданную велечину.
	void	operator +=(Point2 point2);				// Оператор увелечения на заданную велечину. Перегружанный.
	void	operator -=(Point2 point2);				// Оператор уменьшения на заданную велечину. Перегружанный.
	Vector2	operator *(double kf);					// Оператор умножения.
	Vector2	operator /(double kf);					// Оператор деления.
	void	operator *=(double kf);					// Оператор умножения на заданную велечину.
	void	operator /=(double kf);					// Оператор деления на заданную велечину.

	void	SetPolar(double angle, double module);	// Установка полярных координат.

protected:	
	void	SetDecar();								// Расчет и установка декартовых координат по полярным.
	void	SetPolar();								// Расчет и установка полярных координат по декартовым.

	double	angle;									// Угол.
	double	module;									// Модуль.
};
// Класс вектор в плоскости.


// Проецирование векрота в новой системе координат.
void ProjectionVector2(Vector2 *Vector, double dAngle, Vector2 *newVector, Vector2 *xVector, Vector2 *yVector);


#endif