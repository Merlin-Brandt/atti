#ifndef MAINPAGE_HPP
#define MAINPAGE_HPP

#include "../main.hpp"

#include "Card.hpp"
#include "TrovaLaFrase.hpp"

namespace pages
{

class Main
{
public: 

    static
    void init()
    {
        Card::init();
        SecondsParticle::init();
    }

    void update(sf::RenderWindow &target, float time)
    {
        trovaLaFrase.update(target, time);
    }

    void draw(sf::RenderWindow &target, sf::RenderStates states = sf::RenderStates::Default) const
    {
        trovaLaFrase.render(target, states);
    }

private:
    pages::TrovaLaFrase trovaLaFrase;
};

}

#endif