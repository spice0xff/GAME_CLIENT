#include "stdafx.h"

#include "ConfUnit.h"
#include "GuiUnit.h"
#include "DrawUnit.h"
#include "CtrlUnit.h"
#include "GameUnit.h"
#include "MainUnit.h"


#define WSA_ACCEPT WM_USER + 1			// Создание сообщения установки соеденения


bool		MainLoopExit	= false;	// Флаг завершения главного цикла программы
bool		Active			= true;		// Флаг активности окна

bool		GlobalFlag		= false;	// Глобальный флаг для отладки. TODO: удалить его
DWORD		GlobalDWORD		= 0;		// Глобальныое двойное слово для отладки. TODO: удалить его
bool		DebugPause		= false;	// Флаг для дебажной паузы. TODO: удалить


// Процедура обработки оконных сообщений
DWORD __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Проверка сообщения для окна
	switch (uMsg)
	{
		case WM_CREATE:								// Если сообщение о создании окна
		{
//			MessageBox(NULL, "WM_CREATE", "WM_CREATE", MB_OK);
			break;
		}

		case WM_ACTIVATE:							// Если сообщение изменения активности окна
		{
			Active = !HIWORD(wParam);				// Изменение флага активности окна
			break;
		}

		case WSA_ACCEPT:							// Попытка установить соединение?
		{
			printf("WSA_ACCEPT\n");
			//OnWSAAccept(hWnd, uMsg, wParam, lParam);

			break;
		}

		case WM_MOUSEMOVE:							// Если перемещение курсора.
		{
			// Обработка перемещения курсора.
			MouseMove();
			break;
		}

		case WM_LBUTTONDOWN:						// Если нажата левая кнопка мыши.
		{
			// Обработка нажатия левой кнопки мыши.
			MouseLeftButtonDown();
			break;
		}
		case WM_LBUTTONUP:							// Если левая кнопка мыши отпущена.
		{
			// Обработка отпускания левой кнопки мыши.
			MouseLeftButtonUp();
			break;
		}
		case WM_RBUTTONDOWN:						// Если нажата правая кнопка мыши.
		{
			// Обработка нажатия правой кнопки мыши.
			MouseRightButtonDown();
			break;
		}
		case WM_RBUTTONUP:							// Если правая кнопка мыши отпущена.
		{
			// Обработка отпускания правой кнопки мыши.
			MouseRightButtonUp();
			break;
		}
		case WM_MOUSEWHEEL:							// Если	вращение колеса мыши.
		{
			// Обработка вращения колеса мыши.
			MouseWhell((GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? MouseWhellDirect::MouseWhellDirectUp : MouseWhellDirect::MouseWhellDirectDown);
			break;
		}		
		
		case WM_KEYDOWN:							// Если сообщение нажатия клавиши
		{
			// Если клавиша еще не нажата. 30 бит lParam установлен в 0
			if (!(lParam & 0x40000000))
			{
				GlobalDWORD = lParam;
				// Обработка нажатия клавиши
				KeyDown(LOWORD(wParam), lParam);
			}
			else
			{
//				// Обработка нажатия клавиши
//				KeyDown(LOWORD(wParam), 0);
			}
			break;
		}

		case WM_KEYUP:								// Если сообщение отпускания клавиши
		{
			GlobalDWORD = lParam;

			// Обработка отпускания клавиши
			KeyUp(LOWORD(wParam), lParam);
			break;
		}

		case WM_SIZE:								// Если сообщение растягивиния/сжания окна
		{
			// Младшее слово - ширина, старшее слово - высота
			ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));

			break;
		}

		case WM_SYSCOMMAND:							// Если сообщение системной команды
		{
			switch (wParam)							// Проверка типа системной команды
			{
				case SC_SCREENSAVE:					// Если попытка запуска скринсейвера
				case SC_MONITORPOWER:				// Если  попытка перехода монитора в режим энергосбережения
				return 0;							// Игронирование сообщения
			}
			break;
		}
		
		case WM_CLOSE:								// Если сообщение о закрытии окна
		{
			MainLoopExit = true;					// Завершение главного цикла программы
			return 0;								// Игронирование сообщения
		}

		case WM_DESTROY:							// Если сообщение о разрушении окна
		{
//			MessageBox(NULL, "WM_DESTROY", "WM_DESTROY", MB_OK);
			break;
		}		
		
		case WM_QUIT:								// Если сообщение о завершении программы
		{
			MessageBox(NULL, "WM_QUIT", "WM_QUIT", MB_OK);
			break;
		}
	}

	// Пересылка необработанных сообщений в DefWindowProc
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

