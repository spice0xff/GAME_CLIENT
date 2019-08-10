#include "stdafx.h"

#include "MainUnit.h"
#include "GUIUnit.h"
#include "GameUnit.h"
#include "DrawUnit.h"
#include "TextureUnit.h"
#include "Mesh.h"


#define MIN_DISTANCE	1								// Минимальное расстояние для матрицы вида в сферических координатах.
#define MAX_DISTANCE	300								// Максимальное расстояние для матрицы вида в сферических координатах.
#define MIN_ZENIT		-135							// Минимальный зенитный угол для матрицы вида в сферических координатах.
#define MAX_ZENIT		-45								// Максимальный зенитный угол для матрицы вида в сферических координатах.



class Window
{



};




// Трехкомпонентный цвет.
class RGBColor
{
public:
	RGBColor()											// Конструктор по умолчанию.
	{
	}
	
	RGBColor(double red, double green, double blue):	// Конструктор.
	red(red), green(green), blue(blue)
	{
	}

	double red;											// Красная составляющая.
	double green;										// Зеленая составляющая.
	double blue;										// Синяя составляющая.
};
// Трехкомпонентный цвет.

// Класс палитра.
class Palette
{
public:
	// Конструктор
	Palette(RGBColor *rangeColor, BYTE rangeCount, BYTE colorCount):
	colorMap(), count(), index()
	{
		if (rangeCount <= 1)
		{
			this->count = 1;
			this->colorMap = new RGBColor(0, 0, 0);

			return;
		}
		
		// Вычисление количества цветов в каждом диапазоне
		colorCount /= (rangeCount - 1);
		// Вычисление количества цветов во всех диапазонов
		this->count = colorCount * (rangeCount - 1);
		// Создание цветовой карты необходимого размера
		this->colorMap = new RGBColor[this->count];
		
		// Перебор цветовых диапазонов
		for (BYTE idxRange = 0; idxRange != rangeCount - 1; idxRange++)
		{
			// Вычисление необходимых цветовых приращений
			double deltaRed		= (rangeColor[idxRange + 1].red		- rangeColor[idxRange].red)		/ double(colorCount);
			double deltaGreen	= (rangeColor[idxRange + 1].green	- rangeColor[idxRange].green)	/ double(colorCount);
			double deltaBlue	= (rangeColor[idxRange + 1].blue	- rangeColor[idxRange].blue)	/ double(colorCount);

			// Пребор цветов перебираемого диапазона
			for (WORD idx = 0; idx != colorCount; idx++)
			{
				// Приращение цвета
				rangeColor[idxRange].red	+= deltaRed;
				rangeColor[idxRange].green	+= deltaGreen;
				rangeColor[idxRange].blue	+= deltaBlue;

				// Запись цвета
				this->colorMap[idxRange * colorCount + idx] = rangeColor[idxRange];

//				printf("%i: %f, %f, %f\n", idxRange * colorCount + idx, rangeColor[idxRange].red, rangeColor[idxRange].green, rangeColor[idxRange].blue);
			}
//			printf("\n");
//			getchar();
		}
	}

	// Получение цвета по индексу
	RGBColor GetColor(BYTE idx)
	{
		if (idx > this->count)
		{
			idx = this->count;
		}
		return this->colorMap[idx];
	}

	// Получение следующего цвета
	RGBColor NextColor()
	{
		if (this->index++ == this->count - 1)
		{
			this->index = 0;
		}
		return this->colorMap[this->index];
	}

private:
	RGBColor	*colorMap;							// Цветовая карта
	BYTE		count;								// Количество цветов в карте
	BYTE		index;								// Индекс цвета
};
// Класс палитра.


HINSTANCE	hInstance			= GetModuleHandle(NULL);	// Дескриптор приложения.
HWND		hWnd				= NULL;						// Дескриптор окна.
HDC			hDC					= NULL;						// Приватный контекст устройства GDI.
HGLRC		hRC					= NULL;						// Постоянный контекст рендеринга.

char		*wndClassName		= "MainWindow";				// Имя класса окна.

SCREEN		Screen				=							// Инициализация параметров изображения.
{
	"Caption",												// Заголовок окна.
	0,														// Позиция окна по вертикали.
	0,														// Позиция окна по горизонтали.
	800,													// Размер окна по вертикали.
	600,													// Размер окна по горизонтали.
	16,														// Глубина цвета.
	false													// Флаг полноэкранного режима.
};
DWORD		FPS					= 0;						// Количество кадров с секунду.

// TODO: в структуры Coor3
double		ViewDistance		= 50;						// Расстояние для матрицы вида в сферицеских координатах.
double		ViewZenit			= -60;						// Зенитный угол для матрицы вида в сферицеских координатах.
double		ViewAzimut			= 0;						// Азимутный угол для матрицы вида в сферицеских координатах.
//

Palette		*dynamicColor		= NULL;						// Указатель на динамический цвет.
RGBColor	playersColor[6]		=							// Палитра для отрисовки игроков.
{
	RGBColor(1.0, 0.0, 0.0),
	RGBColor(0.0, 1.0, 0.0),
	RGBColor(1.0, 1.0, 0.0),
	RGBColor(0.0, 0.0, 1.0),
	RGBColor(1.0, 0.0, 1.0),
	RGBColor(0.0, 1.0, 1.0),
};

// Тень.
GLfloat		LightPosition[4]	= {};						// Позиция источкика света.
float		PlaneGrid[4]		= {};						// Плоскость сетки.
float		PlaneWall1[4]		= {};						// Плоскость первой стены.
float		PlaneWall2[4]		= {};						// Плоскость второй стены.
float		PlaneWall3[4]		= {};						// Плоскость третей стены.
float		PlaneWall4[4]		= {};						// Плоскость четвертой стены.
float		ShadowMatrix[16]	= {};						// Теневая матрица.
// Тень.

// Парамерты модели.
int			modelX				= 0;						// Координата X для матрицы модели.
int			modelY				= 0;						// Координата Y для матрицы модели.
int			modelZ				= 0;						// Координата Z для матрицы модели.
int			modelAzimut			= 0;						// Азимутный угол для матрицы модели.
// Парамерты модели.

// Парамерты камеры.
double		cameraX				= 0;
double		cameraY				= 0;
// Парамерты камеры.

// ArtPack
//map<BYTE, string> MapTextureNames;							// Карта соответствий текстур и файлов. TODO: инициализация?
// ArtPack


// Протопип функции инициализации графической подсистемы.
bool InitGL();
// Протопип функции деинициализации графической подсистемы.
void DeInitGL();


