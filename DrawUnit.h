#ifndef _DrawUnit_
#define _DrawUnit_


#include "GeomUnit.h"


// Параметры изображения.
struct SCREEN
{
public:
	char	*caption;				// Заголовок окна.
	WORD	left;					// Позиция окна по вертикали.
	WORD	top;					// Позиция окна по горизонтали.
	WORD	width;					// Размер окна по вертикали.
	WORD	height;					// Размер окна по горизонтали.
	int		bits;					// Глубина цвета.
	bool	fullScreen;				// Флаг полноэкранного режима.
};
// Параметры изображения.


extern HWND		hWnd;				// Дескриптор окна.
extern SCREEN	Screen;				// Параметры изображения. TODO: в конфиг игры.

extern double	ViewDistance;		// Расстояние для матрицы вида в сферицеских координатах.
extern double	ViewZenit;			// Зенитный угол для матрицы вида в сферицеских координатах.
extern double	ViewAzimut;			// Азимутный угол для матрицы вида в сферицеских координатах.


extern bool CreateOGLWindow();								// Создение окна.
extern void ChangeScreenMode();								// Изменение видео режима.
extern void DestroyOGLWindow();								// Уничтожение окна.
extern void ReSizeGLScene(GLsizei width, GLsizei height);	// Изменение размеров окна.
extern void DrawGLScene();									// Отрисовка сцены.

extern void OpenGLGameCreate();								// Настройка графических параметров перед созданием игры.


#endif