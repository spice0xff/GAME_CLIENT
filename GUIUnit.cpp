#include "stdafx.h"

#include "MainUnit.h"
#include "ConfUnit.h"
#include "GameUnit.h"
#include "GUIUnit.h"


// Базовый класс всех GUI.
// Конструктор.
GUIBase::GUIBase()
{
}
// GUI в виде строки.
//string GUIBase::ToString()
//{
//	return this->caption;
//}
// Базовый класс всех GUI.

// Меню.
// Конструктор.
Menu::Menu(string caption):
caption(caption), parent(NULL), items(), selectedItem(items.begin())
{
}
// Меню в виде строки.
string Menu::ToString()
{
	return this->caption;
}
// Нажатие клавиши.
void Menu::KeyDown(BYTE virtualKey, DWORD keyData)
{
	// Если есть элемент меню, удерживающий фокус
	if (HoldGUI)
	{
		// Отправка нажатой клавиши удерживающему элементу
		(*this->selectedItem)->KeyDown(virtualKey, keyData);
		return;
	}

	// Если нажата клавиша эскейп
	if (virtualKey == VK_ESCAPE)
	{
		// Переход в родительское меню
		Menu::Leave();
		return;
	}

	// Если в меню нет элементов
	if (this->items.empty())
	{
		return;
	}

	// Если нажата клавиша вниз
	if (virtualKey == VK_DOWN)
	{
		// Переход к слудующему элементу
		Menu::Next();
		return;
	}
	// Если нажата клавиша вверх
	if (virtualKey == VK_UP)
	{
		// Переход к предыдущему элементу
		Menu::Prev();
		return;
	}

	// Если выделенный элемент - меню
	if ((*this->selectedItem)->GetType() == GUITypeMenu)
	{
		// Если нажата клавиша энтер
		if (virtualKey == VK_RETURN)
		{
			// Переход в дочернее меню
			Menu::Enter();
			return;
		}
	}
	// Если выделенный элемент - не меню
	else
	{
		// Отправка нажатой клавиши выделенному элементу
		(*this->selectedItem)->KeyDown(virtualKey, keyData);	
	}
}
// Возврат типа GUI.
const GUIType Menu::GetType()
{
	return GUITypeMenu;
}
// Добавление элемента в меню.
void Menu::AddItem(GUIBase *item)
{
	this->items.push_back(item);
	
	// Если добавлен первый элемент
	if (this->items.size() == 1)
	{
		this->selectedItem = this->items.begin();
	}

	// Если добавлено меню
	if (item->GetType() == GUITypeMenu)
	{
		// Установка его родителя
		((Menu*)(item))->parent = this;
	}
}
// Переход к слудующему элементу.
void Menu::Next()
{
	// Если элементов нет
	if (this->items.empty())
	{
		return;
	}
	
	if (++this->selectedItem == this->items.end())
	{
		this->selectedItem = this->items.begin();
	}
}
// Переход к предыдущему элементу.
void Menu::Prev()
{
	// Если элементов нет
	if (this->items.empty())
	{
		return;
	}
	
	if (this->selectedItem == this->items.begin())
	{
		this->selectedItem = --this->items.end();
	}
	else
	{
		this->selectedItem--;
	}
}
// Переход в дочернее меню.
void Menu::Enter()
{
	// Переход в дочернее меню
	ActiveMenu = (Menu*)(*ActiveMenu->selectedItem);
}
// Переход в родительское меню.
void Menu::Leave()
{
	// Если есть родительское меню
	if (this->parent)
	{
		// Выделение первого элемента
		this->selectedItem = this->items.begin();

		// Переход в родительское меню
		ActiveMenu = this->parent;
	}
}
// Меню.

// Метка.
// Конструктор.
Label::Label(string caption):
caption(caption)
{
}
// Возврат типа GUI.
const GUIType Label::GetType()
{
	return GUITypeLabel;
}
// Метка в виде строки.
string Label::ToString()
{
	return this->caption;
}
// Нажатие клавиши.
void Label::KeyDown(BYTE virtualKey, DWORD keyData)
{
}
// Метка.

