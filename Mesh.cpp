#include "stdafx.h"

#include "Mesh.h"


// Материалы.
// Конструктор.
Materials::Materials(string fileName): numMaterial(0), mapMaterial(), materials(NULL)
{
	// Индекс материала.
	BYTE idxMaterial = -1;

	// Открытие файла для чтения. 
	fstream fMaterial("data//" + fileName, ios_base::in);
	// Если файл не открыт.
	if (!fMaterial.is_open())
	{
		// TODO: Ошибка в конструкторе.
	}

	string	line			= "";	// Строка файла.
	DWORD	firstSpacePos	= 0;	// Позиция первого пробела строки.
	string	word			= "";	// Слово строки.
	// Пока не конец файла.
	while (!fMaterial.eof())
	{
		// Чтение строки файла.
		getline(fMaterial, line);

		// Если пустая строка.
		if (line.empty())
		{
			continue;
		}
		// Если коментарий.
		if (line[0] == '#')
		{
			continue;
		}
		// Пока последний символ строки - пробел.
		while (line[line.length() - 1] == ' ')
		{
			// Удаляем последний символ строки.
			line.erase(line.length() - 1);
		}

		firstSpacePos	= line.find_first_of(' ');			// Вычисление позиции первого пробела строки.
		word			= line.substr(0, firstSpacePos);	// Вычисление слова строки.

		// Если материал.
		if (word == "newmtl")
		{
			// Если конец строки.
			if (firstSpacePos == line.length())
			{
				// TODO: Ошибка в конструкторе.
			}
			word = line.substr(firstSpacePos + 1, line.length());	// Имя материала.
			this->mapMaterial[word] = this->numMaterial++;			// Созданение соответствия названия материала его индексу.
			continue;
		}
	}

	this->materials = new Material[this->numMaterial];	// Выделение памяти под материалы.

	// Очистка состояния файла.
	fMaterial.clear();
	// Установка позиции в начало файла.
	fMaterial.seekg(ios_base::beg);
	// Пока не конец файла.
	while (!fMaterial.eof())
	{
		// Чтение строки файла.
		getline(fMaterial, line);

		// Если пустая строка.
		if (line.empty())
		{
			continue;
		}
		// Если коментарий.
		if (line[0] == '#')
		{
			continue;
		}
		// Пока последний символ строки - пробел.
		while (line[line.length() - 1] == ' ')
		{
			// Удаляем последний символ строки.
			line.erase(line.length() - 1);
		}
		
		firstSpacePos	= line.find_first_of(' ');			// Вычисление позиции первого пробела строки.
		word			= line.substr(0, firstSpacePos);	// Вычисление слова строки.

		// Если материал.
		if (word == "newmtl")
		{
			idxMaterial++;
			continue;
		}

		// Если фоновый свет.
		if (word == "Ka")
		{
			// Чтение фонового света.
			if (sscanf
				(
					line.c_str(),
					"Ka %f %f %f",
					&(this->materials[idxMaterial].ambient[0]),		// Красная составляющая.
					&(this->materials[idxMaterial].ambient[1]),		// Синяя составляющая.
					&(this->materials[idxMaterial].ambient[2])		// Зеленая составляющая.
				) != 3)
			{
				// TODO: ошибка при чтении.
			}
			// По умолчанию, если параметр d, определяющий прозрачность материала, не указан явно, он принимается за 1.0.
			&(this->materials[idxMaterial].ambient[3] = 1);			// Альфа-канал.
			continue;
		}
		// Если рассеянный свет.
		if (word == "Kd")
		{
			// Чтение рассеянного света.
			if (sscanf
				(
					line.c_str(),
					"Kd %f %f %f",
					&(this->materials[idxMaterial].diffuse[0]),		// Красная составляющая.
					&(this->materials[idxMaterial].diffuse[1]),		// Синяя составляющая.
					&(this->materials[idxMaterial].diffuse[2])		// Зеленая составляющая.
				) != 3)
			{
				// TODO: ошибка при чтении.
			}
			// По умолчанию, если параметр d, определяющий прозрачность материала, не указан явно, он принимается за 1.0.
			&(this->materials[idxMaterial].diffuse[3] = 1);			// Альфа-канал.
			continue;
		}
		// Если отраженный свет.
		if (word == "Ks")
		{
			// Чтение отраженного света.
			if (sscanf
				(
					line.c_str(),
					"Ks %f %f %f",
					&(this->materials[idxMaterial].specular[0]),	// Красная составляющая.
					&(this->materials[idxMaterial].specular[1]),	// Синяя составляющая.
					&(this->materials[idxMaterial].specular[2])		// Зеленая составляющая.
				) != 3)
			{
				// TODO: ошибка при чтении.
			}
			// По умолчанию, если параметр d, определяющий прозрачность материала, не указан явно, он принимается за 1.0.
			&(this->materials[idxMaterial].specular[3] = 1);			// Альфа-канал.
			continue;
		}
		// Если мягкость блика.
		if (word == "Ns")
		{
			// Чтение мягкости блика.
			if (sscanf
				(
					line.c_str(),
					"Ns %f",
					&(this->materials[idxMaterial].shininess)
				) != 1)
			{
				// TODO: ошибка при чтении.
			}
			continue;
		}
	}
}

