#include <SFML/Graphics.hpp>
#include "Camera.h"
#include <iostream>

int main (int argc, char* argv[]) {
    if (argc < 2) {
        return 1;
    }
    sf::Image image; 

    if (!image.loadFromFile(argv[1])) {
        std::cerr << "Error opening image" << std::endl;
        return 1;
    }

    sf::Sprite sprite;
    sf::Texture texture;
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    sf::RenderWindow window(sf::VideoMode(image.getSize().x, image.getSize().y), "Image display");
    while(window.isOpen()) {
        sf::Event event;        
        while (window.pollEvent(event))
        {
                if (event.type == sf::Event::Closed) {
                        window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i position = sf::Mouse::getPosition(window);
                    std::cout << position.x << " " << position.y << std::endl;
                }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

}