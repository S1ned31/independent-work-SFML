#include "design.h"

// Реалізація заголовку
void createTitleText(sf::Text& titleText, const sf::Font& font, const std::wstring& text, int charecterSize, float posX, float posY) 
{
    titleText.setFont(font);
    titleText.setString(text);
    titleText.setCharacterSize(charecterSize);
    titleText.setFillColor(sf::Color(34, 139, 34));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(posX, posY);
}

// Реалізація функції для кнопок
void createButton(sf::RectangleShape& button, sf::Text& buttonText, const sf::Font& font, const std::wstring& text,
    const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& buttonColor, int characterSize)
{
    // Налаштування кнопки
    button.setSize(size);
    button.setPosition(position);
    button.setFillColor(buttonColor);

    // Налаштування тексту кнопки
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(characterSize);
    buttonText.setFillColor(sf::Color::White); // Белый колір тексту
    buttonText.setStyle(sf::Text::Bold);

    // Центровка тексту відносно кнопки
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    sf::FloatRect textBounds = buttonText.getGlobalBounds();
    buttonText.setPosition(button.getPosition().x + (buttonBounds.width - textBounds.width) / 2,
        button.getPosition().y + (buttonBounds.height - textBounds.height) / 2 - 5);
}