
#define _USE_MATH_DEFINES
#define SCALE 30.0
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "map.hpp"
#include <math.h>

float UserLevelScore = 0;
float UserTotalScore = 0;

void Map::Draw(sf::RenderWindow& window){
    sf::CircleShape reference_circle;
	reference_circle.setOrigin(300,300);
	reference_circle.setRadius(300);
	reference_circle.setPosition(480,540);

    for(auto i : objects)
    {

        if(i->shape_==GameObject::Shape::circle)
        {

            i->circle_.setTexture(&i->texture_);
            i->circle_.setOrigin(i->size_x_, i->size_x_);
            i->circle_.setRotation(i->Body_->GetAngle()*180/M_PI);
            i->circle_.setPosition(i->Body_->GetPosition().x*SCALE, i->Body_->GetPosition().y*SCALE);
            window.draw(i->circle_);
        }
        else if(i->shape_==GameObject::Shape::rectangular)
        {

            i->rect_.setTexture(&i->texture_);
            i->rect_.setOrigin(i->size_x_/2, i->size_y_/2);
            i->rect_.setPosition(i->Body_->GetPosition().x*SCALE, i->Body_->GetPosition().y*SCALE);
            i->rect_.setRotation(i->Body_->GetAngle()*180/M_PI);

            window.draw(i->rect_);
        }

    }
    for(auto j : birds)
    {
        if(j->shape_==GameObject::Shape::circle)
        {
            if(j->Draw())
            {   
                j->circle_.setTexture(&j->texture_);
                j->circle_.setOrigin(j->size_x_, j->size_x_);
                j->circle_.setRotation(j->Body_->GetAngle()*180/M_PI);
                if (j->get_shooting()==true)
                {
                    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if(reference_circle.getGlobalBounds().contains(mouse.x,mouse.y))
                    {
                        j->circle_.setPosition(mouse.x,mouse.y);
                        j->Body_->SetTransform(b2Vec2(mouse.x/SCALE,mouse.y/SCALE),j->Body_->GetAngle());
                    }
                }
                else
                    j->circle_.setPosition(j->Body_->GetPosition().x*SCALE, j->Body_->GetPosition().y*SCALE);
                window.draw(j->circle_);
            }
        }
    }
    for(auto k : enemies)
    {
        if(k->shape_==GameObject::Shape::circle)
        {

            k->circle_.setTexture(&k->texture_);
            k->circle_.setOrigin(k->size_x_, k->size_x_);
            k->circle_.setRotation(k->Body_->GetAngle()*180/M_PI);
            k->circle_.setPosition(k->Body_->GetPosition().x*SCALE, k->Body_->GetPosition().y*SCALE);
            window.draw(k->circle_);
        }
    }
}

std::vector<std::string> toStringvector(std::string in)
{
    std::vector<std::string> out;
    size_t i = 0,j = 0;

    for (auto k : in)
    {
        if (k == ',')
        {
            out.push_back(in.substr(i,(j-i)));
            i = j+1;
        }

        j++;
    }

    out.push_back(in.substr(i,(j-i)));
    return(out);
}

