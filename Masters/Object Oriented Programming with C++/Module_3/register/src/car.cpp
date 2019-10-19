#include "car.hpp"
//TODO: implement Car's members

Car::Car(   std::string register_number,
            std::string owner, 
            std::string maker, 
            unsigned int range, 
            bool inspection) :

            Vehicle(register_number, owner),
            maker_(maker),
            range_(range),
            inspection_(inspection){};

void Car::Write(std::ostream& stream)
{
    stream << "C;" << GetRegisterNumber();
    stream << ";" << GetOwner();
    stream << ";" << maker_;
    stream << ";" << range_;
    stream << ";" << (inspection_ ? '1' : '0') << "\n";
}

void Car::Print()
{
    std::cout << "C;" << GetRegisterNumber();
    std::cout << ";" << GetOwner();
    std::cout << ";" << maker_;
    std::cout << ";" << range_;
    std::cout << ";" << (inspection_ ? '1' : '0') << "\n";
}
Car* Car::Read(std::istream& stream)
{
    std::string delimiter = ";";
    std::string in;
    stream >> in;

    if(in[0] != 'C')
        return NULL;

    std::string register_number;
    std::string owner;
    std::string maker;
    unsigned int range;
    bool inspection;
    
    size_t pos = 0;
    std::string token;
    int i = 0;


    while ((pos = in.find(delimiter)) != std::string::npos)
    {
        token = in.substr(0, pos);
        //std::cout << token << std::endl;
        if(i == 0)
            register_number = token;
        else if(i == 1)
            owner = token;
        else if(i == 2)
            maker = token;
        else if(i == 3)
            range = (unsigned int)std::stoi(token);
        else
            inspection = ((token == "0") ? false : true);
        
        in.erase(0, pos + delimiter.length());
    }

    return new Car(register_number, owner, maker, range, inspection);
}

