// main.c

#include "scrabble.h"
#include "graphic.h"
#include <time.h>
#include <stdlib.h>

// --------------------
// Main Function
// --------------------

// Entry point of the Scrabble game
int main() {
    // --------------------
    // Initialization
    // --------------------

    // Seed the random number generator with the current time to ensure randomness
    srand(time(NULL));

    // Initialize the game state
    Game game;
    initGame(&game);

    // Initialize the graphical user interface
    initGraphics();

    // Flag to control the main game loop
    bool running = true;

    // --------------------
    // Main Game Loop
    // --------------------
    while (running && !isGameOver(&game)) {
        // Handle user input, update game state, and render the game
        // 'drawGame' returns false if the game window should close
        running = drawGame(&game);

        // Note: All drawing and input handling is managed inside 'drawGame'
    }

    // --------------------
    // Cleanup and Resource Management
    // --------------------

    // Free allocated resources and clean up the game state
    freeGame(&game);

    // Close the graphical interface and release associated resources
    closeGraphics();

    return 0; // Exit the program successfully
}
