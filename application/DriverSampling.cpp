#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

#include "Camera.h"


int main (int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Not enough args, example use: " << argv[0] << " [sample image file]   [sampling center output file]" << std::endl; 
        return 1;
    }
    sf::Image image; 

    if (!image.loadFromFile(argv[1])) {
        std::cerr << "Error opening image" << std::endl;
        return 1;
    }

    std::fstream fs(argv[2]);

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
                    std::cout << "What is the facelet name for this position: " << std::endl;
                    std::string name;
                    std::cin >> name;
                    fs << position.x << " " << position.y << " " << name << std::endl;
                }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

}