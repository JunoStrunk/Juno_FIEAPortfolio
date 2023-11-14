#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include "Headers/Random.h"
#include "Headers/Board.h"
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::ifstream;
using std::vector;

bool LoadConfig(string fileName, vector<int> &numbers);
void makeMineMap(int width, int height, int mineCount, vector<vector<int>> &bluePrint);
int loadTestBoard(string fileName, int width, int height, vector<vector<int>> &bluePrint);

int main()
{
    vector<int> numbers;
    vector<vector<int>> mineMap;

    if (!LoadConfig("../../boards/config.cfg", numbers))
        return -1;

    makeMineMap(numbers.at(0), numbers.at(1), numbers.at(2), mineMap);

    Board board(numbers.at(0), numbers.at(1), numbers.at(2));

    board.initializeBoard(mineMap);

    sf::RenderWindow window(sf::VideoMode(numbers.at(0) * 32, (numbers.at(1) * 32) + 88), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);

    mineMap.clear();

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    /// Left-Click tile////
                    for (int row = 0; row < board.getBoard().size(); row++)
                    {
                        for (int col = 0; col < board.getBoard()[row].size(); col++)
                        {
                            if (board.getBoard()[row][col].clickable()) // if the tile is still hidden (Clickable)
                            {
                                auto bounds = board.getBoard()[row][col].getTileBounds().getGlobalBounds(); // get bounds of button
                                if (bounds.contains(mousePos.x, mousePos.y))
                                {
                                    if (!board.Flip(row, col))
                                        board.gameOver();
                                    else if (board.checkWin())
                                    {
                                        board.win();
                                    }
                                } // if the bounds is where the click happened
                            }
                        }
                    }
                    ///////////////////////////////////////////////////////////////////////////////////////

                    auto bounds = board.getSmiley().getGlobalBounds();
                    if (bounds.contains(mousePos.x, mousePos.y))
                    {
                        makeMineMap(numbers.at(0), numbers.at(1), numbers.at(2), mineMap);
                        board.setMineCount(numbers.at(2));
                        board.initializeBoard(mineMap);

                        mineMap.clear();
                    }

                    bounds = board.getDebug().getGlobalBounds();
                    if (bounds.contains(mousePos.x, mousePos.y))
                    {
                        for (int row = 0; row < board.getBoard().size(); row++)
                        {
                            for (int col = 0; col < board.getBoard()[row].size(); col++)
                            {
                                board.getBoard()[row][col].toggleVisibilty();
                            }
                        }
                    }

                    bounds = board.getTest1().getGlobalBounds();
                    if (bounds.contains(mousePos.x, mousePos.y))
                    {
                        board.setMineCount(loadTestBoard("../boards/testboard1.brd", numbers.at(0), numbers.at(1), mineMap));
                        board.initializeBoard(mineMap);

                        mineMap.clear();
                    }

                    bounds = board.getTest2().getGlobalBounds();
                    if (bounds.contains(mousePos.x, mousePos.y))
                    {
                        board.setMineCount(loadTestBoard("../../boards/testboard2.brd", numbers.at(0), numbers.at(1), mineMap));
                        board.initializeBoard(mineMap);

                        mineMap.clear();
                    }

                    bounds = board.getTest3().getGlobalBounds();
                    if (bounds.contains(mousePos.x, mousePos.y))
                    {
                        board.setMineCount(loadTestBoard("../../boards/testboard3.brd", numbers.at(0), numbers.at(1), mineMap));
                        board.initializeBoard(mineMap);

                        mineMap.clear();
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    /// I didn't put this in a method because it would be too much work to count the flags///
                    /// Or I couldn't think of a simpler way to count the flags so my bad////
                    /// Right-Click tile////
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (int row = 0; row < board.getBoard().size(); row++)
                    {
                        for (int col = 0; col < board.getBoard()[row].size(); col++)
                        {
                            if (board.getBoard()[row][col].clickable()) // if the tile is still hidden (Clickable)
                            {
                                auto bounds = board.getBoard()[row][col].getTileBounds().getGlobalBounds(); // get bounds of button
                                if (bounds.contains(mousePos.x, mousePos.y))                                // if the bounds is where the click happened
                                    board.Flag(row, col);
                            }
                        }
                    }
                    ///////////////////////////////////////////////////////////////////////////////////////
                }
            }
        }
        window.clear();

        board.Draw(window);

        window.display();
    }

    return 0;
}

bool LoadConfig(string fileName, vector<int> &numbers)
{
    ifstream file(fileName);

    int value = 0;

    if (file.is_open())
    {
        for (int i = 0; i < 3; i++)
        {
            file >> value;
            numbers.push_back(value);
        }
    }
    else
    {
        cout << "Couldn't open file" << endl;
        return false;
    }
    return true;
}

void makeMineMap(int width, int height, int mineCount, vector<vector<int>> &bluePrint)
{
    // make map
    int minesOnMap = 0;
    int numRows = 0;
    vector<int> tempRow(width);
    while (numRows < height)
    {
        for (int co = 0; co < width; co++)
        {
            if (minesOnMap < mineCount)
            {
                tempRow[co] = 1;
                minesOnMap++;
            }
            else
            {
                tempRow[co] = 0;
            }
        }
        bluePrint.push_back(tempRow);
        numRows++;
    }

    // shuffle
    for (int row = 0; row < bluePrint.size(); row++)
    {
        for (int col = 0; col < bluePrint[row].size(); col++)
        {
            int randomX = Random::Int(0, height - 1);
            int randomY = Random::Int(0, width - 1);
            int temp = bluePrint[row][col];
            bluePrint[row][col] = bluePrint[randomX][randomY];
            bluePrint[randomX][randomY] = temp;
        }
    }
}

int loadTestBoard(string fileName, int width, int height, vector<vector<int>> &bluePrint)
{
    ifstream file(fileName);
    int mineCount = 0;

    if (file.is_open())
    {
        string line;

        while (getline(file, line))
        {
            std::istringstream stream(line);

            string tempHolder;
            vector<int> tempRow(width);
            getline(stream, tempHolder);
            for (int col = 0; col < width; col++)
            {
                tempRow[col] = tempHolder[col] - 48; // subtract 48 because its a character and casting a character to int converts it to 48
                if (tempRow[col] == 1)
                    mineCount++;
            }
            bluePrint.push_back(tempRow);
        }
    }
    else
        cout << "Couldn't open file" << endl;
    return mineCount;
}
