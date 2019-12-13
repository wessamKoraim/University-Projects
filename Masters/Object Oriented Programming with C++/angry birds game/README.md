# OOP using C++ course
## Angry Birds Game
1. Level#1
<p align="center"> 
<img src="https://github.com/wessamKoraim/University-Projects/blob/master/Masters/Object%20Oriented%20Programming%20with%20C%2B%2B/angry%20birds%20game/src/Images/level_1.png" width="1200px">
</p>

2. Level#2

<p align="center"> 
<img src="https://github.com/wessamKoraim/University-Projects/blob/master/Masters/Object%20Oriented%20Programming%20with%20C%2B%2B/angry%20birds%20game/src/Images/level_2.png" width="1200px">
</p>

3. Level#3

<p align="center"> 
<img src="https://github.com/wessamKoraim/University-Projects/blob/master/Masters/Object%20Oriented%20Programming%20with%20C%2B%2B/angry%20birds%20game/src/Images/level_3.png" width="1200px">
</p>  

---
### Installing SFML and BOX2D
```sh
sudo apt install build-essential
sudo apt install libsfml-dev
sudo apt install libbox2d-dev
```
### How to build and run the game (make sure you have BOX2D and SFML installed to the standard locations)

#### Using CMake
```sh
cd src
cd build  
cmake ..  
make  
./game
```
#### Using gcc
```sh
cd src
cd build
g++ -Wall  ../map.cpp ../main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lBox2D -o game
```