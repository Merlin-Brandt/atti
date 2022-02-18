#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include "main.hpp"
#if __APPLE__
#include "apple/ResourcePath.hpp"
#endif

class Resources
{
    static
    unordered_map<string, sf::Texture *> textures;
    
public:
    static
    std::string path()
    {
#if __APPLE__
        return apple_ResourcePath();
#else
        return std::string("./");
#endif
    }
    
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
        font.loadFromFile(path() + "res/font.otf");
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
            if (!new_texture->loadFromFile(path() + filename))
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
