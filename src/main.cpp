#include "main.hpp"
#include "pages/MainPage.hpp"
#include <fstream>

umap<string, sf::Texture *> Resources::textures;
sf::Font Resources::font;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1600, 800), "SFML window");
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("res/img/1x/tentazione-selected.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("res/font.otf"))
        return EXIT_FAILURE;
    sf::Text text("Hello SFML", font, 50);
    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("res/sound.wav"))
        return EXIT_FAILURE;
    // Play the music
    //music.play();

    pages::Main::Params params;

    std::ifstream config_in("res/config.txt");
    string used_titles_num;
    getline(config_in, used_titles_num);
    for (int i = 0; i < stoi(used_titles_num); ++i)
    {
        string ignore;
        getline(config_in, ignore);
    }
    for (int i = 0; i < NUM_PLAYERS; ++i)
    {
        getline(config_in, params.giocatori_nome[i]);
        getline(config_in, params.giocatori_notizia[i]);
    }
    for (string name : {
        "tempo_iniziale",
        "tempo_manche[0]", //todo
        "tempo_manche[1]", //todo
        "tempo_manche[2]", //todo
        "tempo_sfida[0].aggiunto",  //todo
        "tempo_sfida[0].sottrato",  //todo
        "tempo_sfida[1].aggiunto",  //todo
        "tempo_sfida[1].sottrato", //todo
        "tempo_tentazione",  //todo
        "tempo_giusto",
        "tempo_errore", //todo
        "tempo_errore_risolvi",  //todo
        "tempo_slide",  //todo
        "tempo_suspance",  //todo
        "tempo_alert",  //todo
        "tempo_azione",  //todo
        "tempo_pausa_azione" //todo
    }) {
        string line;
        getline(config_in, line);
        params.other_values.insert(std::make_pair(name, stoi(line)));
    }
    char board[BOARD_W][BOARD_H];
    for (int j = 0; j < BOARD_H; ++j)
    for (int i = 0; i < BOARD_W; ++i)
    {
        string ch;
        getline(config_in, ch);
        params.board[i][j] = toupper(ch[0]);
    }

    Resources::init();
    pages::Main::init();
    pages::Main mainPage(params);

    sf::Clock clock;
    sf::Time lag = sf::microseconds(0);

    while (window.isOpen())
    {
        sf::Time frame_duration = sf::microseconds(1000000 / 40);
        clock.restart();

        sf::Time last_frame_duration = frame_duration + lag;
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        mainPage.update(window, last_frame_duration.asSeconds());

        // Clear screen
        window.clear(sf::Color(0, 64, 128));

        /*
        // Draw the sprite
        window.draw(sprite);
        // Draw the string
        window.draw(text);
        */

        mainPage.draw(window);

        // Update the window
        window.display();

        sf::Time remaining_time = frame_duration - clock.getElapsedTime();
        if (remaining_time.asMicroseconds() > 0)
        {
            sf::sleep(remaining_time);
            lag = sf::microseconds(0);
        }
        else
        {
            lag = -remaining_time;
        }
        
    }
    return EXIT_SUCCESS;
}