#include "stdafx.h"

#include "ConfUnit.h"

// TODO: общий абстрактный родитель для свех конфигурационных классов. Эконоимя функций.
// Класс клавиша.
// Конструктор.
ConfKey::ConfKey(string name):
name(name), key()
{}
// Возврат названия.
const string ConfKey::GetName()
{
	return this->name;
}
// Возврат значения.
const DWORD	ConfKey::GetValue()
{
	return this->key;
}
// Возврат значения в виде строки.
const string ConfKey::GetStrValue()
{
	// Получение названия клавиши.
	GetKeyNameText(this->key, this->keyName, sizeof(this->keyName));
	return this->keyName;
}
// Установка значения.
void ConfKey::SetValue(DWORD value)
{
	this->key = value;
}
// Установка строкового значения.
bool ConfKey::SetRawValue(string rawValue)
{
	// Установка значения.
	return Str2Int(rawValue, &(this->key));
}
// Класс клавиша.

// Класс текст.
// Конструктор.
ConfText::ConfText(string name):
name(name), value()
{}
// Возврат названия.
const string ConfText::GetName()
{
	return this->name;
}
// Возврат значения.
const string ConfText::GetValue()
{
	return this->value;
}
// Установка значения.
void ConfText::SetValue(string value)
{
	this->value = value;
}
// Установка сырово значения.
bool ConfText::SetRawValue(string value)
{
	// Есил длина строки больше максимально заданной.
	if (value.length() > CONF_MAX_TEXT_LEN)
	{
		return false;
	}
	const char *c_value = value.c_str();
	// Перебор всех символов строки.
	for (BYTE idx = 0; idx != value.length(); idx++)
	{
		// Если не допустимый ASCII-код.
		if (c_value[idx] < 33 && c_value[idx] > 126)
		{
			return false;
		}	
	}
	// Установка значения.	
	this->value = value;
	return true;
}
// Класс текст.

// Класс число.
// Конструктор.
ConfNumber::ConfNumber(string name, BYTE minValue, BYTE maxValue):
name(name), value(), minValue(minValue), maxValue(maxValue)
{
}
// Возврат названия.
const string ConfNumber::GetName()
{
	return this->name;
}
// Возврат значения.
const BYTE ConfNumber::GetValue()
{
	return this->value;
}
// Возврат значения в виде строки.
const string ConfNumber::GetStrValue()
{
	return Int2Str(this->value);
}
// Установка значения.
void ConfNumber::SetValue(BYTE value)
{
	this->value = value;
}
// Установка значения.
bool ConfNumber::SetRawValue(string rawValue)
{
	// Есил длина строки больше максимально заданной.
	if (rawValue.length() > CONF_MAX_NUM_LEN)
	{
		return false;
	}
	// Попытка конвертирования.
	DWORD result;
	if (!Str2Int(rawValue, &result))
	{
		return false;
	}
	// Если значение меньше минимально заданного.
	if (result < this->minValue)
	{
		result = this->minValue;
	}
	// Если значение больше максимально заданного.
	if (result > this->maxValue)
	{
		result = this->maxValue;
	}
	// Установка значения.
	this->value = (BYTE)result;
	return true;
}
// Класс число.

// Класс список.
// Конструктор.
ConfList::ConfList(string name, map<BYTE, string> *dic):
name(name), dic(dic)
{
}
// Возврат названия.
const string ConfList::GetName()
{
	return this->name;
}
// Возврат значения.
const BYTE ConfList::GetValue()
{
	return this->value;
}
// Возврат значения в виде строки.
const string ConfList::GetStrValue()
{
	return (*dic)[this->value];
}
// Установка значения.
void ConfList::SetValue(BYTE value)
{
	this->value = value;
}
// Установка сырово значения.
bool ConfList::SetRawValue(string rawValue)
{
	// Попытка конвертирования.
	DWORD result;
	if (!Str2Int(rawValue, &result))
	{
		return false;
	}
	// Если параметр выходит за пределы словаря.
	if (result < 0)
	{
		return false;
	}
	// Если параметр выходит за пределы словаря.
	if (result >= this->dic->size())
	{
		return false;
	}
	// Установка значения.
	this->value = (BYTE)result;	
	return true;
}
// Следующее значение.
void ConfList::Next()
{
	if (this->value != this->dic->size() - 1)
	{
		this->value++;
	}
	else
	{
		this->value = 0;
	}
}
// Предыдущее значение.
void ConfList::Prev()
{
	if (this->value != 0)
	{
		this->value--;
	}
	else
	{
		this->value = this->dic->size() - 1;
	}
}
// Класс список.

