#include "chinese_dragon.hpp"

// Define ChineseDragon's methods here
/*
ChineseDragon::~ChineseDragon(void)
{
    treasures_.clear();
}
*/
void ChineseDragon::Eat(std::list<Food>& food_list)
{
    for(auto it = food_list.begin(); it != food_list.end(); )
    {
        if ( (it->type == Herbs) || (it->type == PeopleFood) )
        {
            size_++;
            std::cout << "Chinese dragon ate: " << it->name << std::endl;
            it = food_list.erase(it);
        }
        else
        {
            it++;
        }
        
    }
}

void ChineseDragon::Hoard(std::list<Treasure>& treasure_list)
{
    std::list<Treasure>::const_iterator i = treasures_.begin();
    for(auto it = treasure_list.begin(); it != treasure_list.end(); )
    {
        if (it->type == Wisdom)
        {
            treasures_.emplace_back(*it);
            std::cout << "Chinese dragon received: " << it->name << std::endl;
            it = treasure_list.erase(it);
            i++;
        }
        else
        {
            it++;
        }
    }
}