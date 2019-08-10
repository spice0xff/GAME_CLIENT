#include "stdafx.h"

#include "MainUnit.h"
#include "ConfUnit.h"
#include "DrawUnit.h"					// OpenGLGameCreate()
#include "GameUnit.h"


//#define MAX_USERS_COUNT	100
//#define MAX_GAMES_COUNT	10
#define MAX_GAME_ROUND	250				// Количество раундов не может быть равно масксимальному занчению типа переменной.


// Направление.
// Конструктор.
Direct::Direct(Line line): line(line)
{
}
// Возвращение значиня line.
Direct::Line Direct::GetLine()
{
	return this->line;
}
// Поворот налево.
void Direct::RotateLeft()
{
	// Анализ текущего направления.
	switch (this->line)
	{
	case Direct::Up:	this->line = Direct::Left;	break;
	case Direct::Right:	this->line = Direct::Up;	break;
	case Direct::Down:	this->line = Direct::Right;	break;
	case Direct::Left:	this->line = Direct::Down;	break;
	}
}
// Поворот направо.
void Direct::RotateRight()
{
	// Анализ текущего направления.
	switch (this->line)
	{
	case Direct::Up:	this->line = Direct::Right;	break;
	case Direct::Right:	this->line = Direct::Down;	break;
	case Direct::Down:	this->line = Direct::Left;	break;
	case Direct::Left:	this->line = Direct::Up;	break;
	}
}
// Направление

// Игрок
// Конструктор.
Player::Player():
coor(0, 0), direct(Direct::Up), wall(), dead(true)
{
}
// Установка игрока на арене.
void Player::SetOnArena(const Point2 &coor, Direct direct)
{
	// Установка координаты.
	this->coor = coor;
	// Установка направления.
	this->direct = direct;
	
	// Если стена существует.
	if (!this->wall.empty())
	{
		// Удаление стены.
		this->wall.clear();
	}
	// Строительство первого сегмента стены. Координата старта и текущая координата.
	this->BuildWall();
	this->BuildWall();

	// Установка флага "жив".
	this->dead = false;
}
// Возвращение следующей координаты игрока на арене.
Point2& Player::GetNextCoor()
{
	// Получение текущищ кооридинат игрока.
	Point2 coor = this->coor;
	
	// Обновление координат игрока.
	switch (this->direct.GetLine())
	{
	case Direct::Up:	coor.dY++; break;
	case Direct::Right:	coor.dX++; break;
	case Direct::Down:	coor.dY--; break;
	case Direct::Left:	coor.dX--; break;
	}

	// Возвращение обнавленных координат.
	return coor;
}
// Установка следующей координаты игрока на арене.
void Player::SetNextCoor(const Point2 &coor)
{
	// Обновление координат игрока на арене.
	this->coor = coor;

	// Обновление полследнего сегмента стены.
	this->wall.back() = this->coor;
}
// Поворот налево.
void Player::RotateLeft()
{
	this->direct.RotateLeft();	// Поворот налево
	this->BuildWall();			// Строительство следующего сегмента стены
}
// Поворот направо.
void Player::RotateRight()
{
	this->direct.RotateRight();	// Поворот направо
	this->BuildWall();			// Строительство следующего сегмента стены
}
// Строительство сегмента стены.
void Player::BuildWall()
{
	this->wall.push_back(this->coor);
}
// Игрок.

