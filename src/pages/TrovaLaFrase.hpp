#ifndef TROVA_LA_FRASE_HPP
#define TROVA_LA_FRASE_HPP

#include <set>

#include "Card.hpp"
#include "../gui/button.hpp"
#include "SecondsParticle.hpp"
#include "../Animation.hpp"

namespace pages
{

class TrovaLaFrase
{
public:
    static
    constexpr int letters_n = 26;

    class Main *mainpage;
    Card cards[BOARD_W][BOARD_H];
    Button *letters[letters_n];
    sf::Sprite player_frame;
    sf::Text player_frame_note, player_frame_name, player_frame_time;
    Button play_button, pause_button, *play_or_pause = &pause_button;
    Button next_player_button;
    Button remove_time;
    float debug_last_click = 0.0f;

    set<char> guessed_letters;

    vector<function<void(sf::RenderWindow &, float)>> update_callbacks;
    function<void(void)> next_player;

    std::vector<SecondsParticle> particles;
    Animation letters_anim;

    set<char> const vocals{'A', 'E', 'I', 'O', 'U'};

    TrovaLaFrase(Main *a_mainpage);

    virtual ~TrovaLaFrase()
    {
        for (int i = 0; i < letters_n; ++i)
        {
            delete letters[i];
        }
    }

    void changetime(float time, int particleX, int particleY, bool);
    void letterClicked(char c);
    void update(sf::RenderWindow &target, float time);

    void render(sf::RenderWindow &target, sf::RenderStates states) const
    {
        for (int i = 0; i < BOARD_W; ++i)
        for (int j = 0; j < BOARD_H; ++j)
        {
            cards[i][j].draw(target);
        }

        for (int i = 0; i < letters_n; ++i)
        {
            letters[i]->draw(target, states);
        }

        target.draw(player_frame);
        target.draw(player_frame_note);
        target.draw(player_frame_name);
        target.draw(player_frame_time);
        play_or_pause->draw(target, states);
        ((Button *)&next_player_button)->draw(target, states);
        ((Button *)&remove_time)->draw(target, states);

        for (SecondsParticle const &p : particles)
        {
            p.render(target);
        }
    }
};

}

#endif