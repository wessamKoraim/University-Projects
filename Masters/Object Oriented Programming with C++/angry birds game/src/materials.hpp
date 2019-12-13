#ifndef MATERIALS_CLASS
#define MATERIALS_CLASS


#include <iostream>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "gameobject.hpp"

#define WOOD_IMPULSE_THRESHOLD  50
#define ICE_IMPULSE_THRESHOLD   20
#define STONE_IMPULSE_THRESHOLD 80

extern float UserLevelScore;

class Wood : public GameObject{
public: 
    Wood(b2World& world,float x, float y,float size_x,float size_y, float angle, float bounce = 0,float density = 2, float friction = 1, GameObject::Shape shape = rectangular,bool dynamic = true)
    : GameObject(world, x, y, size_x, size_y, angle, bounce, density, friction, shape, dynamic) {
        sf::Texture wood;
        wood.loadFromFile("../Images/wood1.png");
        wood.setSmooth(true);
        texture_ = wood;
        type_ = GameObject::Type::Wood;
        my_total_impulse_ = 0;
        Body_->SetUserData( this );
    }
    ~Wood(){ Body_->GetWorld()->DestroyBody(Body_);}
    virtual bool Object_Feature() override
    {
    /*
        Materials don't have any feature
    */

        return true;
    }

    virtual Type GetType(void) const override { return GameObject::Type::Wood; }

    virtual void ContactHandling(void) override
    {
        if( total_impulse_ > 0 )
        {
            my_total_impulse_ += total_impulse_;
            UserLevelScore += my_total_impulse_;
        }

        if( my_total_impulse_ > impulse_threshold_ )
        {
            destroy_ = true;
        }
    }
    private:
        float my_total_impulse_;
        const float impulse_threshold_ = WOOD_IMPULSE_THRESHOLD;
};

class Stone : public GameObject{
public: 
    Stone(b2World& world,float x, float y,float size_x,float size_y, float angle, float bounce = 0,float density = 4, float friction = 1, GameObject::Shape shape = rectangular,bool dynamic = true)
    : GameObject(world, x, y, size_x, size_y, angle, bounce, density, friction, shape, dynamic) {
        sf::Texture stone;
        stone.loadFromFile("../Images/stone.png");
        stone.setSmooth(true);
        texture_ = stone;
        type_ = GameObject::Type::Stone;
        my_total_impulse_ = 0;
        Body_->SetUserData( this );
    }
    ~Stone(){ Body_->GetWorld()->DestroyBody(Body_);}
    virtual bool Object_Feature() override
    {
    /*
        Materials don't have any feature
    */

    return true;
    }

    virtual Type GetType(void) const override { return GameObject::Type::Stone; }

    virtual void ContactHandling(void) override
    {
        if( total_impulse_ > 0 )
        {
            my_total_impulse_ += total_impulse_;
            UserLevelScore += my_total_impulse_;
        }

        if( my_total_impulse_ > impulse_threshold_ )
        {
            destroy_ = true;
        }
    }
    private:
        float my_total_impulse_;

        const float impulse_threshold_ = STONE_IMPULSE_THRESHOLD;
};

class Ice : public GameObject{
public: 
    Ice(b2World& world,float x, float y,float size_x,float size_y, float angle, float bounce = 0,float density = 1, float friction = 1, GameObject::Shape shape = rectangular,bool dynamic = true)
    : GameObject(world, x, y, size_x, size_y, angle, bounce, density, friction, shape, dynamic) {
        sf::Texture ice;
        ice.loadFromFile("../Images/ice.png");
        ice.setSmooth(true);
        texture_ = ice;
        type_ = GameObject::Type::Ice;
        my_total_impulse_ = 0;
        Body_->SetUserData( this );
    }

    ~Ice(){ Body_->GetWorld()->DestroyBody(Body_);}
    virtual bool Object_Feature() override
    {
    /*
        Materials don't have any feature
    */

    return true;
    }

    virtual Type GetType(void) const override { return GameObject::Type::Ice; }

    virtual void ContactHandling(void) override
    {
        if( total_impulse_ > 0 )
        {
            my_total_impulse_ += total_impulse_;
            UserLevelScore += my_total_impulse_;
        }
        

        if( my_total_impulse_ > impulse_threshold_ )
        {
            destroy_ = true;
        }
    }

private:
    float my_total_impulse_;
    const float impulse_threshold_ = ICE_IMPULSE_THRESHOLD;
};

#endif