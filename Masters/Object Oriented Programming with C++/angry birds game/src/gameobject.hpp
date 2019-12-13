#ifndef GAMEOBJECT_CLASS
#define GAMEOBJECT_CLASS


#define SCALE 30.0 
#define MUL 0.5

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include <cmath>


class GameObject{
public:
    enum Shape{
        circle = 1,
        rectangular =2
    };
    
    enum Type{
        BasicBird = 1,
        SpeedBird = 2,
        MonsterBird = 3,
        BigBird = 4,
        Wood = 10,
        Stone = 11,
        Ice = 12,
        Enemy = 20,
    };
    
    GameObject( b2World& world,
                float x, float y,
                float size_x,float size_y,
                float angle, float bounce,
                float density, float friction,
                GameObject::Shape shape,
                bool active = true,
                bool dynamic = true)
                :
                shape_(shape),
                size_x_(size_x),
                size_y_(size_y)
    {
        b2BodyDef BodyDef;
        total_impulse_ = 0;
        if (dynamic){
            BodyDef.type = b2_dynamicBody; // "dynamic" ball! Dynamic objects interact with other dynamic and static objects.
        }
        else
        {
            BodyDef.type = b2_staticBody; //this will be a static body
        }
        
        BodyDef.position = b2Vec2(x/SCALE,y/SCALE); // set position
        BodyDef.angle = angle; // starting angle

        Body_ = world.CreateBody(&BodyDef); // This creates the object in Box2D world
        if(shape == circle){
            b2CircleShape circleShape; // shape for circle object
            circleShape.m_radius = size_x/SCALE; // size_x cm radius
            b2FixtureDef circleFixtureDef;
            circleFixtureDef.shape = &circleShape;
            circleFixtureDef.density = density;
            circleFixtureDef.restitution = bounce;
            circleFixtureDef.friction = friction;
            Body_->CreateFixture(&circleFixtureDef); //This creates a fixture (shape etc.) for the object so it can interact with other objects
            //Body_->ApplyLinearImpulse(b2Vec2(30000,-30000),Body_->GetWorldCenter(),true);
            if(!active)
            {
                Body_->SetActive(false);
            }
            circle_.setRadius(size_x);
        }
        else
        {
            b2PolygonShape boxShape;
            boxShape.SetAsBox((size_x/2)/SCALE,(size_y/2)/SCALE);
            b2FixtureDef boxFixtureDef;
            boxFixtureDef.shape = &boxShape;
            boxFixtureDef.density = density;
            boxFixtureDef.restitution = bounce;
            boxFixtureDef.friction = friction;
            Body_->CreateFixture(&boxFixtureDef);

            rect_.setSize(sf::Vector2f(size_x, size_y));
        }

        destroy_ = false;
    }
    virtual ~GameObject(){}

    void ApplyForce(float x, float y)
    {
        std::cout << "Module: " /*<< vector_module*/ << std::endl;
        Body_->ApplyLinearImpulse(b2Vec2(-MUL*(x - 480.2),MUL*(-1)*(y - 540.2)),Body_->GetWorldCenter(),true);
    }

    virtual Type GetType(void) const = 0;

    virtual bool Object_Feature() = 0;

    void AddToTotalImpulse(float collision_impulse) { total_impulse_ += collision_impulse; }

    void SubtractFromTotalImpulse(float collision_impulse) { total_impulse_ -= collision_impulse; }

    void ClearTotalImpulse(void) { total_impulse_ = 0; }

    virtual void ContactHandling(void) = 0;

    bool Destroy(void) const { return destroy_; }

    virtual void set_inactive(){}

    friend class Map;
    protected:
        b2Body* Body_;
        sf::Texture texture_;
        GameObject::Shape shape_;
        sf::CircleShape circle_;
        sf::RectangleShape rect_;
        Type type_;
        float total_impulse_;
        bool destroy_;
        float size_x_,size_y_;

};

class GameObjectContanctListener : public b2ContactListener
{
    
