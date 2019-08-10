#ifndef _MeshlUnit_
#define _MeshUnit_


#include "stdafx.h"


// Материалы.
class Materials
{
	// Материал.
	struct Material
	{
		float	ambient[4];				// Отражение фонового света.
		float	diffuse[4];				// Отражение дифузного света.
		float	specular[4];			// Отражение зеркального света.
		float	shininess;				// Мягкость блика.
	};

public:
	BYTE				numMaterial;	// Количество материалов.
	map<string, BYTE>	mapMaterial;	// Соответствия названий материалов их индексу.
	Material			*materials;		// Материалы.

	Materials(string fileName);			// Конструктор.
	~Materials();						// Деструктор.
};

// Модель.
class Model
{
public:
	struct Decart3							// Трехмерная декартова координата.
	{
		float	coorX;						// Координата X.
		float	coorY;						// Координата Y.
		float	coorZ;						// Координата Z.
	};

	struct Triangle							// Треугольник.
	{
		DWORD idxVertex[3];					// Индексы вершин.
		DWORD idxNormal[3];					// Индексы нормалей.
	};

	Materials	*materials;					// Материалы модели.

	DWORD		*numTriangles;				// Количество треугольников каждого материала.
	//Decart3		*vertices;				// Вершины модели.
	//Decart3		*normals;				// Нормали модели.
	float		**vertices;					// Вершины модели.
	float		**normals;					// Нормали модели.
//	Triangle	**indexes;					// Индексы каждого материала.

	Model(string fileName);					// Конструктор.
	~Model();								// Деструктор.
};

// Сетка.
class Mesh
{
};

enum										// Перечисление индексов моделей.
{
	ModelIndexLightCycleLow			= 0,	// Световой мотоцикл низкого качества.
	ModelIndexLightCycleMed			= 1,	// Световой мотоцикл среднего качества.
	ModelIndexLightCycleHigh		= 2,	// Световой мотоцикл высокого качества.
	ModelIndexCount					= 3		// Количество моделей.
};


extern	Model	*Models[ModelIndexCount];	// Модели.


extern	bool	LoadModels();				// Загрузка моделей.
extern	void	UnLoadModels();				// Выгрузка моделей.


#endif