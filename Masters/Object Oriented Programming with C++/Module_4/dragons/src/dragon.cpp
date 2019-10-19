#include "dragon.hpp"

std::ostream& operator << (std::ostream& out, const Dragon& dragon)
{

    out << "Dragon named: " << dragon.GetName();
    out << ", age: " << dragon.GetAge();
    out << ", size: " << dragon.GetSize() << std::endl;
    out << "Treasures:" << std::endl;

    const std::list<Treasure>& treasures = dragon.GetTreasures();
    
    for(auto const& treasure : treasures)
    {
        out << treasure.name << std::endl;
    }
    return out;
}