// Бот.
Bot::Bot():
Player(),
leftDist(MAXWORD), rightDist(MAXWORD), 
leftDistLast(MAXWORD), rightDistLast(MAXWORD),
leftWallLen(0), rightWallLen(0),
isLeftAfterWall(false), isRightAfterWall(false),
leftAfterWallLen(0), rightAfterWallLen(0)
{
}
// Тип игрока.
PlayerType Bot::GetType()
{
	return PlayerType::PlayerTypeBot;
}
// Строительство следующего сегмента стены. Переопределение.
void Bot::BuildWall()
{
	this->leftDist			= MAXWORD;	// Сброс расстояния до препядствия слева
	this->rightDist			= MAXWORD;	// Сброс расстояния до препядствия справа
	this->leftDistLast		= MAXWORD;	// Сброс предыдущего расстояния до препядствия слева
	this->rightDistLast		= MAXWORD;	// Сброс предыдущего расстояния до препядствия справа
	this->leftWallLen		= 0;		// Сброс длины стены слева
	this->rightWallLen		= 0;		// Сброс длины стены справа
	this->isLeftAfterWall	= false;	// Сброс флага недавнего присутствия стены слева
	this->isRightAfterWall	= false;	// Сброс флага недавнего присутствия стены справа
	this->leftAfterWallLen	= 0;		// Сброс расстояния после стены справа
	this->rightAfterWallLen	= 0;		// Сброс расстояния после стены слева

	Player::BuildWall();				// Вызов метода родителя
}
// Поворот налево. Переопределение.
void Bot::RotateLeft()
{
	this->direct.RotateLeft();		// Поворот налево
	this->BuildWall();				// Вызов переопределенного метода
}
// Поворот направо. Переопределение.
void Bot::RotateRight()
{
	this->direct.RotateRight();		// Поворот налево
	this->BuildWall();				// Вызов переопределенного метода
}
// Функция расчета расстояний до препядствий.
WORD Bot::GetDist(const int direct)
{
	Point2	localCoor	= this->coor;
	Direct	localDierct	= this->direct;
	
	// Направление расчета
	switch (direct)
	{
		case Direct::Left:	localDierct.RotateLeft(); break;
		case Direct::Down:	return 0;
		case Direct::Right:	localDierct.RotateRight(); break;
	}

	int idxPoint2 = 0;
	while (true)
	{
		// Обновление координат.
		switch (localDierct.GetLine())
		{
			case Direct::Up:	localCoor.dY++; break;
			case Direct::Right:	localCoor.dX++; break;
			case Direct::Down:	localCoor.dY--; break;
			case Direct::Left:	localCoor.dX--; break;
		}

		// Проверка на пересечение с границами поля.
		if
		(
			localCoor.dX == 0 ||
			localCoor.dY == 0 ||
			localCoor.dX == LocalGame->arenaSize + 1 ||
			localCoor.dY == LocalGame->arenaSize + 1
		)
		{
			return idxPoint2;
		}

		// Проверка на пересечение со стеной.
		if (LocalGame->arena[localCoor.dXi() - 1][localCoor.dYi() - 1])
		{
			return idxPoint2;
		}	

		idxPoint2++;
	}
}
// Функция управления игроком.
void Bot::AI() 
{
	// TODO: поворот за перпендикулярные стены. Попадение в расщелину или просто в препядствие.
	// TODO: Путь вдоль стены. Рекурисвный GetDist(). Детектирование подрезания.
	// TODO: Следить за другими игроками. подрезать их.
	
	int constDist		= RND(5, 10);				// Эталонное расстояние до препядствия.
	int constWallDist	= 10;						// Расстояние до стены.
	int frontDist		= GetDist(Direct::Up);		// Расстояние до препядствия спереди.
	int leftDist		= GetDist(Direct::Left);	// Расстояние до препядствия слева.
	int rightDist		= GetDist(Direct::Right);	// Расстояние до препядствия справа.

	// Если была стена слева 
	if (this->isLeftAfterWall == true)
	{
		// Увеличение расстояния после окончания стены слева
		leftAfterWallLen++;

		// Если расстояние после окончания стены слева больше заданного
		if (this->leftAfterWallLen > constDist)
		{
			// Поворот налево
			this->RotateLeft();
			return;
		}
	}

	// Если была стена справа
	if (this->isRightAfterWall == true)
	{
		// Увеличение расстояния после окончания стены справа
		rightAfterWallLen++;

		// Если расстояние после окончания стены слева больше заданного
		if (this->rightAfterWallLen > constDist)
		{
			// Поворот направо
			this->RotateRight();
			return;
		}
	}
	
	// Если расстояние до препядствия слева меньше заданного
	if (leftDist <= constWallDist)
	{	
		// Если расстояние до препядствия слева сохранилось
		if (leftDist == this->leftDistLast)
		{
			// Увеличиваем длинну стены слева
			leftWallLen++;
		}
		else
		{
			// Запись текущего расстояния до препядствия слева
			this->leftDistLast = leftDist;
		}
	}
	// Если расстояние до препядствия слева больше заданного
	else
	{
		// Если длинна стены слева была больше заданного
		if (this->leftWallLen >= constDist)
		{
			this->isLeftAfterWall = true;	// Установка Флага недавнего присутствия стены слева
			this->leftAfterWallLen = 0;		// Сброс расстояния после окончания стены слева
		}
		
		this->leftDistLast	= MAXWORD;		// Сброс последнего расстояния до препядствия слева
		this->leftWallLen	= 0;			// Сброс длинны стены слева
	}

	// Если расстояние до препядствия справа меньше заданного
	if (rightDist <= constWallDist)
	{	
		// Если расстояние до препядствия справа сохранилось
		if (rightDist == this->rightDistLast)
		{
			// Увеличиваем длинну стены справа
			rightWallLen++;
		}
		else
		{
			// Запись текущего расстояния до препядствия справа
			this->rightDistLast = rightDist;
		}
	}
	// Если расстояние до препядствия справа больше заданного
	else
	{
		// Если длинна стены справа была больше заданного
		if (this->rightWallLen >= constDist)
		{
			this->isRightAfterWall = true;	// Установка Флага недавнего присутствия стены справа
			this->rightAfterWallLen = 0;	// Сброс расстояния после окончания стены справа
		}
		
		this->rightDistLast	= MAXWORD;		// Сброс последнего расстояния до препядствия справа
		this->rightWallLen	= 0;			// Сброс длинны стены справа
	}

	// Если предстоит столкновение
	if (frontDist < constDist)
	{
		// Слева больше?
		if (leftDist > rightDist)
		{
			// Поворот налево
			this->RotateLeft();
			return;
		}
		
		// Справа больше?
		if (rightDist > leftDist)
		{
			// Поворот направо
			this->RotateRight();
			return;
		}		
		
		// Расстояния равны
		if (rightDist == leftDist)
		{
			// Если расстояния равны нулю
			if (!rightDist)
			{
				// Продоление движения вперед
				return;
			}

			// Равновероятностное ветвление
			if (RND(0, 1))
			{
				// Поворот налево
				this->RotateLeft();
				return;
			}
			else
			{
				// Поворот направо
				this->RotateRight();
				return;
			}
		}		
	}

	this->leftDist	= GetDist(Direct::Left);
	this->rightDist	= GetDist(Direct::Right);
}
// Бот.

