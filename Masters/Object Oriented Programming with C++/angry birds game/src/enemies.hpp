#pragma once

#include <iostream>

#include "gameobject.hpp"

extern float UserLevelScore;


class Enemies : public GameObject
{
    public:

    Enemies(b2World& world,
            double x                 = 900.0F,
            double y                 = 650.0F,
            double size_x            = 0,
            double size_y            = 0,
            double angle             = 0,
            double bounce            = 0,
            double density           = 0.5,
            double friction          = 0.5,
            GameObject::Shape shape = GameObject::Shape::circle,
            bool active             = true,
            bool dynamic            = true
            )
            :
            GameObject(world, x, y, size_x, size_y, angle, bounce, density, friction, shape, active, dynamic)
            {

                sf::Texture enemey_1;

                if( enemey_1.loadFromFile("../Images/pig_1.png") )
                {
                }

                enemey_1.setSmooth(true);
                texture_ = enemey_1;
                Body_->SetUserData(this);
                my_total_impulse_ = 0;
                destroy_enemy_ = false;
                type_ = GameObject::Type::Enemy;
            }

    ~Enemies(){ Body_->GetWorld()->DestroyBody(Body_);}

    virtual bool Object_Feature() override { return true; }

    virtual Type GetType(void) const override { return GameObject::Type::Enemy; }

    float GetTotalImpulse(void) const { return my_total_impulse_; }

    bool DestroyEnemy(void) const { return destroy_enemy_; }
    
    virtual void ContactHandling(void) override
    { 
        if( total_impulse_ > 0 )
        {
            my_total_impulse_ += total_impulse_;
            UserLevelScore += my_total_impulse_;
        }

        if( (impulse_threshold_[0] < my_total_impulse_) && (my_total_impulse_ <=  impulse_threshold_[1]) )
        {

            sf::Texture enemey_1;

            if(enemey_1.loadFromFile("../Images/pig_2.png"))
            {
                //This should not happen
            }

            enemey_1.setSmooth(true);
            texture_ = enemey_1;
        }
        else if( (impulse_threshold_[1] < my_total_impulse_) && (my_total_impulse_ <=  impulse_threshold_[2]) )
        {
            sf::Texture enemey_1;

            if(enemey_1.loadFromFile("../Images/pig_3.png"))
            {
                //This should not happen
            }

            enemey_1.setSmooth(true);
            texture_ = enemey_1;
        }
        else if( (impulse_threshold_[2] < my_total_impulse_) && (my_total_impulse_ <=  impulse_threshold_[3]) )
        {
            sf::Texture enemey_1;

            if(enemey_1.loadFromFile("../Images/pig_4.png"))
            {
                //This should not happen
            }

            enemey_1.setSmooth(true);
            texture_ = enemey_1;
        }
        else if( my_total_impulse_ > impulse_threshold_[3] )
        {
            destroy_enemy_ = true;
        }
    }
    
    private:

    float my_total_impulse_;
    bool destroy_enemy_;
    const float impulse_threshold_[4] = {1.0, 2.0, 4.0, 7.0};
};