#ifndef TROVA_LA_FRASE_HPP
#define TROVA_LA_FRASE_HPP

#include <set>

#include "Card.hpp"
#include "../gui/button.hpp"
#include "SecondsParticle.hpp"

namespace pages
{

class TrovaLaFrase
{
public:
    static
    constexpr int cards_w = 14, cards_h = 7, letters_n = 26;
    Card cards[cards_w][cards_h];
    Button *letters[letters_n];
    std::vector<SecondsParticle> particles;

    TrovaLaFrase()
    {
        cards[1][1].setChar('H');
        cards[2][1].setChar('E');
        cards[3][1].setChar('L');
        cards[4][1].setChar('L');
        cards[5][1].setChar('O');
        cards[1][1].setState(Card::HIDDEN);
        cards[2][1].setState(Card::HIDDEN);
        cards[3][1].setState(Card::HIDDEN);
        cards[4][1].setState(Card::HIDDEN);
        cards[5][1].setState(Card::HIDDEN);

        float cards_x_offset = 80.f;
        float cards_y_offset = 40.f;
        float gap = -7;
        sf::Vector2u card_size = Card::original_state_sprites[Card::ACTIVE]->getSize();
        for (int i = 0; i < cards_w; ++i)
        for (int j = 0; j < cards_h; ++j)
        {
            cards[i][j].setPosition(cards_x_offset + (card_size.x+gap) * i , cards_y_offset + (card_size.y+gap) * j);
            cards[i][j].setLabelVisibility(false);
        }

        int cards_bottom = cards_y_offset + (card_size.y+gap) * cards_h; 

        int vocals_x = 0;
        int vocals_y = 20;
        int vocals_stride = 80;
        int consonants_x = 50;
        int consonants_y = cards_bottom + 30;
        int consonants_stride = 50;
        int font_size = 45;
        set<char> vocals({'A', 'E', 'I', 'O', 'U'});
        int vocal_i = 0, consonant_i = 0;
        for (int i = 0; i < letters_n; ++i)
        {
            char c = (char) ('A' + i);
            int hcw = font_size - char_width(c, font_size) / 2; // correction with half char width
            sf::IntRect bounds;
            if (vocals.find(c) != vocals.end())
            {
                ++vocal_i;
                bounds = sf::IntRect(vocals_x + hcw, vocals_y + vocals_stride * vocal_i, vocals_stride, vocals_stride);
            }
            else
            {  
                ++consonant_i;
                bounds = sf::IntRect(consonants_x + hcw + consonants_stride * consonant_i, consonants_y, consonants_stride, consonants_stride);
            }

            sf::Text *letter_sprite = new sf::Text(string() + c, Resources::font, font_size);
            letter_sprite->setPosition(bounds.left, bounds.top);
            letter_sprite->setOutlineThickness(3);
            sf::Text *letter_mouse_over = new sf::Text(*letter_sprite);
            sf::Text *letter_mouse_down = new sf::Text(*letter_sprite);
            letter_mouse_over->setFillColor(sf::Color(0xf5, 0xdd, 0x07));
            letter_mouse_down->setFillColor(sf::Color(255, 255, 255));
            sf::Sprite *empty = new sf::Sprite();

            letters[i] = new Button(bounds, letter_sprite, letter_mouse_over, letter_mouse_down, empty);
        }
    }

    virtual ~TrovaLaFrase()
    {
        for (int i = 0; i < letters_n; ++i)
        {
            delete letters[i];
        }
    }

    void update(sf::RenderWindow &target, float time)
    {
        for (int i = 0; i < letters_n; ++i)
        {
            if (letters[i]->click_event(target))
                letterClicked('A' + i);
        }

        for (SecondsParticle &p : particles)
        {
            p.animate(time);
        }

        // remove invisible particles
        for (int i = 0; i < particles.size(); ++i)
            if (particles[i].invisible)
            {
                particles[i] = particles[particles.size() - 1];
                particles.erase(particles.end() - 1); 
            }
    }

    void letterClicked(char c)
    {
        bool correct = false;
        bool already_guessed = false;
        for (int i = 0; i < cards_w; ++i)
        for (int j = 0; j < cards_h; ++j)
        {
            cards[i][j].setCharColor(sf::Color(0, 0, 0));
            if (cards[i][j].getChar() == c)
            {
                if (cards[i][j].getLabelVisibility())
                    already_guessed = true;
                else
                {
                    cards[i][j].setCharColor(sf::Color(0xf5, 0xdd, 0x07));
                    cards[i][j].setLabelVisibility(true);
                    cards[i][j].setState(Card::State::ACTIVE);
                    correct = true;
                }
            }
        }
        

        // todo: what happens if player guesses an already guessed letter?
        // for now, the algorithm above will run as though a wrong letter was guessed.

        auto letter_bounds = letters[c - 'A']->bounds;
        particles.push_back
        (
            SecondsParticle
            (
                correct ? SecondsParticle::time_plus_10s : SecondsParticle::time_minus_10s, 
                letter_bounds.left, 
                letter_bounds.top - SecondsParticle::time_minus_10s->getSize().y + 30
            )
        );
    }

    void render(sf::RenderWindow &target, sf::RenderStates states) const
    {
        for (int i = 0; i < cards_w; ++i)
        for (int j = 0; j < cards_h; ++j)
        {
            cards[i][j].draw(target);
        }

        for (int i = 0; i < letters_n; ++i)
        {
            letters[i]->draw(target, states);
        }

        for (SecondsParticle const &p : particles)
        {
            p.render(target);
        }
    }
};

}

#endif