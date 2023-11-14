#include "SFML/Graphics.hpp"
#include "headers/TileMap.hpp"
#include "headers/Grid.hpp"

// create grid and load into map
int initializeTileMap(TileMap &map, sf::Vector2u tileSize, sf::Vector2u tileMapSize, std::string &filePath)
{
    Grid sample(tileMapSize, tileSize, filePath);
    if (!map.loadLevel("../../Textures/UniqueTiles.png", tileSize, sample.getFinal(), tileMapSize.x, tileMapSize.y))
        return -1;
    return 0;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tilemap");
    // TODO: Programatic resize? based on tilemap?
    sf::View view1;

    /* ------ Important ------- */
    // DMHouses TileSize - 8x8
    // DMMiniWorld TileSize - 16x16
    // TileMapSize - Whatever you want! try out 16x10
    // If tile map size is too big, you might sit for a while,

    sf::Vector2u tileSize(8, 8);
    sf::Vector2u tileMapSize(20, 20);
    std::string sampleImageFilePath = "../../Textures/DMHouses.png";
    // create the tilemap from the level definition
    TileMap map;
    if (initializeTileMap(map, tileSize, tileMapSize, sampleImageFilePath) == -1)
        return -1;

    view1.setCenter(tileSize.x * tileMapSize.x / 2, tileSize.y * tileMapSize.y / 2);
    view1.setSize(tileSize.x * tileMapSize.x, tileSize.y * tileMapSize.y);
    window.setView(view1);

    // run the main loop
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // if (event.type == sf::Event::Resized)
            // {
            //     view1.setSize(event.size.width, event.size.height);
            //     window.setView(view1);
            // }
        }

        // draw the map
        window.clear();
        window.draw(map);
        window.display();
    }

    return 0;
}