// Создание окна.
bool CreateOGLWindow()
{
	DWORD		dwStyle;					// Стиль окна.
//	DWORD		dwExStyle;					// Расширенный стиль окна.
	RECT		wr =						// Размер и позиция окна.
	{
		(long)0,
		(long)0,
		(long)Screen.width,
		(long)Screen.height
	};

	WNDCLASS	wc =						// Класс окна
	{
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC,	// Стиль окна (wc.style)
		(WNDPROC)WndProc,					// Процедура обработки сообщений (wc.lpfnWndProc)
		NULL,								// Нет дополнительной информации для окна (wc.cbClsExtra)
		NULL,								// Нет дополнительной информации для окна (wc.cbWndExtra)
		hInstance,							// Дескриптор приложения (wc.hInstance)
		LoadIcon(NULL, IDI_APPLICATION),	// Загрузка иконки по умолчанию (wc.hIcon)
		LoadCursor(NULL, IDC_ARROW),		// Загрузка указателя мышки (wc.hCursor)
		NULL,								// Фон окна (для OpenGL не трубуется) (wc.hbrBackground)
		NULL,								// Указатель на меню (wc.lpszMenuName)
		"MainWindow"						// Имя класса (wc.lpszClassName)
	};

	// Попытка регистрации класса
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "CreateMainWindow", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	// Если полноэкранный режим
	if (Screen.fullScreen)
	{
		// Настройки экрана
		DEVMODE dmScreenSettings;
		// Очистка настроек экрана
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);						// Размер структуры.
		dmScreenSettings.dmPelsWidth	= Screen.width;									// Ширина экрана.
		dmScreenSettings.dmPelsHeight	= Screen.height;								// Высота экрана.
		dmScreenSettings.dmBitsPerPel	= Screen.bits;									// Глубина цвета.
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;	// Режим пикселя.

		// Попытка установить выбранный режим. CDS_FULLSCREEN убирает панель управления.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, "ChangeScreenSettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL", "CreateMainWindow",  MB_OK | MB_ICONEXCLAMATION);
			return false;
		}

		dwStyle		=		// Установка стиля окна.
		WS_POPUP;			// Скрытие граници вокруг окна.
