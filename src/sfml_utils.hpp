#ifndef SFML_UTILS_HPP
#define SFML_UTILS_HPP

namespace sf
{

inline
sf::Sprite sprite_from_texture(sf::Texture const &texture)
{
    sf::Sprite sprite;
    sprite.setTexture(texture);
    return sprite;
}

}

#endif