// graphic.h

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "scrabble.h"

// --------------------
// Color Definitions
// --------------------

// Colors for multipliers
#define COLOR_TRIPLE_WORD    (Color){ 240, 0, 0, 255 }        // Red
#define COLOR_TRIPLE_LETTER  (Color){ 0, 24, 136, 255 }       // Strong Blue
#define COLOR_DOUBLE_LETTER  (Color){ 74, 196, 192, 255 }     // Cyan
#define COLOR_DOUBLE_WORD    (Color){ 246, 179, 126, 255 }    // Desaturated Mustard Yellow
#define COLOR_NORMAL         (Color){ 230, 230, 230, 255 }    // Beige
#define COLOR_HOVER          (Color){ 230, 230, 230, 120 }    // Transparent Beige

// --------------------
// Window Dimensions
// --------------------

// Base screen dimensions
#define BASE_SCREEN_WIDTH  1000
#define BASE_SCREEN_HEIGHT 550

// Scaling factor for UI elements
#define SCALE_FACTOR 1.7f

// Calculated scaled dimensions based on the scaling factor
#define SCREEN_WIDTH  (int)(BASE_SCREEN_WIDTH * SCALE_FACTOR)
#define SCREEN_HEIGHT (int)(BASE_SCREEN_HEIGHT * SCALE_FACTOR)

// --------------------
// Structures
// --------------------

// Structure to hold all scaled dimensions
typedef struct
{
    // Tile dimensions
    int baseTileSize;
    int tileSize;

    // Font sizes
    int fontSizeLetter;
    int fontSizeSmall;
    int fontSizeLarge;
    int fontSizeScore;
    int fontSizeTurn;
    int fontSizeTitle; // For titles like "Scores" and "Turn"

    // Panel dimensions
    int basePanelWidth;
    int basePanelHeight;
    int panelWidth;
    int panelHeight;
    int panelY;

    // Offsets
    int offsetX;
    int offsetY;
    int baseOffsetX;
    int baseOffsetY;

    // Shapes
    int starSize;

    // Control Panel dimensions
    int controlAreaWidth;    // Width of the control area (450 pixels scaled)
    int controlAreaX;        // Starting X position of the control area
    int paddingBetweenElements; // Padding between elements in the panel
} ScaledDimensions;

// --------------------
// Function Prototypes
// --------------------

// Initializes the graphical interface
void initGraphics();

// Closes the graphical interface
void closeGraphics();

// Handles user input and updates the game state
bool drawGame(Game* game);

// Loads resources (fonts, textures)
void loadResources();

// Unloads resources
void unloadResources();

// Draws the game board
void drawBoard();

// Draws the control panel
void drawPanel(Game* game);

// Determines the direction based on the starting point
WordDirection determineDirection(int x, int y);

// Checks if a letter can be placed in a specific spot
bool isSpotAvailable(Game* game, int x, int y, WordDirection direction, int axis);

// Temporary function to check if a spot is available for placement
bool getTempFunc(int x, int y);

// Retrieves the starting letter's coordinates
Coordinate getStartingLetter();

// Places a letter on the board at the specified coordinates
void placeLetter(int x, int y);

// Removes the last placed letter from the board and player's letters
void unplaceLastLetter();

// Handles the "Accept" button click
void handleSubmit(Game* game);

// Handles the "Reroll Letters" button click
void handleRerollLetters(Game* game);

// Handles the "End Game" button click
void handleEndGame(Game* game);

// Draws an invalid move message on the screen
void drawInvalidMoveMessage(float startX, float currentY);

#endif // GRAPHIC_H
