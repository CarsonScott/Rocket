#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#define PI 3.1415926

class AimSight
{
public:
    bool visible;

    AimSight()
    {
        visible = false;
    }

    sf::ConvexShape get_sight(sf::Vector2f ship_position, sf::Vector2f mouse_position)
    {
        sf::ConvexShape sight;

        sight.setPointCount(4);
        sight.setPoint(0, ship_position);
        sight.setPoint(1, mouse_position);

        sf::Vector2f point = mouse_position;
        point.y -= 2;
        sight.setPoint(2, point);

        point = ship_position;
        point.y -= 2;
        sight.setPoint(3, point);

        sight.setFillColor(sf::Color::Red);
        return sight;
    }
};

class Ship: public sf::Sprite
{
    float speed, accel;
    float x_vel, y_vel;

    sf::Texture texture;
public:
    Ship()
    {
        texture.loadFromFile("img/ship.png");
        setTexture(texture);

        sf::Vector2f origin;
        origin.x = getTextureRect().width / 2;
        origin.y = getTextureRect().height / 2;
        setOrigin(origin);

        x_vel = 0;
        y_vel = 0;
        accel = 30;
        speed = 100;
    }

    void update(float dt, sf::Vector2f mouse_position)
    {
        sf::Vector2f position = getPosition();
        float angle = atan2(mouse_position.y - position.y, mouse_position.x - position.x);

        setRotation(angle*180/PI);

        x_vel = cos(angle) * speed;
        y_vel = sin(angle) * speed;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            move(x_vel * dt, y_vel * dt);
        }

        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            move(-x_vel * dt, -y_vel * dt);
        }
    }
};

class Game
{
public:
    void main(sf::RenderWindow& window)
    {
        Ship ship;
        ship.setPosition(400, 400);

        AimSight sight;

        sf::Clock clock;
        while (window.isOpen())
        {
            float dt = clock.restart().asSeconds();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                if(event.type == sf::Event::KeyPressed)
                {
                    if(event.key.code == sf::Keyboard::V)
                    {
                        if(sight.visible)
                        {
                            sight.visible = false;
                        }
                        else
                        {
                            sight.visible = true;
                        }
                    }
                }
            }
            window.clear();

            sf::Vector2f mouse_position = sf::Vector2f(sf::Mouse::getPosition(window));
            ship.update(dt, mouse_position);

            if(sight.visible)
            {
                window.draw(sight.get_sight(ship.getPosition(), mouse_position));

            }

            window.draw(ship);
            window.display();
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML works!");
    Game().main(window);
    return 0;
}
