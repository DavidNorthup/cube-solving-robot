#include "Options.h"

namespace options {

OptionsManager::OptionsManager(std::string file_path) {
    // Open the stream for both input and output.
    options_file = new std::fstream(file_path);
    std::cout << "Reading options in file: " << file_path << std::endl; 

    // Read in any of the existing options
    std::string opt_str; // A container for the option string
    double value;
    while (*options_file >> opt_str >> value) {
        std::cout << opt_str << ": " << value << std::endl; 
        // Put the option into the map
        options_map.insert(std::make_pair(opt_str, value));
    }
    std::cout << "Finished setting " << options_map.size() <<  " options" << std::endl;
}

OptionsManager::~OptionsManager() {
    write();
    options_file->close();
    delete options_file;
}

double OptionsManager::getOption(std::string opt_string) {
    if (options_map.count(opt_string)) {
        return options_map.find(opt_string)->second;
    }
    return -1;
}

void OptionsManager::setOption(std::string opt_string, double value) {
    if (options_map.count(opt_string)) {
        options_map.erase(opt_string);
    } 
    options_map.insert(std::make_pair(opt_string, value));
}

void OptionsManager::write() {
    std::map<std::string, double>::iterator itr;
    for (itr = options_map.begin(); itr != options_map.end(); ++itr) {
        (*options_file) << itr->first << " " << itr->second << std::endl;
    }
}

}