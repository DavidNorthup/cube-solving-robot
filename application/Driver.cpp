#include "Camera.h"
#include "Robot.h"
#include "Options.h"

#include <chrono>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 700
#define WINDOW_TITLE "Cube Solving Robot"


#define PADDING 20
#define BACKGROUND_COLOR sf::Color(40,40,40,255)
#define BACKDROP_FILL sf::Color(80,80,80, 255)

#define OPTIONS_FILE_PATH "options.opt"
#define CAMERA_BRIGHTNESS_OPTION "Camera_brightness"

/*
This function puts the webcam feed onto the sfml window in the specified
predefined location (top left quarter).
*/
void drawWebcam(sf::RenderWindow * window, sf::Image image) {
    sf::RectangleShape backdrop;
    sf::Vector2f backdrop_size(WINDOW_WIDTH /2 - 2 * PADDING, WINDOW_HEIGHT - 2*PADDING);
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
    if (argc < 3) {
        std::cout << "Usage example: " << argv[0] << " [Path to first] [Path to second]" << std::endl;
        return 1;
    }

    csr::Robot robot(argv[1], argv[2]);

    // Open a window
    std::cout << "Opening window" << std::endl;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);


    int target_camera_size = WINDOW_WIDTH / 2 - 2*PADDING;
    camera::CameraManager cam(0, target_camera_size, target_camera_size);


    options::OptionsManager options(OPTIONS_FILE_PATH);
    // Apply the selected options
    cam.changeCameraBrightness(options.getOption(CAMERA_BRIGHTNESS_OPTION));

    bool ctrl_held = false;

    while(window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::LControl) {
                    ctrl_held = true;
                }
                if (ctrl_held && event.key.code == sf::Keyboard::S) {
                    cv::Mat image = cam.getFrame();
                    camera::CSRImageProcessing::saveImageToFile("sample.jpg", image);
                }
                if (ctrl_held && event.key.code == sf::Keyboard::L) { // Turn the left motor
                    robot.sendCommand(MOTOR_2 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, LR_IDENTIFIER);
                }
                if (ctrl_held && event.key.code == sf::Keyboard::R) { // Turn the right motor
                    robot.sendCommand(MOTOR_3 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, LR_IDENTIFIER);
                }
                if (ctrl_held && event.key.code == sf::Keyboard::F) { // Turn the front motor
                    robot.sendCommand(MOTOR_3  + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, FB_IDENTIFIER);
                }
                if (ctrl_held && event.key.code == sf::Keyboard::B) { // Turn the back motor
                    robot.sendCommand(MOTOR_2 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, FB_IDENTIFIER);
                }
                if (ctrl_held && event.key.code == sf::Keyboard::U) { // Turn the up motor
                    robot.sendCommand(MOTOR_1 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, FB_IDENTIFIER);
                }
                if (ctrl_held && event.key.code == sf::Keyboard::D) { // Turn the down motor
                    robot.sendCommand(MOTOR_1 + DIRECTION_CLOCKWISE + ONE_QUARTER_TURN, LR_IDENTIFIER);
                }

                if (ctrl_held && event.key.code == sf::Keyboard::T) { // Test
                    robot.sendCommand(MOVE_R);
                    robot.sendCommand(MOVE_D);
                    robot.sendCommand(MOVE_L2);
                    robot.sendCommand(MOVE_B);
                    robot.sendCommand(MOVE_F_PRIME);
                    robot.sendCommand(MOVE_U2);
                    robot.sendCommand(MOVE_D_PRIME);
                    robot.sendCommand(MOVE_L_PRIME);
                    robot.sendCommand(MOVE_F_PRIME);
                    robot.sendCommand(MOVE_B_PRIME);
                    robot.sendCommand(MOVE_U_PRIME);
                    robot.sendCommand(MOVE_L);
                    robot.sendCommand(MOVE_D);
                    robot.sendCommand(MOVE_F);
                    robot.sendCommand(MOVE_R_PRIME);
                    robot.sendCommand(MOVE_D2);
                    robot.sendCommand(MOVE_L2);
                }

                if (ctrl_held && event.key.code == sf::Keyboard::Return) {

                    
                    usleep(1000*1000*3);
                    auto start = std::chrono::high_resolution_clock::now();
                    robot.sendCommand(MOVE_L2);
                    robot.sendCommand(MOVE_D2);
                    robot.sendCommand(MOVE_R);
                    robot.sendCommand(MOVE_F_PRIME);
                    robot.sendCommand(MOVE_D_PRIME);
                    robot.sendCommand(MOVE_L_PRIME);
                    robot.sendCommand(MOVE_U);
                    robot.sendCommand(MOVE_B);
                    robot.sendCommand(MOVE_F);
                    robot.sendCommand(MOVE_L);
                    robot.sendCommand(MOVE_D);
                    robot.sendCommand(MOVE_U2);
                    robot.sendCommand(MOVE_F);
                    robot.sendCommand(MOVE_B_PRIME);
                    robot.sendCommand(MOVE_L2);
                    robot.sendCommand(MOVE_D_PRIME);
                    robot.sendCommand(MOVE_R_PRIME);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
                    std::cout << "Cube solved in " << duration.count() << " milliseconds" << std::endl;
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::LControl) {
                    ctrl_held = false;
                }
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