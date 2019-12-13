#pragma once

#include <vector>

#include "enemies.hpp"
#include "materials.hpp"
#include "birds.hpp"
#include "gameobject.hpp"
#include <fstream>
#include <utility>
#include <dirent.h>
#include <sys/types.h>

using namespace std;
class Map{
public:
    Map()
    {
        CallCount = 0;
        Read_Dir("../Maps");
    }
    ~Map(){}
    void Draw(sf::RenderWindow& window);
    void RunGame(sf::RenderWindow& window);
    void Run();
    void ReadMap(b2World & m_world, std::string filename);
    void RunMenu(sf::RenderWindow& window);
    void Read_Dir(const char *path);
    void SaveScore(std::string filename);
    bool KeepTrackOnBirds(BirdsBaseClass*);
    void RestartLevel(b2World *m_world, b2Body* staticBody);
    void RemoveDeadObjects(b2World *m_world);

private:
    vector<GameObject*> objects;
    vector<BirdsBaseClass*> birds;
    vector<Enemies*> enemies;
    vector<std::pair<std::string, sf::RectangleShape>> maps;
    int has_moved = 0;
    int phase = 0;
    string active_map_;
    sf::View view;
    bool win = false;
    int menu_phase_= 0;
    int birds_left_=0;
    //float score;
    sf::RectangleShape LevelObjects(int i);
    vector<pair<string,float>> results;
    pair<string,float> highscore;
    string playertext_;
    int CallCount = 0;
    std::vector<BirdsBaseClass*> TmpV;
    sf::Texture one;
    sf::Texture two;
    sf::Texture three;
};