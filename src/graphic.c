// graphic.c

#include <raylib.h>
#include "raygui.h"
#include "graphic.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Define the global scaled dimensions
ScaledDimensions scaled;

// Global variables for resources
Font gameFont;

// Variables for gameplay
int selectedIndex = -1;
char playerLetters[MAX_LETTERS] = {0};
int currentAxis = 0;
WordDirection currentDirection = Still_None;
Coordinate lettersCoordinates[MAX_LETTERS];
char placedLetters[MAX_LETTERS] = {0};
int numPlacedLetters = 0;
bool isInvalidMove = false; // Variable to track invalid moves

Game clone;

// Initializes the graphical interface
void initGraphics()
{
    // Initialize scaled dimensions
    scaled.baseTileSize = 30;
    scaled.tileSize = (int)(scaled.baseTileSize * SCALE_FACTOR); // 30 *1.7=51

    scaled.fontSizeLetter = (int)(20 * SCALE_FACTOR); // 34
    scaled.fontSizeSmall = (int)(8 * SCALE_FACTOR); // 14
    scaled.fontSizeLarge = (int)(12 * SCALE_FACTOR); // 20
    scaled.fontSizeScore = (int)(20 * SCALE_FACTOR); // 34
    scaled.fontSizeTurn = (int)(20 * SCALE_FACTOR); // 34
    scaled.fontSizeTitle = (int)(24 * SCALE_FACTOR); // 41

    // Initialize panel dimensions
    scaled.basePanelWidth = BASE_SCREEN_WIDTH;
    scaled.basePanelHeight = 80;
    scaled.panelWidth = (int)(scaled.basePanelWidth * SCALE_FACTOR); // 1000 *1.7=1700
    scaled.panelHeight = (int)(scaled.basePanelHeight * SCALE_FACTOR); // 80 *1.7=136
    scaled.panelY = SCREEN_HEIGHT - scaled.panelHeight; // 550*1.7=935, panelY=935-136=799

    // Initialize offsets
    scaled.baseOffsetX = 50;
    scaled.offsetX = (int)(scaled.baseOffsetX * SCALE_FACTOR); // 50*1.7=85
    scaled.baseOffsetY = 50;
    scaled.offsetY = (int)(scaled.baseOffsetY * SCALE_FACTOR); // 50*1.7=85

    // Initialize control panel dimensions
    scaled.controlAreaWidth = (int)(450 * SCALE_FACTOR); // 765
    scaled.controlAreaX = scaled.offsetX + (15 * scaled.tileSize) + (50 * SCALE_FACTOR); // 85 + 15*51=765 +85=850
    scaled.controlAreaX = 85 + 765 + 85; // 935
    scaled.paddingBetweenElements = (int)(20 * SCALE_FACTOR); // 34

    // Initialize the window with scaled dimensions
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scrabble Game");
    SetTargetFPS(60);
    loadResources();
}

// Checks if a move is currently being played
bool beingPlay()
{
    return numPlacedLetters != 0;
}

// Closes the graphical interface
void closeGraphics()
{
    unloadResources();
    CloseWindow();
}

// Loads resources (fonts, textures)
void loadResources()
{
    // Load a font for rendering text, scaled appropriately
    gameFont = LoadFontEx(TextFormat("%sfonts/arial.ttf", ASSETS_PATH), scaled.fontSizeLetter, 0, 0);

    // Load letter tile textures if available
    // For simplicity, we use rectangle drawings
}

// Unloads resources
void unloadResources()
{
    UnloadFont(gameFont);
    // Unload other resources if loaded
}