// Деструктор.
Materials::~Materials()
{
	//TODO: реализовать деструктор.
}
// Материалы.

// Модель.
// Конструктор.
Model::Model(string fileName): materials(NULL), numTriangles(NULL)
{
	DWORD		numVertex		= 0;	// Количество вершин.
	DWORD		numNormal		= 0;	// Количество нормалей.

	DWORD		idxVertex		= 0;	// Индекс вершины.
	DWORD		idxNormal		= 0;	// Индекс нормали.
	DWORD		*idxTriangles	= 0;	// Индексы треугольников каждого материала.

	Decart3		*vertices		= NULL;	// Вершины модели.
	Decart3		*normals		= NULL;	// Нормали модели.
	Triangle	**triangles		= NULL;	// Треугольники модели.

	DWORD		curMaterial		= 0;	// Текущий материал.

	// Открытие файла для чтения. 
	fstream fModel("data//" + fileName, ios_base::in);
	// Если файл не открыт.
	if (!fModel.is_open())
	{
		// TODO: TODO: Ошибка в конструкторе.
	}

	string	line			= "";	// Строка файла.
	DWORD	firstSpacePos	= 0;	// Позиция первого пробела строки.
	string	word			= "";	// Слово строки.
	// Пока не конец файла.
	while (!fModel.eof())
	{
		// Чтение строки файла.
		getline(fModel, line);

		// Если пустая строка.
		if (line.empty())
		{
			continue;
		}
		// Если коментарий.
		if (line[0] == '#')
		{
			continue;
		}
		// Пока последний символ строки - пробел.
		while (line[line.length() - 1] == ' ')
		{
			// Удаляем последний символ строки.
			line.erase(line.length() - 1);
		}

		firstSpacePos	= line.find_first_of(' ');			// Вычисление позиции первого пробела строки.
		word			= line.substr(0, firstSpacePos);	// Вычисление слова строки.

		// Если файл с материалами.
		if (word == "mtllib")
		{
			// Если материалы не созданы.
			if (this->materials == NULL)
			{
				// Если конец строки.
				if (firstSpacePos == line.length())
				{
					// TODO: Ошибка в конструкторе.
				}
				word = line.substr(firstSpacePos + 1, line.length());	// Имя файла, содержащего материал.
				// Создание материалов.
				this->materials = new Materials(word);
				// Если материалы не созданны.
				if (this->materials == NULL)
				{
					// TODO: Ошибка в конструкторе.
				}
				// Выделение памяти под количество треугольников каждого материала.
				this->numTriangles = new DWORD[this->materials->numMaterial];
				// Очистка выделенной памяти.
				memset(this->numTriangles, 0, sizeof(DWORD) * this->materials->numMaterial);
				// Выделение памяти под индексы треугольников каждого материала.
				idxTriangles = new DWORD[this->materials->numMaterial];
				// Очистка выделенной памяти.
				memset(idxTriangles, 0, sizeof(DWORD) * this->materials->numMaterial);
			}
			continue;
		}
		// Если установка материала.
		if (word == "usemtl")
		{
			// Если материалы не созданны.
			if (this->materials != NULL)
			{
				// TODO: TODO: Ошибка в конструкторе.
			}
			// Если конец строки.
			if (firstSpacePos == line.length())
			{
				// TODO: Ошибка в конструкторе.
			}
			word = line.substr(firstSpacePos + 1, line.length());	// Название материала.
			
			// Если индекс по названию материала не найден.
			if (this->materials->mapMaterial.find(word) == this->materials->mapMaterial.end())
			{
				// TODO: Ошибка в конструкторе.
			}
			// Установка текущего материала.
			curMaterial = this->materials->mapMaterial[word];
			continue;
		}
		// Если вершина.
		if (word == "v")
		{
			// Инкрементиция количества вершин.
			numVertex++;
			continue;
		}
		// Если нормаль.
		if (word == "vn")
		{
			// Инкрементиция количества нормалей.
			numNormal++;
			continue;
		}
		// Если трегольник.
		if (word == "f")
		{
			//// Инкрементиция количества треугольников.
			//numTriangle++;
			// Инкрементация количества треугольников текущего материала.
			this->numTriangles[curMaterial]++;
			continue;
		}
	}

	vertices	= new Decart3[numVertex];		// Выделение памяти под вершины модели.
	normals		= new Decart3[numNormal];		// Выделение памяти под нормали модели.
	triangles	= new Triangle *[this->materials->numMaterial];	// Выделение памяти под треугольники.
	// Перебор материалов.
	for (BYTE idxMaterial = 0; idxMaterial != this->materials->numMaterial; idxMaterial++)
	{
		// Выделение памяти под треугольники каждого материала.
		triangles[idxMaterial] = new Triangle[this->numTriangles[idxMaterial]];
	}

	// TODO: дублирование кода при первом и втором чтении файла. Нужно ли совместить?

	// Очистка состояния файла.
	fModel.clear();
	// Установка позиции в начало файла.
	fModel.seekg(ios_base::beg);
	// Пока не конец файла.
	while (!fModel.eof())
	{
		// Чтение строки файла.
		getline(fModel, line);

		// Если пустая строка.
		if (line.empty())
		{
			continue;
		}		
		// Если коментарий.
		if (line[0] == '#')
		{
			continue;
		}
		// Пока последний символ строки - пробел.
		while (line[line.length() - 1] == ' ')
		{
			// Удаляем последний символ строки.
			line.erase(line.length() - 1);
		}
		
		firstSpacePos	= line.find_first_of(' ');			// Вычисление позиции первого пробела строки.
		word			= line.substr(0, firstSpacePos);	// Вычисление слова строки.

		// Если установка материала.
		if (word == "usemtl")
		{
			// Если материалы не созданны.
			if (this->materials != NULL)
			{
				// TODO: TODO: Ошибка в конструкторе.
			}
			// Если конец строки.
			if (firstSpacePos == line.length())
			{
				// TODO: Ошибка в конструкторе.
			}
			word = line.substr(firstSpacePos + 1, line.length());	// Название материала.
			
			// Если индекс по названию материала не найден.
			if (this->materials->mapMaterial.find(word) == this->materials->mapMaterial.end())
			{
				// TODO: Ошибка в конструкторе.
			}
			// Установка текущего материала.
			curMaterial = this->materials->mapMaterial[word];
			continue;
		}
		// Если вершина.
		if (word == "v")
		{
			// Чтение вершины.
			if (sscanf
				(
					line.c_str(),
					"v %f %f %f",
					&(vertices[idxVertex].coorX),
					&(vertices[idxVertex].coorY),
					&(vertices[idxVertex].coorZ)
				) != 3)
			{
				// TODO: ошибка при чтении.
			}
			// Инкрементация счетчика вершин.
			idxVertex++;
			continue;
		}
		// Если нормаль.
		if (word == "vn")
		{
			// Чтение нормали.
			if (sscanf
				(
					line.c_str(),
					"vn %f %f %f",
					&(normals[idxNormal].coorX),
					&(normals[idxNormal].coorY),
					&(normals[idxNormal].coorZ)
				) != 3)
			{
				// TODO: ошибка при чтении.
			}
			// Инкрементация счетчика нормалей.
			idxNormal++;
			continue;
		}
		// Если трегольник.
		if (word == "f")
		{
			// Чтение трегольника.
			if (sscanf
				(
					line.c_str(),
					"f %d//%d %d//%d %d//%d",
					&(triangles[curMaterial][idxTriangles[curMaterial]].idxVertex[0]),
					&(triangles[curMaterial][idxTriangles[curMaterial]].idxNormal[0]),
					&(triangles[curMaterial][idxTriangles[curMaterial]].idxVertex[1]),
					&(triangles[curMaterial][idxTriangles[curMaterial]].idxNormal[1]),
					&(triangles[curMaterial][idxTriangles[curMaterial]].idxVertex[2]),
					&(triangles[curMaterial][idxTriangles[curMaterial]].idxNormal[2])
				) != 6)
			{
 				printf("triagle read error");
				// TODO: ошибка при чтении.
			}
			// Инкрементация счетчика треугольников текущего материала.
			idxTriangles[curMaterial]++;
			continue;
		}
	}

	// Закрытие файла.
	fModel.close();

	// Выделение памяти под обработанные вершины и нормали модели.
	this->vertices	= new float *[this->materials->numMaterial];
	this->normals	= new float *[this->materials->numMaterial];
	for (DWORD idxMaterial = 0; idxMaterial != this->materials->numMaterial; idxMaterial++)
	{
		this->vertices[idxMaterial]	= new float[3 * 3 * numTriangles[idxMaterial]];
		this->normals[idxMaterial]	= new float[3 * 3 * numTriangles[idxMaterial]];
	}
	
	// Перебор материалов.
	for (DWORD idxMaterial = 0; idxMaterial != this->materials->numMaterial; idxMaterial++)
	{
		// Индекс индексов.
		DWORD idxIndex = 0;

		// Перебор треугольников материала.
		for (DWORD idxTriangle = 0; idxTriangle != this->numTriangles[idxMaterial]; idxTriangle++)
		{
			// Перебор вершин треугольника.
			for (BYTE idxVertex = 0; idxVertex != 3; idxVertex++)
			{
				this->vertices[idxMaterial][idxIndex]	= vertices[triangles[idxMaterial][idxTriangle].idxVertex[idxVertex] - 1].coorX;
				this->normals[idxMaterial][idxIndex]	= normals[triangles[idxMaterial][idxTriangle].idxNormal[idxVertex] - 1].coorX;
				idxIndex++;

				this->vertices[idxMaterial][idxIndex]	= vertices[triangles[idxMaterial][idxTriangle].idxVertex[idxVertex] - 1].coorY;
				this->normals[idxMaterial][idxIndex]	= normals[triangles[idxMaterial][idxTriangle].idxNormal[idxVertex] - 1].coorY;
				idxIndex++;

				this->vertices[idxMaterial][idxIndex]	= vertices[triangles[idxMaterial][idxTriangle].idxVertex[idxVertex] - 1].coorZ;
				this->normals[idxMaterial][idxIndex]	= normals[triangles[idxMaterial][idxTriangle].idxNormal[idxVertex] - 1].coorZ;
				idxIndex++;
			}
		}	
	}

	// TODO: Удаление вдумерных массивов с цикле.
	delete [] idxTriangles;	// Освобождение памяти от индексов треугольников каждого материала.
	delete [] vertices;		// Освобождение памяти от вершин модели.
	delete [] normals;		// Освобождение памяти от нормалей модели.
	delete [] triangles;	// Освобождение памяти от треугольников модели.
}