// Кнопка.
// Конструктор.
Button::Button(string caption, void (*function)()):
caption(caption), function(function)
{
}
// Возврат типа GUI.
const GUIType Button::GetType()
{
	return GUITypeButton;
}
// Кнопка в виде строки.
string Button::ToString()
{
	return this->caption;
}
// Нажатие клавиши.
void Button::KeyDown(BYTE virtualKey, DWORD keyData)
{
	if (virtualKey == VK_RETURN)
	{
		// Нажатие кнопки
		Button::Click();
	}
}
// Нажатие кнопки.
void Button::Click()
{
	this->function();
}
// Кнопка.

// Клавиша.
// Конструктор.
Key::Key(ConfKey *confKey):
confKey(confKey), edit(false)
{
}
// Возврат типа GUI.
const GUIType Key::GetType()
{
	return GUITypeKey;
}
// Клавиша в виде строки.
string Key::ToString()
{
	return this->confKey->GetName() + ": " + this->confKey->GetStrValue();
}
// Нажатие клавиши.
void Key::KeyDown(BYTE virtualKey, DWORD keyData)
{
	// Если не режим редактирования.
	if (!this->edit)
	{
		// Есил нажат Enter
		if (virtualKey == VK_RETURN)
		{
			// Удержание фокуса.
			HoldGUI = this;
			// Отображение подсказки.
			HintGUI.caption = "Press a key";
			// Сброс значения конфигурационной клавиши.
			this->confKey->SetValue(0);
			// Преход в режим редактирования.
			this->edit = true;
		}
	}
	// Если режим редактированиея.
	else
	{
		// Если допустимая клавиша.
		if
		(
			(virtualKey >= '0' && virtualKey <= '9') ||
			(virtualKey >= 'A' && virtualKey <= 'Z') ||
			(virtualKey >= VK_NUMPAD0 && virtualKey <= VK_NUMPAD9))
		{
			// Отпускание фокуса.
			HoldGUI = NULL;
			// Скрытие подсказки.
			HintGUI.caption = "";
			// Установка значения конфигурационной клавиши.
			this->confKey->SetValue(keyData);
			// Возврат из режима редактирования.
			this->edit = false;
		}
	}
}
// Клавиша.

// Текст.
// Конструктор.
Text::Text(ConfText *confText): confText(confText), text(""), edit(false)
{
}
// Возврат типа GUI.
const GUIType Text::GetType()
{
	return GUITypeText;
}
// Текст в виде строки.
string Text::ToString()
{
	// Если режим редактирования.
	if (this->edit)
	{
		return this->confText->GetName() + ": " + this->text;
	}
	// Если не режим редактирования.
	else
	{
		return this->confText->GetName() + ": " + this->confText->GetValue();
	}
}
// Нажатие клавиши.
void Text::KeyDown(BYTE virtualKey, DWORD keyData)
{
	// Если не редактирование.
	if (!this->edit)
	{
		// Есил нажат Enter.
		if (virtualKey == VK_RETURN)
		{
			// Удержание фокуса.
			HoldGUI = this;
			// Отображение подсказки.
			HintGUI.caption = "Enter text";
			// Установка текста.
			this->text = this->confText->GetValue();
			// Преход в режим редактирования.
			this->edit = true;
		}
	}
	// Если редактирование.
	else
	{
		// Есил нажат Enter.
		if (virtualKey == VK_RETURN)
		{
			// Отпускание фокуса.
			HoldGUI = NULL;
			// Скрытие подсказки.
			HintGUI.caption = "";
			// Установка значения конфигурационного текста.
			if (!this->confText->SetRawValue(this->text))
			{
			
			}
			// Возврат из режима редактирования.
			this->edit = false;
		}
		else
		{
			// Поучение Ascii-кода клавиши.
			BYTE keys[256];
			GetKeyboardState(keys);
			WORD asciiCode;
			ToAscii(virtualKey, keyData, keys, &asciiCode, NULL);

			// Если backspace.
			if ((char)asciiCode == 8)
			{
				if (this->text.length() != 0)
				{
					this->text = this->text.substr(0, this->text.length() - 1);
				}
			}
			
			// Если допустимый ASCII-код.
			if ((char)asciiCode >= 33 && (char)asciiCode <= 126)
			{
				if (this->text.length() != CONF_MAX_TEXT_LEN)
				{
					this->text += (char)asciiCode;
				}
			}
		}
	}
}
// Текст.

