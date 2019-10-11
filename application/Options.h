#include <opencv2/opencv.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <fstream>

#define OPTIONS_WINDOW_WIDTH 500
#define OPTIONS_WINDOW_HEIGHT 700
#define OPTIONS_WINDOW_TITLE "CSR Options"
#define OPTIONS_BACKGROUND_COLOR sf::Color(90,90,90,255)
#define OPTIONS_PADDING 10

namespace options {

class OptionsManager {
    public:
    OptionsManager(std::string file_path);
    ~OptionsManager();
    double getOption(std::string option_string);
    void setOption(std::string option_string, double value);

    private:
    std::map<std::string, double> options_map;
    std::fstream* options_file;
    
    void write();
};


}