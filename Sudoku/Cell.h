#pragma once
#include <SFML/Graphics.hpp>
#include "IDrawable.h"
#include "Collider.h"
#include <iostream>
#include <string>

// класс одной клеточки
class Cell : public IDrawable // унаследуемся от класса, чтоб иметь возможноть рисовать клетку на экране
{
private:
	sf::RenderWindow* window;// ссылка на окно где будем рисовать
	sf::RenderStates* renderStates;// ссылка на параметры рисования в окне
	sf::RectangleShape sprite;// кртинка нашей клетки (спрайт)
	sf::Texture texture;// текстура клетки
	sf::Text caption; // текст клетки
	sf::Font font;// шрифт текста на клетке

	int userNumber; // цифра, которая меняется пользователем
	bool isFail; // флаг, который говорит, правильное ли число выставил пользователь в соответсвии правилам игры
	bool isLocked; // флаг, который говорит, можно ли менять его значения. используется чтоб для клеток, которые пользователь не будет менять

public:
	int correctNumber; // хранит правильное число. именяется только алгоритмами прогрммы
	Collider collider; // коллайдер (кликабельные границы) клетки

	// конструкторы
	Cell() {}
	Cell(sf::RenderWindow* _window, sf::RenderStates* _renderStates, std::string pathTexture, sf::Vector2f size);

	virtual void Draw();// переопределим функцию рисования

	void SetPosition(sf::Vector2f newPos); // функция установки позиции для клеточки
	void SetUserNumber(int value); // устанавливает пользовательское число
	int GetUserNumber(); // получает текущие пользовательское число
	void SetFail(bool state); // устанавливает не коректное ли выставлено число
	bool IsFail(); // получаем информацию не коректное ли сейчас число
	void Lock();// блокируем изменение числа клеточки
	void UnLock(); // разблокируем изменение числа клеточки
	bool IsLock(); // получаем информацию, заблокирована ли сейчас клеточка от изменения числа
};