//		dwExStyle	=		// Установка расширенного стиля окна.
//		WS_EX_APPWINDOW;	// Скрытие верхней части окна.
	}
	// Если не полноэкранный режим.
	else
	{
		dwStyle		=		// Установка стиля окна.
		WS_CAPTION |		// Окно с заголовком.
		WS_SYSMENU |		// Окно с системным меню.
		WS_MINIMIZEBOX;		// Окно с кнопкой минимизации

//		dwExStyle	=		// Установка расширенного стиля окна.
//		WS_EX_APPWINDOW |
//		WS_EX_WINDOWEDGE;	// Объемный вид окна
	}
	// Вычисление размера окна по заданному размеру клиентской области
	//AdjustWindowRectEx(&wr, dwStyle, FALSE, dwExStyle);
	AdjustWindowRect(&wr, dwStyle, FALSE);

	hWnd = CreateWindow					// Создание окна.
	(
		wndClassName,					// Имя класса окна.
		Screen.caption,					// Заголовок окна.
		dwStyle,						// Стиль окна.
		CW_USEDEFAULT,					// Позиция окна по вертикали.
		CW_USEDEFAULT,					// Позиция окна по горизонтали.
		wr.right - wr.left,				// Размер окна по вертикали.
		wr.bottom - wr.top,				// Размер окна по горизонтали.
		NULL,							// Указатель на родительское окно.
		NULL,							// Указатель на меню.
		hInstance,						// Дескриптор приложения.
		NULL							// Дополнительные параметры.
	);

	//hWnd = CreateWindowEx				// Расширенное создание окна.
	//(
	//	dwExStyle,						// Расширенный стиль.
	//	wndClassName,					// Имя класса окна.
	//	Screen.caption,					// Заголовок окна.
	//	dwStyle,						// Стиль окна.
	//	CW_USEDEFAULT,					// Позиция окна по вертикали.
	//	CW_USEDEFAULT,					// Позиция окна по горизонтали.
	//	wr.right,						// Размер окна по вертикали.
	//	wr.bottom,						// Размер окна по горизонтали.
	//	NULL,							// Указатель на родительское окно.
	//	NULL,							// Указатель на меню.
	//	hInstance,						// Дескриптор приложения.
	//	NULL							// Дополнительные параметры.
	//);

	// Если окно не создано.
	if (!hWnd)
	{
		// Уничтожение окна.
		DestroyOGLWindow();
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd =			// Описание формана пикселя.
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Размер дескриптора данного формата пикселей.
		1,								// Номер версии.
		PFD_DRAW_TO_WINDOW |			// Формат для окна.
		PFD_SUPPORT_OPENGL |			// Формат для OpenGL.
		PFD_DOUBLEBUFFER,				// Формат для двойного буфера.
		PFD_TYPE_RGBA,					// RGBA формат пикселя.
		Screen.bits,					// Цветовой буфер.
		0, 0, 0, 0, 0, 0, 0, 0,			// Игнорирование цветовых и альфа битов и их сдвигов.
		0,								// Буфер накопления.
		0, 0, 0, 0,						// Цветовые и альфа биты накопления.
		16,								// Буфер глубины.
		8,								// Буфера трафарета.
		0,								// Вспомогательные буфера.
		PFD_MAIN_PLANE,					// Главный слой рисования.
		0,								// Зарезервировано.
		0, 0, 0							// Маски слояев.
	};
	
	// Получение контекста устройства.
	hDC = GetDC(hWnd);
	// Если контекст устройства не получен.
	if (!hDC)
	{
		DestroyOGLWindow();
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Поиск подходящего формата пикселя.
	GLuint PixelFormat = ChoosePixelFormat(hDC, &pfd);
	// Если подходящий формат пикселя не найден.
	if (!PixelFormat)
	{
		DestroyOGLWindow();
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Попытка установить формат пикселя.
	if (!SetPixelFormat(hDC,PixelFormat, &pfd))
	{
		DestroyOGLWindow();
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Установка контекста рендеринга.
	hRC = wglCreateContext(hDC);
	// Если неудачная установка контекста рендеринга.
	if (!hRC)
	{
		DestroyOGLWindow();
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Попытка активировать контекст рендеринга.
	if (!wglMakeCurrent(hDC, hRC))
	{
		DestroyOGLWindow();
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	// Получение размеров прямоугольника окна.
	RECT windowsRect = {};
	GetWindowRect(hWnd, &windowsRect);
	// Вычисление глобальных координат клиентской обласит окна.
	Screen.left	= windowsRect.right - wr.right;
	Screen.top	= windowsRect.bottom - wr.bottom;

	// Скрытие курсора.
	ShowCursor(false);
	// Установка координат курсора.
	SetCursorPos
	(
		Screen.left	+ Screen.width	/ 2,
		Screen.top	+ Screen.height	/ 2
	);

	ShowWindow(hWnd, SW_SHOW);								// Показ окна.
	SetForegroundWindow(hWnd);								// Приоритетный режим окна. События клавиатуры - в окно.
	SetFocus(hWnd);											// Установка фокуса на окно.
	ReSizeGLScene(Screen.width, Screen.height);				// Установка размеров окна.

	// Если при инициализации графической подсистемы произошла ошибка.
	if (!InitGL())
	{
		return false;
	}

	//// Подгрузка расширений.
	//bool						bUseExtensions			= true;		// Флаг подгрузки расширений.
	//PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB	= NULL;		// Указатель на функцию-расширение.
	//bool						bExtensions				= true;		// Флаг успешной подгрузки расширений.
	//if (bUseExtensions)
	//{
	//	// Получение адреса функции по имени функции-расширению.
	//	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");

	//	// Если во время получения адреса произошла ошибка.
	//	if (glMultiTexCoord2fARB == NULL)
	//	{
	//		MessageBox(NULL, "Could not init extensinos.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	//		return false;
	//	}
	//	else
	//	{
	//		bExtensions = true;
	//	}
	//}

	return true;
}
// Изменение видео режима.
void ChangeScreenMode()
{
	DestroyOGLWindow();
	Screen.fullScreen = !Screen.fullScreen;
	CreateOGLWindow();
}
// Уничтожение окна.
void DestroyOGLWindow()
{
	// Деинициализация.
	DeInitGL();

	// Показ указателя мышки.
	ShowCursor(true);

	// Если полноэкранный режим.
	if (Screen.fullScreen)
	{
		// Переключение в оконный режим.
		ChangeDisplaySettings(NULL, 0);
	}
	
	// Если существует контекст рендеринга
	if (hRC)
	{
		// Попытка освободить контекст рендеринга. Отсоеденить hRC от hDC.
		if (!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL, "!wglMakeCurrent(NULL, NULL)", "DestroyOGLWindow", MB_OK | MB_ICONINFORMATION);
		}

		// Попытка уничтожить hRC.
		if (!wglDeleteContext(hRC))
		{
			MessageBox(NULL, "!wglDeleteContext(hRC)", "DestroyOGLWindow", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;
	}

	// Если существует контекст устройства.
	if (hDC)
	{
		// Попытка уничтожить hDC.
		if (!ReleaseDC(hWnd, hDC))
		{
			MessageBox(NULL, "!ReleaseDC(hWnd, hDC)", "DestroyOGLWindow", MB_OK | MB_ICONINFORMATION);
		}
		hDC = NULL;
	}

	// Если окно существует.
	if (hWnd)
	{
		// Попытка уничтожить окно.
		if (!DestroyWindow(hWnd))
		{
			MessageBox(NULL, "!DestroyWindow(hWnd)", "DestroyOGLWindow", MB_OK | MB_ICONINFORMATION);
		}
		hWnd = NULL;
	}

	// Попытка разрегистрации класса.
	if (!UnregisterClass(wndClassName, hInstance))
	{
		MessageBox(NULL, "!UnregisterClass(wndClassName, hInstance)", "DestroyOGLWindow", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}

// Изменение размеров окна.
void ReSizeGLScene(GLsizei width, GLsizei height)
{
	Screen.width	= width;
	Screen.height	= height;

	if (Screen.height == 0)
	{
		Screen.height = 1;
	}

	// Определение области просмотра.
	glViewport(0, 0, Screen.width, Screen.height);
}

bool RenderingText(BYTE idxFont, string text)
{
	// Если строка пуста.
	if (text.empty())
	{
		return false;
	}

	// Включение возможности наложения 2D текстуры.
	glEnable(GL_TEXTURE_2D);
	// Выбор текстуры.
	glBindTexture(GL_TEXTURE_2D, Textures[TextureNamesFont]);

	// Сохранение состояния текущей матрицы.
	glPushMatrix();

	// Масштабирование текущей матрицы.
//	glScalef(0.5f, 0.5f, 1);

	// Перебор всех символов выводимой строки.
	for (DWORD idxChar = 0; idxChar != text.length(); idxChar++)
	{
		// Если перебираемый символ выходит за рамки допустимых символов.
		//if (text.c_str()[idxChar] < Fonts[idxFont]->firstChar || text.c_str()[idxChar] > Fonts[idxFont]->lastChar)
		//{
		//	break;
		//}
		
		int fontSize = 16;
		
		int idxTable = text.c_str()[idxChar];				// Первый символ таблицы имеет индекс 0.
		
		int col = (idxTable % 16) + 1;						// Столбец в таблице символов.
		int row = fontSize - (idxTable / fontSize);			// Строка в таблице символов.
		
		double coorCol0 = (1.0f / fontSize) * (col - 1);	// Начало столбца символа.
		double coorRow0 = (1.0f / fontSize) * (row - 1);	// Конец столбца символа.
		double coorCol1 = (1.0f / fontSize) * (col);		// Начало строки символа.
		double coorRow1 = (1.0f / fontSize) * (row);		// Конец строки символа.

		// Вывод квадра.
		glBegin(GL_QUADS);
			glTexCoord2d(coorCol0, coorRow0);
			glVertex3d(0.0f, 0.0f, 0.0f);
			
			glTexCoord2d(coorCol1, coorRow0);
			glVertex3d(fontSize, 0.0f, 0.0f);
			
			glTexCoord2d(coorCol1, coorRow1);
			glVertex3d(fontSize, fontSize, 0.0f);
			
			glTexCoord2d(coorCol0, coorRow1);
			glVertex3d(0.0f, fontSize, 0.0f);
		glEnd();

		// Смещение к следующему символу.
		glTranslated(16, -0, -0);// TODO: привязка к fontSize
	}
	
	// Восстановление состояния текущей матрицы.
	glPopMatrix();

	// Выключение возможности наложения 2D текстуры.
	glDisable(GL_TEXTURE_2D);

	return true;
}

// Инициализация графической подсистемы.
bool InitGL()
{
	// Если при загрузке текстур произошла ошибка.
	if (!LoadGLTextures())
	{
		return false;
	}

	// Если при загрузке моделей произошла ошибка.
	if (!LoadModels())
	{
		return false;
	}

	// Создание палитры для динамического цвета.
	RGBColor dynamicColors[3];
	dynamicColors[0] = RGBColor(1, 0, 0);
	dynamicColors[1] = RGBColor(0, 1, 0);
	dynamicColors[2] = RGBColor(1, 0, 0);
	dynamicColor = new Palette(dynamicColors, 3, 255);
	

//	glEnable(GL_POINT_SMOOTH)								// Включение режима сглаживания точек.
	glEnable(GL_LINE_SMOOTH);								// Включение режима сглаживания линий.

//	glShadeModel(GL_SMOOTH);								// Установка режима сглаживания. ??
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					// Установка цвета очистки экрана.
	glClearDepth(1.0f);										// Установка глубины очистки буфера.

	glDepthFunc(GL_LEQUAL);									// Установка типа теста глубины.
	glEnable(GL_DEPTH_TEST);								// Включение теста глубины.

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// Установка типа смешивания.
	glEnable(GL_BLEND);										// Включение смешивания.

// Освещение.
	GLfloat lightWhite[]	= {1.0f, 1.0f, 1.0f, 0.5f};		// Белый свет.
	GLfloat lightGray66[]	= {0.66f, 0.66f, 0.66f, 1.0f};	// Серый свет 0.66.
	GLfloat lightGray10[]	= {0.1f, 0.1f, 0.1f, 1.0f};		// Серый свет 0.1.
	GLfloat lightBlack[]	= {0.0f, 0.0f, 0.0f, 1.0f};		// Черный свет.
	GLfloat lightGet[]		= {0.0f, 0.0f, 0.0f, 0.0f};		// Полученный свет.

	// Получение параметров освещения.
	//glGetLightfv(GL_LIGHT0, GL_AMBIENT, lightGet);
	//glGetLightfv(GL_LIGHT0, GL_DIFFUSE, lightGet);
	//glGetLightfv(GL_LIGHT0, GL_SPECULAR, lightGet);

	// Настройка модели освещения.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightBlack);		// Установка глобального фонового освещения.
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);		// Выключение двустороннего освещения.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);	// Упращенный расчет бликов.

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightWhite);			// Установка фонового света.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightWhite);			// Установка рассеянного света.
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightWhite);			// Установка отроженного света.
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);		// Установка направленного источника света.

	glEnable(GL_LIGHT0);									// Включение источника света.
	glEnable(GL_LIGHTING);									// Включение освещения.
// Освещение.

	char *gl_vendor		= (char *)glGetString(GL_VENDOR);	// Получение вендора.
	char *gl_version	= (char *)glGetString(GL_VERSION);	// Получение версии.
	char *gl_renderer	= (char *)glGetString(GL_RENDERER);	// Получение рендера.
	
	printf("GL_VENDOR = %s\n", gl_vendor);					
	printf("GL_VERSION = %s\n", gl_version);				
	printf("GL_RENDERER = %s\n", gl_renderer);				

	char *gl_extensions = (char *)glGetString(GL_EXTENSIONS);
	vector<string> gl_extensions_vector = split(string(gl_extensions));
	printf("GL_EXTENSIONS:\n");
	for (vector<string>::iterator idxStr = gl_extensions_vector.begin(); idxStr != gl_extensions_vector.end(); idxStr++)
	{
		printf("%s\n", idxStr->c_str());
	}
//	getchar();

	return true;
}
// Деинициализация графической подсистемы.
void DeInitGL()
{
	// Уничтожение палитры для динамического цвета
	delete[] dynamicColor;

	// Выгрузка моделей.
	UnLoadModels();

	// Выгрузка текстур.
	UnLoadGLTextures();	
}

// Вычисление теневой матрицы.
void CalcShadowMatrix(float fDestMat[16], float fLightPos[4], float fPlane[4])
{
	// Расстояние от источника света до плоскости.
	float dot	=	fPlane[0] * fLightPos[0] + 
					fPlane[1] * fLightPos[1] + 
					fPlane[2] * fLightPos[2] + 
					fPlane[3] * fLightPos[3];

	// 1-ая колонка.
	fDestMat[0]		= dot	- fLightPos[0] * fPlane[0];
	fDestMat[4]		= 0.0f	- fLightPos[0] * fPlane[1];
	fDestMat[8]		= 0.0f	- fLightPos[0] * fPlane[2];
	fDestMat[12]	= 0.0f	- fLightPos[0] * fPlane[3];

	// 2-ая колонка.
	fDestMat[1]		= 0.0f	- fLightPos[1] * fPlane[0];
	fDestMat[5]		= dot	- fLightPos[1] * fPlane[1];
	fDestMat[9]		= 0.0f	- fLightPos[1] * fPlane[2];
	fDestMat[13]	= 0.0f	- fLightPos[1] * fPlane[3];

	// 3-яя колонка.
	fDestMat[2]		= 0.0f	- fLightPos[2] * fPlane[0];
	fDestMat[6]		= 0.0f	- fLightPos[2] * fPlane[1];
	fDestMat[10]	= dot	- fLightPos[2] * fPlane[2];
	fDestMat[14]	= 0.0f	- fLightPos[2] * fPlane[3];

	// 4-ая колонка.
	fDestMat[3]		= 0.0f	- fLightPos[3] * fPlane[0];
	fDestMat[7]		= 0.0f	- fLightPos[3] * fPlane[1];
	fDestMat[11]	= 0.0f	- fLightPos[3] * fPlane[2];
	fDestMat[15]	= dot	- fLightPos[3] * fPlane[3];
}

// Устанавка цвета.
void glSetColor(RGBColor rgbColor, double alphaChannel = 1.0)
{
	glColor4d(rgbColor.red, rgbColor.green, rgbColor.blue, alphaChannel);
}

// Настройка графических параметров перед созданием игры.
void OpenGLGameCreate()
{
	// Плоскость сетки.
	PlaneGrid[0] = 0.0f;
	PlaneGrid[1] = 0.0f;
	PlaneGrid[2] = 1.0f;
	PlaneGrid[3] = 0.0f;

	// Плоскость первой стены.
	PlaneWall1[0] = 0.0f;
	PlaneWall1[1] = 1.0f;
	PlaneWall1[2] = 0.0f;
	PlaneWall1[3] = -1.0f;
	
	// Плоскость второй стены.
	PlaneWall2[0] = -1.0f;
	PlaneWall2[1] = 0.0f;
	PlaneWall2[2] = 0.0f;
	PlaneWall2[3] = LocalGame->arenaSize;

	// Плоскость третей стены.
	PlaneWall3[0] = 0.0f;
	PlaneWall3[1] = -1.0f;
	PlaneWall3[2] = 0.0f;
	PlaneWall3[3] = LocalGame->arenaSize;

	// Плоскость четвертой стены.
	PlaneWall4[0] = 1.0f;
	PlaneWall4[1] = 0.0f;
	PlaneWall4[2] = 0.0f;
	PlaneWall4[3] = -1.0f;
}

// Отрисовка модели.
void DrawModel()
{
	// Цвет светового мотоцикала.
	GLfloat lightColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
	
	// Выбор модели.
	BYTE ModelIndexLightCycle = ModelIndexLightCycleHigh;

	// Включение массивов вершин и нормалей.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Перебор материалов.
	for (DWORD idxMaterial = 0; idxMaterial != Models[ModelIndexLightCycle]->materials->numMaterial; idxMaterial++)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, Models[ModelIndexLightCycle]->materials->materials[idxMaterial].ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Models[ModelIndexLightCycle]->materials->materials[idxMaterial].diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Models[ModelIndexLightCycle]->materials->materials[idxMaterial].specular);
		glMaterialf(GL_FRONT, GL_SHININESS, Models[ModelIndexLightCycle]->materials->materials[idxMaterial].shininess);
		if (Models[ModelIndexLightCycle]->materials->mapMaterial["Hull"] == idxMaterial)
		{
//			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lightColor);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lightColor);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lightColor);
		}

		// Задание массива с координатами вершин и нормалей.
		glVertexPointer(3, GL_FLOAT, 0, Models[ModelIndexLightCycle]->vertices[idxMaterial]);
		glNormalPointer(GL_FLOAT, 0, Models[ModelIndexLightCycle]->normals[idxMaterial]);
		
		// Отрисовка массива примитивов.
		glDrawArrays(GL_TRIANGLES, 0, 3 * Models[ModelIndexLightCycle]->numTriangles[idxMaterial]);
	}

	// Выключение массивов вершин и нормалей.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
// Отрисовка модели/тени модели.
void DrawModel(bool shadow)
{
	// TODO: Пренести.
	GLfloat lightWhite[]	= {1.0f, 1.0f, 1.0f, 1.0f};		// Белый свет.
	GLfloat lightRed[]		= {1.0f, 0.0f, 0.0f, 0.1f};		// Красный свет.
//	glMaterialfv(GL_FRONT, GL_AMBIENT, lightRed);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightRed);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, lightRed);


	// Если не тень.
	if (!shadow)
	{
		// Выключение теста глубины.
		glDisable(GL_DEPTH_TEST);
		// Выбор текстуры.
		glBindTexture(GL_TEXTURE_2D, Textures[TextureNames::TextureNamesWallCycle]);
	}

	// Перебор игроков.
	for (int idxPlayer = 0; idxPlayer != LocalGame->playersCount; idxPlayer++)
	{
		// Если не тень.
		if (!shadow)
		{
			// Установка цвета.
			glSetColor(playersColor[idxPlayer], 0.25f);

			// Включение наложения 2D текстуры.
			glEnable(GL_TEXTURE_2D);
		}
		else
		{
			// Установка цвета.
			glColor4d(0.0f, 0.0f, 0.0f, 0.9f);
		}

		// !
		glMaterialfv(GL_FRONT, GL_AMBIENT, lightRed);

		// Отрисовка стен игроков.
		glBegin(GL_QUADS);
			Point2	prevCoor	= Point2();		// Следующий узел стены.
			Point2	nextCoor	= Point2();		// Предыдущий узел стены.
			WORD	len			= 0;			// Длина сегмента стены. Для вычисления текстурной координаты.
			double	texCoorX	= 0;			// Текстурная координата. TODO: одномерная текстура?
			double	fract		= 0;			// Дробная часть координаты текстуры.

			// Перебор всех узлов стены.
			for (list<Point2>::iterator idxNode = LocalGame->players[idxPlayer]->wall.begin(); idxNode != LocalGame->players[idxPlayer]->wall.end(); idxNode++)
			{
				// Следующий узел стены.
				nextCoor = *idxNode;
				// Если не первый узел стены.
				if (idxNode != LocalGame->players[idxPlayer]->wall.begin())
				{
					// Если не тень.
					if (!shadow)
					{
						// Вычисление длины сегмента стены.
						len = abs(nextCoor.dXi() - prevCoor.dXi()) + abs(nextCoor.dYi() - prevCoor.dYi());
						// Вычисление текстурной координаты.
						texCoorX = len / double(10);
					}

					// Отрисовка начала плоскости стены.
					glTexCoord2d(fract, 0.5f);
					glVertex3d(prevCoor.dX, prevCoor.dY, 0);
					glTexCoord2d(fract, 0.5f);
					glVertex3d(prevCoor.dX, prevCoor.dY, 1);
					// Отрисовка конца плоскости стены.
					glTexCoord2d(texCoorX + fract, 0.5f);
					glVertex3d(nextCoor.dX, nextCoor.dY, 1);
					glTexCoord2d(texCoorX + fract, 0.5f);
					glVertex3d(nextCoor.dX, nextCoor.dY, 0);

					// Если не тень.
					if (!shadow)
					{
						// Вычисление дробной части координаты текстуры.
						fract += texCoorX - (int)texCoorX;
						// Отбрасывание возможной целой части дробной части координаты текстуры
						fract = fract - (int)fract;
					}
				}
				// Предыдущий узел стены.
				prevCoor = nextCoor;
			}
		glEnd();

		// Если не тень.	
		if (!shadow)
		{
			// Выключение наложения 2D текстуры.
			glDisable(GL_TEXTURE_2D);
		}

		// Если не тень.
		if (!shadow)
		{
			// !
			glMaterialfv(GL_FRONT, GL_AMBIENT, lightWhite);

			// Устоновка цвета.
			glColor4d(1.0f, 1.0f, 1.0f, 1.0f);
			// Установка толщины линий.
			glLineWidth(1);
			// Отрисовка линии-кромки стены игроков.
			glBegin(GL_LINE_STRIP);
				// Перебор всех сегментов стены.
				for (list<Point2>::iterator idxNode = LocalGame->players[idxPlayer]->wall.begin(); idxNode != LocalGame->players[idxPlayer]->wall.end(); idxNode++)
				{
					// Отрисовка линии стены.
					glVertex3d(idxNode->dX, idxNode->dY, 1);
				}
			glEnd();
		}

		//// Перенос матрицы модели в координаты модели.
		//glTranslated(LocalGame->players[idxPlayer]->coor.dXi(), LocalGame->players[idxPlayer]->coor.dYi(), 0);
		//// Отрисовка модели.
		//DrawModel();
	}

	// Если не тень.	
	if (!shadow)
	{
		// Включение теста глубины.
		glEnable(GL_DEPTH_TEST);
	}

	
	// TODO: Пренести.
	glMaterialfv(GL_FRONT, GL_AMBIENT, lightWhite);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, lightWhite);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightWhite);
}
// Отрисовка затеняемых объектов.
void DrawShaded()
{
	// Установка цвета.
	glColor4d(1.0f, 1.0f, 1.0f, 1.0f);

	// Включение возможности наложения 2D текстуры.
	glEnable(GL_TEXTURE_2D);
	// Выбор текстуры.
	glBindTexture(GL_TEXTURE_2D, Textures[TextureNames::TextureNamesGrid]);

	// Включение трафаретного теста.
	glEnable(GL_STENCIL_TEST);
	// Тест всегда пройден; значение - 0x00000001; игнорирование маски.
	glStencilFunc(GL_ALWAYS, 0x00000001, 0xFFFFFFFF);
	// sTest=0: прежнее значение; sTest=1&zTest=0: прежнее значение; sTest=1&zTest=1: запись 0x00000001.
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Отрисовка сетки.
	glBegin(GL_QUADS);
		// Вычисление текстурных координат.
		double texCoorX = (LocalGame->arenaSize - 1) / 10.0;
		double texCoorY = (LocalGame->arenaSize - 1) / 10.0;
		
		glTexCoord2d(0.0f, 0.0f);
		glVertex3i(1, 1, 0);

		glTexCoord2d(texCoorX, 0.0f);
		glVertex3i(LocalGame->arenaSize, 1, 0);

		glTexCoord2d(texCoorX, texCoorY);
		glVertex3i(LocalGame->arenaSize, LocalGame->arenaSize, 0);

		glTexCoord2d(0.0f, texCoorY);
		glVertex3i(1, LocalGame->arenaSize, 0);
	glEnd();

	// Если камера находится перед первой стеной.
	if (cameraX >= 1)
	{
		// Выбор текстуры.
		glBindTexture(GL_TEXTURE_2D, Textures[TextureNamesWall1]);

		// Тест всегда пройден; значение - 0x00000002; игнорирование маски.
		glStencilFunc(GL_ALWAYS, 0x00000002, 0xFFFFFFFF);
		// sTest=0: прежнее значение; sTest=1&zTest=0: прежнее значение; sTest=1&zTest=1: запись 0x00000002.
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// Отрисовка первой стены. (1, 1) (LocalGame->arenaSize, 1).
		glBegin(GL_QUADS);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3i(1, 1, 0);

			glTexCoord2d(1.0f, 0.0f);
			glVertex3i(LocalGame->arenaSize, 1, 0);

			glTexCoord2d(1.0f, 1.0f);
			glVertex3i(LocalGame->arenaSize, 1, LocalGame->arenaSize / 4);

			glTexCoord2d(0.0f, 1.0f);
			glVertex3i(1, 1, LocalGame->arenaSize / 4);
		glEnd();
	}

	// Если камера находится перед второй стеной.
	if (cameraY <= LocalGame->arenaSize)
	{
		// Выбор текстуры.
		glBindTexture(GL_TEXTURE_2D, Textures[TextureNamesWall2]);
		
		// Тест всегда пройден; значение - 0x00000003; игнорирование маски.
		glStencilFunc(GL_ALWAYS, 0x00000003, 0xFFFFFFFF);
		// sTest=0: прежнее значение; sTest=1&zTest=0: прежнее значение; sTest=1&zTest=1: запись 0x00000003.
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// Отрисовка второй стены. (LocalGame->arenaSize, 1) (LocalGame->arenaSize, LocalGame->arenaSize).
		glBegin(GL_QUADS);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3i(LocalGame->arenaSize, 1, 0);
			
			glTexCoord2d(1.0f, 0.0f);
			glVertex3i(LocalGame->arenaSize, LocalGame->arenaSize, 0);

			glTexCoord2d(1.0f, 1.0f);
			glVertex3i(LocalGame->arenaSize, LocalGame->arenaSize, LocalGame->arenaSize / 4);
			
			glTexCoord2d(0.0f, 1.0f);
			glVertex3i(LocalGame->arenaSize, 1, LocalGame->arenaSize / 4);
		glEnd();
	}

	// Если камера находится перед третей стеной.
	if (cameraX <= LocalGame->arenaSize)
	{
		// Выбор текстуры.
		glBindTexture(GL_TEXTURE_2D, Textures[TextureNamesWall3]);

		// Тест всегда пройден; значение - 0x00000004; игнорирование маски.
		glStencilFunc(GL_ALWAYS, 0x00000004, 0xFFFFFFFF);
		// sTest=0: прежнее значение; sTest=1&zTest=0: прежнее значение; sTest=1&zTest=1: запись 0x00000004.
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// Отрисовка третей стены. (LocalGame->arenaSize, LocalGame->arenaSize) (1, LocalGame->arenaSize).
		glBegin(GL_QUADS);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3i(1, LocalGame->arenaSize, 0);
			
			glTexCoord2d(1.0f, 0.0f);
			glVertex3i(LocalGame->arenaSize, LocalGame->arenaSize, 0);

			glTexCoord2d(1.0f, 1.0f);
			glVertex3i(LocalGame->arenaSize, LocalGame->arenaSize, LocalGame->arenaSize / 4);
			
			glTexCoord2d(0.0f, 1.0f);
			glVertex3i(1, LocalGame->arenaSize, LocalGame->arenaSize / 4);
		glEnd();
	}

	// Если камера находится перед четвертой стеной.
	if (cameraY >= 1)
	{
		// Выбор текстуры.
		glBindTexture(GL_TEXTURE_2D, Textures[TextureNames::TextureNamesWall4]);

		// Тест всегда пройден; значение - 0x00000005; игнорирование маски.
		glStencilFunc(GL_ALWAYS, 0x00000005, 0xFFFFFFFF);
		// sTest=0: прежнее значение; sTest=1&zTest=0: прежнее значение; sTest=1&zTest=1: запись 0x00000005.
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		// Отрисовка четвертой стены. (1, LocalGame->arenaSize) (1, 1).
		glBegin(GL_QUADS);
			glTexCoord2d(0.0f, 0.0f);
			glVertex3i(1, 1, 0);
			
			glTexCoord2d(1.0f, 0.0f);
			glVertex3i(1, LocalGame->arenaSize, 0);

			glTexCoord2d(1.0f, 1.0f);
			glVertex3i(1, LocalGame->arenaSize, LocalGame->arenaSize / 4);
			
			glTexCoord2d(0.0f, 1.0f);
			glVertex3i(1, 1, LocalGame->arenaSize / 4);
		glEnd();
	}

	// Выключение трафаретного теста.
	glDisable(GL_STENCIL_TEST);
	// Выключение возможности наложения 2D текстуры.
	glDisable(GL_TEXTURE_2D);
}
// Отрисовка тени.
void DrawShadow()
{
	// TODO: если источник света не перемещается нужно расчитать теневые матрицы только один раз в OpenGLStartGame()
	
	// Выключение теста глубины.
	glDisable(GL_DEPTH_TEST);
	// Включение трафаретного теста.
	glEnable(GL_STENCIL_TEST);

	// Если значение буфера трафарета - 0x00000001.
	glStencilFunc(GL_EQUAL, 0x00000001, 0xFFFFFFFF);
	// Буфер трафарета отсается прежним.
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	// Сохранение состояния текущей матрицы.
	glPushMatrix();
		// Вычисление теневой матрицы.
		CalcShadowMatrix(ShadowMatrix, LightPosition, PlaneGrid);
		// Умножение текущей матрицы на теневую.
		glMultMatrixf(ShadowMatrix);
		// Отрисовка каркаса модели.
		DrawModel(true);
	// Восстановление состояния текущей матрицы.
	glPopMatrix();

	// Если значение буфера трафарета - 0x00000002.
	glStencilFunc(GL_EQUAL, 0x00000002, 0xFFFFFFFF);
	// Буфер трафарета отсается прежним.
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	// Сохранение состояния текущей матрицы.
	glPushMatrix();
		// Вычисление теневой матрицы.
		CalcShadowMatrix(ShadowMatrix, LightPosition, PlaneWall1);
		// Умножение текущей матрицы на теневую.
		glMultMatrixf(ShadowMatrix);
		// Отрисовка каркаса модели.
		DrawModel(true);
	// Восстановление состояния текущей матрицы.
	glPopMatrix();

	// Если значение буфера трафарета - 0x00000003.
	glStencilFunc(GL_EQUAL, 0x00000003, 0xFFFFFFFF);
	// Буфер трафарета отсается прежним.
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	// Сохранение состояния текущей матрицы.
	glPushMatrix();
		// Вычисление теневой матрицы.
		CalcShadowMatrix(ShadowMatrix, LightPosition, PlaneWall2);
		// Умножение текущей матрицы на теневую.
		glMultMatrixf(ShadowMatrix);
		// Отрисовка каркаса модели.
		DrawModel(true);
	// Восстановление состояния текущей матрицы.
	glPopMatrix();

	// Если значение буфера трафарета - 0x00000004.
	glStencilFunc(GL_EQUAL, 0x00000004, 0xFFFFFFFF);
	// Буфер трафарета отсается прежним.
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	// Сохранение состояния текущей матрицы.
	glPushMatrix();
		// Вычисление теневой матрицы.
		CalcShadowMatrix(ShadowMatrix, LightPosition, PlaneWall3);
		// Умножение текущей матрицы на теневую.
		glMultMatrixf(ShadowMatrix);
		// Отрисовка каркаса модели.
		DrawModel(true);
	// Восстановление состояния текущей матрицы.
	glPopMatrix();

	// Если значение буфера трафарета - 0x00000005.
	glStencilFunc(GL_EQUAL, 0x00000005, 0xFFFFFFFF);
	// Буфер трафарета отсается прежним.
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	// Сохранение состояния текущей матрицы.
	glPushMatrix();
		// Вычисление теневой матрицы.
		CalcShadowMatrix(ShadowMatrix, LightPosition, PlaneWall4);
		// Умножение текущей матрицы на теневую.
		glMultMatrixf(ShadowMatrix);
		// Отрисовка каркаса модели.
		DrawModel(true);
	// Восстановление состояния текущей матрицы.
	glPopMatrix();

	// Включение теста глубины.
	glEnable(GL_DEPTH_TEST);
	// Выключение трафаретного теста.
	glDisable(GL_STENCIL_TEST);
}
// Отрисовка затеняющих поверхностей.
void DrawShadowed()
{
	// Отрисовка модели.
	DrawModel(false);
}

