#include "poly.hpp"
#include <sstream>

int Poly::operator[](int exp) const {
    Values::const_iterator it = values_.find(exp);
    return it == values_.end() ? 0 : it->second;
}

std::ostream& operator<<(std::ostream& out, const Poly& polygon)
{
    std::map<int, int>::const_reverse_iterator rit;

    if(polygon.begin() == polygon.end())
    {
        out << "";
    }
    else
    {
        
        rit = polygon.begin();
        out << rit->second << 'x' << rit->first;
        rit++;

        for ( ; rit != polygon.end(); rit++)
        {
            if(rit->second >= 0)
            {
                out << '+';
            }
            out << rit->second << 'x' << rit->first;
        }
        
    }
    
    
    return out;
}

std::istream& operator>>(std::istream& is, Poly& p)
{
    std::string InputString;
    is>>InputString;
    
    std::vector<std::pair<const int, int>> v_pair = evaluate(InputString);

    std::vector<std::pair<const int, int>>::iterator rit_src;

    for(rit_src = v_pair.begin(); rit_src != v_pair.end(); rit_src++)
    {
        p[rit_src->first] = rit_src->second;
    }

    return is;
}

Poly operator-(const Poly& p)
{
    Poly polygon;

    std::map<const int, int>::const_reverse_iterator rit_src;

    for(rit_src = p.begin(); rit_src != p.end(); rit_src++)
    {
        polygon[rit_src->first] = -1 * (rit_src->second);
    }
    return polygon;
}

std::vector<std::pair<const int, int>> evaluate(std::string tokens)
{
    std::vector<int> ans;
    std::stack<int> values;
    bool neg = false;

    for(unsigned int i = 0; i < tokens.length(); i++)
    {
        if(isdigit(tokens[i]))
        {
            int val = 0;
            while(i < tokens.length() && isdigit(tokens[i]))
            {
                val = (val * 10) + (tokens[i] - '0');
                i++;
            }
            if(neg)
            {
                val = val * -1;
                neg = false;
            }
            values.push(val);
            i--;
        }
        else if(tokens[i] == '-')
        {
            neg = true;
        }
    }

    while(!values.empty())
    {
        ans.push_back(values.top());
        values.pop();
    }

    std::vector<std::pair<const int, int>> RetVal;
    
    std::vector<int>::reverse_iterator rit = ans.rbegin();

    while(rit != ans.rend())
    {
        if( (*(rit)) != 0)
            RetVal.emplace_back(std::pair<const int, int> ( *(rit+1) , *rit ));
        rit++;
        rit++;
    }

    return RetVal;
}

Poly operator+(const Poly& a, const Poly& b)
{
    Poly RetVal;

    for(auto & v : a)
    {
        RetVal[v.first] += v.second;
    }

    for(auto & v : b)
    {
        RetVal[v.first] += v.second;
    }

    return RetVal;
}

Poly operator-(const Poly& a, const Poly& b)
{
    Poly RetVal;

    RetVal = a + (-b);

    return RetVal;
}

Poly& Poly::operator+=(const Poly& b)
{
    (*this) = (*this) + b;

    return (*this);
}

Poly& Poly::operator-=(const Poly& b)
{
    (*this) = (*this) - b;

    return (*this);
}

bool operator<(const Poly& a, const Poly& b)
{
    if( a.IsEmpty() && b.IsEmpty() )
    {
        return false;
    }
    else if(a.IsEmpty())
    {
        return true;
    }
    else if(b.IsEmpty())
    {
        return false;
    }
    
    
    return ( (a.begin()->first) < (b.begin()->first) );
}

bool operator>(const Poly& a, const Poly& b)
{
    return ( (!(a < b)) && (!(a == b)) );
}

bool operator==(const Poly& a, const Poly& b)
{
    if(a.IsEmpty() && b.IsEmpty())
    {
        return true;
    }
    else if( (a.IsEmpty() && !b.IsEmpty()) || (!a.IsEmpty() && b.IsEmpty()) )
    {
        return false;
    }
    
    return ( (a.begin()->first) == (b.begin()->first) );
}

bool operator!=(const Poly& a, const Poly& b)
{
    return !(a == b);
}