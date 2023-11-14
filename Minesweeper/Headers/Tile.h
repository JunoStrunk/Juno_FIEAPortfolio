//
// Created by Juno Strunk on 4/19/21.
//

#pragma once
#include "TextureManager.h"
#include <iostream>
#include <vector>
using std::vector;

class Tile {
    sf::Sprite tile;
    sf::Sprite flag;
    sf::Sprite overlay;
    int nearMines;
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    bool isClickable;
    bool hidden;
    vector<Tile*> adjTiles;

public:
    Tile();
    void Draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::Sprite getTileBounds() const;
    bool clickable() const;
    void lock();
    bool getMine();
    bool getFlagged();
    bool getHidden();

    void Flip();
    bool Flag();
    void makeMine();
    void addAdjTile(Tile* adjTile);
    void loadOverlayNumber();
    void toggleVisibilty();
};

