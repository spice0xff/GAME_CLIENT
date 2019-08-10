#ifndef _MainUnit_
#define _MainUnit_


extern bool		MainLoopExit;		// Флаг завершения главного цикла программы

extern bool		GlobalFlag;			// Глобальный флаг для отладки. TODO: удалить его
extern DWORD	GlobalDWORD;		// Глобальныое двойное слово для отладки. TODO: удалить его
extern bool		DebugPause;			// Флаг для дебажной паузы. TODO: удалить


extern DWORD __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern void MainLoopBreak();

#endif