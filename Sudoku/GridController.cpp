#include "GridController.h"

extern std::vector<IDrawable*> objectsToDraw;

GridController::GridController(sf::RenderWindow* _window, sf::RenderStates* _renderStates, sf::Text* _winText)
{
	window = _window;
	renderStates = _renderStates;
	winText = _winText;

	difficultGame = Difficult::Easy;

	// ���� ������� �������� �� ���� ���������
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			// ������� ����� ��������, �������� ��� ��� ������������ ������ � �����������
			matrix[y][x] = new Cell(_window, _renderStates, "Cell1.png", sf::Vector2f(64, 64));
			int n = (x + 1) + ((y % 3) * 3) + (y / 3); // ������� ��� ������� ��������� ����� ������� �� ������������� 
			if (n > 9) n -= 9;
			matrix[y][x]->correctNumber = n;
			matrix[y][x]->SetUserNumber(n);
			matrix[y][x]->SetPosition(sf::Vector2f(x * 64 + 100, y * 64 + 50)); // ��������� �������� �� ���� ����� �� ���������
			objectsToDraw.push_back(matrix[y][x]); // ��������� �������� � ������ ��� ���������
		}
	}

	Mix(); // ������������ ������� ��������
}

void GridController::ProcessEvent(sf::Event event)
{
	// ������������� ���������� ���� ���� ��������� �������� ��� ������ ������������ ������
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);

	float f = 1 / *(renderStates->transform.getMatrix());
	mousePos = sf::Vector2f(f * (float)(mousePos.x), f * (float)(mousePos.y));

	if (event.type == sf::Event::MouseButtonReleased &&
		event.key.code == sf::Mouse::Left) // ���� ������ ������� ���������� ����� ������ ����
	{
		// ���������� �� ���� ���������
		for (int y = 0; y < 9; y++)
		{
			for (int x = 0; x < 9; x++)
			{
				if (matrix[y][x]->collider.IsPointInside(mousePos)) // � ��������� ������ �� ���������� ���� � �����-�� ��������
				{
					int n = matrix[y][x]->GetUserNumber() + 1; // �������� ����� � ��������
					if (n > 9) // ����������� ����� 
						n = 0;
					matrix[y][x]->SetUserNumber(n); // �������������� ����� � ��������

					CheckWin(); // ��������� ��������� �� �� ���������� ����� � �������� ��
					return;
				}
			}
		}
	}
}

bool GridController::IsCorrectNumber(int x, int y)
{
	int checkingNum = matrix[y][x]->GetUserNumber(); // �������� ���������������� ����� �� �����������
	int offsetX = x - (x % 3); // �������� �������� �� ��������
	int offsetY = y - (y % 3);

	for (int i = 0; i < 9; i++) // ��������� �� 9 ��� �������
	{
		if (i != x && checkingNum == matrix[y][i]->GetUserNumber()) // ���� ���������� ����� � ����
			return false;
		if (i != y && checkingNum == matrix[i][x]->GetUserNumber()) // ���� ���������� ����� � ��������
			return false;
		if (((i / 3 + offsetY) != y || (i % 3 + offsetX) != x) && // ���� ���������� ����� � �������
			checkingNum == matrix[i / 3 + offsetY][i % 3 + offsetX]->GetUserNumber())
			return false;
	}
	return true; // ���� �� ����� ���������� �� ������� � ������������� ������������
}

void GridController::CheckWin()
{
	bool res = true; // ������ ���� ��������� ���-�� ��������
	for (int y = 0; y < 9; y++) // ���������� �� ���� ���������
	{
		for (int x = 0; x < 9; x++)
		{
			bool fail = !IsCorrectNumber(x, y); // ��������� �� ������������ ������ ��������
			matrix[y][x]->SetFail(fail); // ������������� ������������� ��������� ���������� �� ���

			if (fail || matrix[y][x]->GetUserNumber() == 0) // � ���� ��� �� ���������� ��� ��� ������ (����� 0) ��
				res = false; // ������������� ��� ���������
		}
	}

	// win
	if (res) // ���� ��� ����� �� ����� ������ � �� �������� ���� ��
	{
		std::cout << "Win game" << std::endl; // ������� �������� ����� � �������
		winText->setString("You WIN!!!"); // � � ����� �� ������
	}
}

void GridController::ChangeDifficult()
{
	if (difficultGame == Difficult::Easy) // ����������� ������������ ��������� 
		difficultGame = Difficult::Normal;
	else if (difficultGame == Difficult::Normal)
		difficultGame = Difficult::Hard;
	else if (difficultGame == Difficult::Hard)
		difficultGame = Difficult::Easy;
}

void GridController::Mix()
{
	winText->setString("You playing"); // ���������� � ��������� ����� ����� ������
	for (int y = 0; y < 9; y++) // ���������� �� ���� ��������� 
	{
		for (int x = 0; x < 9; x++)
		{
			matrix[y][x]->UnLock(); // � ��������������
			matrix[y][x]->SetUserNumber(0); // � ������ �������� �� ����������������� �����
		}
	}

	for (int i = 0; i < 15; i++) // 15 ��� ��������� ������� �������������
	{
		switch (rand() % 5) // �������� �������� �������
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
	while (counterLocked < (int)difficultGame) // ������������� ��������� �������� ��� ���������. ���������� ������� �� ��������� ����
	{
		randX = rand() % 9; // ����� ��������� �������
		randY = rand() % 9;
		if (matrix[randY][randX]->IsLock() == false) // � ���� �������� ��� �� �������������
		{
			counterLocked++; // �� ����������� �������������� �������� 
			matrix[randY][randX]->SetUserNumber(matrix[randY][randX]->correctNumber); // ������������� ���������� ����� ��� ����������������
			matrix[randY][randX]->Lock(); // ��������� ��������
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
			// ����� �������� ���� ��������
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
		// ����� �������� ���� ��������
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
		// ����� �������� ���� ��������
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
			// ����� �������� ���� ��������
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
			// ����� �������� ���� ��������
			corNum = matrix[y][area1 * 3 + x]->correctNumber;
			usrNum = matrix[y][area1 * 3 + x]->GetUserNumber();

			matrix[y][area1 * 3 + x]->correctNumber = matrix[y][area2 * 3 + x]->correctNumber;
			matrix[y][area1 * 3 + x]->SetUserNumber(matrix[y][area2 * 3 + x]->GetUserNumber());

			matrix[y][area2 * 3 + x]->correctNumber = corNum;
			matrix[y][area2 * 3 + x]->SetUserNumber(usrNum);
		}
	}
}
