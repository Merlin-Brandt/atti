#ifndef SECONDS_PARTICLE_HPP
#define SECONDS_PARTICLE_HPP

#include "../main.hpp"

class SecondsParticle
{
public:
    static
    sf::Texture *time_minus_10s, *time_plus_10s;

    static
    void init()
    {
        time_minus_10s = Resources::texture("res/1x/-10s.png");
        time_plus_10s = Resources::texture("res/1x/+10s.png");
    }

    sf::Sprite sprite;
    float motion = 100.0f;
    float fade = 350.0f;
    bool invisible = false;

    SecondsParticle(sf::Texture *texture, int x, int y)
    {
        sprite.setTexture(*texture);
        sprite.setPosition(x, y);
    }

    void animate(float time)
    {
        sprite.move(0, -time * motion);
        sf::Color color = sprite.getColor();
        int new_a = int(color.a) - int(time * fade);
        if (new_a >= 0)
            color.a = new_a;
        else
            invisible = true, color.a = 0;
        sprite.setColor(color);
    }   

    void render(sf::RenderWindow &target) const
    {
        target.draw(sprite);
    }
};

#endif