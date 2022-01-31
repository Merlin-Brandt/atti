#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../main.hpp"

class Button
{
public:
    enum State
    {
        MOUSE_OVER, MOUSE_DOWN, NO_MOUSE
    };

    sf::IntRect bounds;
    sf::Drawable *state_sprites[3];
    sf::Drawable *label;

    bool repress_click_event = false;

    Button(sf::IntRect abounds, sf::Drawable *no_mouse, sf::Drawable *mouse_over, sf::Drawable *mouse_down, sf::Drawable *alabel)
    {
        bounds = abounds;
        state_sprites[NO_MOUSE] = no_mouse;
        state_sprites[MOUSE_DOWN] = mouse_down;
        state_sprites[MOUSE_OVER] = mouse_over;
        label = alabel;
    }

    virtual ~Button()
    {
        delete state_sprites[0];
        delete state_sprites[1];
        delete state_sprites[2];
        delete label;
    }

    State getState(sf::RenderWindow &window)
    {
        auto mousePos = (sf::Mouse::getPosition(window));
        if (bounds.contains(mousePos))
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                return MOUSE_DOWN;
            }   
            else
            {
                return MOUSE_OVER;
            }
        }
        return NO_MOUSE;
    }

    void draw(sf::RenderWindow &target, sf::RenderStates states)
    {
        target.draw(*state_sprites[getState(target)]);
        target.draw(*label);
    }

    bool click_event(sf::RenderWindow &window)
    {
        State state = getState(window);

        if (state != MOUSE_DOWN)
            repress_click_event = false;

        if (repress_click_event)
        {
            return false;
        }
        else
        {
            if (state == MOUSE_DOWN)
                repress_click_event = true;
                
            return state == MOUSE_DOWN;
        }
    }
};

#endif