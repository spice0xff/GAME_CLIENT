#include "stdafx.h"

#include "MainUnit.h"
#include "GuiUnit.h"
#include "GeomUnit.h"
#include "GameUnit.h"
#include "DrawUnit.h"
#include "CtrlUnit.h"


bool	VirtualKeys[256]	= {};		// Массив состояний клавиш.
bool	MouseLeftButton		= false;	// Флаг нажатия левой кнопки мышы.
bool	MouseRightButton	= false;	// Флаг нажатия правой кнопки мышы.


void MenuServeKey(BYTE virtualKey, DWORD keyData);
void GameServeKey(DWORD keyData);


// Циклическая обработка кливиатуры и мыши.
void CtrlMain()
{
	// Если левая кнопка мышы нажата.
	if (MouseLeftButton)
	{
		// Увеличение расстояния до сцены.
		ViewDistance++;
	}

	// Если правая кнопка мышы нажата.
	if (MouseRightButton)
	{
		// Уменьшение расстояния до сцены.
		ViewDistance--;
	}
}

// Обработка перемещения курсора.
void MouseMove()
{
	// Получение относительных координат курсора.
	POINT mouseCoor = {};
	GetCursorPos(&mouseCoor);
	mouseCoor.x -= Screen.left;
	mouseCoor.y -= Screen.top;

	// Вычисление относительный центра окна.
	int centerX = Screen.width	/ 2;
	int centerY = Screen.height	/ 2;

	// Если курсор изменил позицию.
	if ((mouseCoor.x != centerX) || (mouseCoor.y != centerY))
	{
		// Установка координат курсора.
		SetCursorPos
		(
			Screen.left	+ Screen.width	/ 2,
			Screen.top	+ Screen.height	/ 2
		);

		// Вычисление углов обзора сцены.
		ViewAzimut	+= (float)((centerX - mouseCoor.x)) / 10.0f;
		ViewZenit	+= (float)((centerY - mouseCoor.y)) / 10.0f;
	}
	

//	printf("x=%i; y=%i\n", Point2.x, Point2.y);

	//// Получение относительных координат курсора.
	//Mouse.coor.dX = LOWORD(lParam);
	//Mouse.coor.dY = Display.height - HIWORD(lParam);
}

// Обработка нажатия левой кнопки мыши.
void MouseLeftButtonDown()
{
	MouseLeftButton	= true;
}
// Обработка отпускания левой кнопки мыши.
void MouseLeftButtonUp()
{
	MouseLeftButton = false;
}

// Обработка нажатия правой кнопки мыши.
void MouseRightButtonDown()
{
	MouseRightButton = true;
}
// Обработка отпускания правой кнопки мыши.
void MouseRightButtonUp()
{
	MouseRightButton = false;
}

// Обработка вращения колеса мыши.
void MouseWhell(MouseWhellDirect mouseWhellDirect)
{
	switch (mouseWhellDirect)
	{
	case MouseWhellDirect::MouseWhellDirectUp:
		{
			// Увеличение расстояния до сцены.
			ViewDistance++;
			break;
		}
	case MouseWhellDirect::MouseWhellDirectDown:
		{
			// Уменьшение расстояния до сцены.
			ViewDistance--;			
			break;
		}
	}
}

// Обработка нажатия клавиши.
void KeyDown(BYTE virtualKey, DWORD keyData)
{
	// Запись нажатой клавиши.
	VirtualKeys[virtualKey] = true;

	// Если нажата клавиша F1.
	if (VirtualKeys[VK_F1])
	{
		// Включение/выключение паузы.
		DebugPause = !DebugPause;
		
		// Сброс клавиши.
		VirtualKeys[VK_F1] = false;
	}

	// Если локальная игра не существует.
	if (LocalGame == NULL)
	{
		// Обработка клавиатуры меню.
		MenuServeKey(virtualKey, keyData);
	}
	// Если локальная игра существует.
	else
	{
		// Обработка клавиатуры игрой.
		GameServeKey(keyData);
	}
}
// Обработка отпускания клавиши.
void KeyUp(BYTE virtualKey, DWORD keyData)
{
	// Запись отпущенной клавиши.
	VirtualKeys[virtualKey] = false;
}

// Обработка клавиатуры меню.
void MenuServeKey(BYTE virtualKey, DWORD keyData)
{
	// Отправка нажатия клавиши в меню.
	ActiveMenu->KeyDown(virtualKey, keyData);
}
// Обработка клавиатуры игрой.
void GameServeKey(DWORD keyData)
{
	// Если нажата клавиша Esc.
	if (VirtualKeys[VK_ESCAPE])
	{
		// Завершение игры.
		DestroyGame();

		// Сброс клавиши.
		VirtualKeys[VK_ESCAPE] = false;
		return;
	}

	// Если нажата клавиша пробел.
	if (VirtualKeys[VK_SPACE])
	{
		// Включение/выключение паузы.
		LocalGame->run = !LocalGame->run;
		
		// Сброс клавиши.
		VirtualKeys[VK_SPACE] = false;
		// Во время паузы действия пользователей не обрабатываются.
		return;
	}

	// Перебор пользователей.
	for (int idxUser = 0; idxUser != 4; idxUser++)
	{
		// Если пользователь существует.
		if (LocalGame->users[idxUser] != NULL)
		{
			// Если пользователь в состоянии "мертв".
			if (LocalGame->users[idxUser]->dead)
			{
				// Переход к следующему пользователю.
				continue;
			}			

			// Если нажата клавиша влево.
			if (keyData == GameConfig::players[idxUser].keyLeft->GetValue())
			{
				// Поворот пользователя налево.
				LocalGame->users[idxUser]->RotateLeft();
			}
			// Если нажата клавиша вправо.
			if (keyData == GameConfig::players[idxUser].keyRight->GetValue())
			{
				// Поворот пользователя направо.
				LocalGame->users[idxUser]->RotateRight();
			}
		}
	}

	// Если нажата клавиша L.
	if (VirtualKeys['L'])
	{
		GLboolean params = 0;
		glGetBooleanv(GL_LIGHTING, &params);
		if (params)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			glEnable(GL_LIGHTING);
		}

		// Сброс клавиши.
		VirtualKeys['L'] = false;
		return;
	}
	
	
	//// Нажата клавиша автопилота. TODO: удалить
	//if (Keys['A'])
	//{
	//	// Включение автопилота
	//	switch (gameSettings.playersSettings[0].role)
	//	{
	//		case PlayerSettings::RoleHuman:
	//		{
	//			gameSettings.playersSettings[0].role = PlayerSettings::RoleAI;
	//			break;
	//		}
	//		case PlayerSettings::RoleAI:
	//		{
	//			gameSettings.playersSettings[0].role = PlayerSettings::RoleHuman;
	//			break;
	//		}
	//	}

	//	// Сброс клавиши
	//	Keys['A'] = false;
	//}
}