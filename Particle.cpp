#include "Particle.h"
#include <cmath>
#include <cstdlib>
using namespace std;
using namespace sf;
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_A(2, numPoints), m_numPoints(numPoints), m_ttl(TTL)
{
    m_radiansPerSec = (static_cast<float>(rand()) / RAND_MAX) * M_PI;

    m_cartesianPlane.setCenter(0.0f, 0.0f);
    m_cartesianPlane.setSize(static_cast<float>(target.getSize().x), -1.0f * static_cast<float>(target.getSize().y));

    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

    m_vx = static_cast<float>((rand() % 401) + 100);
    if (rand() % 2 != 0) {
        m_vx *= -1.0f;
    }
    m_vy = static_cast<float>((rand() % 401) + 100);

    m_color1 = Color::White;
    m_color2 = Color(rand() % 256, rand() % 256, rand() % 256);

    float theta = (static_cast<float>(rand()) / RAND_MAX) * (M_PI / 2.0f);
    float dTheta = 2.0f * M_PI / (numPoints - 1);

    for (int j = 0; j < numPoints; ++j) {
        float r = static_cast<float>((rand() % 61) + 20);
        float dx = r * cos(theta);
        float dy = r * sin(theta);

        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;

        theta += dTheta;
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const {
    VertexArray lines(TriangleFan, m_numPoints + 1);






    Vector2i pixelCenterInt = target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);
    lines[0].position = static_cast<Vector2f>(pixelCenterInt);
    lines[0].color = m_color1;




    for (int j = 1; j <= m_numPoints; ++j) {
        Vector2f cartesianPt(static_cast<float>(m_A(0, j - 1)), static_cast<float>(m_A(1, j - 1)));
        Vector2i pixelPtInt = target.mapCoordsToPixel(cartesianPt, m_cartesianPlane);

        lines[j].position = static_cast<Vector2f>(pixelPtInt);
        lines[j].color = m_color2;
    }

    target.draw(lines, states);
}

void Particle::update(float dt) {
    m_ttl -= dt;

    rotate(dt * m_radiansPerSec);
    scale(SCALE);

    float dx = m_vx * dt;
    m_vy -= G * dt;
    float dy = m_vy * dt;

    translate(dx, dy);
}

void Particle::translate(double xShift, double yShift) {
    TranslationMatrix T(xShift, yShift, m_numPoints);
    m_A = T + m_A;

    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}

void Particle::rotate(double theta) {
    Vector2f temp = m_centerCoordinate;

    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    RotationMatrix R(theta);
    m_A = R * m_A;
    translate(temp.x, temp.y);
}

void Particle::scale(double c) {
    Vector2f temp = m_centerCoordinate;

    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    ScalingMatrix S(c);
    m_A = S * m_A;
    translate(temp.x, temp.y);
}

bool Particle::almostEqual(double a, double b, double eps) {
    return abs(a - b) < eps;
}

void Particle::unitTests() {
    int score = 0;
    cout << "Testing Particle Cartesian Mapping...";
    if (almostEqual(m_centerCoordinate.x, 0.0, 0.1) && almostEqual(m_centerCoordinate.y, 0.0, 0.1)) {
        cout << "PASSED" << endl;
        score++;
    }
    else {
        cout << "FAILED" << endl;
    }

   cout << "Testing Particle Rotation...";
    double headingBefore = atan2(m_A(1, 0) - m_centerCoordinate.y, m_A(0, 0) - m_centerCoordinate.x);
    rotate(M_PI / 2.0);
    double headingAfter = atan2(m_A(1, 0) - m_centerCoordinate.y, m_A(0, 0) - m_centerCoordinate.x);
    double diff = headingAfter - headingBefore;
    if (diff < 0) diff += 2.0 * M_PI;
    if (almostEqual(diff, M_PI / 2.0, 0.01)) {
        cout << "PASSED" << endl;
        score++;
    }
    else {
        cout << "FAILED" << endl;
    }

    cout << "Testing Particle Scaling...";
    double distBefore = hypot(m_A(0, 0) - m_centerCoordinate.x, m_A(1, 0) - m_centerCoordinate.y);
    scale(0.5);
    double distAfter = hypot(m_A(0, 0) - m_centerCoordinate.x, m_A(1, 0) - m_centerCoordinate.y);
    if (almostEqual(distAfter, distBefore * 0.5, 0.01)) {
        cout << "PASSED" << endl;
        score++;
    }
    else {
        cout << "FAILED" << endl;
    }

    cout << "Testing Particle Translation...";
    Vector2f centerBefore = m_centerCoordinate;
    translate(10.0, 5.0);
    if (almostEqual(m_centerCoordinate.x, centerBefore.x + 10.0, 0.01) && almostEqual(m_centerCoordinate.y, centerBefore.y + 5.0, 0.01)) {
        cout << "PASSED" << endl;
        score++;
    }
    else {
        cout << "FAILED" << endl;
    }

    cout << "Particle Unit Test Score: " << score << " / 4" << endl;
}