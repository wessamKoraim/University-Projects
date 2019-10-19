#include "pokemon.hpp"

PokemonCollection::PokemonCollection(PokemonCollection collection, PokemonCollection collection2)
{
    collection.pokemons_.merge(collection2.pokemons_);
    collection.pokemons_.sort();
    collection.pokemons_.unique();
}

void PokemonCollection::Add(const std::string& name, size_t id)
{
    std::pair<std::string, size_t> p;
    p.first = name;
    p.second = id;
    pokemons_.emplace_back(p);
}

bool PokemonCollection::Remove(const std::string& name, size_t id)
{
    bool RetVal = false;
    std::list<std::pair<std::string, size_t>>::iterator i = pokemons_.begin();
    
    for (auto pokemon : pokemons_)
    {
        if(pokemon.first == name && pokemon.second == id)
        {
            pokemons_.erase(i);
            RetVal = true;
            break;
        }

        i++;
    }

    return RetVal;
}

void PokemonCollection::Print() const
{
    for (auto & pokemon : pokemons_)
        std::cout << "id: "<< pokemon.second <<", name: " << pokemon.first << std::endl;
}

void PokemonCollection::SortByName()
{
    pokemons_.sort();
}

void PokemonCollection::SortById()
{
    pokemons_.sort([](const std::pair<std::string, size_t> &x,
                                        const std::pair<std::string, size_t> &y)
    {
        return x.second < y.second;
    });
}

