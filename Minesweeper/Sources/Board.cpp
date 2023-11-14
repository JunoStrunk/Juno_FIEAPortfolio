//
// Created by Juno Strunk on 4/19/21.
//

#include "../Headers/Board.h"
using std::cout;
using std::endl;

Board::Board(int width, int height, int mineCount)
{
    this->width = width;
    this->height = height;
    this->maxMineCount = mineCount;
    mineCount = maxMineCount;
    tileCount = (width * height) - maxMineCount;

    initSprites();
}

// Set texture and position of sprites on board
void Board::initSprites()
{
    ////Mine Counter///
    mineCountLeft.setTexture(TextureManager::GetTexture("digits"));
    mineCountLeft.setTextureRect(sf::IntRect(21 * (mineCount / 100 % 10), 0, 21, 32));
    mineCountLeft.setPosition(21, height * 32);
    mineCountMid.setTexture(TextureManager::GetTexture("digits"));
    mineCountMid.setTextureRect(sf::IntRect(21 * (mineCount / 10 % 10), 0, 21, 32));
    mineCountMid.setPosition(42, height * 32);
    mineCountRight.setTexture(TextureManager::GetTexture("digits"));
    mineCountRight.setTextureRect(sf::IntRect(21 * (mineCount % 10), 0, 21, 32));
    mineCountRight.setPosition(62, height * 32);

    ////Smiley////
    smiley.setTexture(TextureManager::GetTexture("face_happy"));
    smiley.setPosition((width / 2 * 32) - 64, height * 32);

    ////Buttons////
    Debug.setTexture(TextureManager::GetTexture("debug"));
    Debug.setPosition((width * 32) - 256, height * 32);
    Test1.setTexture(TextureManager::GetTexture("test_1"));
    Test1.setPosition((width * 32) - 192, height * 32);
    Test2.setTexture(TextureManager::GetTexture("test_2"));
    Test2.setPosition((width * 32) - 128, height * 32);
    Test3.setTexture(TextureManager::GetTexture("test_3"));
    Test3.setPosition((width * 32) - 64, height * 32);
}

// Draw the all the elements onto the window
void Board::Draw(sf::RenderWindow &window)
{
    for (int row = 0; row < board.size(); row++)
    {
        for (int col = 0; col < board[row].size(); col++)
        {
            board[row][col].Draw(window);
        }
    }
    window.draw(mineCountLeft);
    window.draw(mineCountMid);
    window.draw(mineCountRight);
    window.draw(smiley);
    window.draw(Debug);
    window.draw(Test1);
    window.draw(Test2);
    window.draw(Test3);
}

void Board::setMineCount(int mines)
{
    maxMineCount = mines;
    mineCount = maxMineCount;
}

// fill the board with tiles and mines
void Board::initializeBoard(vector<vector<int>> &mineMap)
{
    // fill board with tiles
    mineCount = maxMineCount;
    board.clear();
    initSprites();
    float rowHeight = 0;
    float colWidth = 0;
    int numRows = 0;
    vector<Tile> tempRow(width);
    while (numRows < height)
    {
        for (int col = 0; col < width; col++)
        {
            tempRow[col] = Tile();
            tempRow[col].setPosition(colWidth, rowHeight);
            colWidth += 32;
        }
        board.push_back(tempRow);
        colWidth = 0;
        rowHeight += 32;
        numRows++;
    }

    // set which tiles are mines
    for (int row = 0; row < board.size(); row++)
    {
        for (int col = 0; col < board[row].size(); col++)
        {
            if (mineMap[row][col] == 1)
                board[row][col].makeMine();
        }
    }

    // initialize each tile
    Tile *adjTile;
    for (int row = 0; row < board.size(); row++)
    {
        for (int col = 0; col < board[row].size(); col++)
        {
            if (!board[row][col].getMine())
            {
                for (int adjR = row - 1; adjR <= row + 1; adjR++)
                {
                    for (int adjC = col - 1; adjC <= col + 1; adjC++)
                    {
                        if (adjR >= 0 && adjR < height && adjC >= 0 && adjC < width)
                        { // if adjR and adjC aren't out of bounds
                            adjTile = &board[adjR][adjC];
                            board[row][col].addAdjTile(adjTile);
                        }
                    }
                }
            }
            board[row][col].loadOverlayNumber();
        }
    }
}

vector<vector<Tile>> &Board::getBoard()
{
    return board;
}

// Getters for various sprites on the board
sf::Sprite Board::getSmiley()
{
    return smiley;
}

sf::Sprite Board::getDebug()
{
    return Debug;
}

sf::Sprite Board::getTest1()
{
    return Test1;
}

sf::Sprite Board::getTest2()
{
    return Test2;
}

sf::Sprite Board::getTest3()
{
    return Test3;
}

// Set a flag on a tile
void Board::Flag(int row, int col)
{
    if (board[row][col].Flag())
        mineCount--;
    else
        mineCount++;
    if (mineCount < 0)
    {
        mineCountLeft.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
        mineCountMid.setTextureRect(sf::IntRect(-21 * (mineCount / 10 % 10), 0, 21, 32));
        mineCountRight.setTextureRect(sf::IntRect(-21 * (mineCount % 10), 0, 21, 32));
    }
    else
    {
        mineCountLeft.setTextureRect(sf::IntRect(21 * (mineCount / 100 % 10), 0, 21, 32));
        mineCountMid.setTextureRect(sf::IntRect(21 * (mineCount / 10 % 10), 0, 21, 32));
        mineCountRight.setTextureRect(sf::IntRect(21 * (mineCount % 10), 0, 21, 32));
    }
}

// uncover a tile, if it's a mine (and wasn't flagged) return false
bool Board::Flip(int row, int col)
{
    board[row][col].Flip();
    if (board[row][col].getMine() && !board[row][col].getFlagged())
        return false;
    return true;
}

// On game over, flip over all the tiles
void Board::gameOver()
{
    for (int row = 0; row < board.size(); row++)
    {
        for (int col = 0; col < board[row].size(); col++)
        {
            if (board[row][col].getMine())
                board[row][col].Flip();
            board[row][col].lock();
        }
    }
    smiley.setTexture(TextureManager::GetTexture("face_lose"));
}

// On win, flip over all the tiles
void Board::win()
{
    for (int row = 0; row < board.size(); row++)
    {
        for (int col = 0; col < board[row].size(); col++)
        {
            board[row][col].Flip();
            board[row][col].lock();
        }
    }
    smiley.setTexture(TextureManager::GetTexture("face_win"));
}

// check if all nonmine tiles have been revealed
bool Board::checkWin()
{
    int revealedTileCount = 0;
    for (int row = 0; row < board.size(); row++)
    {
        for (int col = 0; col < board[row].size(); col++)
        {
            if (!board[row][col].getMine())
            {
                if (!board[row][col].getHidden())
                    revealedTileCount++;
                else
                    return false;
            }
        }
    }
    if (revealedTileCount == tileCount)
        return true;
}
