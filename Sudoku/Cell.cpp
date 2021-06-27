#include "Cell.h"

Cell::Cell(sf::RenderWindow* _window, sf::RenderStates* _renderStates, std::string pathTexture, sf::Vector2f size)
{
	window = _window;
	renderStates = _renderStates;

	texture = sf::Texture();
	texture.loadFromFile("Images/" + pathTexture);

	sprite = sf::RectangleShape(size);
	sprite.setTexture(&texture);
	sprite.setPosition(sf::Vector2f(0, 0));

	if (font.loadFromFile("Fonts/TTRounds-Black.ttf"))
		std::cout << "Loaded Font suceful!" << std::endl;
	caption.setFont(font);
	caption.setFillColor(sf::Color::Black);
	caption.setCharacterSize(25);
	caption.setString("");
	sf::FloatRect b = caption.getGlobalBounds();
	caption.setPosition(sprite.getPosition().x + sprite.getSize().x / 2 - b.width / 2, sprite.getPosition().y + b.height);

	isLocked = false;
	isFail = false;
	correctNumber = 0;
	userNumber = 0;

	collider.position = sprite.getPosition();
	collider.size = sprite.getSize();
}

void Cell::Draw()
{
	window->draw(sprite, *renderStates);// ������ ��������
	window->draw(caption, *renderStates);// ������ ����� 
}

void Cell::SetPosition(sf::Vector2f newPos)
{
	sprite.setPosition(newPos); // ������ ������� ��������

	collider.position = sprite.getPosition(); // ������ ������� ���������� (����������� �� ������� ������� ������)

	// ������� ��������� ������ �� ������ ������
	sf::FloatRect b = caption.getGlobalBounds(); // �������� ������ ������
	caption.setPosition(sprite.getPosition().x + sprite.getSize().x / 2 - b.width / 2, sprite.getPosition().y + b.height);
}

void Cell::SetUserNumber(int value)
{
	if (isLocked) return; // ���� ������������� ��������� ����� �� ������� �� �������

	// ��������� ��������� ������ �����
	if (value > 9) value = 9;
	else if (value < 0) value = 0;

	userNumber = value; // ������ �� ����� �����
	if (userNumber == 0) // ���� ����� 0
		caption.setString(""); // �� ������ ������������ �� ����� ���� � �������� ���� ����� (������� �����)
	else
		caption.setString(std::to_string(userNumber)); // ������������� ����� � ������������ �� ������ ������
}

int Cell::GetUserNumber()
{
	return userNumber; // ���������� ���������������� �����
}

void Cell::SetFail(bool state)
{
	if (isLocked) return; // ���� ������������� ��������� ����� �� ������� �� �������

	isFail = state; // �������� ��������� ����� �� �����
	if (isFail) // ���� ����� � ������ �� ���������� ��
		caption.setFillColor(sf::Color::Red); // ������������� ������� ���� ������
	else 
		caption.setFillColor(sf::Color::Blue); // ����� ������ ����� ���� (������ ���� ��� ��������������� ������)
}

bool Cell::IsFail()
{
	return isFail;
}

void Cell::Lock()
{
	caption.setFillColor(sf::Color::Black); // ������ ������ ���� ���� ������ �������������
	isLocked = true;
}

void Cell::UnLock()
{
	caption.setFillColor(sf::Color::Blue); // ������ ����� ���� ���� ������ �������������� � �������� ������� ������
	isLocked = false;
}

bool Cell::IsLock()
{
	return isLocked;
}