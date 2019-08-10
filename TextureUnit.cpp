#include "stdafx.h"

#include "TextureUnit.h"


// Загрузка изображения из BMP-файла.
BYTE *LoadBitmapImage(string fileName, BITMAPINFOHEADER *bitmapInfoHeader)
{
	// TODO: BM80/BM6 - заголовки файлов. Проверка версии.
	//BITMAPV4HEADER
	//BI_JPEG
	//HBITMAP
	//LoadImage

	FILE				*ptrFile			= NULL;		// Указатель на файл.
	BITMAPFILEHEADER	bitmapFileHeader	= {};		// Заголовок bitmap-файла.
	BYTE				*imgBuffer			= NULL;		// Указатель на данные изображения.
	
	// Открытие файла.
	ptrFile = fopen(fileName.c_str(), "rb");
	// Если при открытии файла произошла ошибака.
	if (ptrFile == NULL)
	{
		return NULL;
	}
	
	// Чтение заголовка bitmap-файла.
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, ptrFile);
	
	// Если тип файла не равен универсальному идентификатору bitmap-файла ("BM").
	if (bitmapFileHeader.bfType != 0x4D42)
	{
		// Закрытие файла.
		fclose(ptrFile);
		return NULL;
	}
	
	// Чтение информационного заголовка bitmap-файла.
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, ptrFile);
	// Если формат не 24 бит на пиксель.	
	if (bitmapInfoHeader->biBitCount != 0x18)
	{
		// Закрытие файла.
		fclose(ptrFile);
		return NULL;
	}
	
	// Перемещение указателя файла на начало данных изображения.
	fseek(ptrFile, bitmapFileHeader.bfOffBits, SEEK_SET);
	
	// Хуй знает что. Соблюдать стандарты надо!!
	bitmapInfoHeader->biSizeImage = 3 * bitmapInfoHeader->biWidth * bitmapInfoHeader->biHeight;

	// Выделение памяти под данные изображения.
	imgBuffer = new BYTE[bitmapInfoHeader->biSizeImage];
	// Если во время выделения памяти произошла ошибка.
	// TODO: обернуть ВСЕ new в исключения. необходимости в проверки указателя на NULL нет - спецификация.
	//if (!bitmapImage)
	//{
	//  free(bitmapImage);
	//  // Закрытие файла.
	//  fclose(ptrFile);
	//  return NULL;
	//}
	
	// Чтение данных изображения.
	fread(imgBuffer, 1, bitmapInfoHeader->biSizeImage, ptrFile);
	// TODO: if (bitmapInfoHeader->biSizeImage != readCount)
	// make sure bitmap image data was read
	// Если данные не прочитаны.
	if (imgBuffer == NULL)
	{
		// Закрытие файла.
		fclose(ptrFile);
		return NULL;
	}
	
	// Закрытие файла.
	fclose(ptrFile);

	return imgBuffer;
} 
// Выгрузка BMP-файла.
void UnLoadBitmapImage(BYTE *imgBuffer)
{
	// Освобождение памяти от данных изображения.
	delete[] imgBuffer;
}

// Загрузка текстуры.
bool LoadTexture(string fileName, GLuint &textureID)
{
	//// Изменение размера изображения.
	//gluScaleImage(GL_RGB, imgTexture->sizeX, imgTexture->sizeY, GL_UNSIGNED_BYTE, imgTexture->data, 1, 1, GL_UNSIGNED_BYTE, imgTexture->data);	
	
	BITMAPINFOHEADER	bitmapInfoHeader	= {};		// Заголовок bitmap-файла.
	BYTE				*imgBuffer			= NULL;		// Буфер изображения.

	// Загрузка изображения в буфер из bitmap-файла.
	imgBuffer = LoadBitmapImage("data\\" + fileName, &bitmapInfoHeader);
	// Если при загрузки файла произошла ошибка.
	if (imgBuffer == NULL)
	{
		return false;
	}
	
	// Генерация уникального идентификатора для одной текстуры.
	glGenTextures(1, &textureID);
	// Выбор текстуры.
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Определение типа фильтарации для сжатия текстуры.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Определение типа фильтарации для растяжения текстуры.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	// Создание текстуры.
//	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, imgBuffer);
	// Создание двумерной карты текстуры.
	GLint result = gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, imgBuffer);

	// Выгрузка изображения из буфера.
	UnLoadBitmapImage(imgBuffer);

	// Возврат значения в зависимости от результата сзднания двумерной карты текстуты.
	return (result == 0);
}
// Выгрузка текстуры.
void UnLoadTexture(GLuint &textureID)
{
	// Удаление одной текстуры.
	glDeleteTextures(1, &textureID);
}


string	TextureFileName[] =					// Массив имен файлов текстур.
{
	"Grid.bmp",								// Текстура сетки.
	"WallCycle.bmp",						// Текстута стены светового мотоцикла.
	"Wall1.bmp",							// Текстута стены слева.
	"Wall2.bmp",							// Текстута стены спереди.
	"Wall3.bmp",							// Текстута стены справа.
	"Wall4.bmp",							// Текстута стены сзади.
	"Font.bmp"								// Текстута шрифта.
};

GLuint	Textures[TextureIndexCount]	= {};	// Текстуры.


// Загрузка текстур.
bool LoadGLTextures()
{
	// Перебор всех теустур.
	for (BYTE idxTexture = 0; idxTexture != TextureIndexCount; idxTexture++)
	{
		// Если при загрузке текстуры произошла ошибка.
		if (!LoadTexture(TextureFileName[idxTexture], Textures[idxTexture]))
		{
			return false;
		}
	}
	return true;
}

// Выгрузка текстур.
void UnLoadGLTextures()
{
	// Перебор всех теустур.
	for (BYTE idxTexture = 0; idxTexture != TextureIndexCount; idxTexture++)
	{
		// Если текстура существует.
		if (Textures[idxTexture] != NULL)
		{
			// Выгрузка текстуры.
			UnLoadTexture(Textures[idxTexture]);
		}
	}
}