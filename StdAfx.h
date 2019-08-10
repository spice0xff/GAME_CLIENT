#ifndef _StdAfx_
#define _StdAfx_


#define _USE_MATH_DEFINES	// Математические константы.
#define _WINSOCKAPI_		// Работа с сокетами.

#define RAD(grad)		(M_PI / 180) * (grad)
#define GRAD(rad)		(180 / M_PI) * (rad)
#define RND(min, max)	(min + rand() % (max - min + 1))


// WINAPI
#include <windows.h>
#include <winsock2.h>	// ws2_32.lib
// STL
#include <string>		// Работа со строками.
#include <list>			// Работа со списками.
#include <vector>		// Работа с векторами.
#include <map>			// Работа со словарями.
#include <algorithm>	// Алгоритмы обработки контейнеров.
#include <strstream>	// Работа со сороковыми потоками.
#include <fstream>		// Работа с файловыми потоками.
//#include <sstream>
//#include <iostream>
// C
#include <math.h>
// OpenGL
#include <gl\gl.h>		// OpenGL32.lib
#include <gl\glu.h>		// GLu32.lib
//#include <gl\glaux.h>	// GLaux.lib
//#include "gl\glext.h"


using namespace std;


string			Int2Str(DWORD value);										// Конвертация числа в строку.
bool			Str2Int(string strValue, DWORD *int64Value);				// Конвертирования строки в число. В случии неудачи число не имзеняется.
bool			Str2Double(string strValue, double *doubleValue);			// Конвертирования строки в вешественное число. В случии неудачи число не имзеняется.
string			replace(string text, string sour, string dest);				// Замена подстроки в строке.
vector<string>	split(const string& str, const string& delimiters = " ");	// Расщепление строки.


#endif