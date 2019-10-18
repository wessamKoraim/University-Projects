#include "aircraft.hpp"
//TODO: implement Aircraft's members

Aircraft::Aircraft( std::string register_number,
                    std::string owner,
                    std::string model,
                    double wingspan,
                    unsigned int cruise_speed) :
                    Vehicle(register_number, owner),
                    model_(model),
                    wingspan_(wingspan),
                    cruise_speed_(cruise_speed)
                    {};

void Aircraft::Write(std::ostream& stream)
{
    stream << "A;" << GetRegisterNumber();
    stream << ";" << GetOwner();
    stream << ";" << model_;
    stream << ";" << wingspan_;
    stream << ";" << cruise_speed_ << "\n";
}

void Aircraft::Print()
{
    std::cout << "A;" << GetRegisterNumber();
    std::cout << ";" << GetOwner();
    std::cout << ";" << model_;
    std::cout << ";" << wingspan_;
    std::cout << ";" << cruise_speed_ << "\n";
}

Aircraft* Aircraft::Read(std::istream& stream)
{
    std::string delimiter = ";";
    std::string in;
    stream >> in;

    if(in[0] != 'A')
        return NULL;

    std::string register_number;
    std::string owner;
    std::string model;
    double wingspan;
    unsigned int cruise_speed;
    
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
            model = token;
        else if(i == 3)
            wingspan = std::stod(token);
        else
            cruise_speed = (unsigned int) std::stoi(token);
        
        in.erase(0, pos + delimiter.length());
    }

    return new Aircraft(register_number, owner, model, wingspan, cruise_speed);
}

