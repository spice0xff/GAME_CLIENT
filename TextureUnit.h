#ifndef _TextureUnit_
#define _TextureUnit_


#include "stdafx.h"


enum TextureNames: BYTE						// Перечисление индексов текстур.
{
	TextureNamesGrid				= 0,	// Текстура сетки.
	TextureNamesWallCycle			= 1,	// Текстута стены светового мотоцикла.
	TextureNamesWall1				= 2,	// Текстута 1ой стены.
	TextureNamesWall2				= 3,	// Текстута 2ой стены.
	TextureNamesWall3				= 4,	// Текстута 3ей стены.
	TextureNamesWall4				= 5,	// Текстута 4ой стены.
	TextureNamesFont				= 6,	// Текстута шрифта.
	TextureIndexCount				= 7		// Количество текстур.
};


extern GLuint	Textures[TextureIndexCount];// Текстуры.


extern bool		LoadGLTextures();			// Загрузка текстур.
extern void		UnLoadGLTextures();			// Выгрузка текстур.

#endif