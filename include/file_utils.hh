#include <exception>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string read_file(const std::string& file_name) {
    std::fstream file(file_name);
    std::stringstream contents_stream;
    try {
	contents_stream << file.rdbuf();
	file.close();
    }
    catch (const std::exception& e) {
	std::cout << "ERROR: " << e.what() << std::endl;
    }
    return contents_stream.str();
}
