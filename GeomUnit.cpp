#include "stdafx.h"

#include "GeomUnit.h"

// Класс точка на плоскости.
// Конструктор по умолчанию.
Point2::Point2(): dX(0), dY(0)
{
}
// Конструктор.
Point2::Point2(double dX, double dY): dX(dX), dY(dY)
{
}

// Установка координат.
void Point2::Set(double dX, double dY)
{
	this->dX = dX;
	this->dY = dY;
}

// Возврат координаты X.
int Point2::dXi()
{
	return int(this->dX);
}
// Возврат координаты Y.
int Point2::dYi()
{
	return int(this->dY);
}

// Оператор сравнения равно.
bool Point2::operator ==(const Point2 &point2)
{
	return (this->dX == point2.dX && this->dY == point2.dY);
}
// Оператор сравниея не равно.
bool Point2::operator !=(const Point2 &point2)
{
	return (this->dX != point2.dX || this->dY != point2.dY);
}

// Оператор сложения.
Point2 Point2::operator +(const Point2 &point2)
{
	return Point2(this->dX + point2.dX, this->dY + point2.dY);
}
// Оператор вычитания.
Point2 Point2::operator -(const Point2 &point2)
{
	return Point2(this->dX - point2.dX, this->dY - point2.dY);
}

// Оператор увелечения на заданную велечину.
void Point2::operator +=(const Point2 &point2)
{
	this->dX += point2.dX;
	this->dY += point2.dY;
}
// Оператор уменьшения на заданную велечину.
void Point2::operator -=(const Point2 &point2)
{
	this->dX -= point2.dX;
	this->dY -= point2.dY;
}
// Класс точка на плоскости.

// Класс вектор в плоскости.
// Конструктор по умолчанию.
Vector2::Vector2(): Point2()
{
}
// Конструктор.
Vector2::Vector2(double dX, double dY): Point2(dX, dY)
{
	this->SetPolar();
}

// Установка координаты X.
void Vector2::SetDX(double dX)
{
	this->dX = dX;
	this->SetPolar();
}
// Получение координаты X.
double Vector2::GetDX()
{
	return this->dX;
}
// Изменение координаты X.
void Vector2::ChangeDX(double dX)
{
	this->dX += dX;
	this->SetPolar();
}

// Установка координаты Y.
void Vector2::SetDY(double dY)
{
	this->dY = dY;
	this->SetPolar();
}
// Получение координаты Y.
double Vector2::GetDY()
{
	return this->dY;
}
// Изменение координаты Y.
void Vector2::ChangeDY(double dY)
{
	this->dY += dY;
	this->SetPolar();
}

// Установка угла.
void Vector2::SetAngle(double angle)
{
	//// Пока угол больше 360 градусов
	//while (angle > 2*M_PI)
	//{
	//	angle -= M_PI;
	//}

	//// Пока угол меньше -360 градусов
	//while (angle < -2 * M_PI)
	//{
	//	angle += M_PI;
	//}
	
	this->angle = angle;
	this->SetDecar();
}
// Получение угла.
double Vector2::GetAngle()
{
	return this->angle;
}
// Поворот на угол.
void Vector2::Rotate(double angle)
{
	this->angle += angle;
	this->SetDecar();
}

// Установка модуля.
void Vector2::SetModule(double module)
{
	this->module = module;
	this->SetDecar();
}
// Получения модуля.
double Vector2::GetModule()
{
	return this->module;
}
// Маштабирование модуля.
void Vector2::Resize(double module)
{
	this->module += module;
	this->SetDecar();
}

// Оперетор сравнения на равентсво.
bool Vector2::operator ==(Vector2 vector2)
{
	return (this->dX == vector2.dX && this->dY == vector2.dY);
}
// Оперетор сравнения на неравентсво.
bool Vector2::operator !=(Vector2 vector2)
{
	return (this->dX != vector2.dX || this->dY != vector2.dY);
}

// Оператор сложения.
Vector2 Vector2::operator +(Vector2 vector2)
{
	return Vector2(this->dX + vector2.dX, this->dY + vector2.dY);
}
// Оператор вычитания.
Vector2 Vector2::operator -(Vector2 vector2)
{
	return Vector2(this->dX + vector2.dX, this->dY + vector2.dY);
}

// Оператор увелечения на заданную велечину.
void Vector2::operator +=(Vector2 vector2)
{
	this->dX += vector2.dX;
	this->dY += vector2.dY;
	this->SetPolar();
}
// Оператор уменьшения на заданную велечину.
void Vector2::operator -=(Vector2 vector2)
{
	this->dX -= vector2.dX;
	this->dY -= vector2.dY;
	this->SetPolar();
}
// Оператор увелечения на заданную велечину. Перегружанный.
void Vector2::operator +=(Point2 point2)
{
	this->dX += point2.dX;
	this->dY += point2.dY;
	this->SetPolar();
}
// Оператор уменьшения на заданную велечину. Перегружанный.
void Vector2::operator -=(Point2 point2)
{
	this->dX -= point2.dX;
	this->dY -= point2.dY;
	this->SetPolar();
}

// Оператор умножения.
Vector2 Vector2::operator *(double kf)
{
	return Vector2(this->dX * kf, this->dY * kf);
}
// Оператор деления.
Vector2 Vector2::operator /(double kf)
{
	if (kf == 0)
	{
		printf("Vector2 Vector2::operator /(double kf)\nkf == 0");
		return Vector2(0, 0);
	}
	return Vector2(this->dX / kf, this->dY / kf);
}

// Оператор умножения на заданную велечину.
void Vector2::operator *=(double kf)
{
	this->module *= kf;
	this->SetDecar();
}
// Оператор деления на заданную велечину.
void Vector2::operator /=(double kf)
{
	this->module /= kf;
	this->SetDecar();
}

// Установка полярных координат.
void Vector2::SetPolar(double angle, double module)
{
	this->angle = angle;
	this->module = module;
	SetDecar();
}

// Расчет и установка декартовых координат по полярным.
void Vector2::SetDecar()
{
	this->dX = this->module * cos(this->angle);
	this->dY = this->module * sin(this->angle);
}
// Расчет и установка полярных координат по декартовым.
void Vector2::SetPolar()
{
	if (this->dY >= 0)
	{
		this->angle = atan2f(this->dY, this->dX);
	}
	else
	{
		this->angle = atan2f(this->dY, this->dX) + 2 * M_PI;
	}
	// TODO: добавить intPoint2 и doublePoint2
	//this->module = sqrt(this->dX * this->dX + this->dY * this->dY); 
}
// Класс вектор в плоскости.

// Проецирование векрота в новой системе координат.
void ProjectionVector2(Vector2 *vector, double dAngle, Vector2 *newVector, Vector2 *xVector, Vector2 *yVector)
{
	// Угол поворота новой системы координат относительно абсолютной. 
	double newAngle = vector->GetAngle() - dAngle;
	
	// Вектор в новой системе координат.
	newVector->SetPolar(newAngle, vector->GetModule());
	
	// Проекция вектора в новой системе координат на ось X, в абсолютной системе координат.
	xVector->SetDX(newVector->GetDX() * cos(dAngle));
	xVector->SetDY(newVector->GetDX() * sin(dAngle));
	
	// Проекция вектора в новой системе координат на ось Y, в абсолютной системе координат.
	yVector->SetDX(newVector->GetDY() * cos(dAngle + M_PI / 2));
	yVector->SetDY(newVector->GetDY() * sin(dAngle + M_PI / 2));
}