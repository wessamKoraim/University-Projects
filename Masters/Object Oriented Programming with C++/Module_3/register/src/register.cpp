#include "register.hpp"

//TODO: implement Register's members

void Register::Add(Vehicle* v)
{
    vehicles_.emplace_back(v);
}

void Register::Save(const std::string& filename) const
{
    std::ofstream outfile;

    outfile.open(filename, std::ios_base::app);

    for(auto & v : vehicles_) 
    {
        v->Write(outfile);
    }
}

bool Register::ReadLine(std::istream& stream)
{
    
}