    void BeginContact(b2Contact* contact)
    {
        b2Body* Body_A = contact->GetFixtureA()->GetBody();
        GameObject* Object_A = static_cast<GameObject*>(Body_A->GetUserData());

        b2Body* Body_B = contact->GetFixtureB()->GetBody();
        GameObject* Object_B = static_cast<GameObject*>(Body_B->GetUserData());

        if( ( Object_A != (NULL) ) && ( Object_B != (NULL) ) )
        {
           switch ( Object_A->GetType() )
            {
                case GameObject::Type::Enemy:
                case GameObject::Type::Wood:
                case GameObject::Type::Ice:
                case GameObject::Type::Stone:
                {
                    auto Impulse_A = Body_A->GetMass() * Body_A->GetLinearVelocity().Length();

                    Object_A->ClearTotalImpulse();

                    Object_A->SubtractFromTotalImpulse(Impulse_A);

                    switch (Object_B->GetType())
                    {
                        case GameObject::Type::Enemy:
                        case GameObject::Type::Wood:
                        case GameObject::Type::Ice:
                        case GameObject::Type::Stone:
                        {
                            auto Impulse_B = Body_B->GetMass() * Body_B->GetLinearVelocity().Length();

                            Object_B->ClearTotalImpulse();

                            Object_B->SubtractFromTotalImpulse(Impulse_B);
                        }
                        break;

                        case GameObject::Type::BasicBird:
                        case GameObject::Type::SpeedBird:
                        case GameObject::Type::MonsterBird:
                        case GameObject::Type::BigBird:
                        {
                            Object_B->set_inactive();
                        }
                        break;
                    
                        default: break;
                    }
                }
                break;

                case GameObject::Type::BasicBird:
                case GameObject::Type::SpeedBird:
                case GameObject::Type::MonsterBird:
                case GameObject::Type::BigBird:
                {   
                    Object_A->set_inactive();

                    switch (Object_B->GetType())
                    {
                        case GameObject::Type::Enemy:
                        case GameObject::Type::Wood:
                        case GameObject::Type::Ice:
                        case GameObject::Type::Stone:
                        {
                            auto Impulse_B = Body_B->GetMass() * Body_B->GetLinearVelocity().Length();

                            Object_B->ClearTotalImpulse();

                            Object_B->SubtractFromTotalImpulse(Impulse_B);
                        }
                        break;
                    
                        default: break;
                    }
                }
                default: break;
            } 
        }
    }

    void EndContact(b2Contact* contact)
    {
  
        b2Body* Body_A = contact->GetFixtureA()->GetBody();
        GameObject* Object_A = static_cast<GameObject*>(Body_A->GetUserData());

        b2Body* Body_B = contact->GetFixtureB()->GetBody();
        GameObject* Object_B = static_cast<GameObject*>(Body_B->GetUserData());

        if( ( Object_A != (NULL) ) && ( Object_B != (NULL) ) )
        {
            switch ( Object_A->GetType() )
            {
                case GameObject::Type::Enemy:
                case GameObject::Type::Wood:
                case GameObject::Type::Ice:
                case GameObject::Type::Stone:
                {
                    auto Impulse_A = Body_A->GetMass() * Body_A->GetLinearVelocity().Length();

                    Object_A->AddToTotalImpulse(Impulse_A);

                    Object_A->ContactHandling();

                    switch (Object_B->GetType())
                    {
                        case GameObject::Type::Enemy:
                        case GameObject::Type::Wood:
                        case GameObject::Type::Ice:
                        case GameObject::Type::Stone:
                        {
                            auto Impulse_B = Body_B->GetMass() * Body_B->GetLinearVelocity().Length();

                            Object_B->AddToTotalImpulse(Impulse_B);

                            Object_B->ContactHandling();
                        }
                        break;

                        case GameObject::Type::BasicBird:
                        case GameObject::Type::SpeedBird:
                        case GameObject::Type::MonsterBird:
                        case GameObject::Type::BigBird:

                        break;
                    
                        default: break;
                    }
                }
                break;

                case GameObject::Type::BasicBird:
                case GameObject::Type::SpeedBird:
                case GameObject::Type::MonsterBird:
                case GameObject::Type::BigBird:
                {
                    switch (Object_B->GetType())
                    {
                        case GameObject::Type::Enemy:
                        case GameObject::Type::Wood:
                        case GameObject::Type::Ice:
                        case GameObject::Type::Stone:
                        {
                            auto Impulse_B = Body_B->GetMass() * Body_B->GetLinearVelocity().Length();

                            Object_B->AddToTotalImpulse(Impulse_B);

                            Object_B->ContactHandling();
                        }
                        break;
                    
                        default: break;
                    }
                }
                default: break;
            }
        }
    }
};
#endif