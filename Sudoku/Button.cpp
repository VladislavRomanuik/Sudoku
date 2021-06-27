#include "Button.h"

Button::Button(sf::RenderWindow* _window, sf::RenderStates* _renderStates, std::string _caption, sf::Vector2f pos, sf::Vector2f size)
{
	window = _window;
	renderStates = _renderStates;

	texture = sf::Texture();
	texture.loadFromFile("Images/Button.png");

	sprite = sf::RectangleShape(size);
	sprite.setTexture(&texture);
	sprite.setPosition(pos);

	if (font.loadFromFile("Fonts/TTRounds-Black.ttf"))
		std::cout << "Loaded Font suceful!" << std::endl;
	caption.setFont(font);
	caption.setFillColor(sf::Color::Black);
	caption.setCharacterSize(25);
	caption.setString(_caption);
	sf::FloatRect b = caption.getGlobalBounds(); // �������� ������� ������ 
	caption.setPosition(sprite.getPosition().x + sprite.getSize().x / 2 - b.width / 2, sprite.getPosition().y + b.height); //� ���������� ����� �� ������ ������

	collider.position = sprite.getPosition(); // ������������� �������
	collider.size = sprite.getSize(); // � ������� ���������� (�������, ��� ����� ����������� ������� ������)
}

void Button::ProcessEvent(sf::Event event)
{
	// �������� ���������� ����
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);

	// ��������������� ���������� ���������� ������ ��� ������������ ��� �������� ������ ����
	float f = 1 / *(renderStates->transform.getMatrix());
	mousePos = sf::Vector2f(f * (float)(mousePos.x), f * (float)(mousePos.y));

	if (event.type == sf::Event::MouseButtonReleased &&
		event.key.code == sf::Mouse::Left) // ���� ������ ������� ���������� ����� ������ ����
	{
		if (collider.IsPointInside(mousePos)) // � ���������� ���� ������ � ������� ����������
		{
			Click(); // �� �������� ������� ������� ������, ������ ��������� ����������� �������� �������� � ��������� ������ ���� ���������� ������� � ����� �������
		}
	}
}

void Button::Draw()
{
	window->draw(sprite, *renderStates); // ������ �������� ������
	window->draw(caption, *renderStates); // ������ ����� ������ ������
}