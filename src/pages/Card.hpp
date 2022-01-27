#ifndef CARD_HPP
#define CARD_HPP

#include "../main.hpp"

class Card
{
public:
    enum State
    {
        ACTIVE, INACTIVE, HIDDEN
    };

    static 
    sf::Texture *original_state_sprites[3];

    static 
    void init()
    {
        original_state_sprites[ACTIVE] = Resources::texture("res/1x/card-active.png");
        original_state_sprites[INACTIVE] = Resources::texture("res/1x/card-inactive.png");
        original_state_sprites[HIDDEN] = Resources::texture("res/1x/card-hide.png");
    }

private:
    State sprite_state;
    sf::Sprite sprite;
    sf::Text label;
    bool label_visible = true;
    bool position_dirty = false;

public:

    static
    int fontSize()
    {
        return original_state_sprites[ACTIVE]->getSize().x / 8 * 5;
    }

    Card() : label("", Resources::font, fontSize())
    {
        setState(INACTIVE);
        setChar(' ');
        setCharColor(sf::Color::Black);

        label.setOutlineThickness(2);

        setPosition(0, 0);
    }

    char getChar() const
    {
        return label.getString()[0];
    }

    void setChar(char c)
    {
        label.setString(string() + c);
        position_dirty = true;
    }

    void setCharColor(sf::Color color)
    {
        label.setFillColor(color);
    }

    void setState(State state)
    {
        sprite_state = state;
        sprite.setTexture(*original_state_sprites[state]);
    }

    State getState()
    {
        return sprite_state;
    }

    void setLabelVisibility(bool visible)
    {
        label_visible = visible;
    }

    bool getLabelVisibility()
    {
        return label_visible;
    }

    sf::Sprite const &getSprite() const
    {
        return sprite;
    }

    void setPosition(float x, float y)
    {
        sprite.setPosition(x, y);
        int hcw = char_width(getChar(), label.getCharacterSize()) / 2; // half char width
        int sprite_hw = sprite.getLocalBounds().width / 2; // sprite half width
        label.setPosition(x + sprite_hw - hcw - 5, y + 10);

        position_dirty = false;
    }

    void setScale(float x, float y)
    {
        sprite.setScale(x, y);
        label.setScale(x, y);
    }

    void draw(sf::RenderWindow &window) const
    {
        if (position_dirty)
            cout << "Warning: Rendering cards with invalid internal data\n";

        window.draw(sprite);
        if (label_visible) 
        {
            window.draw(label);
        }
    }

private:
};

#endif