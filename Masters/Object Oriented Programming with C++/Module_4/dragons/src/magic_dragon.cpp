#include "magic_dragon.hpp"

// Define MagicDragon's methods here
/*
MagicDragon::~MagicDragon(void)
{
    treasures_.clear();
}
*/

void MagicDragon::Eat(std::list<Food>& food_list)
{
    
    for(auto it = food_list.begin(); it != food_list.end(); )
    {
        if (it->type == Herbs)
        {
            size_++;
            std::cout << "Magic dragon ate: " << it->name << std::endl;
            it = food_list.erase(it);
        }
        else
        {
            it++;
        }
        
    }
}

void MagicDragon::Hoard(std::list<Treasure>& treasure_list)
{
    std::list<Treasure>::const_iterator i = treasures_.begin();
    for(auto it = treasure_list.begin(); it != treasure_list.end(); )
    {
        if (it->type == Potions)
        {
            treasures_.emplace_back(*it);
            std::cout << "Magic dragon received: " << it->name << std::endl;
            it = treasure_list.erase(it);
            i++;
        }
        else
        {
            it++;
        }
    }
}