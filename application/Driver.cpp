#include "Camera.h"
#include "Robot.h"
#include "Options.h"
#include <string>

#include <chrono>
#include <algorithm>
#include <thread>
#include <future>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 700
#define WINDOW_TITLE "Cube Solving Robot"

#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240

#define SAMPLING_FILE(corner) std::string("training/sampling_locations_") + std::string(corner) + std::string(".dat")

#define NUM_CAMERAS 4
#define BLD_CAMERA_INDEX 4
#define FRD_CAMERA_INDEX 2
#define FLU_CAMERA_INDEX 6
#define BRU_CAMERA_INDEX 0

#define PADDING 20
#define BACKGROUND_COLOR sf::Color(40,40,40,255)
#define BACKDROP_FILL sf::Color(80,80,80, 255)
#define CUBE_STROKE 2


#define OPTIONS_FILE_PATH "options.opt"
#define CAMERA_BRIGHTNESS_OPTION(corner) std::string("Camera_brightness_") + std::string(corner)
#define CAMERA_CONTRAST_OPTION(corner) std::string("Camera_contrast_") + std::string(corner)
#define CAMERA_SATURATION_OPTION(corner) std::string("Camera_saturation_") + std::string(corner)

/*
This function puts the webcam feed onto the sfml window in the specified
predefined location (top left quarter).
*/
void drawWebcam(sf::RenderWindow * window, sf::Image images[]) {
    sf::RectangleShape backdrop;
    sf::Vector2f backdrop_size(WINDOW_WIDTH /2 - 2 * PADDING, WINDOW_HEIGHT - 2*PADDING);
    backdrop.setSize(backdrop_size);
    backdrop.setFillColor(BACKDROP_FILL);
    backdrop.setPosition(PADDING, PADDING);

    sf::Vector2f image_max(backdrop_size.x/2 - PADDING/4 - 2, backdrop_size.y/2 - PADDING/4 - 2);
    sf::Image image = images[0];
    double image_scale_factor = 1/std::max(image.getSize().x / image_max.x, image.getSize().y / image_max.y);
    window->draw(backdrop);

    sf::Vector2f positions[] = {sf::Vector2f(0,0), sf::Vector2f(image_max.x + PADDING/4,0), 
        sf::Vector2f(0, image_max.y + PADDING/4),
        sf::Vector2f(image_max.x + PADDING/4, image_max.y + PADDING/4)};

    for (int i = 0; i < NUM_CAMERAS; i++) {
        sf::Sprite image_sprite;
        sf::Texture image_texture;
        image_texture.loadFromImage(images[i]);
        image_texture.setSmooth(true);
        image_sprite.setTexture(image_texture);
        image_sprite.scale(sf::Vector2f(image_scale_factor, image_scale_factor));
        double dx = (image_max.x - (image.getSize().x * image_scale_factor))/2;
        double dy = (image_max.y - (image.getSize().y * image_scale_factor))/2;
        image_sprite.setPosition(PADDING + PADDING/4 + dx + positions[i].x, PADDING + PADDING / 4 + dy + positions[i].y);
        window->draw(image_sprite);
    }
}


/*
The cube is defined in the following orientation for display purposes: 
    U
   LFRB
    D
And the order of the data should be left to right, top to bottom (in this orientation),
face by face. So the first 9 entries are the up face, the next are the left ... 
*/
void drawScan(sf::RenderWindow * window, int cube_data[]) {
    sf::RectangleShape backdrop;
    sf::Vector2f backdrop_size(WINDOW_WIDTH /2 - 2 * PADDING, WINDOW_HEIGHT - 2*PADDING);
    backdrop.setSize(backdrop_size);
    backdrop.setFillColor(BACKDROP_FILL);
    backdrop.setPosition(PADDING + WINDOW_WIDTH/2, PADDING);
    window->draw(backdrop);

    // Compute the size of each facelet.
    int size = std::min((backdrop_size.x - 2* PADDING)/12, (backdrop_size.y - 2*PADDING)/9); // The view is 12x9 facelets
    sf::Vector2f facelet_size(size, size);
    sf::Vector2f delta(backdrop_size.x - 2*PADDING - (12 * size), backdrop_size.y - 2*PADDING - (9 * size));
    int face_size = 3 * size;
    sf::Vector2f face_deltas[] = {sf::Vector2f(PADDING + face_size, PADDING), sf::Vector2f(PADDING, PADDING + face_size),
                                  sf::Vector2f(PADDING + face_size, PADDING + face_size),
                                  sf::Vector2f(PADDING + 2 * face_size, PADDING + face_size),
                                  sf::Vector2f(PADDING + 3 * face_size, PADDING + face_size),
                                  sf::Vector2f(PADDING + face_size, PADDING + 2 * face_size)};
    
    int index = 0;
    sf::Color colors[] = SFML_COLOR_ORDER;
    for (int i = 0; i < 6; i++) {
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                sf::RectangleShape facelet_outline;
                facelet_outline.setSize(facelet_size);
                facelet_outline.setFillColor(sf::Color::Black);
                facelet_outline.setPosition(PADDING + WINDOW_WIDTH/2 + face_deltas[i].x + col * size + delta.x/2,
                     PADDING + face_deltas[i].y + row * size + delta.y/2);
                window->draw(facelet_outline);
                
                sf::RectangleShape facelet;
                facelet.setSize(sf::Vector2f(size - 2 * CUBE_STROKE, size - 2 * CUBE_STROKE));
                facelet.setFillColor(colors[cube_data[index++]]);
                facelet.setPosition(sf::Vector2f(facelet_outline.getPosition().x + CUBE_STROKE, 
                    facelet_outline.getPosition().y + CUBE_STROKE ));
                window->draw(facelet);
            }
        }
    }
}

