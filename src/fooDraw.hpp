#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

class FooDraw : public b2Draw {
public:
    FooDraw(sf::RenderWindow& window) : m_window(window) {}

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override {
        sf::ConvexShape polygon(vertexCount);
        for (int32 i = 0; i < vertexCount; ++i) {
            polygon.setPoint(i, sf::Vector2f(vertices[i].x, vertices[i].y));
        }
        polygon.setOutlineColor(ConvertColor(color));
        polygon.setOutlineThickness(1.0f);
        polygon.setFillColor(sf::Color::Transparent);
        std::cout << "drawPolygon" << std::endl;
        m_window.draw(polygon);
    }

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override {
        sf::ConvexShape polygon;
        polygon.setPointCount(vertexCount);

        for (int32 i = 0; i < vertexCount; ++i) {
            polygon.setPoint(i, sf::Vector2f(vertices[i].x * 30, vertices[i].y * 30));
        }

        polygon.setFillColor(sf::Color(
            static_cast<sf::Uint8>(color.r * 255),
            static_cast<sf::Uint8>(color.g * 255),
            static_cast<sf::Uint8>(color.b * 255),
            100
        ));

        polygon.setOutlineColor(sf::Color(
            static_cast<sf::Uint8>(color.r * 255),
            static_cast<sf::Uint8>(color.g * 255),
            static_cast<sf::Uint8>(color.b * 255)
        ));

        polygon.setOutlineThickness(-1.0f);

        m_window.draw(polygon);
    }

    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override {
        sf::CircleShape circle(radius);
        circle.setPosition(center.x - radius, center.y - radius);
        circle.setOutlineColor(ConvertColor(color));
        circle.setOutlineThickness(1.0f);
        circle.setFillColor(sf::Color::Transparent);
        m_window.draw(circle);
    }

    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override {
        sf::CircleShape circle(radius);
        circle.setPosition(center.x - radius, center.y - radius);
        circle.setFillColor(ConvertColor(color));
        m_window.draw(circle);

        // Draw the axis line
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(center.x, center.y), ConvertColor(color)),
            sf::Vertex(sf::Vector2f(center.x + axis.x * radius, center.y + axis.y * radius), ConvertColor(color))
        };
        m_window.draw(line, 2, sf::Lines);
    }

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(p1.x, p1.y), ConvertColor(color)),
            sf::Vertex(sf::Vector2f(p2.x, p2.y), ConvertColor(color))
        };
        m_window.draw(line, 2, sf::Lines);
    }

    void DrawTransform(const b2Transform& xf) override {
        const float lineLength = 0.4f;

        b2Vec2 p1 = xf.p;
        b2Vec2 p2 = p1 + lineLength * xf.q.GetXAxis();
        DrawSegment(p1, p2, b2Color(1, 0, 0)); // Red for the x-axis

        p2 = p1 + lineLength * xf.q.GetYAxis();
        DrawSegment(p1, p2, b2Color(0, 1, 0)); // Green for the y-axis
    }
    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
        sf::CircleShape point(size / 2);
        point.setPosition(p.x - size / 2, p.y - size / 2);
        point.setFillColor(ConvertColor(color));
        m_window.draw(point);
    }

private:
    sf::RenderWindow& m_window;

    sf::Color ConvertColor(const b2Color& color) {
        return sf::Color(static_cast<sf::Uint8>(color.r * 255),
                         static_cast<sf::Uint8>(color.g * 255),
                         static_cast<sf::Uint8>(color.b * 255));
    }
};