void Map::ReadMap(b2World & m_world, std::string filename){
    
    GameObject* Platform = new Wood(m_world,500,620,200,100,0,false);
    Platform->Body_->SetActive(false);

    objects.push_back(Platform);
    std::ifstream f(filename);
    std::string buf;
    std::vector<std::string> buf_vec;
    while(std::getline(f,buf))
    {
        buf_vec = toStringvector(buf);
        switch(std::atoi(buf_vec[0].c_str()))
        {
            case(GameObject::Type::BasicBird):
                birds.push_back(new StandardBird(m_world, std::atoi(buf_vec[1].c_str()), std::atoi(buf_vec[2].c_str()), std::atoi(buf_vec[3].c_str())));
                break;
            case(GameObject::Type::SpeedBird):
                birds.push_back(new SpeedBird(m_world, std::atoi(buf_vec[1].c_str()), std::atoi(buf_vec[2].c_str()), std::atoi(buf_vec[3].c_str())));
                break;
            case(GameObject::Type::MonsterBird):
                birds.push_back(new MonsterBird(m_world, std::atoi(buf_vec[1].c_str()), std::atoi(buf_vec[2].c_str()), std::atoi(buf_vec[3].c_str())));
                break;
            case(GameObject::Type::BigBird):
                birds.push_back(new BigBird(m_world, std::atoi(buf_vec[1].c_str()), std::atoi(buf_vec[2].c_str()), std::atoi(buf_vec[3].c_str())));
                break;
            case(GameObject::Type::Wood):
                objects.push_back(new Wood(m_world, std::atoi(buf_vec[1].c_str()), std::atoi(buf_vec[2].c_str()), std::atoi(buf_vec[3].c_str()), std::atoi(buf_vec[4].c_str()), std::atoi(buf_vec[5].c_str())*(M_PI/180)));          
                break;
            case(GameObject::Type::Stone):
                objects.push_back(new Stone(m_world, std::atoi(buf_vec[1].c_str()), std::atoi(buf_vec[2].c_str()), std::atoi(buf_vec[3].c_str()), std::atoi(buf_vec[4].c_str()), std::atoi(buf_vec[5].c_str())*(M_PI/180)));          
                break;
            case(GameObject::Type::Ice):
                objects.push_back(new Ice(m_world, std::atoi(buf_vec[1].c_str()), std::atoi(buf_vec[2].c_str()), std::atoi(buf_vec[3].c_str()), std::atoi(buf_vec[4].c_str()), std::atoi(buf_vec[5].c_str())*(M_PI/180)));          
                break;
            case(GameObject::Type::Enemy):
                enemies.push_back(new Enemies(  m_world, 
                                                std::atof(buf_vec[1].c_str()),
                                                std::atof(buf_vec[2].c_str()),
                                                std::atof(buf_vec[3].c_str()),
                                                std::atof(buf_vec[4].c_str()),
                                                std::atof(buf_vec[5].c_str())*(M_PI/180)));          
                break;
            case(0):
                highscore.first = buf_vec[1].c_str();
                highscore.second = std::atoi(buf_vec[2].c_str());
                while(std::getline(f,buf))
                {
                    buf_vec = toStringvector(buf);
                    results.push_back(make_pair(buf_vec[0].c_str(),std::atoi(buf_vec[1].c_str())));
                }
        }       
    }

    //std::reverse(birds.begin(), birds.end());
}

void Map::Run()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "My window");
    window.setFramerateLimit(60);
  
    while (window.isOpen())
    {       
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if(phase==1)
        {
            RunGame(window);
        }
        else if(phase==0){
            RunMenu(window);
        }
    }   

}

