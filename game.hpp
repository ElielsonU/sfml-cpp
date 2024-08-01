#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map>
#include <string>
#include <thread>

struct Vector2D
{
  float x = 0;
  float y = 0;
};

namespace Player
{
  extern int onAir;
  extern Vector2D speed;
  extern std::unordered_map<std::string, std::function<void(bool)>> moves;
  extern sf::RectangleShape shape;
  void moveY(bool oposite);
  void moveX(bool oposite);
}

namespace Window
{
  extern sf::RenderWindow *window;
  extern sf::RectangleShape Player;
  extern Vector2D playerSpeed;
  extern const int height;
  extern const int width;
}

namespace GameEvents
{
  extern sf::Event event;
}

#endif