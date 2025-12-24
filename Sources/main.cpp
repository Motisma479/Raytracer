#include <iostream>
#include <fstream>
#include <iomanip>

#include "Core/Color.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";
    for(int i = 0; i<argc; ++i)
    {
        std::cout << argv[i] << std::endl;
    }

    int image_width = 256;
    int image_height = 256;
    std::string filename = "Test.PPM";
    std::ofstream ostrm(filename, std::ios::binary);
    ostrm << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        
    std::cout << std::setfill('0') << "progress:\nline: 000; row: 000";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            Color col = {

                double(i) / (image_width - 1),
                0.0,
                double(j) / (image_height - 1)
            };

            
            ostrm << col.RGB255_str() << '\n';
            std::cout << "\rline: " << std::setw(3) << j << "; row: " << std::setw(3) << i;
        }
    }

    std::cout << "\nImage Finished" << std::endl;

    return 0;
}