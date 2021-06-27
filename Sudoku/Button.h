#pragma once
#include <SFML/Graphics.hpp>
#include "IDrawable.h"
#include "Collider.h"
#include "Action.h"
#include <string>
#include <iostream>

// класс кнопки 
class Button :
    public IDrawable // унаследываем его от класса для рисования на экране
{
private:
	sf::RenderWindow* window; // ссылка на окно где будем рисовать
	sf::RenderStates* renderStates; // ссылка на параметры рисования в окне
	sf::RectangleShape sprite; // кртинка нашей кнопки (спрайт)
	sf::Texture texture; // текстура кнопки
	sf::Font font; // шрифт текста на кнопке

public:
    Collider collider; // коллайдер для определения нажатия на кнопку
	Action Click; // событие которое сработает при нажатии кнопки
	sf::Text caption; // текст кнопки

	// конструкторы
	Button() {}
	Button(sf::RenderWindow* _window, sf::RenderStates* _renderStates, std::string _caption, sf::Vector2f pos, sf::Vector2f size);

	void ProcessEvent(sf::Event event); // функция для обработки событий 

	virtual void Draw(); // переопределим функцию рисования
};

