#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Cell.h"
#include "Difficult.h"
#include "IDrawable.h"
#include <vector>

// класс контроллирующий процесс игры 
class GridController
{
private:
	sf::RenderWindow* window;
	sf::RenderStates* renderStates;
	sf::Text* winText;

	Cell* matrix[9][9]; // матрица (многомерный массив) всех клеточек (в данном случае игра размером 9х9)

	bool IsCorrectNumber(int x, int y); // фукнкци€ проверки может ли быть число по указаным координатам в соответсвии с правилами игры

	// фукнции дл€ перемешивани€ клеточек, при этом сохран€€ правильность чисел 
	void Transposion(); // функци€ транспонировани€ матрицы клеток
	void SwapRowsSmall(); // обмен строками в одной области (область это 3 на 3 клеточки) по всей ширине
	void SwapColumsSmall(); // аналогичный обмен столбцами в одной области
	void SwapRowsArea(); // обмен радами (беретс€ не строчки а сразу по 3 строки)
	void SwapColumsArea(); // обмен столбцами
	
public:
	Difficult difficultGame; // текуща€ сложность игры
	
	GridController(sf::RenderWindow* _window, sf::RenderStates* _renderStates, sf::Text* _winText); // конструктор

	void ProcessEvent(sf::Event event); // обработчик событий 

	void CheckWin(); // фукнци€ проверки на победу

	void Mix(); // фукнци€ перемешивани€ всех клеточек (заодно и сброс всех пользовательских чисел) дл€ новой игры

	void ChangeDifficult(); // фукцни€ переключени€ сложности
};

