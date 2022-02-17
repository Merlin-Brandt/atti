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
    sf::Texture *original_state_textures[3];

    static 
    void init()
    {
        original_state_textures[ACTIVE] = Resources::texture("res/img/1x/card-active.png");
        original_state_textures[INACTIVE] = Resources::texture("res/img/1x/card-inactive.png");
        original_state_textures[HIDDEN] = Resources::texture("res/img/1x/card-hide.png");

        bool same_dimensions = true;
        for (int i = 0; i < 2; ++i)
            if (abs(int(original_state_textures[i]->getSize().x - original_state_textures[i+1]->getSize().x)) > 5)
                same_dimensions = false;
        if (!same_dimensions)
            std::cout << "Error: The resources card-active.png, card-inactive.png and card-hide.png do not have identical dimensions. This will lead to layout issues.";
    }

private:
    sf::Text label;
    bool shown = false;
    sf::Sprite sprite;
public:

    static
    int fontSize()
    {
        return original_state_textures[ACTIVE]->getSize().x / 8 * 5;
    }

    Card() : label("", Resources::font, fontSize())
    {
        setChar(' ');
        hide();
        setCharColor(sf::Color::Black);

        label.setOutlineThickness(2);

        setPosition(0, 0);
    }

    // logic state

    char getChar() const
    {
        return label.getString()[0];
    }

    State getState() const
    {
        char ch = getChar();
        if (ch == ' ')
            return State::INACTIVE;
        else if (shown)
            return State::ACTIVE;
        else
            return State::HIDDEN;
    }

    void setChar(char c)
    {
        label.setString(string() + c);
        sprite.setTexture(*original_state_textures[getState()]);
        adjust_label_pos();
    }

    void hide()
    {
        //sprite_state = State::HIDDEN;
        shown = false;
        sprite.setTexture(*original_state_textures[getState()]);
    }

    void show()
    {
        //sprite_state = State::ACTIVE;
        shown = true;
        sprite.setTexture(*original_state_textures[getState()]);
    }

    bool isShown() const
    {
        return getState() == State::ACTIVE;
    }

    void setCharColor(sf::Color color)
    {
        label.setFillColor(color);
    }

    sf::Sprite const &getSprite() const
    {
        return sprite;
    }

    // graphics state

    void setPosition(float x, float y)
    {
        sprite.setPosition(x, y);
        adjust_label_pos();
    }

    // depends on sprite.getPosition() and on getChar() and on label.getCharacterSize()
    void adjust_label_pos()
    {
        float x = sprite.getPosition().x;
        float y = sprite.getPosition().y;
        int hcw = char_width(getChar(), label.getCharacterSize()) / 2; // half char width
        int sprite_hw = sprite.getLocalBounds().width / 2; // sprite half width
        label.setPosition(x + sprite_hw - hcw - 15, y + 7);
    }

    void setScale(float x, float y)
    {
        sprite.setScale(x, y);
        label.setScale(x, y);
    }

    void draw(sf::RenderWindow &window) const
    {
        window.draw(sprite);
        if (isShown()) 
        {
            window.draw(label);
        }
    }
};

#endif