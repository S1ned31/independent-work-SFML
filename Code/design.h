#pragma once
#ifndef DESIGN
#define DESIGN

#include <SFML/Graphics.hpp>
#include <iostream>


////////////////////////////////////////////////////////////////
// Файл який спрощує роботу з дизайном
// має функції для більш зручного створення кнопок та заголовків
////////////////////////////////////////////////////////////////

// Реалізація заголовку
void createTitleText( sf::Text& titleText, const sf::Font& fontText , const std::wstring& text, int charecterSize, float posX, float posY );

// Функція для створення кнопки
void createButton(sf::RectangleShape& button, sf::Text& buttonText, const sf::Font& font, const std::wstring& text,
    const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& buttonColor, int characterSize);

#endif // !DESIGN