// Деструктор.
Model::~Model()
{
	// Если материалы модели существуют.
	if (this->materials != NULL)
	{
		// Освобождение памяти от материалов модели.
		delete this->materials;
	}

	// Если количество треугольникоу каждого материала существуюет.
	if (this->numTriangles != NULL)
	{
		// Освобождение памяти от количества треугольников каждого материала.
		delete this->numTriangles;
	}
	
	delete [] this->vertices;	// Освобождение памяти от обработанных вершин модели.
	delete [] this->normals;	// Освобождение памяти от обработанных нормалей модели.
}
// Модель.


string	ModelFileName[] =								// Массив имен файлов моделей.
{
	"lightcycle-low.obj",								// Световой мотоцикл низкого качества.
	"lightcycle-med.obj",								// Световой мотоцикл среднего качества.
	"lightcycle-high.obj"								// Световой мотоцикл высокого качества.
};

Model	*Models[ModelIndexCount]	= {};				// Модели.


// Загрузка моделей.
bool LoadModels()
{
	// Перебор всех моделей.
	for (BYTE idxModel = 0; idxModel != ModelIndexCount; idxModel++)
	{
		// Создание модели.
		Models[idxModel] = new Model(ModelFileName[idxModel]);
		// Если модель не создана.
		if (Models[idxModel] == NULL)
		{
			return false;
		}
	}
	return true;
}

// Выгрузка моделей.
void UnLoadModels()
{
	// Перебор всех моделей.
	for (BYTE idxModel = 0; idxModel != ModelIndexCount; idxModel++)
	{
		// Если модель существует.
		if (Models[idxModel] != NULL)
		{
			// Удаление модели.
			delete Models[idxModel];
		}		
	}
}