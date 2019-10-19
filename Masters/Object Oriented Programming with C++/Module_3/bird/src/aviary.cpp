#include "aviary.hpp"

void Aviary::Add(Bird* bird)
{
    if(bird == NULL)
    {
        throw std::logic_error("Error!");
    }

    birds.emplace_back((bird));
}

void Aviary::SpeakAll(std::ostream& out) const
{
    for (auto& bird : birds)
    {
        bird->Speak(out);
    }
}

Aviary::sizeType Aviary::Size() const
{
    return birds.size();
}

Bird* Aviary::operator[](Aviary::sizeType index)
{
    if( index >= Size() )
    {
        throw std::out_of_range("Error!");
    }

    return birds[index];
}

const Bird* Aviary::operator[](Aviary::sizeType index) const
{
    if( index >= Size() )
    {
        throw std::out_of_range("Error!");
    }

    return birds[index];
}

Aviary::~Aviary()
{
    for(auto ptr:birds)
    {
        delete ptr;
    }
}