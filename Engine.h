
#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include <vector>

using namespace sf;
using namespace std;

class Engine
{
private:
    // A regular RenderWindow
    RenderWindow m_Window;
    //vector for Particles
    vector<Particle> m_particles;


    // Private functions for internal use only
    struct Rocket {
        Vector2f pos;
        Vector2f vel;
        float ttl = 0.0f;
        RectangleShape shape;
    };
    vector<Rocket> m_rockets;

    void input();
    void update(float dtAsSeconds);
    void draw();

public:
    // The Engine constructor
    Engine();
    // Run will call all the private functions
    void run();
};