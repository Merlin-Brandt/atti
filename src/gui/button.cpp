
#include "../main.hpp"
#include "button.hpp"

ConstructButton *ConstructButton::bounds(sf::IntRect const &bounds)
{
    this->bounds_buf = bounds;
    this->bounds_ptr = &this->bounds_buf;
    return this;
}

ConstructButton *ConstructButton::mouse_over(sf::Drawable *d)
{
    state_sprites[Button::State::MOUSE_OVER] = d;
    return this;
}

ConstructButton *ConstructButton::mouse_down(sf::Drawable *d)
{
    state_sprites[Button::State::MOUSE_DOWN] = d;
    return this;
}

ConstructButton *ConstructButton::no_mouse(sf::Drawable *d)
{
    state_sprites[Button::State::NO_MOUSE] = d;
    return this;
}

ConstructButton *ConstructButton::disabled(sf::Drawable *d)
{
    state_sprites[Button::State::DISABLED] = d;
    return this;
}

ConstructButton *ConstructButton::label(sf::Drawable *label)
{
    this->label_ptr = label;
    return this;
}

ConstructButton *ConstructButton::relative()
{
    this->is_relative = true;
    return this;
}

Button *ConstructButton::construct()
{
    return new Button(this);
}