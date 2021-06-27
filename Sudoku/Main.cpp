#include <SFML/Graphics.hpp>
#include "GridController.h"
#include "IDrawable.h"
#include "Difficult.h"
#include "Button.h"
#include <vector>
#include <functional>
#include <time.h>
#include <Windows.h>

using namespace sf;
using namespace std;
using namespace placeholders;

// список объектов для рисования на экране
vector<IDrawable*> objectsToDraw;

int main()
{
	srand(time(0)); // ставим сид случайности по времени
	FreeConsole(); // отключаем вывод консоли

	RenderWindow* window = new RenderWindow(VideoMode(1280, 720), "Sudoku"); // создаем окно с разрешение HD
	RenderStates* renderStates = new RenderStates(); // настройки окна

	// win text
	sf::Text* winText = new Text(); // текст победы
	sf::Font font; // шрифт для текта
	if (font.loadFromFile("Fonts/TTRounds-Black.ttf")) // загружаем шрифт из папочки
		std::cout << "Loaded Font suceful!" << std::endl; // если без проблем загрузилось то выводим инфу в консоль
	winText->setFont(font); // устанавливаем шрифт тексту победы
	winText->setFillColor(sf::Color::Black); // ставим черный цвет текста
	winText->setCharacterSize(25); // 25 размер шрифта
	winText->setString("You playing"); // начальный текст
	winText->setPosition(Vector2f(1000, 240)); // размещаем на экране

	GridController* gridC = new GridController(window, renderStates, winText); // создаем контроллер сетки

	// создаем и настраиваем параметры линий разделяющие большие области сетки
	sf::RectangleShape line1(sf::Vector2f(9 * 64, 4));
	sf::RectangleShape line2(sf::Vector2f(9 * 64, 4));
	sf::RectangleShape line3(sf::Vector2f(4, 9 * 64));
	sf::RectangleShape line4(sf::Vector2f(4, 9 * 64));
	line1.setPosition(100, 3 * 64 + 50 - 2);
	line2.setPosition(100, 6 * 64 + 50 - 2);
	line3.setPosition(3 * 64 + 100 - 2, 50);
	line4.setPosition(6 * 64 + 100 - 2, 50);
	line1.setFillColor(sf::Color::Black);
	line2.setFillColor(sf::Color::Black);
	line3.setFillColor(sf::Color::Black);
	line4.setFillColor(sf::Color::Black);

	// кнопки
	// кнопка перемешать все
	Button* buttonMix = new Button(window, renderStates, "Mix", Vector2f(1000, 50), Vector2f(200, 64));
	buttonMix->Click += bind(&GridController::Mix, gridC); // привязываем к указателю на функцию функцию перемешивания из контроллера сетки
	objectsToDraw.push_back(buttonMix); // добавляем кнопку в список отрисовки

	// кнопка переключения сложности
	Button* buttonDifficult = new Button(window, renderStates, "Easy", Vector2f(1000, 120), Vector2f(200, 64));
	buttonDifficult->Click += bind(&GridController::ChangeDifficult, gridC); // привязываем функцию изменения сложности 
	objectsToDraw.push_back(buttonDifficult); // добавляем кнопку в список отрисовки

	while (window->isOpen()) // игровой цикл пока открыто окно
	{
		Event event; // хранит информацию про текущие событие
		while (window->pollEvent(event)) // цикл для прохода по списку всех произошедших событий
		{
			if (event.type == Event::Closed) // если событие закрытия окна
				window->close(); // то закрываем окно

			if (event.type == Event::Resized) // если событие изменения размера окна
			{
				FloatRect visibleArea = FloatRect(0, 0, event.size.width, event.size.height); // получаем новые размеры окна
				window->setView(View(visibleArea)); // устанавливаем видимую область окна как и размер всего окна
				// пересчитываем режим отображения чтоб масштабировались все елементы в окне по ширине экрана
				renderStates->transform = Transform((float)(event.size.width) / 1280, 0, 0, 0, (float)(event.size.width) / 1280, 0, 0, 0, 1);
			}

			gridC->ProcessEvent(event); // вызываем фукнцию обработки событий для контроллера сетки
			buttonMix->ProcessEvent(event); // аналогичная функция обработки для кнопки перемешивания
			buttonDifficult->ProcessEvent(event); // и кнопки переключения сложности

			switch (gridC->difficultGame) // смотрим на текущую выставленную сложность 
			{
			case Difficult::Easy: // и соответсвтенно меняем текст у кнопки
				buttonDifficult->caption.setString("Easy");
				break;
			case Difficult::Normal:
				buttonDifficult->caption.setString("Normal");
				break;
			case Difficult::Hard:
				buttonDifficult->caption.setString("Hard");
				break;
			}
		}

		window->clear(Color::White); // очищаем экран в белый цвет

		// проходим по всему списку объектов для отрисовки 
		for (auto o : objectsToDraw)
			o->Draw(); // и вызываем ихнию функцию рисования

		window->draw(line1, *renderStates); // дополнительно в ручную рисуем все линии разделения областей
		window->draw(line2, *renderStates);
		window->draw(line3, *renderStates);
		window->draw(line4, *renderStates);

		window->draw(*winText, *renderStates); // и текст победы

		window->display(); // вызываем функцию чтоб все что мы нарисовали отобразилось на экране
	}

	// очищаем память от всех выделеных объектов в динамической памяти
	delete window;
	delete renderStates;
	for (auto o : objectsToDraw)
		delete o;
	objectsToDraw.clear();
	
	return 0; // конец программы
}