//bool StartTCPServer()
//{
//	WORD		wVersionRequested = MAKEWORD(2, 0);	// Версия WinSock 2.0
//	WSAData		wsaData;							// Ссылка на структуру с информацияе по WinSock
//	if (WSAStartup(wVersionRequested, &wsaData))	// Если ошибка инициализации интерфейса WinSock
//	{
//		printf("WSAStartup error\n");				// TODO: WSAGetLastError()
//		return false;
//	}
//	printf("Server use: %s\n", wsaData.szDescription);
//	printf("WinSock state: %s\n", wsaData.szSystemStatus);
//
//	// Создание сокета. Формат адреса: internet; тип сокета: TCP; протокол: IPv6
//	SOCKET serverSocket = socket(AF_INET , SOCK_STREAM, IPPROTO_TCP);
//	if (serverSocket == INVALID_SOCKET)				// Если при создении сокете произошла ошибка
//	{
//		printf("socket error\n");					// TODO: WSAGetLastError() Erace Memory
//		return false;
//	}
//
//	sockaddr_in soketParam;						// Параметры сокета
//	soketParam.sin_family	= AF_INET;			// Формат адреса - internet
//	soketParam.sin_port		= htons(2012);		// Номер порта сервера
//	soketParam.sin_addr.s_addr = htonl(INADDR_ANY);	// Адрес клиента для работа с сокетом
//
//	BYTE socketResult = 0;						// Результаты команд
//	socketResult = bind(serverSocket, (LPSOCKADDR)&soketParam, sizeof(soketParam));
//	if (socketResult == SOCKET_ERROR)			// Если произошла ошибка
//	{
//		printf("bind error\n");					// TODO: WSAGetLastError() Erace Memory
//		return false;
//	}
//
//	// === Сервер ===
//	socketResult = listen(serverSocket, 1);		// Создание канала связи
//	if (socketResult == SOCKET_ERROR)			// Если произошла ошибка
//	{
//		printf("listen error\n");				// TODO: WSAGetLastError() Erace Memory
//		return false;
//	}
//
//	// При попытке соединения окно получит сообщение WSA_ACCEPT
//	int rc = WSAAsyncSelect(serverSocket, hWnd, WSA_ACCEPT, FD_ACCEPT);
//	if (rc > 0)									// Если произошла ошибка
//	{
//		printf("WSAAsyncSelect error\n");		// TODO: WSAGetLastError() Erace Memory
//		return false;
//	}
//
//	return true;
//
//	// === Сервер ===	
//	
//	//cout << "std::cout\n";
//	//cerr << "std::cerr\n";
//	//clog << "std::clog\n";
//
//	//char buff[256];
//	//ostrstream (buff, sizeof(buff), ios::out/ios::ate) << "aayy";
//
//	//cout << buff;
//}
//
//bool StartTCPClient()
//{
//	PHOSTENT phe;
//
//	// Создаем сокет 
//	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//	if(clientSocket == INVALID_SOCKET)
//	{
//		MessageBox(NULL, "socket  Error", "Error", MB_OK);
//		return false;
//	}
//
//	sockaddr_in dest_sin;
//	// Устанавливаем адрес IP и номер порта
//	dest_sin.sin_family = AF_INET ;
//	dest_sin.sin_port = htons(2012);
//	dest_sin.sin_addr.s_addr = inet_addr("127.0.0.1");
//	
//
//	// Устанавливаем соединение
//	if (connect(clientSocket, (PSOCKADDR )&dest_sin, sizeof(dest_sin)) < 0)
//	{
//		MessageBox(NULL, "connect Error", "Error", MB_OK);
//		return false;
//	}
//	
//	return true;
//}

// Функция прерывания главного цикла программы.
void MainLoopBreak()
{
	MainLoopExit = true;
}

// Функция главного цикла программы.
int MainLoopFunc()
{
	// Оконное сообщение.
	MSG	msg;
	// Главный цикл программы.
	while (!MainLoopExit)
	{
		// Если есть сообщение в очереди.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);	// Анализ сообщения и отправка WM_CHAR, если введен символ.
			DispatchMessage(&msg);	// Отправка сообщения оконной процедуре.
		}
		// Если нет сообщений в очереди.
		else
		{
			// Если окно активно.
			if (Active)
			{
				CtrlMain();				// Обработка кливиатуры и мыши.
				
				// Есил не пауза. TODO: удалить. для отладки
				if (!DebugPause)
				{

					CalcGame();			// Расчет игры.
					DrawGLScene();		// Отрисовка сцены.
				}
			}
		}
	}
	return msg.wParam;
}

// Точка входа.
int __cdecl main(int argc, char *argv[])
{
	int MainResult = 0;	// Код возврата программы.

	// TODO: Перенос инициализации
	// Инициализация генератора псевдослучайных чисел
	srand(GetTickCount());

	// Создание окна.
	if (!CreateOGLWindow())
	{
		MainResult = 1;	// Программа завершена неудачно.
		goto CreateOGLWindow_Error;
	}

	// Создание конфигурации.
	GameConfig::CreateConfig();

	// Создание GUI. TODO: ошибка при создании?
	CreateGUI();

	//if (!StartTCPClient())
	//{
	//	return 1;
	//}	

//	// Подготовка к игре
//	InitGame();

	// Запуск функции главного цикла программы.
	MainResult = MainLoopFunc();

	// Уничтожение GUI.
	DestroyGUI();

	// Уничтожение конфигурации.
	GameConfig::DestroyConfig();

// Метка для перехода при ошибке в функции CreateOGLWindow.
CreateOGLWindow_Error:
	// Уничтожение окна.
	DestroyOGLWindow();

	return MainResult;
}