camera::CameraManager openCamera(int index, int width, int height, std::string sampling_file) {
    return camera::CameraManager(index, width, height, sampling_file);
}

int getIndexOf(std::vector<std::string> list, std::string val) {
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i] == val) {
            return (int) i;
        }
    }
    return 0;
}

std::string get_cubestring(int data[], std::string face_order_chars) {
    std::string cs = "";
    for (size_t i = 0;i < 54; i++) {
        cs = cs + face_order_chars[data[i]];
    }

    cs = cs.substr(0, 9) + cs.substr(27, 9) + cs.substr(18, 9) + cs.substr(45, 9) + cs.substr(9, 9) + cs.substr(36, 9);
    std::cout << cs << std::endl;
    return cs;
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


    std::future<camera::CameraManager> cam_bld_future = std::async(openCamera, BLD_CAMERA_INDEX, CAMERA_WIDTH, CAMERA_HEIGHT, SAMPLING_FILE("bld"));
    std::future<camera::CameraManager> cam_frd_future = std::async(openCamera, FRD_CAMERA_INDEX, CAMERA_WIDTH, CAMERA_HEIGHT, SAMPLING_FILE("frd"));
    std::future<camera::CameraManager> cam_flu_future = std::async(openCamera, FLU_CAMERA_INDEX, CAMERA_WIDTH, CAMERA_HEIGHT, SAMPLING_FILE("flu"));
    std::future<camera::CameraManager> cam_bru_future = std::async(openCamera, BRU_CAMERA_INDEX, CAMERA_WIDTH, CAMERA_HEIGHT, SAMPLING_FILE("bru"));

    camera::CameraManager cam_bld = cam_bld_future.get();
    camera::CameraManager cam_frd = cam_frd_future.get();
    camera::CameraManager cam_flu = cam_flu_future.get();
    camera::CameraManager cam_bru = cam_bru_future.get();

    options::OptionsManager options(OPTIONS_FILE_PATH);
    // Apply the selected options
    cam_bld.changeCameraBrightness(options.getOption(CAMERA_BRIGHTNESS_OPTION("bld")));
    cam_frd.changeCameraBrightness(options.getOption(CAMERA_BRIGHTNESS_OPTION("frd")));
    cam_flu.changeCameraBrightness(options.getOption(CAMERA_BRIGHTNESS_OPTION("flu")));
    cam_bru.changeCameraBrightness(options.getOption(CAMERA_BRIGHTNESS_OPTION("bru")));

    cam_bld.changeCameraContrast(options.getOption(CAMERA_CONTRAST_OPTION("bld")));
    cam_frd.changeCameraContrast(options.getOption(CAMERA_CONTRAST_OPTION("frd")));
    cam_flu.changeCameraContrast(options.getOption(CAMERA_CONTRAST_OPTION("flu")));
    cam_bru.changeCameraContrast(options.getOption(CAMERA_CONTRAST_OPTION("bru")));

    cam_bld.changeCameraContrast(options.getOption(CAMERA_SATURATION_OPTION("bld")));
    cam_frd.changeCameraContrast(options.getOption(CAMERA_SATURATION_OPTION("frd")));
    cam_flu.changeCameraContrast(options.getOption(CAMERA_SATURATION_OPTION("flu")));
    cam_bru.changeCameraContrast(options.getOption(CAMERA_SATURATION_OPTION("bru")));

    std::string scramble = "";

    bool ctrl_held = false;
    bool alt_held = false;

    // Set the display order
    int data[54];
    int face_order[] = {4, 2, 1, 3, 0, 5};
    std::string face_order_chars = "ULFRBD";
    std::vector<std::string> locations;
    for ( size_t i = 0; i < face_order_chars.size(); i++ ) {
        for ( int j = 0; j < 9; j++ ) {
            std::string facelet = face_order_chars[i] + std::to_string(j + 1);
            locations.push_back(facelet);
        }
    }

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
                if (event.key.code == sf::Keyboard::LAlt) {
                    alt_held = true;
                }
                if (ctrl_held && !alt_held && event.key.code == sf::Keyboard::S) {
                    cv::Mat image_bld = cam_bld.getUnmarkedImage();
                    cv::Mat image_frd = cam_frd.getUnmarkedImage();
                    cv::Mat image_flu = cam_flu.getUnmarkedImage();
                    cv::Mat image_bru = cam_bru.getUnmarkedImage();
                    camera::CSRImageProcessing::saveImageToFile("sample_bld.jpg", image_bld);
                    camera::CSRImageProcessing::saveImageToFile("sample_frd.jpg", image_frd);
                    camera::CSRImageProcessing::saveImageToFile("sample_flu.jpg", image_flu);
                    camera::CSRImageProcessing::saveImageToFile("sample_bru.jpg", image_bru);
                }

                if (ctrl_held && alt_held) {
                    if (event.key.code == sf::Keyboard::R) {
                        robot.performMove("R2");
                    }
                    if (event.key.code == sf::Keyboard::L) {
                        robot.performMove("L2");
                    }
                    if (event.key.code == sf::Keyboard::U) {
                        robot.performMove("U2");
                    }
                    if (event.key.code == sf::Keyboard::D) {
                        robot.performMove("D2");
                    }
                    if (event.key.code == sf::Keyboard::F) {
                        robot.performMove("F2");
                    }
                    if (event.key.code == sf::Keyboard::B) {
                        robot.performMove("B2");
                    }
                } else if (ctrl_held) {
                    if (event.key.code == sf::Keyboard::R) {
                        robot.performMove("R");
                    }
                    if (event.key.code == sf::Keyboard::L) {
                        robot.performMove("L");
                    }
                    if (event.key.code == sf::Keyboard::U) {
                        robot.performMove("U");
                    }
                    if (event.key.code == sf::Keyboard::D) {
                        robot.performMove("D");
                    }
                    if (event.key.code == sf::Keyboard::F) {
                        robot.performMove("F");
                    }
                    if (event.key.code == sf::Keyboard::B) {
                        robot.performMove("B");
                    }
                } else if (alt_held) {
                    if (event.key.code == sf::Keyboard::R) {
                        robot.performMove("R'");
                    }
                    if (event.key.code == sf::Keyboard::L) {
                        robot.performMove("L'");
                    }
                    if (event.key.code == sf::Keyboard::U) {
                        robot.performMove("U'");
                    }
                    if (event.key.code == sf::Keyboard::D) {
                        robot.performMove("D'");
                    }
                    if (event.key.code == sf::Keyboard::F) {
                        robot.performMove("F'");
                    }
                    if (event.key.code == sf::Keyboard::B) {
                        robot.performMove("B'");
                    }
                }



                if (ctrl_held && event.key.code == sf::Keyboard::T) {
                    scramble += robot.scramble(SCRAMBLE_LENGTH);
                }
                if (ctrl_held && event.key.code == sf::Keyboard::BackSpace) {
                    std::cout << "Cleared" << std::endl;
                    scramble = "";
                }
                if (ctrl_held && event.key.code == sf::Keyboard::Return) {
                    auto start = std::chrono::high_resolution_clock::now();
                    std::string cubestring = get_cubestring(data, "BFLRUD");
                    std::string command = std::string("./kociemba ") + cubestring + " > solution.dat";
                    int solved = ! system(command.c_str());
                    if (solved) {
                        std::fstream sol("solution.dat");
                        std::string move, solution = "";
                        while (sol >> move) {
                           solution += move + " ";
                        }
                        robot.performMoves(solution.substr(0, solution.size() - 1));
                        system("rm solution.dat &> /dev/null");
                    } else {
                        std::cout << "Cannot solve: " << cubestring <<std::endl;
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
                    if (solved) {
                        std::cout << "Cube solved in " << duration.count() << " milliseconds" << std::endl;
                    }
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::LControl) {
                    ctrl_held = false;
                }
                if (event.key.code == sf::Keyboard::LAlt) {
                    alt_held = false;
                }
            }
        }

        // Paint the background
        window.clear();
        sf::RectangleShape background;
        background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        background.setFillColor(BACKGROUND_COLOR);
        window.draw(background);

        // Draw the webcam feed
        cam_bld.update();
        cam_frd.update();
        cam_flu.update();
        cam_bru.update();
        
       
        sf::Image images[] = {cam_bld.getDisplayableImage(), 
                              cam_frd.getDisplayableImage(),
                              cam_flu.getDisplayableImage(),
                              cam_bru.getDisplayableImage()};

        camera::CameraManager* cams[] = {&cam_bld, &cam_frd, &cam_flu, &cam_bru};
        for (int i = 0; i < 4; i++) {
            std::vector<std::string> facelets_cam = cams[i]->getFacelets();
            for (size_t j = 0; j < facelets_cam.size(); j++) {
                std::string facelet = facelets_cam[j];
                int index_data = getIndexOf(locations, facelet);
                data[index_data] = cams[i]->getData()[j];
            }
        }

        for (size_t i = 0; i < 54; i++) {
            if ((i+5) % 9 == 0) {
                data[i] = face_order[i / 9];
            }
        }

        int counts [6];

        for (int i = 0; i < 6; i++) { counts[i] = 0; }

        for(int i = 0; i < 54; i++) {
            counts[data[i]]++;
        }

        bool valid = true;
        for(int i = 0; i < 6 && valid; i++) {
            valid = counts[i] == 9;
        }

        drawWebcam(&window, images);
        drawScan(&window, data);

        window.display();
    }
    return 0;
}

