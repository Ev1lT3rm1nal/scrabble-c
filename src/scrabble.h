// scrabble.h

#ifndef SCRABBLE_H
#define SCRABBLE_H

#include <stdbool.h>

// --------------------
// Constants and Definitions
// --------------------

// Maximum number of words in the dictionary
#define MAX_WORDS 200000

// Maximum length of a single word
#define MAX_WORD_LENGTH_LOCAL 30

// The size of the game board (15 x 15)
#define LENGTH 15

// Maximum number of letters a player can hold
#define MAX_LETTERS 7

// --------------------
// Enumerations
// --------------------

// Represents the type of multiplier on a board tile
typedef enum
{
    None,
    Center,
    Double_Word,
    Triple_Word,
    Double_Letter,
    Triple_Letter,
} Multiplier;

// Represents the direction of a word placement
typedef enum
{
    Still_None,
    Vertical,
    Horizontal,
} WordDirection;

// Represents the current player's turn
typedef enum
{
    Player1,
    Player2,
} PlayerTurn;

// --------------------
// Structures
// --------------------

// Represents a single tile on the game board
typedef struct Piece
{
    bool is_placed;               // Indicates if a letter has been placed on this tile
    Multiplier multiplier;        // The multiplier type of this tile
    char letter;                  // The letter placed on this tile

    struct Piece* up;             // Pointer to the tile above
    struct Piece* down;           // Pointer to the tile below
    struct Piece* left;           // Pointer to the tile to the left
    struct Piece* right;          // Pointer to the tile to the right
} Piece;

// Represents a player in the game
typedef struct
{
    int score;                     // Player's current score
    char letters[MAX_LETTERS];     // Letters currently held by the player
} Player;

// Represents the game board as a 2D array of Pieces
typedef Piece Board[LENGTH][LENGTH];

// Represents the bag containing all available letters
typedef struct
{
    char letters[100];             // Array of letters in the bag
    int remaining;                 // Number of letters remaining in the bag
} LetterBag;

// Represents the overall game state
typedef struct
{
    Board board;                   // The game board
    Player player1;                // Player 1's data
    Player player2;                // Player 2's data
    PlayerTurn turn;               // Indicates whose turn it is
    LetterBag bag;                 // The letter bag
    bool gameOver;                 // Flag to indicate if the game is over
    bool player1WantsToEnd;        // Indicates if Player 1 wants to end the game
    bool player2WantsToEnd;        // Indicates if Player 2 wants to end the game
} Game;

// Represents a coordinate on the game board
typedef struct
{
    int x;                         // X-coordinate (column)
    int y;                         // Y-coordinate (row)
} Coordinate;

// --------------------
// Function Prototypes
// --------------------

// --------------------
// Board Initialization Functions
// --------------------

// Initializes the game board with default settings and multipliers
void initBoard(Board board);

// Links each piece on the board to its neighboring pieces
void linkPieces(Board board);

// --------------------
// Letter Bag Functions
// --------------------

// Initializes the letter bag with all available letters
void initLetterBag(LetterBag* bag);

// Refills a player's letters from the letter bag
void refillPlayerLetters(LetterBag* bag, Player* player);

// Counts the number of letters in a player's hand
int countPlayerLetters(const Player* player);

// --------------------
// Game State Functions
// --------------------

// Initializes the entire game state
void initGame(Game* game);

// Switches the turn to the next player
void switchTurn(Game* game);

// Checks if the game is over based on the letter bag and players' letters
bool isGameOver(const Game* game);

// Clones the current game state to another game instance
void cloneGame(const Game* original, Game* clone);

// Frees resources used by the game, including the dictionary BST
void freeGame(Game* game);

// --------------------
// Dictionary Functions
// --------------------

// Loads valid words from a file into the Binary Search Tree (BST)
void loadValidWords(const char* filename);

// Checks if a word is valid by searching in the BST
bool isValidWord(const char* word);

// --------------------
// Word Validation and Scoring Functions
// --------------------

// Validates word placements and calculates the total score for the move
int validateAndScoreWords(Game* game, Coordinate placedLetters[], int numPlacedLetters);

// Calculates the score for a single word placement
int calculateScore(Game* game, const char* word, int x, int y, bool horizontal);

// --------------------
// Player Functions
// --------------------

// Removes used letters from the player's hand after a valid move
void removeLettersFromPlayer(Player* player, const char* word);

// Checks if a player has the necessary letters to form a word
bool playerHasLetters(const Player* player, const char* word);

// --------------------
// Helper Functions
// --------------------

// Retrieves the score of a given letter based on Scrabble rules
int getLetterScore(char letter);

#endif // SCRABBLE_H
