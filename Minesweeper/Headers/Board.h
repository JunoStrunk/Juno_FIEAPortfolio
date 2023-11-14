//
// Created by Juno Strunk on 4/19/21.
//

#pragma once
#include "Tile.h"
#include <iostream>
#include <vector>
using std::vector;

class Board {
    sf::Sprite mineCountLeft;
    sf::Sprite mineCountMid;
    sf::Sprite mineCountRight;
    sf::Sprite smiley;
    sf::Sprite Debug;
    sf::Sprite Test1;
    sf::Sprite Test2;
    sf::Sprite Test3;
    int width;
    int height;
    int maxMineCount;
    int mineCount;
    int tileCount;
    vector<vector<Tile>> board;
public:
    Board(int width, int height, int mineCount);
    void initSprites();
    void initializeBoard(vector<vector<int>>& mineMap);
    void Draw(sf::RenderWindow& window);
    void setMineCount(int mines);

    vector<vector<Tile>>& getBoard();
    sf::Sprite getSmiley();
    sf::Sprite getDebug();
    sf::Sprite getTest1();
    sf::Sprite getTest2();
    sf::Sprite getTest3();

    void Flag(int row, int col);
    bool Flip(int row, int col);

    void gameOver();
    void win();
    bool checkWin();
};