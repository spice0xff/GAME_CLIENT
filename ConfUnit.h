#ifndef _ConfUnit_
#define _ConfUnit_


#define CONF_MIN_PLAYERS	1			// Минимальное количество игроков.
#define CONF_MAX_PLAYERS	6			// Максимальное количество игроков.
#define CONF_MIN_ROUNDS		1			// Минимальное количество раундов.
#define CONF_MAX_ROUNDS		250			// Максимальное количество раундов.

#define CONF_MAX_NUM_LEN	10			// Максимальная длинна числа.
#define CONF_MAX_TEXT_LEN	10			// Максимальная длинна текстовой строки.


// Класс клавиша.
class ConfKey
{
public:
	ConfKey(string name);							// Конструктор.
	const string	GetName();						// Возврат названия.

	const DWORD		GetValue();						// Возврат значения.
	const string	GetStrValue();					// Возврат значения в виде строки.

	void			SetValue(DWORD value);			// Установка значения.
	bool			SetRawValue(string rawValue);	// Установка сырово значения.

private:
	string		name;								// Название.
	DWORD		key;								// Клавиша.
	char		keyName[256];						// Название клавиши.
};
// Класс клавиша.

// Класс текст.
class ConfText
{
public:
	ConfText(string name);							// Конструктор.
	const string	GetName();						// Возврат названия.
	
	const string	GetValue();						// Возврат значения.

	void			SetValue(string value);			// Установка значения.
	bool			SetRawValue(string rawValue);	// Установка сырово значения.

private:
	string		name;								// Название.
	string		value;								// Значение.
};
// Класс текст.

// Класс число.
// Число - байт с контролем максимально и минимально возможного значения. GUI является оберткой.
class ConfNumber
{
public:
	ConfNumber(string name, BYTE minValue, BYTE maxValue);	// Конструктор.
	const string	GetName();						// Возврат названия.
	
	const BYTE		GetValue();						// Возврат значения.
	const string	GetStrValue();					// Возврат значения в виде строки.
	
	void			SetValue(BYTE value);			// Установка значения.
	bool			SetRawValue(string rawValue);	// Установка сырово значения.

private:
	string		name;								// Название.
	BYTE		value;								// Число.
	BYTE		maxValue;							// Максимальное значение чила.
	BYTE		minValue;							// Минимальное значение чила.
};
// Класс число.

// Класс список.
// Список - гибрид мапа и енума. Хранит текстовое и числовое значение параметра. GUI является оберткой.
class ConfList
{
public:
	ConfList(string name, map<BYTE, string> *dic);	// Конструктор.
	const string		GetName();					// Возврат названия.
	
	const BYTE			GetValue();					// Возврат значения.
	const string		GetStrValue();				// Возврат значения в виде строки.

	void				SetValue(BYTE value);		// Установка значения.	
	bool				SetRawValue(string rawValue);	// Установка сырово значения.
	
	void				Next();						// Переход к следующему элементу.
	void				Prev();						// Переход к предыдущему элементу.

private:
	string				name;						// Название.
	BYTE				value;						// Индекс элемента.
	map<BYTE, string>	*dic;						// Массив элементов.
};
// Класс список.

// Класс переключатель.
class ConfSwitch
{
public:
	ConfSwitch(string name);						// Конструктор.
	const string	GetName();						// Возврат названия.

	const BYTE		GetValue();						// Возврат значения.
	const string	GetStrValue();					// Возврат значения в виде строки.

	void			SetValue(BYTE value);			// Установка значения.
	bool			SetRawValue(string rawValue);	// Установка сырово значения.

	void			Switch();						// Переключение значения.

private:
	string			name;							// Название.
	BYTE			value;							// Значение.
};
// Класс переключатель.

// Класс прогресс.
class ConfProgress
{
public:
	ConfProgress(string name);						// Конструктор.
	const string	GetName();						// Возврат названия.

	const BYTE		GetValue();						// Возврат значения.
	const string	GetStrValue();					// Возврат значения в виде строки.

	void			SetValue(BYTE value);			// Установка значения.
	bool			SetRawValue(string rawValue);	// Установка сырово значения.

	void			More();							// Увелечение значения.
	void			Less();							// Уменьшение значения.

private:
	string			name;							// Название.
	BYTE			value;							// Значение.
};
// Класс прогресс.


