#include "GridController.h"

extern std::vector<IDrawable*> objectsToDraw;

GridController::GridController(sf::RenderWindow* _window, sf::RenderStates* _renderStates, sf::Text* _winText)
{
	window = _window;
	renderStates = _renderStates;
	winText = _winText;

	difficultGame = Difficult::Easy;

	// цикл который проходит по всем клеточкам
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			// создаем новую клеточку, выделяем под нее динамическую память и настраиваем
			matrix[y][x] = new Cell(_window, _renderStates, "Cell1.png", sf::Vector2f(64, 64));
			int n = (x + 1) + ((y % 3) * 3) + (y / 3); // формула для задания начальных чисел которые не пересекаються 
			if (n > 9) n -= 9;
			matrix[y][x]->correctNumber = n;
			matrix[y][x]->SetUserNumber(n);
			matrix[y][x]->SetPosition(sf::Vector2f(x * 64 + 100, y * 64 + 50)); // размещаем клеточку на свое место со смещением
			objectsToDraw.push_back(matrix[y][x]); // добавляем клеточку в список для отрисовки
		}
	}

	Mix(); // перемешиваем матрицу клеточек
}

void GridController::ProcessEvent(sf::Event event)
{
	// пересчитываем координаты мыши чтоб правильно работало при разных соотношениях экрана
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);

	float f = 1 / *(renderStates->transform.getMatrix());
	mousePos = sf::Vector2f(f * (float)(mousePos.x), f * (float)(mousePos.y));

	if (event.type == sf::Event::MouseButtonReleased &&
		event.key.code == sf::Mouse::Left) // если сейчас событие отпускания левой кнопки мыши
	{
		// проходимся по всем клеточкам
		for (int y = 0; y < 9; y++)
		{
			for (int x = 0; x < 9; x++)
			{
				if (matrix[y][x]->collider.IsPointInside(mousePos)) // и проверяем попали ли координаты мыши в какую-то клеточку
				{
					int n = matrix[y][x]->GetUserNumber() + 1; // получаем число в клеточке
					if (n > 9) // зацикливаем число 
						n = 0;
					matrix[y][x]->SetUserNumber(n); // устанавиливаем число в клеточку

					CheckWin(); // проверяем правильно ли мы установили число и выиграли ли
					return;
				}
			}
		}
	}
}

bool GridController::IsCorrectNumber(int x, int y)
{
	int checkingNum = matrix[y][x]->GetUserNumber(); // получаем пользовательское число по координатам
	int offsetX = x - (x % 3); // получаем смещение по областям
	int offsetY = y - (y % 3);

	for (int i = 0; i < 9; i++) // проверяем по 9 раз условия
	{
		if (i != x && checkingNum == matrix[y][i]->GetUserNumber()) // ищим одинаковые числа в ряде
			return false;
		if (i != y && checkingNum == matrix[i][x]->GetUserNumber()) // ищим одинаковые числа в столбике
			return false;
		if (((i / 3 + offsetY) != y || (i % 3 + offsetX) != x) && // ищим одинаковые числа в области
			checkingNum == matrix[i / 3 + offsetY][i % 3 + offsetX]->GetUserNumber())
			return false;
	}
	return true; // если не нашли одинаковых то выходим с положительным резуальтатом
}

void GridController::CheckWin()
{
	bool res = true; // ставим флаг результат буд-то выиграли
	for (int y = 0; y < 9; y++) // проходимся по всем клеточкам
	{
		for (int x = 0; x < 9; x++)
		{
			bool fail = !IsCorrectNumber(x, y); // проверяем на правильность каждую клеточку
			matrix[y][x]->SetFail(fail); // устанавливаем соответсвенно результат правильная ли она

			if (fail || matrix[y][x]->GetUserNumber() == 0) // и если она не правильная или она пустая (равна 0) то
				res = false; // устанавливаем что проиграли
		}
	}

	// win
	if (res) // если все числа на своих местах и по правилам игры то
	{
		std::cout << "Win game" << std::endl; // выводим победный текст в консоль
		winText->setString("You WIN!!!"); // и в текст на экране
	}
}

void GridController::ChangeDifficult()
{
	if (difficultGame == Difficult::Easy) // зацикленное переключение сложности 
		difficultGame = Difficult::Normal;
	else if (difficultGame == Difficult::Normal)
		difficultGame = Difficult::Hard;
	else if (difficultGame == Difficult::Hard)
		difficultGame = Difficult::Easy;
}

