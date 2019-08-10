#ifndef _CtrlUnit_
#define _CtrlUnit_


// Возможные варианты вращения колеса мыши.
enum MouseWhellDirect
{
	MouseWhellDirectUp,				// Вращение вверх.
	MouseWhellDirectDown			// Вращение вниз.
};


extern void CtrlMain();										// Циклическая обработка кливиатуры и мыши.
extern void MouseMove();									// Обработка перемещения курсора.
extern void MouseLeftButtonDown();							// Обработка нажатия левой кнопки мыши.
extern void MouseLeftButtonUp();							// Обработка отпускания левой кнопки мыши.
extern void MouseRightButtonDown();							// Обработка нажатия правой кнопки мыши.
extern void MouseRightButtonUp();							// Обработка отпускания правой кнопки мыши.
extern void MouseWhell(MouseWhellDirect mouseWhellDirect);	// Обработка вращения колеса мыши.
extern void KeyDown(BYTE virtualKey, DWORD keyData);		// Обработка нажатия клавиши.
extern void KeyUp(BYTE virtualKey, DWORD keyData);			// Обработка отпускания клавиши.


#endif