// Число.
// Конструктор.
Number::Number(ConfNumber *confNumber): confNumber(confNumber), text(""), edit(false)
{
}
// Возврат типа GUI.
const GUIType Number::GetType()
{
	return GUITypeNumber;
}
// Число в виде строки.
string Number::ToString()
{
	// Если режим редактирования.
	if (this->edit)
	{
		return this->confNumber->GetName() + ": " + this->text;
	}
	// Если не режим редактирования.
	else
	{
		return this->confNumber->GetName() + ": " + this->confNumber->GetStrValue();
	}
}
// Нажатие клавиши.
void Number::KeyDown(BYTE virtualKey, DWORD keyData)
{
	// Если не режим редактирования.
	if (!this->edit)
	{
		// Есил нажат Enter.
		if (virtualKey == VK_RETURN)
		{
			// Удержание фокуса.
			HoldGUI = this;
			// Отображение подсказки.
			HintGUI.caption = "Enter number";
			// Установка текста.
			this->text = this->confNumber->GetStrValue();
			// Преход в режим редактирования.
			this->edit = true;
		}
	}
	// Если режим редактирования.
	else
	{
		// Если нажат Enter.
		if (virtualKey == VK_RETURN)
		{
			// Отпускание фокуса.
			HoldGUI = NULL;
			// Скрытие подсказки.
			HintGUI.caption = "";
			// Установка значения конфигурационного числа.
			this->confNumber->SetRawValue(this->text);
			// Возврат из режима редактирования.
			this->edit = false;
		}
		// Если нажат не Enter.
		else
		{
			// Если backspace.
			if (virtualKey == VK_BACK)
			{
				if (this->text.length() != 0)
				{
					this->text = this->text.substr(0, this->text.length() - 1);
				}
			}

			// Если допустимый VIRTUAL-код.
			if (virtualKey >= '0' && virtualKey <= '9')
			{
				if (this->text.length() != 3)
				{
					this->text += (char)virtualKey;
				}
			}
		}
	}
}
// Число.

// Список.
// Конструктор.
List::List(ConfList *confList):
confList(confList)
{
}
// Возврат типа GUI.
const GUIType List::GetType()
{
	return GUITypeList;
}
// Список в виде строки.
string List::ToString()
{
	return this->confList->GetName() + ": " + this->confList->GetStrValue();
}
// Нажатие клавиши.
void List::KeyDown(BYTE virtualKey, DWORD keyData)
{
	// Если клавиша вправо или энтер.
	if (virtualKey == VK_RIGHT || virtualKey == VK_RETURN)
	{
		// Слудующий элемент.
		this->confList->Next();
	}

	// Если клавиша влево.
	if (virtualKey == VK_LEFT)
	{
		// Предыдущий элемент.
		this->confList->Prev();
	}
}
// Список.

// Переключатель.
// Конструктор.
Switch::Switch(ConfSwitch *confSwitch):
confSwitch(confSwitch)
{
}
// Возврат типа GUI.
const GUIType Switch::GetType()
{
	return GUITypeSwitch;
}
// Переключатель в виде строки.
string Switch::ToString()
{
	return this->confSwitch->GetName() + ": " + this->confSwitch->GetStrValue();
}
// Нажатек клавиши.
void Switch::KeyDown(BYTE virtualKey, DWORD keyData)
{
	// Если нажат Enter.
	if (virtualKey == VK_RETURN)
	{
		// Перекллючение.
		this->confSwitch->Switch();
	}	
}
// Переключатель.

// Прогресс.
// Конструктор.
Progress::Progress(ConfProgress *confProgress):
confProgress(confProgress)
{
}
// Возврат типа GUI.
const GUIType Progress::GetType()
{
	return GUITypeProgress;
}
// Прогресс в виде строки.
string Progress::ToString()
{
	return this->confProgress->GetName() + ": " + this->confProgress->GetStrValue() + "%%";
}
// Нажатек клавиши.
void Progress::KeyDown(BYTE virtualKey, DWORD keyData)
{
	// Если клавиша вправо.
	if (virtualKey == VK_RIGHT)
	{
		// Увелечение значения.
		this->confProgress->More();
	}

	// Если клавиша влево.
	if (virtualKey == VK_LEFT)
	{
		// Уменьшенеие значения.
		this->confProgress->Less();
	}
}
// Прогресс.


