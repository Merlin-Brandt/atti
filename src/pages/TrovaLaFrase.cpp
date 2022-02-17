#include <set>

#include "../main.hpp"
#include "MainPage.hpp"
#include "TrovaLaFrase.hpp"
#include "Card.hpp"
#include "../gui/button.hpp"
#include "SecondsParticle.hpp"

using namespace pages;

TrovaLaFrase::TrovaLaFrase(Main *a_mainpage) : mainpage(a_mainpage)
{
    {
        int *i = new int(0);
        int *j = new int(0);
        letters_anim = Animation::steps(0.1, [=]
        {
            char ch; do
            {
                ch = mainpage->params.board[*i][*j];
                cards[*i][*j].setChar(ch);
                
                if (!isalpha(ch))
                    cards[*i][*j].show();
                    
                ++*i;
                if (*i >= BOARD_W)
                    *i = 0, ++*j;
                if (*j >= BOARD_H)
                {
                    delete i;
                    delete j;
                    return false;
                }
            }
            while (ch == ' ');
            
            return true;
        });
    }

    float cards_x_offset = 80.f;
    float cards_y_offset = 40.f;
    float card_scale = 0.8;
    float gap = -7;
    sf::Vector2u card_size = Card::original_state_textures[Card::ACTIVE]->getSize();
    card_size.x *= card_scale;
    card_size.y *= card_scale;
    for (int i = 0; i < BOARD_W; ++i)
    for (int j = 0; j < BOARD_H; ++j)
    {
        cards[i][j].setPosition(cards_x_offset + (card_size.x+gap) * i , cards_y_offset + (card_size.y+gap) * j);
        cards[i][j].setScale(card_scale, card_scale);
        //cards[i][j].setLabelVisibility(false);
    }
    int cards_right = cards_x_offset + (card_size.x+gap) * BOARD_W;
    int cards_bottom = cards_y_offset + (card_size.y+gap) * BOARD_H; 

    int vocals_x = 0;
    int vocals_y = 20;
    int vocals_stride = 65;
    int consonants_x = 50;
    int consonants_y = cards_bottom + 30;
    int consonants_stride = 45;
    int font_size = 35;
    int vocal_i = 0, consonant_i = 0;
    for (int i = 0; i < letters_n; ++i)
    {
        char c = (char) ('A' + i);
        int hcw = font_size - char_width(c, font_size) / 2; // correction with half char width
        sf::IntRect bounds;
        bool is_vocal = false;
        if (vocals.find(c) != vocals.end())
        {
            ++vocal_i;
            bounds = sf::IntRect(vocals_x + hcw, vocals_y + vocals_stride * vocal_i, vocals_stride, vocals_stride);
            is_vocal = true;
        }
        else
        {  
            ++consonant_i;
            bounds = sf::IntRect(consonants_x + hcw + consonants_stride * consonant_i, consonants_y, consonants_stride, consonants_stride);
        }

        sf::Text *letter_sprite = new sf::Text(string() + c, Resources::font, font_size);
        letter_sprite->setPosition(bounds.left, bounds.top);
        letter_sprite->setOutlineThickness(3);

        letters[i] = Button::with_()
            ->bounds(bounds)
            ->no_mouse(letter_sprite)
            ->mouse_over([=] {
                with_ptr(new sf::Text(*letter_sprite));
                _->setFillColor(sf::Color(YELLOW));
                return _;
            }())
            ->mouse_down([=] {
                with_ptr(new sf::Text(*letter_sprite));
                _->setFillColor(sf::Color(255, 255, 255));
                return _;
            }())
            ->disabled([=] {
                with_ptr(new sf::Text(*letter_sprite));
                _->setFillColor(sf::Color(GREY));
                return _;
            }())
            ->label(new sf::Sprite())
            ->construct();

        if (is_vocal)
            letters[i]->disable();
    }

    player_frame.setTexture(*Resources::texture("res/img/player-frame-normal.png"));
    player_frame.setPosition(cards_right + 40, cards_y_offset);
    float sc = .65;
    player_frame.setScale(sc, sc);
    int player_frame_bottom = player_frame.getPosition().y + player_frame.getTexture()->getSize().y * sc;
    int player_frame_right = player_frame.getPosition().x + player_frame.getTexture()->getSize().x * sc;
    int player_frame_left = player_frame.getPosition().x;

    int notizia_font_size = 30;
    string notizia = mainpage->params.giocatori_notizia[mainpage->current_player];
    // insert linebreaks
    int i = -1, line_start = 0;
    while ((i = notizia.find(' ', i + 1)) != string::npos)
    {
        int word_end = notizia.find(' ', i + 1);
        if (word_end == string::npos) word_end = notizia.length();
        //if (string_width(notizia.substr(line_start, word_end), notizia_font_size) > 340*sc)
        if (word_end - line_start > 20)
            notizia.insert(i, "\n"),
            line_start = ++i;
    }
    player_frame_note = Resources::create_text(notizia);
    player_frame_note.setPosition(player_frame.getPosition());
    player_frame_note.setCharacterSize(notizia_font_size);
    player_frame_note.move(55*sc, 115*sc);

    int name_font_size = 30;
    string name = mainpage->params.giocatori_nome[mainpage->current_player];
    player_frame_name = Resources::create_text(name);
    player_frame_name.setCharacterSize(name_font_size);
    player_frame_name.setPosition(player_frame.getPosition());
    player_frame_name.move(240*sc, 660*sc);
    player_frame_name.move(-string_width(name, name_font_size) / 2, 0);

    player_frame_time = Resources::create_text("00:00");
    player_frame_time.setPosition(player_frame.getPosition());
    player_frame_time.move(200*sc, 10*sc);

    auto create_button = [=] (string title) 
    {
        return Button::with_()
            ->bounds(sf::IntRect(player_frame_left, player_frame_bottom + 15, 60, 60))
            ->no_mouse([=] {
                with_ptr(Resources::create_new_text(title));
                _->setPosition(player_frame_left, player_frame_bottom + 15);
                return _;
            }())
            ->mouse_over([=] {
                with_ptr(Resources::create_new_text(title));
                _->setFillColor(sf::Color(YELLOW));
                _->setPosition(player_frame_left, player_frame_bottom + 15);
                return _;
            }())
            ->mouse_down([=] {
                with_ptr(Resources::create_new_text(title));
                _->setFillColor(sf::Color(255, 255, 255));
                _->setPosition(player_frame_left, player_frame_bottom + 15);
                return _;
            }())
            ->relative()
            ->construct();
    };

    // todo memory leak
    play_button = *create_button("PAUSE");
    pause_button = *create_button("PLAY");
}

