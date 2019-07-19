#include "Camera.h"
#include "Robot.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700
#define WINDOW_TITLE "Cube Solving Robot"

/*
Currently the program attempts to communicate with the arudinos one byte 
at a time via serial connenctions on the COM ports. 
*/
int main(int argc, char *argv[]) {
    // if (argc < 3) {
    //     std::cout << "Usage example: " << argv[0] << " [Path to first] [Path to second]" << std::endl;
    //     return 1;
    // }

    //csr::Robot robot(argv[1], argv[2]);

    // Open a window
    std::cout << "Opening window" << std::endl;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    camera::CameraManager cam(0);

    while(window.isOpen()) {

        // Draw the webcam feed
        cam.update();
        sf::Image image = cam.getDisplayableImage();
        sf::Sprite image_sprite;
        sf::Texture image_texture;
        image_texture.loadFromImage(image);
        image_sprite.setTexture(image_texture);


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }


        window.draw(image_sprite);
        window.display();
    }
    return 0;
}