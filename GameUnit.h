#ifndef _GameUnit_
#define _GameUnit_

#include "stdafx.h"

#include "ConfUnit.h"
#include "GeomUnit.h"


// Направление
class Direct
{
public:
	enum Line							// Возможные направления движения
	{
		Up,								// Вверх
		Right,							// Вправо
		Down,							// Вниз
		Left							// Влево
	};

	Direct(Line line);					// Конструктор

	Line GetLine();						// Возвращение значиня line
	void RotateLeft();					// Поворот налево
	void RotateRight();					// Поворот направо

protected:
	Line line;							// Направление движения
};
// Направление

// Возможные типы игроков
enum PlayerType: BYTE
{
//	PlayerTypeNone,		// Базовый тип
	PlayerTypeUser,		// Пользователь
	PlayerTypeBot		// Бот
};
// Возможные типы игроков

// Игрок
class Player
{
public:
	Point2				coor;							// Координаты игрока на арене.
	Direct				direct;							// Направление движения игрока.
	list<Point2>			wall;							// Стена игрока.
	bool				dead;							// Флаг смерти игрока.

	Player();											// Конструктор.
	virtual PlayerType	GetType() = 0;					// Тип игрока.

	void SetOnArena(const Point2 &coor, Direct direct);	// Установка игрока на арене.
	Point2&			GetNextCoor();						// Возвращение следующей координаты игрока на арене.
	void			SetNextCoor(const Point2 &coor);		// Установка следующей координаты игрока на арене.
	virtual void	RotateLeft();						// Поворот налево.
	virtual void	RotateRight();						// Поворот направо.
//	const list<Point2>&	GetWall();						// Возвращение стены.
//protected: TODO

	void				BuildWall();					// Строительство сегмента стены
};
// Игрок

// Бот
class Bot: public Player
{
public: 
	Bot();										// Конструктор
	PlayerType	GetType();						// Тип игрока

	void		BuildWall();					// Строительство следующего сегмента стены
	void		RotateLeft();					// Поворот налево
	void		RotateRight();					// Поворот направо
	void		AI();							// Функция управления игроком

	WORD		leftDist;						// Расстояние до препядствия слева
	WORD		rightDist;						// Расстояние до препядствия справа
	WORD		leftDistLast;					// Предыдущее расстояния до препядствия слева
	WORD		rightDistLast;					// Предыдущее расстояния до препядствия справа
	WORD		leftWallLen;					// Длинна стены слева
	WORD		rightWallLen;					// Длинна стены справа
	bool		isLeftAfterWall;				// Флаг недавнего присутствия стены слева
	bool		isRightAfterWall;				// Флаг недавнего присутствия стены справа
	WORD		leftAfterWallLen;				// Расстояние после окончания стены слева
	WORD		rightAfterWallLen;				// Расстояние после окончания стены справа

protected:
	WORD		GetDist(const int direct);		// Функция расчета расстояний до препядствий
};
// Бот

// Пользователь
class User: public Player
{
public:
	User();										// Конструктор
	PlayerType	GetType();						// Тип игрока
};
// Пользователь

// Игра
class Game
{
public:
	GameConfig::GameRules	*gameRules;		// Правила игры.
	BYTE					playersCount;	// Текущее количество игроков.
	WORD					motoSpeed;		// Числовое значение скорости (задержки).
	WORD					arenaSize;		// Числовое значение размера арены.

	User					*users[4];		// Пользователи.
	Player					*players[255];	// Игроки.
	BYTE					**arena;		// Арена.
	BYTE					round;			// Текущий раунд.
	bool					run;			// Флаг запущеной игры.
	bool					singleGame;		// Флаг одиночной игры.

	Game(GameConfig::GameRules *gameRules);	// Конструктор.
	~Game();								// Деструктор.

	void StartRound();						// Старт раунда.
	void ClearArena(BYTE colorIndex);		// Очистка арены от стены игрока.
	void CalcGame();						// Расчет игры.
};
// Игра


extern Game *LocalGame;						// Локальная игра.


extern void CreateGame();					// Создание игры.
extern void ResetScores();					// Сброс счета.
extern void CalcGame();						// Расчет игры.
extern void DestroyGame();					// Уничтожение игры.


#endif