void TrovaLaFrase::update(sf::RenderWindow &target, float time)
{
    if (play_or_pause == &play_button)
        mainpage->giocatori_tempi[mainpage->current_player] -= time;

    debug_last_click += time;
    if (play_or_pause->click_event(target) && debug_last_click > .2)
    {
        debug_last_click = 0;

        play_or_pause = (play_or_pause == &play_button) ? &pause_button : &play_button;
        if (play_or_pause == &play_button)
            player_frame_time.setFillColor(sf::Color::Red);
        else
            player_frame_time.setFillColor(sf::Color::Green);
    }

    int secs = (int)mainpage->giocatori_tempi[mainpage->current_player];
    auto leading_zero = [] (int i) {
        string s = to_string(i);
        if (s.length() == 1)
            return "0" + s;
        else
            return s;
    };
    player_frame_time.setString(leading_zero(secs/60) + ":" + leading_zero(secs % 60));

    for (int i = 0; i < letters_n; ++i)
    {
        if (letters[i]->click_event(target))
            letterClicked('A' + i);
    }

    letters_anim.animate(time);

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

void TrovaLaFrase::letterClicked(char c)
{
    ((sf::Text *) letters[c - 'A']->state_sprites[Button::State::NO_MOUSE])
        ->setFillColor(sf::Color(YELLOW));

    for (int i = 0; i < BOARD_W; ++i)
    for (int j = 0; j < BOARD_H; ++j)
    {
        if (cards[i][j].getChar() == c && cards[i][j].isShown())
            return; // ensure(letter not already guessed)
    }

    bool correct = false;
    bool already_guessed = false;
    for (int i = 0; i < BOARD_W; ++i)
    for (int j = 0; j < BOARD_H; ++j)
    {
        cards[i][j].setCharColor(sf::Color(0, 0, 0));
        if (cards[i][j].getChar() == c)
        {
            // require(letter not already guessed)
            cards[i][j].setCharColor(sf::Color(YELLOW));
            //cards[i][j].setLabelVisibility(true);
            cards[i][j].show();
            correct = true;
        }
    }
    
    // require(letter not already guessed)
    int timechange = correct ? mainpage->params.other_values["tempo_giusto"] : -mainpage->params.other_values["tempo_errore"];
    mainpage->giocatori_tempi[mainpage->current_player] += timechange;

    auto letter_bounds = letters[c - 'A']->bounds;
    particles.push_back( SecondsParticle
    (
        // require(resource available for tempo_giusto)
        // require(resource available for tempo_errore)
        SecondsParticle::seconds_texture(timechange), 
        letter_bounds.left, 
        letter_bounds.top - SecondsParticle::time_minus_10s->getSize().y + 30
    ));
}