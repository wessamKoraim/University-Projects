#include "rectangle.hpp"

#include <iostream>

// Write you function here and test it with the main.

void Rectangle() {
    
    float Width, Height;
    std::cout << "Please enter width and height" << std::endl;
    std::cin >> Width;
    std::cin >> Height;
    std::cout << "Area: " << Width * Height << std::endl;
    std::cout << "Circumference: " << 2 * (Width + Height) << std::endl;
    
}