void GridController::Mix()
{
	winText->setString("You playing"); // сбрасываем в начальный текст текст победы
	for (int y = 0; y < 9; y++) // проходимся по всем клеточкам 
	{
		for (int x = 0; x < 9; x++)
		{
			matrix[y][x]->UnLock(); // и разблокирываем
			matrix[y][x]->SetUserNumber(0); // и чистим клеточку от пользовательского ввода
		}
	}

	for (int i = 0; i < 15; i++) // 15 раз применяем фукнции перемешивания
	{
		switch (rand() % 5) // случайно выбираем функцию
		{
		case 0: Transposion(); break;
		case 1: SwapRowsSmall(); break;
		case 2: SwapColumsSmall(); break;
		case 3: SwapRowsArea(); break;
		case 4: SwapColumsArea(); break;
		}
	}

	int counterLocked = 0;
	int randX, randY;
	while (counterLocked < (int)difficultGame) // устанавливаем случайные клеточки как подсказки. количество зависит от сложности игры
	{
		randX = rand() % 9; // берем случайную позицию
		randY = rand() % 9;
		if (matrix[randY][randX]->IsLock() == false) // и если клеточка уже не заблокирована
		{
			counterLocked++; // то отсчитываем заблокированые клеточки 
			matrix[randY][randX]->SetUserNumber(matrix[randY][randX]->correctNumber); // устанавливаем правильное число как пользовательское
			matrix[randY][randX]->Lock(); // блокируем клеточку
		}
	}
}

void GridController::Transposion()
{
	int corNum = 0;
	int usrNum = 0;
	for (int y = 0; y < 9; y++)
	{
		for (int x = y + 1; x < 9; x++)
		{
			// обмен значений двух клеточек
			corNum = matrix[x][y]->correctNumber;
			usrNum = matrix[x][y]->GetUserNumber();

			matrix[x][y]->correctNumber = matrix[y][x]->correctNumber;
			matrix[x][y]->SetUserNumber(matrix[y][x]->GetUserNumber());

			matrix[y][x]->correctNumber = corNum;
			matrix[y][x]->SetUserNumber(usrNum);
		}
	}
}

void GridController::SwapRowsSmall()
{
	int corNum = 0;
	int usrNum = 0;
	int area = rand() % 3;
	int line1 = rand() % 3;
	int line2 = rand() % 3;
	while (line2 == line1)
		line2 = rand() % 3;

	line1 += area * 3;
	line2 += area * 3;

	for (int x = 0; x < 9; x++)
	{
		// обмен значений двух клеточек
		corNum = matrix[line1][x]->correctNumber;
		usrNum = matrix[line1][x]->GetUserNumber();

		matrix[line1][x]->correctNumber = matrix[line2][x]->correctNumber;
		matrix[line1][x]->SetUserNumber(matrix[line2][x]->GetUserNumber());

		matrix[line2][x]->correctNumber = corNum;
		matrix[line2][x]->SetUserNumber(usrNum);
	}
}

void GridController::SwapColumsSmall()
{
	int corNum = 0;
	int usrNum = 0;
	int area = rand() % 3;
	int col1 = rand() % 3;
	int col2 = rand() % 3;
	while (col2 == col1)
		col2 = rand() % 3;

	col1 += area * 3;
	col2 += area * 3;

	for (int y = 0; y < 9; y++)
	{
		// обмен значений двух клеточек
		corNum = matrix[y][col1]->correctNumber;
		usrNum = matrix[y][col1]->GetUserNumber();

		matrix[y][col1]->correctNumber = matrix[y][col2]->correctNumber;
		matrix[y][col1]->SetUserNumber(matrix[y][col2]->GetUserNumber());

		matrix[y][col2]->correctNumber = corNum;
		matrix[y][col2]->SetUserNumber(usrNum);
	}
}

void GridController::SwapRowsArea()
{
	int corNum = 0;
	int usrNum = 0;
	int area1 = rand() % 3;
	int area2 = rand() % 3;
	while (area2 == area1)
		area2 = rand() % 3;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			// обмен значений двух клеточек
			corNum = matrix[area1 * 3 + y][x]->correctNumber;
			usrNum = matrix[area1 * 3 + y][x]->GetUserNumber();

			matrix[area1 * 3 + y][x]->correctNumber = matrix[area2 * 3 + y][x]->correctNumber;
			matrix[area1 * 3 + y][x]->SetUserNumber(matrix[area2 * 3 + y][x]->GetUserNumber());

			matrix[area2 * 3 + y][x]->correctNumber = corNum;
			matrix[area2 * 3 + y][x]->SetUserNumber(usrNum);
		}
	}
}

void GridController::SwapColumsArea()
{
	int corNum = 0;
	int usrNum = 0;
	int area1 = rand() % 3;
	int area2 = rand() % 3;
	while (area2 == area1)
		area2 = rand() % 3;

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 9; y++)
		{
			// обмен значений двух клеточек
			corNum = matrix[y][area1 * 3 + x]->correctNumber;
			usrNum = matrix[y][area1 * 3 + x]->GetUserNumber();

			matrix[y][area1 * 3 + x]->correctNumber = matrix[y][area2 * 3 + x]->correctNumber;
			matrix[y][area1 * 3 + x]->SetUserNumber(matrix[y][area2 * 3 + x]->GetUserNumber());

			matrix[y][area2 * 3 + x]->correctNumber = corNum;
			matrix[y][area2 * 3 + x]->SetUserNumber(usrNum);
		}
	}
}