// Отрисовка информацонных примитивов.
void DrawIntoPrim()
{
	// Выключение теста глубины.
	glDisable(GL_DEPTH_TEST);

	// Отрисовка сетки.
	if (0)
	{
		// Установка цвета.
		glColor3d(1.0f, 1.0f, 1.0f);
		// Установка толщины линии.
		glLineWidth(1);
		// Сетка OXY.
		glBegin(GL_LINES);
			for (int idx = 1; idx != LocalGame->arenaSize + 10; idx += 10)
			{
				glVertex3i(idx, 1, 0);
				glVertex3i(idx, LocalGame->arenaSize, 0);
			
				glVertex3i(1, idx, 0);
				glVertex3i(LocalGame->arenaSize, idx, 0);
			}
		glEnd();
	}
		
	// Отрисовка осей.
	if (1)
	{	
		// Длина осей.
		BYTE axisLen = 25;

		// Установка размера точки.
		glPointSize(10);
		// Установка толщины линии.
		glLineWidth(5);
		
		// Установка цвета.
		glColor3d(1.0, 0.0, 0.0);
		// Ось X.
		glBegin(GL_LINES);
			glVertex3i(1, 1, 0);
			glVertex3i(axisLen, 1, 0);
		glEnd();
		glBegin(GL_POINTS);
			glVertex3i(axisLen, 1, 0);
		glEnd();
		
		// Установка цвета.
		glColor3d(0.0f, 1.0f, 0.0f);
		// Ось Y.
		glBegin(GL_LINES);
			glVertex3i(1, 1, 0);
			glVertex3i(1, axisLen, 0);
		glEnd();
		glBegin(GL_POINTS);
			glVertex3i(0, axisLen, 0);
		glEnd();
		
		// Установка цвета.
		glColor3d(0.0, 0.0, 1.0);
		// Ось Z.
		glBegin(GL_LINES);
			glVertex3i(1, 1, 0);
			glVertex3i(1, 1, axisLen);
		glEnd();
		glBegin(GL_POINTS);
			glVertex3i(1, 1, axisLen);
		glEnd();
	}

	// Включение теста глубины.
	glEnable(GL_DEPTH_TEST);
}
// Установка параметров источника света.
void SetLightParam()
{
	// TODO: если источник света не перемещается функция должна вызыватся один раз.

	// Сохранение состояния текущей матрицы.
	glPushMatrix();

	// Установка позиции источника света по центру над ареной.
	LightPosition[0] = LocalGame->arenaSize / 2;
	LightPosition[1] = LocalGame->arenaSize / 2;
	LightPosition[2] = LocalGame->arenaSize / 16;
	LightPosition[3] = 1;	// Источник света не находится бесконечно далеко.

	//// Установка цвета.
	//glColor3d(1.0f, 1.0f, 1.0f);
	//// Перенос матрицы модели. Перенос начала координат в центр источника света.
	//glTranslated(LightPosition[0], LightPosition[1], LightPosition[2]);
	//// Отрисовка источкика света.
	//GLUquadricObj *mySphere = gluNewQuadric();
	//gluQuadricDrawStyle(mySphere, GLU_FILL);
	//gluSphere(mySphere, 5.0, 20, 20);

	// Восстановление состояния текущей матрицы.
	glPopMatrix();
}