void Map::RunGame(sf::RenderWindow &window)
{
    sf::Font font;   
    if (!font.loadFromFile("arial.ttf"))
    {
        // error...
    }
    int bird_count = 1;
    view.setCenter(sf::Vector2f(840.f, 360.f));
    view.setSize(sf::Vector2f(1280.f, 720.f));
    window.setView(view);
    sf::Texture texture;
    if (!texture.loadFromFile("../Images/background.png"))
    {
        // error...
    }
    // run the program as long as the window is open
    b2World *m_world = new b2World(b2Vec2(0.0f, 10.0f));
    ReadMap(*m_world, "../Maps/" + active_map_);
    birds_left_=birds.size();
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_staticBody; //this will be a static body
    myBodyDef.position = b2Vec2((2560/2)/SCALE, 695/SCALE); //set the starting position
    myBodyDef.angle = 0; //set the starting angle
    b2PolygonShape boxShape;
    boxShape.SetAsBox((2560/2)/SCALE,(50/2)/SCALE);
    b2Body* staticBody = m_world->CreateBody(&myBodyDef); //add body to world
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = 1;
    boxFixtureDef.friction = 100;
    staticBody->CreateFixture(&boxFixtureDef);

    sf::RectangleShape background(sf::Vector2f(2560.f, 1080.f));
    background.setTexture(&texture);
    //background.setTextureRect(sf::IntRect(0, 0, 2560, 1080));
    sf::RectangleShape ground(sf::Vector2f(2560.f, 50.f));
    ground.setFillColor(sf::Color(139,69,19));
    ground.setPosition(0.f, 670.f);
    sf::Clock clock;
    birds[0]->set_active();
    birds[0]->SetDraw();
    
    sf::Text Text1;
    Text1.setFont(font);
    Text1.setCharacterSize(20);
    Text1.setFillColor(sf::Color::White);
    Text1.setStyle(sf::Text::Bold);

    sf::Text Text2;
    Text2.setFont(font);
    Text2.setCharacterSize(120);
    Text2.setFillColor(sf::Color::White);
    Text2.setStyle(sf::Text::Bold);
    Text2.setString("You Win");
    Text2.setPosition(view.getCenter().x - 80,view.getCenter().y -10);

    sf::Text game_over_text;

    // select the font
    game_over_text.setFont(font); // font is a sf::Font


    // set the character size
    game_over_text.setCharacterSize(40); // in pixels, not points!

    // set the color
    game_over_text.setFillColor(sf::Color::Black);

    // set the text style
    game_over_text.setStyle(sf::Text::Bold);

    

    game_over_text.setString("\t\t\t\tGAME OVER :(\nRestart Level - R\tChoose Level - ESC");

    GameObjectContanctListener myContactListenerInstance;

    m_world->SetContactListener(&myContactListenerInstance);

    
    while (window.isOpen())
    {      
        Text1.setPosition(view.getCenter().x - 620,view.getCenter().y -350);
        // check all the window's events that were triggered since the last iteration of the loop
        window.clear(sf::Color::Black);
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {

                    for(auto i : birds)
                    {
                        delete i;
                    }
                    for(auto i : enemies)
                    {
                        delete i;
                    }
                    for(auto it : objects)
                    {
                        delete it;
                    }
                    if(win==true){
                        SaveScore("../Maps/" + active_map_);
                    }
                    staticBody->GetWorld()->DestroyBody(staticBody);
                    objects.clear();
                    birds.clear();
                    enemies.clear();
                    delete m_world;
                    phase = 0;
                    win==false;
                    TmpV.clear();
                    CallCount = 0;

                    UserTotalScore += UserLevelScore;
                    UserLevelScore = 0;
                    
                    return;
                }
                else if (event.key.code == sf::Keyboard::R)
                {
                    RestartLevel(m_world, staticBody);
                    return;
                }
                else if (event.key.code == sf::Keyboard::Right && view.getCenter().x < 1900.f)
                {
                    view.move(10.f, 0.f);
                }
                else if (event.key.code == sf::Keyboard::Left && view.getCenter().x > 640.f)
                {
                    view.move(-10.f, 0.f);
                }
            }
        }
		// Step the physics

		float32 timeStep = 1/60.0; // timeStep length to simulate in seconds (1/60 seconds)
		int32 velocityIterations = 9; // bigger number is more accurate, but takes more CPU
		int32 positionIterations = 4; // bigger number is more accurate, but takes more CPU

		m_world->Step(timeStep, velocityIterations, positionIterations);
        
        window.draw(background);
        window.draw(ground);
        Draw(window);


        Text1.setString("Highscore: "+ highscore.first +" : " + to_string(highscore.second)+"\nEnemies left: " + to_string(enemies.size())
        +"\nBirds left: " + to_string(birds_left_)+"\nYour score: "+ to_string(UserLevelScore));
        window.draw(Text1);

        game_over_text.setPosition(view.getCenter().x - 310,view.getCenter().y);
        if( (birds.size() == 1) &&
            ( ((*(birds.begin()))->get_state()) == false ) &&
             !(enemies.empty()) )
        {
            window.draw(game_over_text);
        }
    
        for (auto i : birds)
        {
            if(i->get_state()==true && i->circle_.getPosition().x> view.getCenter().x && i->circle_.getPosition().x < 1900.f) 
            {
                view.setCenter(sf::Vector2f(i->circle_.getPosition().x, 360.f));
            }
        }
        
        //This condition checks if you click on a bird
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
        {
            sf::Vector2f mouse = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y));
            
            for(auto i : birds)
            {
                if((i->circle_).getGlobalBounds().contains(mouse.x,mouse.y) && i->get_state() == true)
                {
                    std::cout << "Click on bird boundaries | Shooting: " << i->get_shooting() << std::endl;
                    i->set_shooting();
                }
            }
        }

        //This condition checks if the bird is thrown, if that is the case, then a force is applied on it
        if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
          
            for(auto i : birds)
            {
                if(i->get_shooting()==true)
                {
                    sf::Vector2f bird_location = i->circle_.getPosition();
                    std::cout << "Shooting .... | x: " << bird_location.x << " | y: " << bird_location.y << std::endl;
                    i->Body_->SetActive(true);
                    i->ApplyForce(bird_location.x,bird_location.y);
                    i->reset_shooting();
                    i->set_feature();
                    birds_left_ -= 1;
                    if(KeepTrackOnBirds(i))
                    {
                        m_world->SetContactListener(NULL);
                        birds.erase( birds.begin() );
                        m_world->SetContactListener(&myContactListenerInstance);
                        bird_count--;
                    }

                    birds[bird_count]->set_active();
                    birds[bird_count]->SetDraw();
                    Draw(window);

                    if(bird_count < birds.size() - 1)
                    {
                        bird_count += 1;
                    }
                }
                
            }
        }

        //This condition checks if a feature is applied to a bird and if that is the case applies the feature
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            for(auto i : birds)
            {
                if(i->get_feature() == 1 && i->get_state() == true)
                {
                    std::cout << "Apply feature!" << std::endl;
                    i->Object_Feature();
                    i->reset_feature();
                    i->set_inactive();

                    break;
                }
            }
        }
        if (enemies.empty())
        {
            win = true;
            for(auto it : birds)
            {
                it->set_inactive();
            }
            clock.restart();
        }
        //sf::Time time = clock.getElapsedTime();
        if(win == true)
        {
            window.draw(Text2);
        }

        /* Disabling contact listener to avoid crashes */
        m_world->SetContactListener(NULL);
        RemoveDeadObjects(m_world);
        /* Re-enabling the contact listener */
        m_world->SetContactListener(&myContactListenerInstance);

        window.setView(view);
        window.display();
    }


}


