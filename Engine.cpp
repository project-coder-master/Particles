#include "Engine.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;

Engine::Engine() {
    m_Window.create(VideoMode::getDesktopMode(), "Polymorphic Matrix Particles");
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Engine::run() {
    Clock clock;

    cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete.  Starting engine..." << endl;

    while (m_Window.isOpen()) {
        float dtAsSeconds = clock.restart().asSeconds();

        input();
        update(dtAsSeconds);
        draw();
    }
}

void Engine::input() {
    Event event;
    while (m_Window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            m_Window.close();
        }
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
            m_Window.close();
        }

        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i clickPos(event.mouseButton.x, event.mouseButton.y);

            for (int i = 0; i < 5; ++i) {
                int numPoints = (rand() % 26) + 25;
                m_particles.push_back(Particle(m_Window, numPoints, clickPos));
            }
        }

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::F) {
            Rocket r;
            float screenX = m_Window.getSize().x;
            float screenY = m_Window.getSize().y;

            float randomX = static_cast<float>(rand() % static_cast<int>(screenX));
            r.pos = Vector2f(randomX, screenY);

            float randomXVel = static_cast<float>((rand() % 200) - 100);
            r.vel = Vector2f(randomXVel, -600.0f - (rand() % 200));

            r.ttl = 0.8f + (static_cast<float>(rand() % 100) / 100.0f);

            r.shape.setSize(Vector2f(8.0f, 25.0f));
            r.shape.setFillColor(Color(255, 140, 0));
            r.shape.setOrigin(4.0f, 12.5f);

            m_rockets.push_back(r);
        }
    }
}

void Engine::update(float dtAsSeconds) {
    for (auto it = m_particles.begin(); it != m_particles.end(); ) {
        if (it->getTTL() > 0.0f) {
            it->update(dtAsSeconds);
            ++it;
        }
        else {
            it = m_particles.erase(it);
        }
    }

    for (auto it = m_rockets.begin(); it != m_rockets.end(); ) {
        it->ttl -= dtAsSeconds;
        it->pos += it->vel * dtAsSeconds;
        it->shape.setPosition(it->pos);

        if (it->ttl <= 0.0f) {
            Vector2i explodePoint(static_cast<int>(it->pos.x), static_cast<int>(it->pos.y));
            for (int i = 0; i < 30; ++i) {
                int points = (rand() % 26) + 25;
                m_particles.push_back(Particle(m_Window, points, explodePoint));
            }
            it = m_rockets.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Engine::draw() {
    m_Window.clear();

    for (const auto& particle : m_particles) {
        m_Window.draw(particle);
    }

    for (const auto& rocket : m_rockets) {
        m_Window.draw(rocket.shape);
    }

    m_Window.display();
}