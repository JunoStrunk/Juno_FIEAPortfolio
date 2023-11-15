# Juno_FIEAPortfolio
Juno Strunk's programming portfolio for applying to FIEA.

Hello! Thank you for taking the time to look at my application. This readme will go over some important information about my projects.

Both projects are configured with CMake through Visual Studio Code.

## Wave Function Collapse (WFC Folder)
In this project, I explore the wave function collapse algorithm through tile sets. This project is made using **C++** and **SFML**. All scripts were made by me apart from Tilemap.cpp, which is from SFML's documentation. This is a personal project I completed outside of classes.

Here is a demo of the project:

[![Image to a youtube link of my WFC demo](https://img.youtube.com/vi/cnrVNLI1Ivk/0.jpg)](https://www.youtube.com/watch?v=cnrVNLI1Ivk)

Wave function collapse is a procedural generation algorithm that utilizes cells that can have many states and eventually collapse to one state. The states that are possible for each cell are determined by the state of the cells around them. Some general rules about what cells can be next to each other are also needed.

I made a small image to showcase how this works.
- The rules state that cells next to each other must have arrows pointing to each other.
- Cell 0 and Cell 1 are both uncollapsed, each one could possibly be an arrow pointing in any direction.
- Cell 0 collapses into an upward arrow.
- Cell 1's possibilities are now limited to a down arrow based on the given rules.
- Cell 1 collapses into a downward arrow.

<img width="1000" alt="Screen Shot 2023-11-14 at 9 46 21 PM" src="https://github.com/JunoStrunk/Juno_FIEAPortfolio/assets/98421780/9a0658af-c13d-44a6-a336-4261d434854d">

## Minesweeper (Minesweeper Folder)
This project is a Minesweeper clone. I completed it for Programming Fundamentals 2 with Professor Joshua Fox. I learned a lot in this project, like how to put together and organize a larger project.

Here is a demo of the project:

[![Image to a youtube link of my Minesweeper demo](https://img.youtube.com/vi/ryKBlafYiAI/0.jpg)](https://www.youtube.com/watch?v=ryKBlafYiAI)
