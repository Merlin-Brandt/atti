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

    struct Params
    {
        string giocatori_nome[NUM_PLAYERS];
        string giocatori_notizia[NUM_PLAYERS];
        char board[BOARD_W][BOARD_H];
        umap<string, int> other_values;
    } params;

    string (&giocatori_nome)[NUM_PLAYERS] = params.giocatori_nome;
    string (&giocatori_notizia)[NUM_PLAYERS] = params.giocatori_notizia;
    
    float giocatori_tempi[NUM_PLAYERS];

    int current_player = 0;

    Main(Params a_params) : params(a_params), trovaLaFrase(this) {
        for (int i = 0; i < NUM_PLAYERS; ++i)
            giocatori_tempi[i] = params.other_values["tempo_iniziale"];
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
