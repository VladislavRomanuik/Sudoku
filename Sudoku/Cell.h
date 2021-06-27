#pragma once
#include <SFML/Graphics.hpp>
#include "IDrawable.h"
#include "Collider.h"
#include <iostream>
#include <string>

// ����� ����� ��������
class Cell : public IDrawable // ������������ �� ������, ���� ����� ���������� �������� ������ �� ������
{
private:
	sf::RenderWindow* window;// ������ �� ���� ��� ����� ��������
	sf::RenderStates* renderStates;// ������ �� ��������� ��������� � ����
	sf::RectangleShape sprite;// ������� ����� ������ (������)
	sf::Texture texture;// �������� ������
	sf::Text caption; // ����� ������
	sf::Font font;// ����� ������ �� ������

	int userNumber; // �����, ������� �������� �������������
	bool isFail; // ����, ������� �������, ���������� �� ����� �������� ������������ � ����������� �������� ����
	bool isLocked; // ����, ������� �������, ����� �� ������ ��� ��������. ������������ ���� ��� ������, ������� ������������ �� ����� ������

public:
	int correctNumber; // ������ ���������� �����. ��������� ������ ����������� ��������
	Collider collider; // ��������� (������������ �������) ������

	// ������������
	Cell() {}
	Cell(sf::RenderWindow* _window, sf::RenderStates* _renderStates, std::string pathTexture, sf::Vector2f size);

	virtual void Draw();// ������������� ������� ���������

	void SetPosition(sf::Vector2f newPos); // ������� ��������� ������� ��� ��������
	void SetUserNumber(int value); // ������������� ���������������� �����
	int GetUserNumber(); // �������� ������� ���������������� �����
	void SetFail(bool state); // ������������� �� ��������� �� ���������� �����
	bool IsFail(); // �������� ���������� �� ��������� �� ������ �����
	void Lock();// ��������� ��������� ����� ��������
	void UnLock(); // ������������ ��������� ����� ��������
	bool IsLock(); // �������� ����������, ������������� �� ������ �������� �� ��������� �����
};

