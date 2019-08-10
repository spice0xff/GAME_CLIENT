#ifndef _GUIUnit_
#define _GUIUnit_


#include "ConfUnit.h"
#include "stdafx.h"


// Типы GUI.

enum GUIType
{
	GUITypeMenu,									// Меню.
	GUITypeLabel,									// Метка.
	GUITypeButton,									// Кнопка.
	GUITypeKey,										// Клавиша.
	GUITypeText,									// Текст.
	GUITypeNumber,									// Число.
	GUITypeList,									// Список.
	GUITypeSwitch,									// Переключатель.
	GUITypeProgress									// Прогресс.
};
// Типы GUI.

// Базовый класс всех GUI

class GUIBase
{
public:
	GUIBase();														// Конструктор.
	virtual const GUIType GetType()							= 0;	// Возврат типа GUI.
	virtual string ToString()								= 0;	// В виде строки.
	virtual void KeyDown(BYTE virtualKey, DWORD keyData)	= 0;	// Нажатие клавиши.
};
// Базовый класс всех GUI.

// Меню.
class Menu: public GUIBase
{
public:
	string						caption;			// Текст метки.
	Menu						*parent;			// Родительское меню.
	list<GUIBase*>				items;				// Элементы меню.
	list<GUIBase*>::iterator	selectedItem;		// Выделенный элемент.

	Menu(string caption);							// Конструктор.
	const GUIType GetType();						// Возврат типа GUI.
	string ToString();								// В виде строки.
	void KeyDown(BYTE virtualKey, DWORD keyData);	// Нажатие клавиши.

	void	AddItem(GUIBase *item);					// Добавление элемента в меню.

protected:
	void	Next();									// Переход к слудующему элементу.
	void	Prev();									// Переход к предыдущему элементу.
	void	Enter();								// Переход в дочернее меню.
	void	Leave();								// Переход в родительское меню.
};
// Меню.

// Метка.
class Label: public GUIBase
{
public:
	string caption;									// Текст метки.
	
	Label(string caption);							// Конструктор.
	const GUIType GetType();						// Возврат типа GUI.
	string ToString();								// В виде строки.
	void KeyDown(BYTE virtualKey, DWORD keyData);	// Нажатие клавиши.
};
// Метка.

// Кнопка.
class Button: public GUIBase
{
public:
	string caption;									// Текст кнопки.	
	
	Button(string caption, void (*function)());		// Конструктор.
	const GUIType GetType();						// Возврат типа GUI.
	string ToString();								// В виде строки.
	void KeyDown(BYTE virtualKey, DWORD keyData);	// Нажатие клавиши.

protected:
	void	(*function)();							// Дествие.

	void	Click();								// Нажатие кнопки.
};
// Кнопка.

// Клавиша.
class Key: public GUIBase
{
public:
	Key(ConfKey *confKey);							// Конструктор.
	const GUIType GetType();						// Возврат типа GUI.
	string ToString();								// В виде строки.
	void KeyDown(BYTE virtualKey, DWORD keyData);	// Нажатие клавиши.

protected:
	ConfKey	*confKey;								// Клавиша.
	bool	edit;									// Флаг редактирования.

};
// Клавиша.

// Текст.
class Text: public GUIBase
{
public:
	Text(ConfText *confText);						// Конструктор.
	const GUIType GetType();						// Возврат типа GUI.
	string ToString();								// В виде строки.
	void KeyDown(BYTE virtualKey, DWORD keyData);	// Нажатие клавиши.

protected:
	ConfText	*confText;							// Указатель на конфигурационный текст.
	string		text;								// Текст GUI.
	bool		edit;								// Флаг редактирования.
};
// Текст

// Число.
class Number: public GUIBase
{
public:
	Number(ConfNumber *confNumber);					// Конструктор.
	const GUIType GetType();						// Возврат типа GUI.
	string ToString();								// В виде строки.
	void KeyDown(BYTE virtualKey, DWORD keyData);	// Нажатие клавиши.

protected:
	ConfNumber	*confNumber;						// Указатель на конфигурационное число.
	string		text;								// Текст GUI.
	bool		edit;								// Флаг редактирования.
};
// Число.

// Список.
class List: public GUIBase
{
public:
	List(ConfList *confList);						// Конструктор.
	const GUIType GetType();						// Возврат типа GUI.
	string ToString();								// В виде строки.
	void KeyDown(BYTE virtualKey, DWORD keyData);	// Нажатие клавиши.

protected:
	ConfList *confList;								// Указатель на конфигурационный список.
};
// Список.

// Переключатель.
class Switch: public GUIBase
{
public:
	Switch(ConfSwitch *confSwitch);					// Конструктор.
	const GUIType GetType();						// Возврат типа GUI.
	string ToString();								// В виде строки.
	void KeyDown(BYTE virtualKey, DWORD keyData);	// Нажатие клавиши.

protected:
	ConfSwitch	*confSwitch;						// Указатель на значение переключателя.
};
// Переключатель.

// Прогресс
class Progress: public GUIBase
{
public:
	Progress(ConfProgress *confProgress);			// Конструктор.
	const GUIType GetType();						// Возврат типа GUI.
	string ToString();								// В виде строки.
	void KeyDown(BYTE virtualKey, DWORD keyData);	// Нажатие клавиши.

protected:
	ConfProgress *confProgress;						// Указатель на значение прогресса.
};
// Прогресс


extern Menu		*ActiveMenu;						// Активное меню.
extern GUIBase	*HoldGUI;							// Элемент GUI, удерживающий фокус.
extern Label	HintGUI;							// Подсказка.

extern void CreateGUI();							// Создание GUI.
extern void DestroyGUI();							// Уничтожение GUI.


#endif