Menu		*ActiveMenu		= NULL;			// Активное меню.
GUIBase		*HoldGUI		= NULL;			// GUI, удерживающий фокус.
Label		HintGUI("");					// Подсказка.


// Создание GUI
void CreateGUI()
{
	// root
	Menu *guiMenuRoot = new Menu("Root");
		// local game
		Menu *guiMenuLocalGame = new Menu("Local game");
			// game rules
			Menu *guiMenuGameRules = new Menu("Game rules");

			guiMenuGameRules->AddItem((GUIBase*)new Number(GameConfig::gameRules.playersCount));
			guiMenuGameRules->AddItem((GUIBase*)new Number(GameConfig::gameRules.roundsCount));
			guiMenuGameRules->AddItem((GUIBase*)new List(GameConfig::gameRules.motoSpeed));
			guiMenuGameRules->AddItem((GUIBase*)new List(GameConfig::gameRules.arenaSize));
			guiMenuGameRules->AddItem((GUIBase*)new List(GameConfig::gameRules.botLevel));
			guiMenuGameRules->AddItem((GUIBase*)new Switch(GameConfig::gameRules.eraseDeathPlayer));
			// game rules

			// configure players
			Menu *guiMenuPlayers = new Menu("Players");

			guiMenuPlayers->AddItem((GUIBase*)new Switch(GameConfig::players[0].exist));
			guiMenuPlayers->AddItem((GUIBase*)new Switch(GameConfig::players[1].exist));
			guiMenuPlayers->AddItem((GUIBase*)new Switch(GameConfig::players[2].exist));
			guiMenuPlayers->AddItem((GUIBase*)new Switch(GameConfig::players[3].exist));
			// configure players

			// configure keys
			Menu *guiMenuKeys = new Menu("Keys");
				// player 1
					Menu *guiMenuPlayer1 = new Menu("Player 1");

					guiMenuPlayer1->AddItem((GUIBase*)new Key(GameConfig::players[0].keyLeft));
					guiMenuPlayer1->AddItem((GUIBase*)new Key(GameConfig::players[0].keyRight));
				// player 1

				// player 2
					Menu *guiMenuPlayer2 = new Menu("Player 2");

					guiMenuPlayer2->AddItem((GUIBase*)new Key(GameConfig::players[1].keyLeft));
					guiMenuPlayer2->AddItem((GUIBase*)new Key(GameConfig::players[1].keyRight));
				// player 2

				// player 3
					Menu *guiMenuPlayer3 = new Menu("Player 3");

					guiMenuPlayer3->AddItem((GUIBase*)new Key(GameConfig::players[2].keyLeft));
					guiMenuPlayer3->AddItem((GUIBase*)new Key(GameConfig::players[2].keyRight));
				// player 3

				// player 4
					Menu *guiMenuPlayer4 = new Menu("Player 4");

					guiMenuPlayer4->AddItem((GUIBase*)new Key(GameConfig::players[3].keyLeft));
					guiMenuPlayer4->AddItem((GUIBase*)new Key(GameConfig::players[3].keyRight));
				// player 4

			guiMenuKeys->AddItem(guiMenuPlayer1);
			guiMenuKeys->AddItem(guiMenuPlayer2);
			guiMenuKeys->AddItem(guiMenuPlayer3);
			guiMenuKeys->AddItem(guiMenuPlayer4);
			// configure keys

		guiMenuLocalGame->AddItem((GUIBase*)new Button("Start game", CreateGame));
		guiMenuLocalGame->AddItem((GUIBase*)new Button("Reset scores", ResetScores));
		guiMenuLocalGame->AddItem((GUIBase*)guiMenuGameRules);
		guiMenuLocalGame->AddItem((GUIBase*)guiMenuPlayers);
		guiMenuLocalGame->AddItem((GUIBase*)guiMenuKeys);
		// local game

		// net game
		Menu *guiMenuNetGame = new Menu("Net game");
			// create game
			Menu *guiMenuCreateGame = new Menu("Create game");

//			guiMenuCreateGame->AddItem((GUIBase*)new Text("Name", "1"));//TODO: хранить имя пользователя и игры??
			guiMenuCreateGame->AddItem((GUIBase*)new Number(GameConfig::gameRules.playersCount));
			guiMenuCreateGame->AddItem((GUIBase*)new Number(GameConfig::gameRules.roundsCount));
			guiMenuCreateGame->AddItem((GUIBase*)new List(GameConfig::gameRules.motoSpeed));
			guiMenuCreateGame->AddItem((GUIBase*)new List(GameConfig::gameRules.arenaSize));
			guiMenuCreateGame->AddItem((GUIBase*)new List( GameConfig::gameRules.botLevel));
			guiMenuCreateGame->AddItem((GUIBase*)new Switch(GameConfig::gameRules.eraseDeathPlayer));
//			guiMenuCreateGame->AddItem((GUIBase*)new Button("Create", &CreateGame));
			guiMenuCreateGame->AddItem((GUIBase*)new Button("Create", NULL));
			// create game

			// connect to game
			Menu *guiMenuConnectToGame = new Menu("Connect to game");

//			guiMenuConnectToGame->AddItem((GUIBase*)guiListGames);
//			guiMenuConnectToGame->AddItem((GUIBase*)new Button("Connect", ConnectToGame));
			// connect to game
		
		guiMenuNetGame->AddItem((GUIBase*)guiMenuCreateGame);
		guiMenuNetGame->AddItem((GUIBase*)guiMenuConnectToGame);
		// net game

		// settings
		Menu *guiMenuSettings = new Menu("Settings");
			// video
			Menu *guiMenuVideo = new Menu("Video");
				// detail
				Menu *guiMenuDetail = new Menu("Detail");
				// detail
			
				// screen
				Menu *guiMenuScreen = new Menu("Screen");

				guiMenuScreen->AddItem((GUIBase*)new List(GameConfig::video.screenMode));
				guiMenuScreen->AddItem((GUIBase*)new Switch(GameConfig::video.fullScreen));
				// screen

			guiMenuVideo->AddItem((GUIBase*)new List(GameConfig::video.artPack));
			guiMenuVideo->AddItem((GUIBase*)guiMenuDetail);
			guiMenuVideo->AddItem((GUIBase*)guiMenuScreen);
			// video

			// audio
			Menu *guiMenuAudio = new Menu("Audio");

			guiMenuAudio->AddItem((GUIBase*)new Switch(GameConfig::audio.music));
			guiMenuAudio->AddItem((GUIBase*)new Switch(GameConfig::audio.sound));
			guiMenuAudio->AddItem((GUIBase*)new Progress(GameConfig::audio.musicVolume));
			guiMenuAudio->AddItem((GUIBase*)new Progress(GameConfig::audio.soundVolume));
//			guiMenuAudio->AddItem((GUIBase*)guiLabelSong);
			// audio

			// play
			Menu *guiMenuPlay = new Menu("Play");

			guiMenuPlay->AddItem((GUIBase*)new Switch(GameConfig::play.fastFinish));
			guiMenuPlay->AddItem((GUIBase*)new List(GameConfig::play.cameraMode));
			guiMenuPlay->AddItem((GUIBase*)new List(GameConfig::play.viewports));
			guiMenuPlay->AddItem((GUIBase*)new List(GameConfig::play.miniMap));			
			// play

		guiMenuSettings->AddItem((GUIBase*)guiMenuVideo);
		guiMenuSettings->AddItem((GUIBase*)guiMenuAudio);
		guiMenuSettings->AddItem((GUIBase*)guiMenuPlay);
		guiMenuSettings->AddItem((GUIBase*)new Button("Reset all settings", &(GameConfig::ResetConfig)));
		// settings

	guiMenuRoot->AddItem((GUIBase*)guiMenuLocalGame);
	guiMenuRoot->AddItem((GUIBase*)guiMenuNetGame);
	guiMenuRoot->AddItem((GUIBase*)guiMenuSettings);
	guiMenuRoot->AddItem((GUIBase*)new Button("Exit", &MainLoopBreak));
	// root

	ActiveMenu = guiMenuRoot;
}

// Уничтожение GUI
void DestroyGUI()
{
	// TODO: new -> delete
}