// Класс переключатель.
// Конструктор.
ConfSwitch::ConfSwitch(string name):
name(name)
{
}
// Возврат названия.
const string ConfSwitch::GetName()
{
	return this->name;
}
// Возврат значения.
const BYTE ConfSwitch::GetValue()
{
	return this->value;
}
// Возврат значения в виде строки.
const string ConfSwitch::GetStrValue()
{
	return (this->value) ? "on" : "off";
}
// Установка значения.
void ConfSwitch::SetValue(BYTE value)
{
	this->value = value;
}
// Установка значения.
bool ConfSwitch::SetRawValue(string rawValue)
{
	// Попытка конвертирования.
	DWORD result;
	if (!Str2Int(rawValue, &result))
	{
		return false;
	}	
	// Если значение некорректно.
	if ((this->value != 0) && (this->value != 1))
	{
		return false;
	}
	// Установка значения.
	this->value = (BYTE)result;
	return true;
}
// Переключение значения.
void ConfSwitch::Switch()
{
	this->value = !this->value;
}
// Класс переключатель.

// Класс прогресс.
// Конструктор.
ConfProgress::ConfProgress(string name):
name(name)
{
}
// Возврат названия.
const string ConfProgress::GetName()
{
	return this->name;
}
// Возврат значения.
const BYTE ConfProgress::GetValue()
{
	return this->value;
}
// Возврат значения в виде строки.
const string ConfProgress::GetStrValue()
{
	return Int2Str(this->value);
}
// Установка значения.
void ConfProgress::SetValue(BYTE value)
{
	this->value = value;
}
// Установка сырово значения.
bool ConfProgress::SetRawValue(string rawValue)
{
	// Попытка конвертирования.
	DWORD result;
	if (!Str2Int(rawValue, &result))
	{
		return false;
	}
	// Если значение выходит за заданные пределы.
	if (this->value < 0)
	{
		return false;
	}
	// Если значение выходит за заданные пределы.
	if (this->value > 100)
	{
		return false;
	}
	// Установка значения.
	this->value = (BYTE)result;
	return true;
}
// Увелечение значения.
void ConfProgress::More()
{
	if (this->value < 95)
	{
		this->value += 5;
	}
	else
	{
		this->value = 100;
	}
}
// Уменьшенеие значения.
void ConfProgress::Less()
{
	if (this->value > 5)
	{
		this->value -= 5;
	}
	else
	{
		this->value = 0;
	}
}
// Класс прогресс.


// Инициализация конфигурации.
GameConfig::GameRules	GameConfig::gameRules	= {};	// Правила игры.
GameConfig::Player		GameConfig::players[4]	= {};	// Настройки пользователей.
GameConfig::Video		GameConfig::video		= {};	// Настройки видео.
GameConfig::Audio		GameConfig::audio		= {};	// Настройки аудио.
GameConfig::Play		GameConfig::play		= {};	// Настройки игры.


