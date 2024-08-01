#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include <string>
#include <list>

#include "game.hpp"

#define GRAVITY_MULTIPLIER 0.3
#define MAX_GRAVITY_FORCE 10

namespace Colision
{
  std::list<sf::Shape *> static_objects;

  void player_colision()
  {
    for (sf::Shape *static_obj : static_objects)
    {
      if (!Player::shape.getGlobalBounds().intersects(static_obj->getGlobalBounds()))
      {
        Player::onAir = 1;
        return;
      }
      sf::Vector2f static_position = static_obj->getGlobalBounds().getPosition();
      static_position.x = Player::shape.getGlobalBounds().getPosition().x;
      static_position.y -= Player::shape.getGlobalBounds().height + 8;
      Player::onAir = 0;
      Player::shape.setPosition(static_position);
    }
  }
}

class Projectile
{
  const int height = 5;
  const int width = 5;
  sf::RectangleShape shape;
  
  Projectile()
  {
    shape = sf::RectangleShape(sf::Vector2f(width, height));
  }
};

namespace Floor
{
  const int height = 50;
  const int width = Window::width;
  sf::RectangleShape shape(sf::Vector2f(width, height));
}

namespace GameEvents
{
  sf::Event event;

  std::unordered_set<sf::Keyboard::Key> keysPressed;

  std::unordered_map<sf::Keyboard::Key, std::function<void()>> keysEvents = {
      {sf::Keyboard::Escape, []()
       {
         exit(0);
       }},
      {sf::Keyboard::A, []()
       { Player::moves[(std::string) "go_left"](GameEvents::keysPressed.count(sf::Keyboard::A)); }},
      {sf::Keyboard::D, []()
       { Player::moves[(std::string) "go_right"](!GameEvents::keysPressed.count(sf::Keyboard::D)); }},
      {sf::Keyboard::Left, []()
       { Player::moves[(std::string) "go_left"](GameEvents::keysPressed.count(sf::Keyboard::Left)); }},
      {sf::Keyboard::Right, []()
       { Player::moves[(std::string) "go_right"](!GameEvents::keysPressed.count(sf::Keyboard::Right)); }},
      {sf::Keyboard::Space, []()
       { Player::moves[(std::string) "jump"](GameEvents::keysPressed.count(sf::Keyboard::Space)); }},
  };

  namespace Keyboard
  {
    bool keyExists()
    {
      return keysEvents.count(event.key.code);
    }

    bool keyAlreadyPressed()
    {
      return keysPressed.count(event.key.code);
    }

    void keyPressedHandler()
    {
      if (!keyExists() || keyAlreadyPressed())
        return;
      keysPressed.insert(event.key.code);
      keysEvents[event.key.code]();
    }

    void keyReleasedHandler()
    {
      if (!keyExists())
        return;
      keysPressed.extract(event.key.code);
      keysEvents[event.key.code]();
    }
  }

  std::unordered_map<sf::Event::EventType, std::function<void()>> eventsMap = {
      {sf::Event::KeyPressed, GameEvents::Keyboard::keyPressedHandler},
      {sf::Event::KeyReleased, GameEvents::Keyboard::keyReleasedHandler},
  };

  void handleEvent()
  {
    if (!eventsMap.count(event.type))
      return;
    eventsMap[event.type]();
  }
}

namespace Player
{
  const int heigth = 20;
  const int width = 20;
  int onAir = 1;
  int acelerationX = 5;
  int acelerationY = -MAX_GRAVITY_FORCE * 4;
  Vector2D speed;
  sf::RectangleShape shape(sf::Vector2f(width, heigth));
  std::unordered_map<std::string, std::function<void(bool)>> moves = {
      {
          "jump",
          [](bool oposite)
          {
            if (onAir || !oposite)
              return;
            moveY(0);
          },
      },
      {"go_down",
       [](bool oposite)
       {
         moveY(oposite);
       }},
      {"go_left",
       [](bool oposite)
       {
         moveX(oposite);
       }},
      {"go_right",
       [](bool oposite)
       {
         moveX(oposite);
       }},
  };

  std::unordered_map<std::string, std::function<void()>> actions = {
      {"shoot", []() {

       }}};

  void initialize()
  {
    shape.setFillColor(sf::Color::White);
  }

  void compute()
  {
    Colision::player_colision();
    float gravity = speed.y ? speed.y + std::pow(std::abs(speed.y), GRAVITY_MULTIPLIER) : onAir;

    if (gravity > MAX_GRAVITY_FORCE)
      gravity = MAX_GRAVITY_FORCE;
    // std::cout << gravity << std::endl;
    speed.y = gravity;
    shape.move(sf::Vector2f(speed.x, speed.y));
  }

  void moveY(bool oposite)
  {
    speed.y += oposite ? -acelerationY : acelerationY;
  }

  void moveX(bool oposite)
  {
    speed.x += oposite ? -acelerationX : acelerationX;
  }
}

namespace Window
{
  const float tickRate = 1.0f / 60.0f;
  const int height = 400;
  const int width = 600;
  sf::RenderWindow *window;
  sf::Clock clock;

  void start()
  {
    Colision::static_objects.push_back(&Floor::shape);
    Floor::shape.setPosition(0, height - Floor::height);
    Floor::shape.setFillColor(sf::Color::Cyan);
    window = new sf::RenderWindow(sf::VideoMode(width, height), "Hello World!");
    while (window->isOpen())
    {
      sf::Time elapsed = clock.getElapsedTime();
      if (elapsed.asSeconds() < tickRate)
        continue;
      Player::compute();
      clock.restart();
      window->clear();
      window->draw(Floor::shape);
      window->draw(Player::shape);
      window->display();
      while (window->pollEvent(GameEvents::event))
        GameEvents::handleEvent();
    }
  }
}

int main()
{
  Window::start();
  return 0;
}