// Пользователь.
// Конструктор.
User::User(): Player()
{
}
// Тип игрока.
PlayerType User::GetType()
{
	return PlayerType::PlayerTypeUser;
}
// Пользователь.

// Игра.
// Конструктор.
Game::Game(GameConfig::GameRules *gameRules):
gameRules(gameRules), playersCount(0), motoSpeed(0), arenaSize(0), users(), players(), arena(NULL), round(0), run(false), singleGame(false)
{
	// Перебор конфигурации пользователей.
	for (int idxPlayer = 0; idxPlayer != 4; idxPlayer++)
	{
		// Если пользователь существует.
		if (GameConfig::players[idxPlayer].exist)
		{
			// Создание пользователя.
			this->users[idxPlayer] = (User*)(new User());
			// Занесение пользователя в сипсок игроков.
			this->players[playersCount] = (Player*)(this->users[idxPlayer]);
			// Увеличение количества игроков.
			this->playersCount++;
			break;
		}
		// Если пользователь не существует.
		else
		{
			this->users[idxPlayer] = NULL;
			break;		
		}
	}
	// Если количество игроков, заданное в правилах игры меньше количества пользователей.
	if (this->gameRules->playersCount->GetValue() <= this->playersCount)
	{
		this->gameRules->playersCount->SetValue(this->playersCount);
	}
	else
	{
		// Пока количество игроков, заданное в правилах игры не равно фактическому количеству игроков.
		while (this->gameRules->playersCount->GetValue() != this->playersCount)
		{
			// Создание бота и занесение его в список игроков.
			this->players[playersCount] = (Player*)(new Bot());
			// Увеличение количества игроков.
			this->playersCount++;
		}
	}
	// Если количество игроков, заданное в правилах игры равно одному - игра одиночная.
	this->singleGame = this->gameRules->playersCount->GetValue() == 1;

	// Установка скорости.
	switch (gameRules->motoSpeed->GetValue())
	{
	case (GameConfig::MotoSpeedLow):	this->motoSpeed = 30;	break;
	case (GameConfig::MotoSpeedNormal):	this->motoSpeed = 20;	break;
	case (GameConfig::MotoSpeedFast):	this->motoSpeed = 10;	break;
	case (GameConfig::MotoSpeedCrazy):	this->motoSpeed = 0;	break;
	}

	// Установка размера арены. Добавление единицы для зеркального растягивания текстуры по узлам арены.
	// TODO: если есть освещение, то растягивать не нужно. Надо формировать каждый полигон, для корректрого освещения.
	switch (gameRules->arenaSize->GetValue())
	{
	case (GameConfig::ArenaSizeTiny):	this->arenaSize = 301;	break;
	case (GameConfig::ArenaSizeSmoll):	this->arenaSize = 601;	break;
	case (GameConfig::ArenaSizeMedium):	this->arenaSize = 901;	break;
	case (GameConfig::ArenaSizeBig):	this->arenaSize = 1201;	break;
	case (GameConfig::ArenaSizeHuge):	this->arenaSize = 1501;	break;
	}
	// Создание арены.
	this->arena = new BYTE*[this->arenaSize];
	for (WORD idx = 0; idx != this->arenaSize; idx++)
	{
		this->arena[idx] = new BYTE[this->arenaSize];
	}
}
// Деструктор.
Game::~Game()
{
	// Перебор игроков.
	for (int idxPlayer = 0; idxPlayer != this->playersCount; idxPlayer++)
	{
		// Проверка типа игрока.
		switch (this->players[idxPlayer]->GetType())
		{
		// Если игрок - пользователь.
		case PlayerTypeUser:
			{
				// Удаление игрока
				delete ((User*)this->players[idxPlayer]);
				break;
			}
		// Если игрок - бот.
		case PlayerTypeBot:
			{
				// Удаление бота.
				delete ((Bot*)this->players[idxPlayer]);
				break;
			}
		}
		// Обнуление указателя.
		this->players[idxPlayer] = NULL;
	}	
	
	// Уничтожение арены.
	for (WORD idx = 0; idx != this->arenaSize; idx++)
	{
		delete[] this->arena[idx];
	}
	delete[] this->arena;
	// Обнуление указателя.
	this->arena = NULL;
}
// Старт раунда.
void Game::StartRound()
{
	// Увеличение номера раунда.
	this->round++;

	// Если номер раунда превысил максимально заданное количество раундов.
	if (this->round > this->gameRules->roundsCount->GetValue())
	{
		// TODO: передача пользователям результатов игры.
		// Обнуление номера раунда.
		this->round = 0;
		// Остановка игры.
		this->run = false;
		return;
	}

	// Очистка арены.
	for (WORD idx = 0; idx != this->arenaSize; idx++)
	{
		memset(this->arena[idx], 0, this->arenaSize);	
	}

	// Список возможных начальных координат игроков.
	vector<Point2>	playersCoor = vector<Point2>();
	// Размер арены минус один. Для целочисленного расчета координат с последующим прибавлением единицы.
//	WORD			arenaSize	= this->arenaSize - 1;

	// Если в игре только один игрок.
	if (this->playersCount == 1)
	{
		// Только одна возможная начальная координа игрока.
		playersCoor.push_back(Point2(this->arenaSize / 2 + 1, this->arenaSize / 2 + 1));
		
		// Установка игрока на арене.
		this->players[0]->SetOnArena(playersCoor[0], Direct(Direct::Line(RND(0, 3))));
		// Запись позиции игрока на поле.
		this->arena[playersCoor[0].dXi() - 1][playersCoor[0].dYi() - 1] = 1;
	}
	// Если в игре более одного игрока.
	else
	{
		// Вычисление стороны наименьшего квадрата, большего или равного количеству игроков
		BYTE side = 2;	// Сторона квадрата
		while (side * side < this->playersCount)
		{
			side++;
		}

		// Заполнение списка возможных начальных координат игроков.
		WORD coorX = this->arenaSize / (2 * side);
		while (coorX <= (this->arenaSize - this->arenaSize / (2 * side)))
		{
			WORD coorY = arenaSize / (2 * side);
			while (coorY <= (this->arenaSize - this->arenaSize / (2 * side)))
			{
				playersCoor.push_back(Point2(coorX + 1, coorY + 1));
				
				coorY += this->arenaSize / side;
			}
			coorX += this->arenaSize / side;
		}

		// Перебор игроков.
		for (int idxPlayer = 0; idxPlayer != this->playersCount; idxPlayer++)
		{
			// Получение случайного индекса для массива координат начальных позиций.
			int idxCoor = -1;
			while (idxCoor == -1)
			{
				idxCoor = RND(0, side * side - 1);
				if (!playersCoor[idxCoor].dX)
				{
					idxCoor = -1;
				}
			}
			// Установка игрока на арене.
			this->players[idxPlayer]->SetOnArena(playersCoor[idxCoor], Direct(Direct::Line(RND(0, 3))));
			// Запись позиции игрока на поле.
			this->arena[this->players[idxPlayer]->coor.dXi() - 1][this->players[idxPlayer]->coor.dYi() - 1] = idxPlayer + 1;

			// Предотвращение повторного использования координаты из списка возможных начальных координат.
			playersCoor[idxCoor].dX = 0;

			// TODO: вразумить последовательность - расчет координат, прибавление одного, установка игрока, отнятие двух, запись игрока
		}
	}

	//coor[0].Set(300, 300);
	//coor[1].Set(310, 300);
	//coor[2].Set(0, 0);
	//coor[3].Set(0, 0);
	//Players[0] = (DWORD)(new Player(coor[0], RND(0, 3), false));
	//for (int i = 300; i != 450 + 1; i++)
	//{
	//	GameArena[300 - 1][i - 1] = 1;
	//}

	//((Player*)Players[0])->coor = Point2(300, 450);
	//((Player*)Players[0])->death = true;
	//Players[1] = (DWORD)(new PlayerAI(coor[1], Direct::Up, false));
}
// Очистка арены от стены игрока.
void Game::ClearArena(BYTE playerIndex)
{
	for (WORD idxX = 0; idxX != this->arenaSize; idxX++)
	{
		for (WORD idxY = 0; idxY != this->arenaSize; idxY++)
		{
			if (this->arena[idxX][idxY] == playerIndex)
			{
				this->arena[idxX][idxY] = 0;
			}
		}	
	}
}
// Расчет игры.
void Game::CalcGame()
{
	// Если игра не запущена.
	if (!this->run)
	{
		return;
	}
	
	// Пауза.
	Sleep(this->motoSpeed);

	// Счетчик живых игроков.
	BYTE lifeCount = this->playersCount;

	// Перебор игроков.
	for (int idxPlayer = 0; idxPlayer != this->playersCount; idxPlayer++)
	{
		// Если игрок в состоянии "мертв".
		if (this->players[idxPlayer]->dead)
		{
			// Переход к следующему игроку.
			continue;
		}

		// Если игрок - бот.
		if (this->players[idxPlayer]->GetType() == PlayerTypeBot)
		{
			// Вызов функции управления ботом.
			((Bot*)this->players[idxPlayer])->AI();
		}
		
		// Возвращение следующей координаты игрока на арене.
		Point2 coor = this->players[idxPlayer]->GetNextCoor();
		// Если пересечение с границами поля.
		if
		(
			(coor.dXi() == 0) ||
			(coor.dYi() == 0) ||
			(coor.dXi() == this->arenaSize + 1) ||
			(coor.dYi() == this->arenaSize + 1)
		)
		{
			// Установка игрока в состояние "мертв".
			this->players[idxPlayer]->dead = true;
		}
		// Если не пересечение с границами поля.
		else
		{
			// Если пересечение со стеной.
			if (this->arena[coor.dXi() - 1][coor.dYi() - 1])
			{
				// Установка игрока в состояние "мертв".
				this->players[idxPlayer]->dead = true;
			}

			// Установка следующей координаты игрока на арене.
			this->players[idxPlayer]->SetNextCoor(coor);
			// Запись позиции игрока на поле.
			this->arena[this->players[idxPlayer]->coor.dXi() - 1][this->players[idxPlayer]->coor.dYi() - 1] = idxPlayer + 1;
		}

		// Если игрок в состоянии "мертв".
		if (this->players[idxPlayer]->dead)
		{
			// Уменьшение счетчика живых игроков.
			lifeCount--;

			// Если в конфигурации есть указание на очистку арены от стены игрока.
			if (this->gameRules->eraseDeathPlayer->GetValue())
			{
				// Очистка стены игрока.
				this->players[idxPlayer]->wall.clear();
				// Очистка арены от стены игрока.
				this->ClearArena(idxPlayer + 1);
			}
		}
	}

	// Если игра одиночная.
	if (this->singleGame)
	{
		// Если не осталось игроков.
		if (lifeCount == 0)
		{
			// Начало нового раунда.
//			this->StartRound();
		}
	}
	// Если игра не одиночная.
	else
	{
		// Если осталось менее одного или один живой игрок.
		if (lifeCount <= 1)
		{
			// Начало нового раунда.
			this->StartRound();
		}
	}
}
// Игра.


Game *LocalGame;								// Локальная игра.


// Создание игры.
void CreateGame()
{
	// Создание игры.
	LocalGame = new Game(&GameConfig::gameRules);
	LocalGame->run = true;
	LocalGame->StartRound();

	// Настройка графических параметров перед созданием игры.
	OpenGLGameCreate();
}

// Сброс счета.
void ResetScores()
{
	// Если игра существует.
	if (LocalGame != NULL)
	{
		// TODO
	}
}

// Расчет игры.
void CalcGame()
{
	// Если игра существует.
	if (LocalGame != NULL)
	{
		// Если игра запущена.
		if (LocalGame->run)
		{
			// Расчет игры.
			LocalGame->CalcGame();
		}
	}
}

// Уничтожение игры.
void DestroyGame()
{
	delete LocalGame;
	LocalGame = NULL;
}