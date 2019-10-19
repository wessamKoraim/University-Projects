#include "fantasy_dragon.hpp"

// Define FantasyDragon's methods here
/*
FantasyDragon::~FantasyDragon(void)
{
    treasures_.clear();
}
*/
void FantasyDragon::Eat(std::list<Food>& food_list)
{
    for(std::list<Food>::const_iterator it = food_list.begin(); it != food_list.end();)
    {
        if ( (it->type == People) || (it->type == PeopleFood) )
        {
            size_++;
            std::cout << "Fantasy dragon ate: " << it->name << std::endl;
            it = food_list.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void FantasyDragon::Hoard(std::list<Treasure>& treasure_list)
{
    std::list<Treasure>::const_iterator i = treasures_.begin();
    for(auto it = treasure_list.begin(); it != treasure_list.end(); )
    {
        if (it->type == Jewellery)
        {
            treasures_.emplace_back(*it);
            std::cout << "Fantasy dragon received: " << it->name << std::endl;
            it = treasure_list.erase(it);
            i++;
        }
        else
        {
            it++;
        }
    }
}
