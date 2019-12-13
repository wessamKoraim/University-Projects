# OOP using C++ course
---
## Enemies Branch
### Installing SFML and BOX2D
```
sudo apt install libsfml-dev
sudo apt install libbox2d-dev
```
### How to build and run the game (make sure you have BOX2D and SFML installed to the standard locations)

#### Using Cmake
```
cd src
cd build  
cmake ..  
make  
./game
```
#### Using gcc
```
g++ -Wall  map.cpp main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lBox2D
```