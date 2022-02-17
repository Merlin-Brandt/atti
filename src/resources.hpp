#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include "main.hpp"

template <typename Key, typename T>
using umap = unordered_map<Key, T>;

class Resources
{
    static
    umap<string, sf::Texture *> textures;
public:
    static 
    sf::Font font;

    [[deprecated]]
    static
    sf::Text create_text(string txt)
    {
        sf::Text text(txt, font);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(3);
        return text;
    }

    static
    sf::Text *create_new_text(string txt)
    {
        sf::Text *text = new sf::Text(txt, font);
        text->setFillColor(sf::Color::White);
        text->setOutlineColor(sf::Color::Black);
        text->setOutlineThickness(3);
        return text;
    }

    static
    void init()
    {
        font.loadFromFile("res/font.otf");
    }

    static
    sf::Texture *texture(string &&filename)
    {
        if (textures.find(filename) != textures.end())
        {
            return textures[filename];
        }
        else
        {
            sf::Texture *new_texture = new sf::Texture();
            if (!new_texture->loadFromFile(filename))
            {
                textures.insert({filename, 0});
                delete new_texture;
            }
            else
            {
                textures.insert({filename, new_texture});
            }
            return textures[filename];
        }
    }

    static
    void clear()
    {
        for (auto it : textures)
        {
            sf::Texture *texture = it.second;
            if (texture)
            {
                delete texture;
            }
        }
        textures.clear();
    }
};

#endif