// Draws the game board
void drawBoard()
{
    Piece (*board)[15] = clone.board;

    for (int y = 0; y < LENGTH; y++)
    {
        for (int x = 0; x < LENGTH; x++)
        {
            // Determine tile color based on multiplier
            Color tileColor;
            switch (board[y][x].multiplier)
            {
            case Center:
            case Double_Word:
                tileColor = COLOR_DOUBLE_WORD;
                break;
            case Triple_Word:
                tileColor = COLOR_TRIPLE_WORD;
                break;
            case Double_Letter:
                tileColor = COLOR_DOUBLE_LETTER;
                break;
            case Triple_Letter:
                tileColor = COLOR_TRIPLE_LETTER;
                break;
            default:
                tileColor = COLOR_NORMAL;
                break;
            }

            // Draw the tile rectangle
            Rectangle tileRect = {
                (float)(scaled.offsetX + x * scaled.tileSize),
                (float)(scaled.offsetY + y * scaled.tileSize),
                (float)scaled.tileSize,
                (float)scaled.tileSize
            };
            DrawRectangleRec(tileRect, tileColor);
            DrawRectangleLinesEx(tileRect, 1 * SCALE_FACTOR, WHITE);

            // Draw the center star if applicable
            if (board[y][x].multiplier == Center && !board[y][x].is_placed)
            {
                // Calculate center of the tile
                float cx = tileRect.x + scaled.tileSize / 2.0f;
                float cy = tileRect.y + scaled.tileSize / 2.0f;

                // Calculate star radius (adjusted to fit nicely)
                float r = scaled.tileSize * 0.4f; // 51 *0.4=20.4

                // Calculate 60-degree angle in radians
                float angle60 = 60.0f * DEG2RAD;

                // Define vertices for upward-pointing triangle
                Vector2 p1 = {cx, cy - r};
                Vector2 p2 = {cx - r * sinf(angle60), cy + r * cosf(angle60)};
                Vector2 p3 = {cx + r * sinf(angle60), cy + r * cosf(angle60)};

                // Define vertices for downward-pointing triangle
                Vector2 p4 = {cx, cy + r};
                Vector2 p5 = {cx - r * sinf(angle60), cy - r * cosf(angle60)};
                Vector2 p6 = {cx + r * sinf(angle60), cy - r * cosf(angle60)};

                // Draw upward-pointing triangle
                DrawTriangle(p1, p2, p3, WHITE);

                // Draw downward-pointing triangle
                DrawTriangle(p5, p4, p6, WHITE);
            }

            // Draw the letter if it has been placed
            if (board[y][x].is_placed && board[y][x].letter != '\0')
            {
                char letterStr[2] = {board[y][x].letter, '\0'};
                Vector2 textSize = MeasureTextEx(gameFont, letterStr, scaled.fontSizeLetter, 1);
                Vector2 textPos = {
                    tileRect.x + (scaled.tileSize - textSize.x) / 2,
                    tileRect.y + (scaled.tileSize - textSize.y) / 2
                };

                char value[5];
                DrawTextEx(gameFont, letterStr, textPos, scaled.fontSizeLetter, 1, BLACK);

                sprintf(value, "%d", getLetterScore(board[y][x].letter));

                Vector2 valuePos = {
                    tileRect.x + scaled.tileSize - 6 * SCALE_FACTOR,
                    tileRect.y + scaled.tileSize - 8 * SCALE_FACTOR
                };

                DrawTextEx(gameFont, value, valuePos, scaled.fontSizeLetter / 3, 1, BLACK);
            }

            // Draw multiplier text for special tiles if not placed
            if (!board[y][x].is_placed && board[y][x].multiplier != None && board[y][x].multiplier != Center)
            {
                const char* typeText = NULL;
                int multiplierNumber = 1;

                switch (board[y][x].multiplier)
                {
                case Double_Word:
                    typeText = "Palabra";
                    multiplierNumber = 2;
                    break;
                case Triple_Word:
                    typeText = "Palabra";
                    multiplierNumber = 3;
                    break;
                case Double_Letter:
                    typeText = "Letra";
                    multiplierNumber = 2;
                    break;
                case Triple_Letter:
                    typeText = "Letra";
                    multiplierNumber = 3;
                    break;
                default:
                    break;
                }

                if (typeText != NULL)
                {
                    // Define font sizes
                    int fontSizeSmall = scaled.fontSizeSmall;
                    int fontSizeLarge = scaled.fontSizeLarge;
                    float spacing = 0.0f; // No additional spacing
                    float yOffset = 0.0f; // Adjust as needed

                    // Measure text sizes
                    Vector2 typeTextSize = MeasureTextEx(gameFont, typeText, fontSizeSmall, 1);
                    char multiplierStr[4]; // For "x2" or "x3"
                    snprintf(multiplierStr, sizeof(multiplierStr), "x%d", multiplierNumber);
                    Vector2 multiplierTextSize = MeasureTextEx(gameFont, multiplierStr, fontSizeLarge, 1);

                    // Calculate total text height
                    float totalTextHeight = typeTextSize.y + multiplierTextSize.y + spacing;

                    // Calculate initial position to center vertically
                    float textStartY = tileRect.y + (scaled.tileSize - totalTextHeight) / 2 + yOffset;

                    // Draw type text ("Palabra" or "Letra")
                    Vector2 typeTextPos = {
                        tileRect.x + (scaled.tileSize - typeTextSize.x) / 2,
                        textStartY
                    };
                    DrawTextEx(gameFont, typeText, typeTextPos, fontSizeSmall, 1, WHITE);

                    // Draw multiplier text below type text
                    Vector2 multiplierTextPos = {
                        tileRect.x + (scaled.tileSize - multiplierTextSize.x) / 2,
                        typeTextPos.y + typeTextSize.y + spacing
                    };
                    DrawTextEx(gameFont, multiplierStr, multiplierTextPos, fontSizeLarge, 1, WHITE);
                }
            }

            // Check if the tile is available for placement
            bool available = getTempFunc(x, y);

            if (selectedIndex != -1)
            {
                if (!available)
                {
                    DrawRectangleRec(tileRect, COLOR_HOVER);
                }
                else
                {
                    DrawRectangleLinesEx(tileRect, 1 * SCALE_FACTOR, GREEN);
                }
            }

            // Handle left mouse button press for placing a letter
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), tileRect) &&
                available && selectedIndex != -1)
            {
                placeLetter(x, y);
            }

            // Handle right mouse button press for unplacing the last letter
            if (numPlacedLetters > 0)
            {
                Coordinate lastLetter = lettersCoordinates[numPlacedLetters - 1];
                bool wannaErase = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                    CheckCollisionPointRec(GetMousePosition(), tileRect) && lastLetter.x == x && lastLetter.y == y &&
                    selectedIndex == -1;
                if (wannaErase)
                {
                    unplaceLastLetter();
                }
            }

            // Highlight invalid placed letters with red border
            if (isInvalidMove)
            {
                for (int i = 0; i < numPlacedLetters; i++)
                {
                    if (lettersCoordinates[i].x == x && lettersCoordinates[i].y == y)
                    {
                        DrawRectangleLinesEx(tileRect, 2 * SCALE_FACTOR, RED);
                        break;
                    }
                }
            }
        }
    }
}

