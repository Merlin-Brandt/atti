#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../main.hpp"

class ConstructButton
{
    friend class Button;

    sf::IntRect bounds_buf;
    sf::IntRect *bounds_ptr;
    sf::Drawable *state_sprites[4];
    sf::Drawable *label_ptr;
    bool is_relative = false;

public:
    ConstructButton *bounds(sf::IntRect const &bounds);

    ConstructButton *mouse_over(sf::Drawable *d);

    ConstructButton *mouse_down(sf::Drawable *d);

    ConstructButton *no_mouse(sf::Drawable *d);

    ConstructButton *disabled(sf::Drawable *d);

    ConstructButton *label(sf::Drawable *label);

    [[deprecated]]
    ConstructButton *relative();

    class Button *construct();
};

class Button
{
public:
    enum State
    {
        MOUSE_OVER, MOUSE_DOWN, NO_MOUSE, DISABLED
    };

    sf::IntRect bounds;
    sf::Drawable *state_sprites[4];
    sf::Drawable *label;
    bool disabled = false;
    ConstructButton *construct;

    bool repress_click_event = false;

    static
    ConstructButton *with_()
    {
        return new ConstructButton();
    }

    Button() {}

    Button(ConstructButton *construct)
    {
        this->construct = construct;

        if (!construct->bounds_ptr || !construct->state_sprites[NO_MOUSE])
        {
            std::cout << "Error constructing Button\n";
        }

        /*if (construct->is_relative)
        {
            for (int i = 0; i < 4; ++i) if (state_sprites[i])
                ((sf::Text *)state_sprites[i])->move(bounds.left, bounds.top);
            if (label)
                ((sf::Text *)label)->move(bounds.left, bounds.top);
        }*/
        
        if (!construct->label_ptr)
            construct->label_ptr = new sf::Sprite();
        if (!construct->state_sprites[DISABLED])
            construct->state_sprites[DISABLED] = construct->state_sprites[NO_MOUSE];
        if (!construct->state_sprites[MOUSE_OVER])
            construct->state_sprites[MOUSE_OVER] = construct->state_sprites[NO_MOUSE];
        if (!construct->state_sprites[MOUSE_DOWN])
            construct->state_sprites[MOUSE_DOWN] = construct->state_sprites[NO_MOUSE];

        bounds = *construct->bounds_ptr;
        for (int i = 0; i < 4; ++i)
            state_sprites[i] = construct->state_sprites[i];
        label = construct->label_ptr;
        disabled = false;
    }

    virtual ~Button()
    {
        delete state_sprites[0];
        delete state_sprites[1];
        delete state_sprites[2];
        delete label;
        delete construct;
    }

    void disable(sf::Drawable *disabled_sprite)
    {
        state_sprites[DISABLED] = disabled_sprite;
        disable();
    }

    void disable()
    {
        disabled = true;
    }

    void enable()
    {
        disabled = false;
    }

    State getState(sf::RenderWindow &window)
    {
        if (disabled)
            return DISABLED;
        else
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
    }

    void draw(sf::RenderWindow &target, sf::RenderStates states)
    {
        if (state_sprites[getState(target)])
            target.draw(*state_sprites[getState(target)]);
        if (label)
            target.draw(*label);
    }

    bool click_event(sf::RenderWindow &window)
    {
        if (!disabled)
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
        else 
        {
            return false;
        }
    }
};

#endif