// Конфигурация игры.
// Словари и диапазоны читаются из конфига, они же туда пишутся. С коментариями.
class GameConfig
{
public:
	// Возможные значения скорости.
	enum MotoSpeed: BYTE
	{
		MotoSpeedLow		= 0,
		MotoSpeedNormal		= 1,
		MotoSpeedFast		= 2,
		MotoSpeedCrazy		= 3
	};
	// Возможные значения размера арены.
	enum ArenaSize: BYTE
	{
		ArenaSizeTiny		= 0,
		ArenaSizeSmoll		= 1,
		ArenaSizeMedium		= 2,
		ArenaSizeBig		= 3,
		ArenaSizeHuge		= 4
	};
	// Возможные значения уровеня ботов.
	enum BotLevel: BYTE
	{
		BotLevelDummy		= 0,
		BotLevelStupid		= 1,
		BotLevelMedium		= 2,
		BotLevelClever		= 3
	};
	// Возможные значения оформления.
	enum ArtPack: BYTE
	{
		ArtPackDefault		= 0,
		ArtPackClassic		= 1
	};
	// Возможные значения видеорежима.
	enum ScreenMode: BYTE
	{
		ScreenMode320x240	= 0,
		ScreenMode512x384	= 1,
		ScreenMode640x480	= 2,
		ScreenMode800x600	= 3,
		ScreenMode1024x768	= 4,
		ScreenMode1280x1024	= 5,
		ScreenMode1600x1200	= 6
	};
	// Возможные значения режима камеры.
	enum CameraMode: BYTE
	{
		CameraModeBehind	= 0,
		CameraModeCircling	= 1
	};
	// Возможные значения вида.
	enum Viewports: BYTE
	{
		ViewportsSingle		= 0,
		ViewportsSplit		= 1,
		Viewports4player	= 2,
		ViewportsAuto		= 3
	};
	// Возможные значения мини карты.
	enum MiniMap: BYTE
	{
		MiniMapNone			= 0,
		MiniMapSmoll		= 1,
		MiniMapBig			= 2
	};

	// Правила игры.
	struct GameRules
	{
		ConfNumber		*playersCount;		// Количество игроков.
		ConfNumber		*roundsCount;		// Количество раундов.
		ConfList		*motoSpeed;			// Скорость.
		ConfList		*arenaSize;			// Размер арены.
		ConfList		*botLevel;			// Уровень ботов.
		ConfSwitch		*eraseDeathPlayer;	// Очистка арены от стены игрока после его смерти.
	};
	// Настройки пользователей.
	struct Player
	{
		ConfSwitch		*exist;				// Существования пользователя.
		ConfKey			*keyLeft;			// Клавиша поворока налево.
		ConfKey			*keyRight;			// Клавиша поворока направо.
	};
	// Настройки видео.
	struct Video
	{
		ConfList		*artPack;			// Оформление.
		ConfList		*screenMode;		// Видеорежим.
		ConfSwitch		*fullScreen;		// Полный экран.
	};
	// Настройки аудио.
	struct Audio
	{
		ConfSwitch		*music;				// Музыка.
		ConfSwitch		*sound;				// Звук.
		ConfProgress	*musicVolume;		// Громкость музыки.
		ConfProgress	*soundVolume;		// Громкость звука.
	};
	// Настройки игры.
	struct Play
	{
		ConfSwitch		*fastFinish;		// Быстрый финиш.
		ConfList		*cameraMode;		// Режим камеры.
		ConfList		*viewports;			// Вид.
		ConfList		*miniMap;			// Мини карта.
	};

	static GameRules	gameRules;				// Правила игры.
	static Player		players[4];				// Настройки пользователей.
	static Video		video;					// Настройки видео.
	static Audio		audio;					// Настройки аудио.
	static Play			play;					// Настройки игры.

	static void			CreateConfig();			// Создание конфигурации.
	static void			ResetConfig();			// Сброс конфигурации.
	static void			DestroyConfig();		// Уничтожение конфигурации.

protected:
	static map<BYTE, string> mapMotoSpeed;		// Возможные варианты скорости.
	static map<BYTE, string> mapArenaSize;		// Возможные варианты размера арены.
	static map<BYTE, string> mapBotLevel;		// Возможные варианты уровня ботов.

	static map<BYTE, string> mapArtPack;		// Возможные варианты оформления.
	static map<BYTE, string> mapScreenMode;		// Возможные варианты видеорежимов.

	static map<BYTE, string> mapCameraMode;		// Возможные варианты режима камеры.
	static map<BYTE, string> mapViewports;		// Возможные варианты вида.
	static map<BYTE, string> mapMiniMap;		// Возможные варианты миникары.

	static bool			ReadConfig();			// Чтение конфигурации из файла.
	static void			WriteConfig();			// Запись конфигурации в файл.
};
// Конфигурация игры.


#endif