sf::RectangleShape Map::LevelObjects(int i)
{
    if (!one.loadFromFile("../Images/1.png"))
    {
        // error...
        cout << "Ouh" << endl;
    }
    if (!two.loadFromFile("../Images/2.png"))
    {
        // error...
        cout << "Ouh" << endl;
    }
    if (!three.loadFromFile("../Images/3.png"))
    {
        // error...
        cout << "Ouh" << endl;
    }
    sf::RectangleShape level(sf::Vector2f(80.f, 80.f));
    if(i == 1)
    {
        level.setTexture(&one);
    }
    if(i == 2)
    {
        level.setTexture(&two);
    }
    if(i == 3)
    {
        level.setTexture(&three);
    }
    level.setPosition(100.f,i*150);
    return level;
}

void Map::Read_Dir(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
          return;
    }
    int i = 1;
    while ((entry = readdir(dir)) != NULL) 
    {
        if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){
            sf::RectangleShape level = LevelObjects(i);
            cout << "Map name: "<<entry->d_name << endl;
            maps.push_back(make_pair(entry->d_name, level));
            i++;
        }
    }
    closedir(dir);
}

void Map::RunMenu(sf::RenderWindow& window){
    sf::Font font;
    sf::Text playerText;
    //sf::String playerInput;
    if (!font.loadFromFile("../Fonts/arial.ttf"))
    {
        // error
    }
    playerText.setFont(font);
    playerText.setCharacterSize(24);
    playerText.setFillColor(sf::Color::Red);
    playerText.setStyle(sf::Text::Bold);
    
    window.clear(sf::Color::Black);
    view.setSize(sf::Vector2f(1280.f, 720.f));
    view.setCenter(sf::Vector2f(640.f, 360.f));
    window.setView(view);
    sf::Texture texture;
    if (!texture.loadFromFile("../Images/new_menu_bg.png"))
    {
        // error...
    }
    sf::RectangleShape background(sf::Vector2f(1280.f, 720.f));
    background.setTexture(&texture);
    window.draw(background);
    if(menu_phase_==1)
    {
        playerText.setPosition(0,0);
        playerText.setString(playertext_);
        window.draw(playerText);

        sf::Text instructions;

        // select the font
        instructions.setFont(font); // font is a sf::Font


        // set the character size
        instructions.setCharacterSize(25); // in pixels, not points!

        // set the color
        instructions.setFillColor(sf::Color::Black);

        // set the text style
        instructions.setStyle(sf::Text::Bold | sf::Text::Underlined);

        instructions.setPosition(30,600);

        // set the string to display
        instructions.setString("Instructions:");
        
        instructions.setString("\nRestart Level - R\tChoose Level - ESC\tChange player - Tab\tMove view - left/right arrow");

        instructions.setStyle(sf::Text::Bold);

        window.draw(instructions);

        for(auto i:maps)
        {
            window.draw(i.second);
        }
        
        for (auto it : maps)
        {
            sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if((it.second).getGlobalBounds().contains(mouse.x,mouse.y))
            {
                it.second.setOutlineThickness(10.f);
                it.second.setOutlineColor(sf::Color(0, 0, 0));
                
            }
            else
            {
                it.second.setOutlineThickness(10.f);
                it.second.setOutlineColor(sf::Color::Transparent);
            }
            window.draw(it.second);

        }
    }
    if(menu_phase_==0)
    {
        sf::Text Text;
        playerText.setPosition(500,300);
        if (!font.loadFromFile("arial.ttf"))
        {
            // error...
        }
        Text.setFont(font);
        Text.setCharacterSize(24);
        Text.setFillColor(sf::Color::White);
        Text.setStyle(sf::Text::Bold);
        Text.setString("Enter username (max 12 characters) and press enter:");
        Text.setPosition(300,250);
        window.draw(Text);
        
        sf::RectangleShape squere(sf::Vector2f(180.f, 40.f));
        squere.setFillColor(sf::Color::White);
        squere.setPosition(500,290);
        window.draw(squere);

        playerText.setString(playertext_);
        window.draw(playerText);
        

    }
    sf::Event event;
    while (window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();
        
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouse =window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x,event.mouseButton.y));
            for (auto it : maps)
            {
                if((it.second).getGlobalBounds().contains(mouse.x,mouse.y))
                {
                    active_map_ = it.first;
                    phase = 1;
                    break;
                }
            }
        }

        if (event.type == sf::Event::TextEntered && menu_phase_==0)
        {
            //playerInput +=event.text.unicode;           
            if(event.text.unicode < 127 && event.text.unicode > 31 && playertext_.size()<12){
                playertext_.push_back((char)event.text.unicode);
            }
            
    
            //cout<<playertext_<<endl;
            
        } 
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Return && menu_phase_==0 && playertext_.size()>=1)
            {
                menu_phase_=1;
            }

            if (event.key.code == sf::Keyboard::BackSpace && menu_phase_==0)
            {
                playertext_.pop_back();
            }
            if (event.key.code == sf::Keyboard::Tab && menu_phase_==1)
            {
                playertext_.erase();
                menu_phase_=0;
            }
        }
    }
    
    window.display();
}
void Map::SaveScore(std::string filename)
{
    if(UserLevelScore>highscore.second){
        highscore.first = playertext_;
        highscore.second = UserLevelScore;
    }
    auto t = find_if(results.begin(), results.end(),
            [=](const pair<string,float>& a)
            { return playertext_==a.first; });    
    if(t!=results.end() && UserLevelScore > t->second)
    {
        t->second = UserLevelScore;
    }
    else if(t==results.end())
    {
        results.push_back(make_pair(playertext_,UserLevelScore));
    }
    std::ifstream f(filename);
    std::string buf;
    std::vector<std::string> buf_vec;
    std::vector<std::string> vec;
    while(std::getline(f,buf))
    {
        buf_vec = toStringvector(buf);
        if(std::atoi(buf_vec[0].c_str())==0){
            break;
        }    
        else
        {
            vec.push_back(buf);
        }
    }
    f.close();
    vec.push_back("0,"+highscore.first+","+to_string(highscore.second));
    for(auto it : results)
    {
        vec.push_back(it.first+","+to_string(it.second));
    }
    std::ofstream output(filename);
    for(auto i: vec)
        output << i << std::endl;
    output.close();
    results.clear();

}

