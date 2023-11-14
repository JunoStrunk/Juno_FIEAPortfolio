#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#pragma once

/*
Base tile map class courtesy of SFML Official Tutorials, Thanks SFML!
Making my own additions to work with input sample images => Create tilemap texture
*/

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool loadLevel(const std::string &tileset, sf::Vector2u tileSize, const int *tiles, unsigned int width, unsigned int height);

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};