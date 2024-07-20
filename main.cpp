#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <array>
#include <map>
#include <set>
#define TICK_RATE 1.0f / 60.0f

namespace GameWindow
{
  sf::Window *window;
  sf::RectangleShape player(sf::Vector2f(300, 300));
  sf::Clock clock;

  void start()
  {
    player.setFillColor(sf::Color::Blue);
    window = new sf::Window(sf::VideoMode(800, 800), "Hello World!");
    sf::Event window_event;

    while (window->isOpen())
    {
      sf::Time elapsed = clock.getElapsedTime();
      if (elapsed.asSeconds() < TICK_RATE) continue;
      clock.restart();
      std::cout << elapsed.asSeconds() << " " << TICK_RATE << " " << std::endl;
      // window->;
      window->pollEvent(window_event);
    }
  }
}

int main()
{
  GameWindow::start();
  return 0;
}