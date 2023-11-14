//
// Created by Juno Strunk on 4/19/21.
//

#include "../Headers/Tile.h"

Tile::Tile() {
    nearMines = 0;
    tile.setTexture(TextureManager::GetTexture("tile_hidden"));
    flag.setTexture(TextureManager::GetTexture("flag"));
    isMine = false;
    isRevealed = false;
    isFlagged = false;
    isClickable = true;
    hidden = true;
    setPosition(0, 0);
}

void Tile::Draw(sf::RenderWindow& window) {
    window.draw(tile);
    if(isFlagged)
        window.draw(flag);
    if(isRevealed)
        window.draw(overlay);
}

void Tile::setPosition(float x, float y) {
    tile.setPosition(x,y);
    flag.setPosition(x,y);
    overlay.setPosition(x,y);
}

sf::Sprite Tile::getTileBounds() const{
    return tile;
}

bool Tile::clickable() const{
    return isClickable;
}

void Tile::lock() {
    isClickable = false;
}

bool Tile::getMine(){
    return isMine;
}

bool Tile::getFlagged() {
    return isFlagged;
}

bool Tile::getHidden() {
    return hidden;
}

void Tile::Flip() {
    if(!isFlagged) {
        isRevealed = true;
        hidden = false;
        lock();
        tile.setTexture(TextureManager::GetTexture("tile_revealed"));
        if (nearMines == 0)
            for (int i = 0; i < adjTiles.size(); i++)
                if (adjTiles.at(i)->clickable())
                    adjTiles.at(i)->Flip();
    }
}

bool Tile::Flag() {
    isFlagged = !isFlagged;
    if(isFlagged){
        return true;
    }
    return false;
}

void Tile::makeMine() {
    isMine = true;
    overlay.setTexture(TextureManager::GetTexture("mine"));
}

void Tile::addAdjTile(Tile* adjTile) {
    adjTiles.push_back(adjTile);
    if(adjTile->getMine())
        nearMines++;
}

void Tile::loadOverlayNumber() {
    if(!isMine && nearMines > 0)
        overlay.setTexture(TextureManager::GetTexture("number_" + std::to_string(nearMines)));
}

void Tile::toggleVisibilty() {
    if(isMine)
        isRevealed = !isRevealed;
}


