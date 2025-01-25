#include "design.h"

// ��������� ���������
void createTitleText(sf::Text& titleText, const sf::Font& font, const std::wstring& text, int charecterSize, float posX, float posY) 
{
    titleText.setFont(font);
    titleText.setString(text);
    titleText.setCharacterSize(charecterSize);
    titleText.setFillColor(sf::Color(34, 139, 34));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(posX, posY);
}

// ��������� ������� ��� ������
void createButton(sf::RectangleShape& button, sf::Text& buttonText, const sf::Font& font, const std::wstring& text,
    const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& buttonColor, int characterSize)
{
    // ������������ ������
    button.setSize(size);
    button.setPosition(position);
    button.setFillColor(buttonColor);

    // ������������ ������ ������
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(characterSize);
    buttonText.setFillColor(sf::Color::White); // ����� ���� ������
    buttonText.setStyle(sf::Text::Bold);

    // ��������� ������ ������� ������
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    sf::FloatRect textBounds = buttonText.getGlobalBounds();
    buttonText.setPosition(button.getPosition().x + (buttonBounds.width - textBounds.width) / 2,
        button.getPosition().y + (buttonBounds.height - textBounds.height) / 2 - 5);
}