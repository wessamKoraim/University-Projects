#ifndef BIRDS_CLASS
#define BIRDS_CLASS
#include "birdsBaseClass.hpp"


class StandardBird : public BirdsBaseClass
{

	public:
	StandardBird(b2World& world,float x, float y,float size_x = 30,float size_y = 0, float angle = 0,float bounce = 0.05,float density = 1) 
		: BirdsBaseClass(world, x, y, size_x, size_y, angle, bounce, density) 
		{
			type_ = GameObject::Type::BasicBird;
			sf::Texture bird_texture;
			bird_texture.loadFromFile("../Images/standard_bird.png");
			bird_texture.setSmooth(true);
			texture_ = bird_texture;
			Body_->SetUserData( this );
		}
	~StandardBird(){std::cout << "standard bird destructor" << std::endl; Body_->GetWorld()->DestroyBody(Body_);}
	
	virtual bool Object_Feature() override
	{
		/*
			This is a standard bird, does not have any feature
		*/

		return true;
	}
	virtual Type GetType(void) const override { return GameObject::Type::BasicBird; }
	virtual void ContactHandling(void) override{}
	
};


class SpeedBird : public BirdsBaseClass
{

	public:
	SpeedBird(b2World& world,float x, float y,float size_x = 30,float size_y = 0, float angle = 0,float bounce = 0.05,float density = 1) 
		: BirdsBaseClass(world, x, y, size_x, size_y, angle, bounce, density) 
		{
			type_ = GameObject::Type::SpeedBird;
			sf::Texture bird_texture;
			bird_texture.loadFromFile("../Images/speedbird.png");
			bird_texture.setSmooth(true);
			texture_ = bird_texture;
			Body_->SetUserData( this );
		}
	
	~SpeedBird(){std::cout << "Speed bird destructor" << std::endl; Body_->GetWorld()->DestroyBody(Body_); }
	
	virtual bool Object_Feature() override
	{
		/*
			This bird increases its velocity
		*/
		Body_->ApplyLinearImpulse(b2Vec2(3*(Body_->GetLinearVelocity().x),3*(Body_->GetLinearVelocity().y)), Body_->GetWorldCenter(),true);

		return true;
	}

	virtual Type GetType(void) const override { return GameObject::Type::SpeedBird; }
	virtual void ContactHandling(void) override{}
};

class BigBird : public BirdsBaseClass
{

	public:
	BigBird(b2World& world,float x, float y,float size_x = 42,float size_y = 0, float angle = 0,float bounce = 0.3,float density = 0.8) 
		: BirdsBaseClass(world, x, y, size_x, size_y, angle, bounce, density) 
		{
			type_ = GameObject::Type::BigBird;
			sf::Texture bird_texture;
			bird_texture.loadFromFile("../Images/big_bird.png");
			bird_texture.setSmooth(true);
			texture_ = bird_texture;
			Body_->SetUserData( this );
		}
	
	~BigBird(){std::cout << "Big bird destructor" << std::endl; Body_->GetWorld()->DestroyBody(Body_); }
	
	virtual bool Object_Feature()
	{
		/*
			This is a standard bird, does not have any feature
		*/

		return true;
	}

	virtual Type GetType(void) const override { return GameObject::Type::BigBird; }
	virtual void ContactHandling(void) override{}
};


class MonsterBird : public BirdsBaseClass
{

	public:
	MonsterBird(b2World& world,float x, float y,float size_x = 30,float size_y = 0, float angle = 0,float bounce = 0.35,float density = 0.5) 
		: BirdsBaseClass(world, x, y, size_x, size_y, angle, bounce, density) 
		{
			type_ = GameObject::Type::MonsterBird;
			sf::Texture bird_texture;
			bird_texture.loadFromFile("../Images/small_bird.png");
			bird_texture.setSmooth(true);
			texture_ = bird_texture;
			Body_->SetUserData( this );
		}

	~MonsterBird(){std::cout << "Monster bird destructor" << std::endl; Body_->GetWorld()->DestroyBody(Body_); }
	
	virtual bool Object_Feature()
	{
		/*
			This bird increases its size!
		*/
		
		for (b2Fixture* f = Body_->GetFixtureList(); f; f = f->GetNext()) 
		{
			
			//Increasing radio of the Box2D object
			f->GetShape()->m_radius = circle_.getRadius()*2/SCALE;
		}

		Body_->ApplyLinearImpulse(b2Vec2(1.5*(Body_->GetLinearVelocity().x),1.5*(Body_->GetLinearVelocity().y)), Body_->GetWorldCenter(),true);

		//Increasing radio of the SFML circle and updating the textures
		size_x_ = size_x_*2;
		circle_.setRadius(size_x_);
		sf::Texture bird_texture;
		bird_texture.loadFromFile("../Images/small_bird.png");
		bird_texture.setSmooth(true);
		texture_ = bird_texture;

		

		return true;
	}

	virtual Type GetType(void) const override { return GameObject::Type::MonsterBird; }
	virtual void ContactHandling(void) override{}
};

#endif