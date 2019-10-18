#include "dragon_cave.hpp"

// Define DragonCave's methods here

DragonCave::~DragonCave(void)
{
    dragons_.clear();
}

const std::list<Dragon*>& DragonCave::GetDragons(void) const
{
    return dragons_;
}

void DragonCave::Accommodate(Dragon* dragon)
{
    dragons_.emplace_back(dragon);
}

void DragonCave::Evict(const std::string& dragon_name)
{
    for(auto it = dragons_.begin(); it != dragons_.end();)
    {
        if ((*(*it)).GetName() == dragon_name)
        {
            it = dragons_.erase(it);
        }
        else
        {
            it++;
        }
    }
}

std::ostream& operator << (std::ostream& out, DragonCave dragon_cave)
{
    out << "DragonCave dwellers:" << std::endl << std::endl;

    auto dragon_list = dragon_cave.GetDragons();
    auto list_size = dragon_list.size() - 1;
    unsigned int i = 0;
    auto it = dragon_list.begin();

    for(; it != dragon_list.end(); it++)
    {   
        out << (*(*it));
        
        if(i < list_size)
        {
            out << std::endl;
        }
        i++;
    }

    return out;
}
