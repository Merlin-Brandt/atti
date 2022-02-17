#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <cctype>
#include <functional>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

#include "resources.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include "sfml_utils.hpp"

using sstd::umap;

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

inline
int string_width(string s, int font_size)
{
    int sw = 0;
    for (char c : s)
        sw += char_width(c, font_size);
    return sw;
}

#endif