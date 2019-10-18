#include "boat.hpp"
//TODO: implement Boat's members

Boat::Boat( std::string register_number,
            std::string owner, 
            std::string name,
            double draft,
            double power) :
            Vehicle(register_number, owner),
            name_(name),
            draft_(draft),
            power_(power){};

void Boat::Write(std::ostream& stream)
{
    stream << "B;" << GetRegisterNumber();
    stream << ";" << GetOwner();
    stream << ";" << name_;
    stream << ";" << draft_;
    stream << ";" << power_ << "\n";
}

void Boat::Print()
{
    std::cout << "B;" << GetRegisterNumber();
    std::cout << ";" << GetOwner();
    std::cout << ";" << name_;
    std::cout << ";" << draft_;
    std::cout << ";" << power_ << "\n";
}

Boat* Boat::Read(std::istream& stream)
{
    std::string delimiter = ";";
    std::string in;
    stream >> in;

    if(in[0] != 'B')
        return NULL;

    std::string register_number;
    std::string owner;
    std::string name;
    double draft;
    double power;
    
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
            name = token;
        else if(i == 3)
            draft = std::stod(token);
        else
            power = std::stod(token);
        
        in.erase(0, pos + delimiter.length());
    }

    return new Boat(register_number, owner, name, draft, power);
}