// Removes the last placed letter from the board and player’s letters
void unplaceLastLetter()
{
    if (numPlacedLetters <= 0) return;

    isInvalidMove = false;

    Coordinate lastLetter = lettersCoordinates[numPlacedLetters - 1];

    clone.board[lastLetter.y][lastLetter.x].is_placed = false;
    clone.board[lastLetter.y][lastLetter.x].letter = '\0';

    // Find the first empty slot in playerLetters to put back the letter
    int index = 0;
    while (index < MAX_LETTERS && playerLetters[index] != '\0')
    {
        index++;
    }
    if (index < MAX_LETTERS)
    {
        playerLetters[index] = placedLetters[numPlacedLetters - 1];
    }

    placedLetters[numPlacedLetters - 1] = '\0';
    numPlacedLetters--;

    if (numPlacedLetters <= 1)
    {
        currentDirection = Still_None;
    }
}

// Places a letter on the board at the specified coordinates
void placeLetter(int x, int y)
{
    clone.board[y][x].is_placed = true;
    clone.board[y][x].letter = playerLetters[selectedIndex];
    placedLetters[numPlacedLetters] = playerLetters[selectedIndex];
    lettersCoordinates[numPlacedLetters] = (Coordinate){x, y};
    numPlacedLetters++;
    playerLetters[selectedIndex] = '\0';
    selectedIndex = -1;

    if (numPlacedLetters == 2)
    {
        currentDirection = determineDirection(x, y);

        switch (currentDirection)
        {
        case Vertical:
            currentAxis = x;
            break;
        case Horizontal:
            currentAxis = y;
            break;
        default:
            break;
        }
    }
}

