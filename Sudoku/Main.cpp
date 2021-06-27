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

// ������ �������� ��� ��������� �� ������
vector<IDrawable*> objectsToDraw;

int main()
{
	srand(time(0)); // ������ ��� ����������� �� �������
	FreeConsole(); // ��������� ����� �������

	RenderWindow* window = new RenderWindow(VideoMode(1280, 720), "Sudoku"); // ������� ���� � ���������� HD
	RenderStates* renderStates = new RenderStates(); // ��������� ����

	// win text
	sf::Text* winText = new Text(); // ����� ������
	sf::Font font; // ����� ��� �����
	if (font.loadFromFile("Fonts/TTRounds-Black.ttf")) // ��������� ����� �� �������
		std::cout << "Loaded Font suceful!" << std::endl; // ���� ��� ������� ����������� �� ������� ���� � �������
	winText->setFont(font); // ������������� ����� ������ ������
	winText->setFillColor(sf::Color::Black); // ������ ������ ���� ������
	winText->setCharacterSize(25); // 25 ������ ������
	winText->setString("You playing"); // ��������� �����
	winText->setPosition(Vector2f(1000, 240)); // ��������� �� ������

	GridController* gridC = new GridController(window, renderStates, winText); // ������� ���������� �����

	// ������� � ����������� ��������� ����� ����������� ������� ������� �����
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

	// ������
	// ������ ���������� ���
	Button* buttonMix = new Button(window, renderStates, "Mix", Vector2f(1000, 50), Vector2f(200, 64));
	buttonMix->Click += bind(&GridController::Mix, gridC); // ����������� � ��������� �� ������� ������� ������������� �� ����������� �����
	objectsToDraw.push_back(buttonMix); // ��������� ������ � ������ ���������

	// ������ ������������ ���������
	Button* buttonDifficult = new Button(window, renderStates, "Easy", Vector2f(1000, 120), Vector2f(200, 64));
	buttonDifficult->Click += bind(&GridController::ChangeDifficult, gridC); // ����������� ������� ��������� ��������� 
	objectsToDraw.push_back(buttonDifficult); // ��������� ������ � ������ ���������

	while (window->isOpen()) // ������� ���� ���� ������� ����
	{
		Event event; // ������ ���������� ��� ������� �������
		while (window->pollEvent(event)) // ���� ��� ������� �� ������ ���� ������������ �������
		{
			if (event.type == Event::Closed) // ���� ������� �������� ����
				window->close(); // �� ��������� ����

			if (event.type == Event::Resized) // ���� ������� ��������� ������� ����
			{
				FloatRect visibleArea = FloatRect(0, 0, event.size.width, event.size.height); // �������� ����� ������� ����
				window->setView(View(visibleArea)); // ������������� ������� ������� ���� ��� � ������ ����� ����
				// ������������� ����� ����������� ���� ���������������� ��� �������� � ���� �� ������ ������
				renderStates->transform = Transform((float)(event.size.width) / 1280, 0, 0, 0, (float)(event.size.width) / 1280, 0, 0, 0, 1);
			}

			gridC->ProcessEvent(event); // �������� ������� ��������� ������� ��� ����������� �����
			buttonMix->ProcessEvent(event); // ����������� ������� ��������� ��� ������ �������������
			buttonDifficult->ProcessEvent(event); // � ������ ������������ ���������

			switch (gridC->difficultGame) // ������� �� ������� ������������ ��������� 
			{
			case Difficult::Easy: // � �������������� ������ ����� � ������
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

		window->clear(Color::White); // ������� ����� � ����� ����

		// �������� �� ����� ������ �������� ��� ��������� 
		for (auto o : objectsToDraw)
			o->Draw(); // � �������� ����� ������� ���������

		window->draw(line1, *renderStates); // ������������� � ������ ������ ��� ����� ���������� ��������
		window->draw(line2, *renderStates);
		window->draw(line3, *renderStates);
		window->draw(line4, *renderStates);

		window->draw(*winText, *renderStates); // � ����� ������

		window->display(); // �������� ������� ���� ��� ��� �� ���������� ������������ �� ������
	}

	// ������� ������ �� ���� ��������� �������� � ������������ ������
	delete window;
	delete renderStates;
	for (auto o : objectsToDraw)
		delete o;
	objectsToDraw.clear();
	
	return 0; // ����� ���������
}