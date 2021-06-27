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
	sf::FloatRect b = caption.getGlobalBounds(); // получаем размеры текста 
	caption.setPosition(sprite.getPosition().x + sprite.getSize().x / 2 - b.width / 2, sprite.getPosition().y + b.height); //и выставляем текст по центру кнопки

	collider.position = sprite.getPosition(); // устанавливаем позицию
	collider.size = sprite.getSize(); // и размеры коллайдера (границы, где будет срабатывать нажатие кнопки)
}

void Button::ProcessEvent(sf::Event event)
{
	// получаем координаты мыши
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);

	// преобразовываем полученные координаты потому что пользователь мог изменить размер окна
	float f = 1 / *(renderStates->transform.getMatrix());
	mousePos = sf::Vector2f(f * (float)(mousePos.x), f * (float)(mousePos.y));

	if (event.type == sf::Event::MouseButtonReleased &&
		event.key.code == sf::Mouse::Left) // если сейчас событие отпускания левой кнопки мыши
	{
		if (collider.IsPointInside(mousePos)) // и координата мыши попала в границы коллейдера
		{
			Click(); // то вызываем событие нажатия кнопки, тоесть вызовется прегруженый оператор скобочек и вызовется список всех привязаных функций к этому событию
		}
	}
}

void Button::Draw()
{
	window->draw(sprite, *renderStates); // рисуем картинку кнопки
	window->draw(caption, *renderStates); // рисуем текст поверх кнопки
}