#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include "main.hpp"

class Resources
{
    static
    umap<string, sf::Texture *> textures;
public:
    static sf::Font font;

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