// Temporary function to check if a spot is available for placement
bool getTempFunc(int x, int y)
{
    bool available = false;

    if (currentDirection != Still_None)
    {
        available = isSpotAvailable(&clone, x, y, currentDirection, currentAxis);
    }
    else
    {
        if (!beingPlay())
        {
            available = isSpotAvailable(&clone, x, y, currentDirection, currentAxis);
        }
        else
        {
            WordDirection tempDirection = determineDirection(x, y);

            Coordinate firstLetter = getStartingLetter();
            int yStartLetter = firstLetter.y;
            int xStartLetter = firstLetter.x;

            switch (tempDirection)
            {
            case Still_None:
                available = false;
                break;
            case Vertical:
                available = isSpotAvailable(&clone, x, y, tempDirection, xStartLetter);
                break;
            case Horizontal:
                available = isSpotAvailable(&clone, x, y, tempDirection, yStartLetter);
                break;
            }
        }
    }

    return available;
}

// Checks if a letter can be placed in one spot
bool isSpotAvailable(Game* game, int x, int y, WordDirection direction, int axis)
{
    // The start of the game since no score has been registered
    if (game->player1.score == 0 && game->player2.score == 0 && direction == Still_None)
    {
        // The middle of the board is the only possible position that can be placed
        if (x == 7 && y == 7 && !game->board[y][x].is_placed)
        {
            return true;
        }
        return false;
    }

    // If there is already a letter, can't place another
    if (game->board[y][x].is_placed)
    {
        return false;
    }

    bool aligned = false;

    // Check if the piece is aligned with the previous letters positioned
    switch (direction)
    {
    case Still_None:
        aligned = true;
        break;
    case Vertical:
        if (x == axis) aligned = true;
        break;
    case Horizontal:
        if (y == axis) aligned = true;
        break;
    }

    // Check neighboring tiles for existing letters
    Piece* up = game->board[y][x].up;
    Piece* down = game->board[y][x].down;
    Piece* left = game->board[y][x].left;
    Piece* right = game->board[y][x].right;

    bool neighborLetter = false;
    neighborLetter |= (up != NULL) && up->is_placed;
    neighborLetter |= (down != NULL) && down->is_placed;
    neighborLetter |= (left != NULL) && left->is_placed;
    neighborLetter |= (right != NULL) && right->is_placed;

    return aligned && neighborLetter;
}

// Determines the direction based on the starting point
WordDirection determineDirection(int x, int y)
{
    Coordinate firstLetter = getStartingLetter();
    int yStartLetter = firstLetter.y;
    int xStartLetter = firstLetter.x;

    if (xStartLetter == x) return Vertical;
    if (yStartLetter == y) return Horizontal;

    return Still_None;
}

