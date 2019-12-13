#ifndef BIRDS_BASE_CLASS
#define BIRDS_BASE_CLASS
#include "gameobject.hpp"

//This is the base clase for the birds that inherits from GameObject
class BirdsBaseClass : public GameObject
{

	public:
		BirdsBaseClass(b2World& world,float x, float y,float size_x,float size_y, float angle, float bounce, float density = 0.8,float friction = 0.7)
			: GameObject(world, x, y, size_x, size_y, angle, bounce, density, friction, (GameObject::Shape) circle, false, true) 
			{}
		
		virtual ~BirdsBaseClass() {std::cout << "BirdsBaseClass destructor" << std::endl;} //Destructor
		//These are the methods to control the actions of the birds
		void set_active(){is_active_ = true;}
		virtual void set_inactive() override {is_active_ = false;}
		bool get_state(){return is_active_;}
		bool get_shooting(){return shooting_;}
		void set_shooting(){shooting_=true;}
		void reset_shooting(){shooting_=false;}
		bool get_feature(){return apply_feature_;}
		void set_feature(){apply_feature_=true;}
		void reset_feature(){apply_feature_=false;}
		virtual void ContactHandling(void) override{}
		bool Draw(void) const { return draw_; }
		void SetDraw(void){ draw_ = true; }
		void ClearDraw(void){ draw_ = false; }
		
	protected:
		bool draw_ = false;
		bool is_active_ = false;
		bool shooting_ = false;
		bool apply_feature_ = false;
};


#endif