// Отрисовка сцены.
void DrawGLScene()
{
	// Счетчик милисекунд.
	DWORD TickCount = GetTickCount();

	// Очистка буфера цвета, буфера глубины и буфера трафарета.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Если локальная игра не существует, то отрисовка меню.
	if (LocalGame == NULL)
	{
		// Выключение освещения.
		glDisable(GL_LIGHTING);

		// Выбор матрицы проекций.
		glMatrixMode(GL_PROJECTION);
		// Сброс матрицы проекций.
		glLoadIdentity();
		// Умножение текущей матрицы на матрицу параллельного проецирования.
		glOrtho(0, 800 , 0, 600, -1, 1);

		// Выбор матрицы модели.
		glMatrixMode(GL_MODELVIEW);
		// Сброс матрицы модели.
		glLoadIdentity();

		// Координаты меню.
		int xPos = 10;
		int yPos = Screen.height - 200;
		// Перенос текущей матрицы.
		glTranslated(xPos, yPos, 0);

		// Перебор элементов активного меню.
		for (list<GUIBase*>::iterator idxPtrGUI = ActiveMenu->items.begin(); idxPtrGUI != ActiveMenu->items.end(); idxPtrGUI++)
		{
			// Если перебираемый элемент выделен.
			if (*idxPtrGUI == *(ActiveMenu->selectedItem))
			{
				glSetColor(dynamicColor->NextColor(), 1.0f);
			}
			// Если перебираемый элемент не выделен.
			else
			{
				glColor3f(1.0f, 1.0f, 1.0f);
			}
			// Перенос текущей матрицы.
			glTranslated(0, -15, 0);
			// Рендеринг текста.
			RenderingText(0, (*idxPtrGUI)->ToString());
		}		

		// Вывод подсказки.
		glColor3f(0.5f, 0.5f, 0.5f);
		// Перенос текущей матрицы.
		glTranslated(xPos, yPos-=64, 0);
		// Рендеринг текста.
		RenderingText(0, HintGUI.ToString().c_str());
	}	
	// Если локальная игра существует, то отрисовка сцены.
	else
	{
		// Включение освещения.
		glEnable(GL_LIGHTING);

		// Если зенитный угол обзора сцены меньше минимально заданного.
		if (ViewZenit < MIN_ZENIT)
		{
			ViewZenit = MIN_ZENIT;
		}
		// Если горизонтальный угол обзора сцены больше максимально заданного.
		if (ViewZenit > MAX_ZENIT)
		{
			ViewZenit = MAX_ZENIT;
		}
		// Если вертикальный угол обзора сцены меньше 0 градусов.
		if (ViewAzimut < 0)
		{
			ViewAzimut += 360;
		}
		// Если вертикальный угол обзора сцены больше 360 градусов.
		if (ViewAzimut > 360)
		{
			ViewAzimut -= 360;
		}
		// Если расстояние до сцены меньше MIN_DISTANCE.
		if (ViewDistance < MIN_DISTANCE)
		{
			ViewDistance = MIN_DISTANCE;
		}
		// Если расстояние до сцены больше MAX_DISTANCE.
		if (ViewDistance > MAX_DISTANCE)
		{
			ViewDistance = MAX_DISTANCE;
		}

		// Перебор игроков.
		for (int idxPlayer = 0; idxPlayer != LocalGame->playersCount; idxPlayer++)
		{
			switch (LocalGame->players[idxPlayer]->GetType())
			{
			case PlayerTypeUser:
				{
					// Вычисление координат для матрицы вида.
					modelX = LocalGame->players[idxPlayer]->coor.dX;
					modelY = LocalGame->players[idxPlayer]->coor.dY;
					modelZ = 0;

					// Вычисление зенитного угла модели.
					switch (LocalGame->players[idxPlayer]->direct.GetLine())
					{
					case Direct::Up:
						{
							modelAzimut = 0;
							break;
						}
					case Direct::Right:
						{
							modelAzimut = 90;
							break;
						}
					case Direct::Down:
						{
							modelAzimut = 180;
							break;
						}
					case Direct::Left:
						{
							modelAzimut = 270;
							break;
						}
					}
				}
			}
			// TODO: break?
		}

		// Вычисление координат камеры.
		// TODO: перевенуть локальную систему координат вверх тармашками - направления сойдутся!
		cameraY = modelX + ViewDistance * sin(RAD(ViewZenit)) * sin(RAD(ViewAzimut + modelAzimut));
		cameraX = modelY + ViewDistance * sin(RAD(ViewZenit)) * cos(RAD(ViewAzimut + modelAzimut));

		// Выбор матрицы проекций.
		glMatrixMode(GL_PROJECTION);
		// Сброс матрицы проекций.
		glLoadIdentity();
		// Установка перспективной проекции матрицы. Глубина зависит от размера карты и позиции камеры.
		gluPerspective(45.0f, (GLdouble)Screen.width / (GLdouble)Screen.height, 0.1f, M_SQRT2 * LocalGame->arenaSize + MAX_DISTANCE);
		// Переност матрицы проекций на расстояние до центра сферических координат. Перемещение камеры от/к центра сцены.
		glTranslated(-0, -0, -ViewDistance);
		// Повотор матрицы проекций на угол зенита. Перенос камеры вокруг центра сцены на угол зенита.
		glRotated(ViewZenit, 1.0f, 0.0f, 0.0f);
		// Повотор матрицы проекций на угол азимута. Перенос камеры вокруг центра сцены на угол азимута.
		glRotated(ViewAzimut + modelAzimut, 0.0f, 0.0f, 1.0f);
		// Перенос матрицы проекций в координаты модели.
		glTranslated(-modelX, -modelY, 0);

		// Выбор матрицы модели.
		glMatrixMode(GL_MODELVIEW);
		// Сброс матрицы модели.
		glLoadIdentity();		

		// Установка параметров источника света.
		SetLightParam();
		
		// Отрисовка затеняемых поверхностей.
		DrawShaded();
		//// Отрисовка тени.
		//DrawShadow();

		// Отрисовка информацонных примитивов.
		DrawIntoPrim();

		// Отрисовка затеняющих поверхностей.
		DrawShadowed();

//		// Выбор матрицы проекций.
//		glMatrixMode(GL_PROJECTION);
//		// Сброс матрицы проекций.
//		glLoadIdentity();
//		// Установка перспективной проекции матрицы.
//		gluPerspective(45.0f, (GLdouble)Screen.width / (GLdouble)Screen.height, 0.0f, 0.0f);
//
//		// Выбор матрицы модели.
//		glMatrixMode(GL_MODELVIEW);
//		// Сброс матрицы модели.
//		glLoadIdentity();
//
//		int xPos = 10;
//		int yPos = Screen.height - 10;
//		// Перебор игроков.
//		for (int idxPlayer = 0; idxPlayer != LocalGame->playersCount; idxPlayer++)
//		{
//			ostrstream tmpText;
//			tmpText << "Player " << idxPlayer + 1 << ": ";
//
//			switch (LocalGame->players[idxPlayer]->GetType())
//			{
//			case PlayerTypeUser: tmpText << "user"; break;
//			case PlayerTypeBot: tmpText << "bot  "; break;
//			}
//		
//			if (LocalGame->players[idxPlayer]->dead)
//			{
//				tmpText << " [death]";
//			}
//			
//			glColor3f(1.0f, 1.0f, 1.0f);
//			glSetTextCoor(xPos, yPos-=64);
////			glPrint(tmpText.str());
//		}
	}

	// Смена буфера.
	SwapBuffers(hDC);

	// Вычисление времени отрисовки сцены.
	DWORD dTickCount = GetTickCount() - TickCount;
	// Есил время отрисовка сцены не равно нулю.
	if (dTickCount != 0)
	{
		// Расчет FPS.
		FPS = 1000 / dTickCount;
	}
}