// Draws the control panel on the right side
void drawPanel(Game* game)
{
    // Draw the control panel background
    DrawRectangle(scaled.controlAreaX, 0, scaled.controlAreaWidth, SCREEN_HEIGHT, LIGHTGRAY);

    // Define starting positions with padding
    float startX = scaled.controlAreaX + 20 * SCALE_FACTOR; // 20 pixels padding
    float currentY = 20 * SCALE_FACTOR; // 20 pixels from the top

    // Draw Player 1 Score
    char scoreText1[50];
    sprintf(scoreText1, "Player 1 Score: %d", game->player1.score);
    DrawTextEx(gameFont, scoreText1, (Vector2){startX, currentY}, scaled.fontSizeTitle, 1, BLACK);

    // Increment Y position
    currentY += scaled.fontSizeTitle + scaled.paddingBetweenElements;

    // Draw Player 2 Score
    char scoreText2[50];
    sprintf(scoreText2, "Player 2 Score: %d", game->player2.score);
    DrawTextEx(gameFont, scoreText2, (Vector2){startX, currentY}, scaled.fontSizeTitle, 1, BLACK);

    // Increment Y position
    currentY += scaled.fontSizeTitle + scaled.paddingBetweenElements;

    // Draw current turn
    const char* turnText = (game->turn == Player1) ? "Turno: Player 1" : "Turno: Player 2";
    DrawTextEx(gameFont, turnText, (Vector2){startX, currentY}, scaled.fontSizeTitle, 1, BLACK);

    // Increment Y position
    currentY += scaled.fontSizeTitle + scaled.paddingBetweenElements;

    // Draw "Letters:"
    DrawTextEx(gameFont, "Letras:", (Vector2){startX, currentY}, scaled.fontSizeTitle, 1, BLACK);

    // Increment Y position
    currentY += scaled.fontSizeTitle + scaled.paddingBetweenElements / 2;

    // Define initial position for letters
    float lettersStartX = startX;
    float lettersStartY = currentY;

    // Counter to position letters correctly
    int letterCount = 0;

    // Iterate through player's letters and draw them
    for (int i = 0; i < MAX_LETTERS; i++)
    {
        if (playerLetters[i] != '\0')
        {
            // Define the rectangle for the letter
            Rectangle letterBox = {
                lettersStartX + letterCount * (scaled.tileSize + scaled.paddingBetweenElements / 2),
                lettersStartY,
                (float)scaled.tileSize,
                (float)scaled.tileSize
            };

            // Handle left mouse button click on a letter box
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), letterBox))
            {
                if (selectedIndex < 0 || selectedIndex != i)
                {
                    selectedIndex = i;
                }
                else
                {
                    selectedIndex = -1;
                }
            }

            // Draw the letter box
            DrawRectangleRec(letterBox, BEIGE);
            DrawRectangleLinesEx(letterBox, 2 * SCALE_FACTOR, BLACK);

            // Draw the letter centered in the box
            char letterStr[2] = {playerLetters[i], '\0'};
            Vector2 textSize = MeasureTextEx(gameFont, letterStr, scaled.fontSizeLetter, 1);
            Vector2 textPos = {
                letterBox.x + (scaled.tileSize - textSize.x) / 2,
                letterBox.y + (scaled.tileSize - textSize.y) / 2
            };
            DrawTextEx(gameFont, letterStr, textPos, scaled.fontSizeLetter, 1, BLACK);

            // Highlight selected letter
            if (selectedIndex == i)
            {
                DrawRectangleLinesEx(letterBox, 2 * SCALE_FACTOR, GREEN);
            }
            else if (selectedIndex >= 0 && selectedIndex != i)
            {
                DrawRectangleRec(letterBox, COLOR_HOVER);
            }

            letterCount++;
        }
    }

    // Increment Y position for buttons
    currentY = lettersStartY + scaled.tileSize + scaled.paddingBetweenElements;

    // Draw the "Accept" or "Reroll Letters" button
    Rectangle actionButtonRect = {
        startX,
        currentY,
        scaled.controlAreaWidth - 40 * SCALE_FACTOR, // Adjusted for padding
        40 * SCALE_FACTOR
    };

    if (beingPlay())
    {
        // "Accept" button in green
        Color acceptColor = GREEN;
        if (GuiButton(actionButtonRect, "Aceptar"))
        {
            handleSubmit(game);
        }
    }
    else
    {
        // "Reroll Letters" button in green
        Color rerollColor = GREEN;
        if (GuiButton(actionButtonRect, "Rerrollear Letras"))
        {
            handleRerollLetters(game);
        }
    }

    // Increment Y position
    currentY += actionButtonRect.height + scaled.paddingBetweenElements;

    // Draw invalid move message if needed
    if (isInvalidMove)
    {
        drawInvalidMoveMessage(startX, currentY);
        currentY += scaled.fontSizeTitle + scaled.paddingBetweenElements;
    }

    // Draw "End Game" button
    Rectangle endGameButtonRect = {
        startX,
        currentY,
        scaled.controlAreaWidth - 40 * SCALE_FACTOR,
        40 * SCALE_FACTOR
    };

    if (GuiButton(endGameButtonRect, "Terminar Partida"))
    {
        handleEndGame(game);
    }

    // Increment Y position
    currentY += endGameButtonRect.height + scaled.paddingBetweenElements;

    // Display if a player wants to end the game
    if (game->player1WantsToEnd && !game->player2WantsToEnd)
    {
        DrawTextEx(gameFont, "Player 1 quiere terminar la partida.", (Vector2){startX, currentY}, scaled.fontSizeLarge,
                   1, BLACK);
    }
    else if (!game->player1WantsToEnd && game->player2WantsToEnd)
    {
        DrawTextEx(gameFont, "Player 2 quiere terminar la partida.", (Vector2){startX, currentY}, scaled.fontSizeLarge,
                   1, BLACK);
    }
}

// Draws an invalid move message on the screen
void drawInvalidMoveMessage(float startX, float currentY)
{
    const char* errorMessage = "Movimiento invalido. Intenta de nuevo.";
    DrawTextEx(gameFont, errorMessage, (Vector2){startX, currentY}, scaled.fontSizeTitle, 1, RED);
}

