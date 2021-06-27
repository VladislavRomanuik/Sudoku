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
	window->draw(sprite, *renderStates);// рисуем картинку
	window->draw(caption, *renderStates);// рисуем цифру 
}

void Cell::SetPosition(sf::Vector2f newPos)
{
	sprite.setPosition(newPos); // меняем позицию картинки

	collider.position = sprite.getPosition(); // меняем позицию коллайдера (проверяемой на нажатие границы клетки)

	// именяем положение текста по центру клетки
	sf::FloatRect b = caption.getGlobalBounds(); // получаем размер текста
	caption.setPosition(sprite.getPosition().x + sprite.getSize().x / 2 - b.width / 2, sprite.getPosition().y + b.height);
}

void Cell::SetUserNumber(int value)
{
	if (isLocked) return; // если заблокировано изменение числа то выходим из фукнции

	// проверяем коректной нового числа
	if (value > 9) value = 9;
	else if (value < 0) value = 0;

	userNumber = value; // меняем на новое число
	if (userNumber == 0) // если число 0
		caption.setString(""); // то значит пользователь не хочет чтоб в клеточке были цифры (стереть цисло)
	else
		caption.setString(std::to_string(userNumber)); // устанавливает текст в соответствии со своеим числом
}

int Cell::GetUserNumber()
{
	return userNumber; // возвращаем пользовательское число
}

void Cell::SetFail(bool state)
{
	if (isLocked) return; // если заблокировано изменение числа то выходим из фукнции

	isFail = state; // изменяем состояние флага на новое
	if (isFail) // если число в клетке не правильное то
		caption.setFillColor(sf::Color::Red); // устанавливаем красный цвет тексту
	else 
		caption.setFillColor(sf::Color::Blue); // иначе ставим синий цвет (черный цвет для заблокированыйх клеток)
}

bool Cell::IsFail()
{
	return isFail;
}

void Cell::Lock()
{
	caption.setFillColor(sf::Color::Black); // ставим черный цвет если клетка заблокирована
	isLocked = true;
}

void Cell::UnLock()
{
	caption.setFillColor(sf::Color::Blue); // ставим синий цвет если клетка разблокирована и является клеткой игрока
	isLocked = false;
}

bool Cell::IsLock()
{
	return isLocked;
}