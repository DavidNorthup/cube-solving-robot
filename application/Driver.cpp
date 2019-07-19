#include "Camera.h"
#include "Robot.h"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define WINDOW_TITLE "Cube Solving Robot"


#define PADDING 20
#define BACKGROUND_COLOR sf::Color(40,40,40,255)
#define BACKDROP_FILL sf::Color(80,80,80, 255)


/*
This function puts the webcam feed onto the sfml window in the specified
predefined location (top left quarter).
*/
void drawWebcam(sf::RenderWindow * window, sf::Image image) {
    sf::RectangleShape backdrop;
    sf::Vector2f backdrop_size(WINDOW_WIDTH /2 - 2 * PADDING, WINDOW_HEIGHT /2 - 2*PADDING);
    backdrop.setSize(backdrop_size);
    backdrop.setFillColor(BACKDROP_FILL);
    backdrop.setPosition(PADDING, PADDING);

    sf::Sprite image_sprite;
    sf::Texture image_texture;
    image_texture.loadFromImage(image);
    image_sprite.setTexture(image_texture);
    sf::Vector2u image_size = image.getSize();
    sf::Vector2f delta(backdrop_size.x-image_size.x, backdrop_size.y-image_size.y);
    image_sprite.setPosition(PADDING + delta.x/2, PADDING + delta.y/2);
    window->draw(backdrop);
    window->draw(image_sprite);
}

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

    camera::CameraManager cam(0, WINDOW_WIDTH / 2 - 2*PADDING, WINDOW_HEIGHT / 2 -2*PADDING);

    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Paint the background
        sf::RectangleShape background;
        background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        background.setFillColor(BACKGROUND_COLOR);
        window.draw(background);

        // Draw the webcam feed
        cam.update();
        sf::Image image = cam.getDisplayableImage();
        drawWebcam(&window, image);

        window.display();
    }
    return 0;
}