// Function to handle the "Accept" button click
void handleSubmit(Game* game)
{
    if (numPlacedLetters > 0)
    {
        int moveScore = validateAndScoreWords(&clone, lettersCoordinates, numPlacedLetters);

        if (moveScore >= 0)
        {
            // Valid move
            isInvalidMove = false;
            Player* currentPlayer = (clone.turn == Player1) ? &clone.player1 : &clone.player2;
            currentPlayer->score += moveScore;

            // Remove used letters from the player's hand
            removeLettersFromPlayer(currentPlayer, placedLetters);

            // Refill the player's letters
            refillPlayerLetters(&clone.bag, currentPlayer);

            // Reset movement variables
            numPlacedLetters = 0;
            memset(lettersCoordinates, 0, sizeof(lettersCoordinates));
            memset(placedLetters, 0, sizeof(placedLetters));
            selectedIndex = -1;
            currentDirection = Still_None;
            currentAxis = 0;

            // Switch turn to the next player
            switchTurn(&clone);

            // Clone the updated game state
            cloneGame(&clone, game);
        }
        else
        {
            // Invalid move
            isInvalidMove = true;
        }
    }
}

// Function to handle "Reroll Letters" button click
void handleRerollLetters(Game* game)
{
    Player* currentPlayer = (game->turn == Player1) ? &game->player1 : &game->player2;

    // Return letters to the bag
    for (int i = 0; i < MAX_LETTERS; i++)
    {
        if (currentPlayer->letters[i] != '\0')
        {
            game->bag.letters[game->bag.remaining++] = currentPlayer->letters[i];
            currentPlayer->letters[i] = '\0';
        }
    }

    // Refill the player's letters
    refillPlayerLetters(&game->bag, currentPlayer);

    // Switch turn to the next player
    switchTurn(game);
}

// Function to handle "End Game" button click
void handleEndGame(Game* game)
{
    if (game->turn == Player1)
    {
        game->player1WantsToEnd = true;
        switchTurn(game);
    }
    else
    {
        game->player2WantsToEnd = true;
        switchTurn(game);
    }

    // If both players want to end, set the game as over
    if (game->player1WantsToEnd && game->player2WantsToEnd)
    {
        game->gameOver = true;
    }
}

// Draws the entire game state
bool drawGame(Game* game)
{
    if (!game->gameOver)
    {
        if (!beingPlay())
        {
            Player* player = (game->turn == Player1) ? &game->player1 : &game->player2;
            memcpy(playerLetters, player->letters, sizeof(playerLetters));
            cloneGame(game, &clone);
        }

        // Start drawing
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the game board
        drawBoard();

        // Draw the control panel
        drawPanel(game); // Pass the `game` pointer
    }
    else // If the game has ended, show the winner screen
    {
        // Clear the screen
        ClearBackground(RAYWHITE);

        // Determine the winner
        char winnerText[100];
        if (game->player1.score > game->player2.score)
        {
            sprintf(winnerText, "El jugador 1 gana con %d puntos", game->player1.score);
        }
        else if (game->player2.score > game->player1.score)
        {
            sprintf(winnerText, "El jugador 2 gana con %d puntos", game->player2.score);
        }
        else
        {
            sprintf(winnerText, "Empate a %d puntos", game->player1.score);
        }

        // Measure the size of the text
        Vector2 textSize = MeasureTextEx(gameFont, winnerText, scaled.fontSizeTitle, 1);
        Vector2 textPos = {
            (SCREEN_WIDTH - textSize.x) / 2,
            (SCREEN_HEIGHT - textSize.y) / 2
        };

        // Draw the winner text
        DrawTextEx(gameFont, winnerText, textPos, scaled.fontSizeTitle, 1, BLACK);

        // Draw a button to close the game
        Rectangle closeButtonRect = {
            (SCREEN_WIDTH - 200 * SCALE_FACTOR) / 2,
            textPos.y + textSize.y + scaled.paddingBetweenElements,
            200 * SCALE_FACTOR,
            40 * SCALE_FACTOR
        };

        if (GuiButton(closeButtonRect, "Cerrar Juego"))
        {
            return false; // Signal to close the game
        }
    }

    // End drawing
    EndDrawing();

    return true; // Continue the game
}

// Retrieves the starting letter's coordinates
Coordinate getStartingLetter()
{
    return lettersCoordinates[0];
}