bool Map::KeepTrackOnBirds(BirdsBaseClass* Bird)
{
    bool RetVal = false;

    TmpV.push_back(Bird);

    if( CallCount > 0  )
    {
        delete (*(TmpV.begin()));
        TmpV.erase( TmpV.begin() );
        RetVal = true;
    }

    CallCount++;

    return RetVal;
}

void Map::RestartLevel(b2World *m_world, b2Body* staticBody)
{
    for(auto i : birds)
    {
        delete i;
    }
    for(auto i : enemies)
    {
        delete i;
    }
    for(auto it : objects)
    {
        delete it;
    }
    if(win==true){
        SaveScore("../Maps/" + active_map_);
    }
    staticBody->GetWorld()->DestroyBody(staticBody);
    objects.clear();
    birds.clear();
    enemies.clear();
    win==false;
    
    TmpV.clear();
    CallCount = 0;

    UserLevelScore = 0;

    delete m_world;
    phase = 1;
    return;
}

void Map::RemoveDeadObjects(b2World *m_world)
{
    for(std::vector<Enemies*>::const_iterator it = enemies.begin(); it != enemies.end(); it++)
    {
        if( (*it)->DestroyEnemy() )
        {   
            delete (*it);
            it = enemies.erase(it);
            break;
            
        }
    }

    for(std::vector<GameObject*>::const_iterator it = objects.begin(); it != objects.end(); it++)
    {
        if( (*it)->Destroy() )
        {
            delete (*it);
            it = objects.erase(it);
            break;
            
        }
    }
}