// Создание конфигурации. !! Пустой словарь вызовит необработанное исключение !!
void GameConfig::CreateConfig()
{
	// Количество игроков.
	GameConfig::gameRules.playersCount = new ConfNumber("Players count", CONF_MIN_PLAYERS, CONF_MAX_PLAYERS);
	// Количество раундов.
	GameConfig::gameRules.roundsCount = new ConfNumber("Rounds count", CONF_MIN_ROUNDS, CONF_MAX_ROUNDS);
	// Скорость.
	mapMotoSpeed[MotoSpeedLow]			= "Low";
	mapMotoSpeed[MotoSpeedNormal]		= "Normal";
	mapMotoSpeed[MotoSpeedFast]			= "Fast";
	mapMotoSpeed[MotoSpeedCrazy]		= "Crazy";
	GameConfig::gameRules.motoSpeed = new ConfList("Speed", &mapMotoSpeed);
	// Размер арены.
	mapArenaSize[ArenaSizeTiny]			= "Tiny";
	mapArenaSize[ArenaSizeSmoll]		= "Smoll";
	mapArenaSize[ArenaSizeMedium]		= "Medium";
	mapArenaSize[ArenaSizeBig]			= "Big";
	mapArenaSize[ArenaSizeHuge]			= "Huge";
	GameConfig::gameRules.arenaSize = new ConfList("Arena size", &mapArenaSize);
	// Уровень ботов.
	mapBotLevel[BotLevelDummy]			= "Dummy";
	mapBotLevel[BotLevelStupid]			= "Stupid";
	mapBotLevel[BotLevelMedium]			= "Medium";
	mapBotLevel[BotLevelClever]			= "Clever";
	GameConfig::gameRules.botLevel = new ConfList("Bot level", &mapBotLevel);
	// Очистка арены от стены игрока после его смерти.
	GameConfig::gameRules.eraseDeathPlayer = new ConfSwitch("Erase death player");

	// Существования пользователя.
	GameConfig::players[0].exist = new ConfSwitch("Exist");
	// Клавиша поворока налево.
	GameConfig::players[0].keyLeft = new ConfKey("Left");
	// Клавиша поворока направо.
	GameConfig::players[0].keyRight = new ConfKey("Right");
	// Существования пользователя.
	GameConfig::players[1].exist = new ConfSwitch("Exist");
	// Клавиша поворока налево.
	GameConfig::players[1].keyLeft = new ConfKey("Left");
	// Клавиша поворока направо.
	GameConfig::players[1].keyRight = new ConfKey("Right");
	// Существования пользователя.
	GameConfig::players[2].exist = new ConfSwitch("Exist");
	// Клавиша поворока налево.
	GameConfig::players[2].keyLeft = new ConfKey("Left");
	// Клавиша поворока направо.
	GameConfig::players[2].keyRight = new ConfKey("Right");
	// Существования пользователя.
	GameConfig::players[3].exist = new ConfSwitch("Exist");
	// Клавиша поворока налево.
	GameConfig::players[3].keyLeft = new ConfKey("Left");
	// Клавиша поворока направо.
	GameConfig::players[3].keyRight = new ConfKey("Right");
	
	// Оформление.
	mapArtPack[ArtPackDefault]			= "Default";
	mapArtPack[ArtPackClassic]			= "Classic";
	GameConfig::video.artPack = new ConfList("Art pack", &mapArtPack);
	// Видеорежим.
	mapScreenMode[ScreenMode320x240]	= "320x240";
	mapScreenMode[ScreenMode512x384]	= "512x384";
	mapScreenMode[ScreenMode640x480]	= "640x480";
	mapScreenMode[ScreenMode800x600]	= "800x600";
	mapScreenMode[ScreenMode1024x768]	= "1024x768";
	mapScreenMode[ScreenMode1280x1024]	= "1280x1024";
	mapScreenMode[ScreenMode1600x1200]	= "1600x1200";
	GameConfig::video.screenMode = new ConfList("Screen mode", &mapScreenMode);
	// Полный экран.
	GameConfig::video.fullScreen = new ConfSwitch("Full screen");

	// Музыка.
	GameConfig::audio.music = new ConfSwitch("Music");
	// Звук.
	GameConfig::audio.sound = new ConfSwitch("Sound");
	// Громкость музыки.
	GameConfig::audio.musicVolume = new ConfProgress("Music volume");
	// Громкость звука.
	GameConfig::audio.soundVolume = new ConfProgress("Sound volume");

	// Быстрый финиш.
	GameConfig::play.fastFinish = new ConfSwitch("Fast finish");
	// Режим камеры.
	mapCameraMode[CameraModeBehind]		= "Behind";
	mapCameraMode[CameraModeCircling]	= "Circling";
	GameConfig::play.cameraMode = new ConfList("Camera mode", &mapCameraMode);
	// Вид.
	mapViewports[ViewportsSingle]		= "Single";
	mapViewports[ViewportsSplit]		= "Split";
	mapViewports[Viewports4player]		= "4 player";
	mapViewports[ViewportsAuto]			= "Auto";
	play.viewports = new ConfList("View ports", &mapViewports);
	// Мини карта.
	mapMiniMap[MiniMapNone]				= "None";
	mapMiniMap[MiniMapSmoll]			= "Smoll";
	mapMiniMap[MiniMapBig]				= "Big";
	play.miniMap = new ConfList("Mini map", &mapMiniMap);

	// Если ошибка при чтении конфигурации из файла.
	if (!ReadConfig())
	{
		// Сброс конфигурации.
		ResetConfig();
	}
}
// Сброс конфигурации.
void GameConfig::ResetConfig()
{
	// Сброс правил игры.
	gameRules.playersCount->SetValue(1);
	gameRules.roundsCount->SetValue(10);
	gameRules.motoSpeed->SetValue(GameConfig::MotoSpeedFast);
	gameRules.arenaSize->SetValue(GameConfig::ArenaSizeSmoll);
	gameRules.botLevel->SetValue(GameConfig::BotLevelMedium);
	gameRules.eraseDeathPlayer->SetValue(0);

	// Сброс настроек игроков.
	players[0].exist->SetValue(1);
	players[0].keyLeft->SetValue(0x014b0001);	// Клавиша влево.
	players[0].keyRight->SetValue(0x014d0001);	// Клавиша вправо.
	players[1].exist->SetValue(0);
	players[1].keyLeft->SetValue(0);			// TODO: управление по-умолчанию.
	players[1].keyRight->SetValue(0);			// TODO: управление по-умолчанию.
	players[2].exist->SetValue(0);
	players[2].keyLeft->SetValue(0);			// TODO: управление по-умолчанию.
	players[2].keyRight->SetValue(0);			// TODO: управление по-умолчанию.
	players[3].exist->SetValue(0);
	players[3].keyLeft->SetValue(0);			// TODO: управление по-умолчанию.
	players[3].keyRight->SetValue(0);			// TODO: управление по-умолчанию.

	// Сброс настроек видео.
	video.artPack->SetValue(GameConfig::ArtPackDefault);
	video.screenMode->SetValue(GameConfig::ScreenMode800x600);
	video.fullScreen->SetValue(0);

	// Сброс настроек аудио.
	audio.music->SetValue(1);
	audio.sound->SetValue(1);
	audio.musicVolume->SetValue(50);
	audio.soundVolume->SetValue(25);

	// Сброс настроек игры.
	play.fastFinish->SetValue(1);
	play.cameraMode->SetValue(GameConfig::CameraModeBehind);
	play.viewports->SetValue(GameConfig::ViewportsSingle);
	play.miniMap->SetValue(GameConfig::MiniMapNone);
}
// Уничтожение конфигурации.
void GameConfig::DestroyConfig()
{
	// Запись конфигурации в файл
	WriteConfig();

	// Уничтожение количества игроков.
	delete gameRules.playersCount;
	// Уничтожение количества раундов.
	delete gameRules.roundsCount;
	// Уничтожение скорости.
	delete gameRules.motoSpeed;
	// Уничтожение размера арены.
	delete gameRules.arenaSize;
	// Уничтожение уровня ботов.
	delete gameRules.botLevel;
	// Уничтожение вариантов оформления.
	delete video.artPack;
	// Уничтожение видеорежима.
	delete video.screenMode;
	// Уничтожение режима камеры.
	delete play.cameraMode;
	// Уничтожение вида.
	delete play.viewports;
	// Уничтожение мини карты.
	delete play.miniMap;	
}

map<BYTE, string> GameConfig::mapMotoSpeed;		// Возможные варианты скорости.    // TODO: инициализация NULL
map<BYTE, string> GameConfig::mapArenaSize;		// Возможные варианты размера арены.
map<BYTE, string> GameConfig::mapBotLevel;		// Возможные варианты уровня ботов.

map<BYTE, string> GameConfig::mapArtPack;		// Возможные варианты оформления.
map<BYTE, string> GameConfig::mapScreenMode;	// Возможные варианты видеорежимоа.

map<BYTE, string> GameConfig::mapCameraMode;	// Возможные варианты режима камеры.
map<BYTE, string> GameConfig::mapViewports;		// Возможные варианты вида.
map<BYTE, string> GameConfig::mapMiniMap;		// Возможные варианты миникары.

// Чтение конфигурации из файла.
bool GameConfig::ReadConfig()
{
	return false;
}

// Запись конфигурации в файл.
void GameConfig::WriteConfig()
{
}
// Конфигурация игры.