#pragma once
#include <SFML/Graphics.hpp>
#include "IDrawable.h"
#include "Collider.h"
#include "Action.h"
#include <string>
#include <iostream>

// ����� ������ 
class Button :
    public IDrawable // ������������ ��� �� ������ ��� ��������� �� ������
{
private:
	sf::RenderWindow* window; // ������ �� ���� ��� ����� ��������
	sf::RenderStates* renderStates; // ������ �� ��������� ��������� � ����
	sf::RectangleShape sprite; // ������� ����� ������ (������)
	sf::Texture texture; // �������� ������
	sf::Font font; // ����� ������ �� ������

public:
    Collider collider; // ��������� ��� ����������� ������� �� ������
	Action Click; // ������� ������� ��������� ��� ������� ������
	sf::Text caption; // ����� ������

	// ������������
	Button() {}
	Button(sf::RenderWindow* _window, sf::RenderStates* _renderStates, std::string _caption, sf::Vector2f pos, sf::Vector2f size);

	void ProcessEvent(sf::Event event); // ������� ��� ��������� ������� 

	virtual void Draw(); // ������������� ������� ���������
};

