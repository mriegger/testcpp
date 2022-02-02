#include "PPMCreator.h"
#include <fstream>
#include <string>

void PPMCreator::Create(const std::string_view filename, const int width, const int height, const std::span<char> rgbData)
{
    using namespace std;
    ofstream myFile;
    myFile.open(filename.data(), ios::out | ios::binary);

    static constexpr string_view MagicNumber = "P6\n";
    myFile.write(MagicNumber.data(), MagicNumber.size());

    const std::string widthHeightStr = std::to_string(width) + " " + std::to_string(height) + "\n";

    myFile.write(widthHeightStr.data(), widthHeightStr.size());

    static constexpr string_view MaxValue = "255\n";
    myFile.write(MaxValue.data(), MaxValue.size());
    int numLeftToWrite = 256 * 256;
    myFile.write(rgbData.data(), rgbData.size());

  /*  while (numLeftToWrite-- > 0)
    {
        char black = 0;
        char red = 255;
        myFile.write(&red, sizeof(uint8_t));
        myFile.write(&black, sizeof(uint8_t));
        myFile.write(&black, sizeof(uint8_t));

    }*/


    myFile.close();

}
