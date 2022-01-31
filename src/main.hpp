#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <unordered_map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

template <typename Key, typename T>
using umap = unordered_map<Key, T>;

#include "resources.hpp"

// returned resource must be deleted by caller
inline
sf::Texture *load_texture(string &&filename)
{
    sf::Texture *result = new sf::Texture;
    if (!result->loadFromFile(filename))
    {
        delete result;
        throw string("Error loading ") + filename; 
    }
    else
    {
        return result;
    }
}

inline
int char_width(char c, int font_size)
{
    return Resources::font.getGlyph(c, font_size, false).bounds.width;
}

#endif