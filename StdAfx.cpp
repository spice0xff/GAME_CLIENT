#include "stdafx.h"


// Конвертация числа в строку.
string Int2Str(DWORD value)
{
	static char tmp[10];	// Максимальная длина DWORD в десятеричном представлении. 2.147.483.648
	_itoa(value, tmp, 10);
//	_itoa_s(value, tmp, sizeof(tmp), 10);
	return string(tmp);
}

// Конвертирования строки в число. В случии неудачи число не имзеняется.
bool Str2Int(string strValue, DWORD *int64Value)
{
	static char *ptr;
	static DWORD result;
	result = strtol(strValue.c_str(), &ptr, 10);
	if (*ptr)
	{
		return false;
	}
	if ((result == LONG_MAX || result == LONG_MIN) && errno == ERANGE)
	{
		return false;
	}
	*int64Value = result;
	return true;
}

// Конвертирования строки в вешественное число. В случии неудачи число не имзеняется.
bool Str2Double(string strValue, double *doubleValue)
{
	static char *ptr;
	static double result;
	
	result = strtod(strValue.c_str(), NULL);
	if ((result == HUGE || result == HUGE) && errno == ERANGE)
	{
		return false;
	}
	*doubleValue = result;
	return true;
}

// Замена подстроки в строке.
string replace(string text, string sour, string dest)
{
	for (DWORD index = 0; index = text.find(sour, index); index != string::npos)
	{
		text.replace(index, sour.length(), dest);
		index += dest.length();
	}
	return text;
}

// Расщепление строки.
vector<string> split(const string& str, const string& delimiters)
{
    // Результат.
	std::vector<std::string> result;
	
	// Позиция первого не символа - разделителя.
	string::size_type lastPos	= str.find_first_not_of(delimiters, 0);
    // Позиция первого символа - разделителя.
	string::size_type pos		= str.find_first_of(delimiters, lastPos);
    
	// Пока одна из позиции найдена.
	while (pos != string::npos || lastPos != string::npos)
    {
        // Добавление найденой подстроки в результат.
		result.push_back(str.substr(lastPos, pos - lastPos));
        // Позиция следующего не символа - разделителя.
		lastPos	= str.find_first_not_of(delimiters, pos);
        // Позиция следующего символа - разделителя.
		pos		= str.find_first_of(delimiters, lastPos);
    }

	// Возвращение результата.
	return result;
}