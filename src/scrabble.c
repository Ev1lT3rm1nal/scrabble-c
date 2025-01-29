// scrabble.c

#include "scrabble.h"
#include "arbol_diccionario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Root node of the dictionary BST
Node* dictionaryRoot = NULL;

// --------------------
// Dictionary Functions
// --------------------

// Loads valid words from a file into the Binary Search Tree (BST)
void loadValidWords(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening dictionary file");
        exit(EXIT_FAILURE);
    }
    char word[MAX_WORD_LENGTH_LOCAL];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Convert the word to lowercase
        for (int i = 0; word[i]; i++)
        {
            word[i] = tolower(word[i]);
        }
        // Insert the word into the BST
        dictionaryRoot = insert(dictionaryRoot, word);
    }
    fclose(file);
}

// Checks if a word is valid by searching in the BST
bool isValidWord(const char* word)
{
    return search(dictionaryRoot, word);
}

// Frees the memory allocated for the game, including the BST
void freeGame(Game* game)
{
    // Free other resources if necessary
    freeTree(dictionaryRoot);
}

// --------------------
// Board Initialization
// --------------------

// Links each piece on the board to its neighboring pieces
void linkPieces(Board board)
{
    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            board[i][j].up = (i > 0) ? &board[i - 1][j] : NULL;
            board[i][j].down = (i < LENGTH - 1) ? &board[i + 1][j] : NULL;
            board[i][j].left = (j > 0) ? &board[i][j - 1] : NULL;
            board[i][j].right = (j < LENGTH - 1) ? &board[i][j + 1] : NULL;
        }
    }
}

// Initializes the game board with default settings and multipliers
void initBoard(Board board)
{
    // Initialize all tiles on the board
    for (int i = 0; i < LENGTH; i++)
    {
        for (int j = 0; j < LENGTH; j++)
        {
            board[i][j].is_placed = false;
            board[i][j].multiplier = None;
            board[i][j].letter = '\0';
        }
    }

    // Link each tile to its neighbors
    linkPieces(board);

    // Set the center tile multiplier
    board[7][7].multiplier = Center;

    // Define and set multipliers based on official Scrabble positions

    // 1. Triple Word (TW) positions
    int tripleWordPositions[][2] = {
        {0, 0}, {0, 7}, {0, 14},
        {7, 0}, {7, 14},
        {14, 0}, {14, 7}, {14, 14}
    };
    for (int i = 0; i < 8; i++)
    {
        int x = tripleWordPositions[i][0];
        int y = tripleWordPositions[i][1];
        board[x][y].multiplier = Triple_Word;
    }

    // 2. Double Word (DW) positions
    int doubleWordPositions[][2] = {
        {1, 1}, {1, 13},
        {2, 2}, {2, 12},
        {3, 3}, {3, 11},
        {4, 4}, {4, 10},
        {10, 4}, {10, 10},
        {11, 3}, {11, 11},
        {12, 2}, {12, 12},
        {13, 1}, {13, 13}
    };
    for (int i = 0; i < 16; i++)
    {
        int x = doubleWordPositions[i][0];
        int y = doubleWordPositions[i][1];
        board[x][y].multiplier = Double_Word;
    }

    // 3. Triple Letter (TL) positions
    int tripleLetterPositions[][2] = {
        {0, 5}, {0, 9},
        {5, 0}, {5, 5}, {5, 9}, {5, 14},
        {9, 0}, {9, 5}, {9, 9}, {9, 14},
        {14, 5}, {14, 9}
    };
    for (int i = 0; i < 12; i++)
    {
        int x = tripleLetterPositions[i][0];
        int y = tripleLetterPositions[i][1];
        board[x][y].multiplier = Triple_Letter;
    }

    // 4. Double Letter (DL) positions
    int doubleLetterPositions[][2] = {
        {0, 3}, {0, 11},
        {2, 6}, {2, 8},
        {3, 0}, {3, 7}, {3, 14},
        {6, 2}, {6, 6}, {6, 8}, {6, 12},
        {7, 3}, {7, 11},
        {8, 2}, {8, 6}, {8, 8}, {8, 12},
        {11, 0}, {11, 7}, {11, 14},
        {12, 6}, {12, 8},
        {14, 3}, {14, 11}
    };
    for (int i = 0; i < 24; i++)
    {
        int x = doubleLetterPositions[i][0];
        int y = doubleLetterPositions[i][1];
        board[x][y].multiplier = Double_Letter;
    }
}

// --------------------
// Letter Bag Functions
// --------------------

// Initializes the letter bag with all available letters
void initLetterBag(LetterBag* bag)
{
    strcpy(bag->letters,
           "EEEEEEEEEEEEAAAAAAAAAIIIIIIIII"
           "OOOOOOOOONNNNNNRRRRRR"
           "TTTTTTLLLLSSSSUUUUDDDD"
           "GGGGBBCCMMPPFFHHVVWWYYKJXQZ");
    bag->remaining = strlen(bag->letters);
}

// Refills a player's letters from the letter bag
void refillPlayerLetters(LetterBag* bag, Player* player)
{
    for (int i = 0; i < MAX_LETTERS; i++)
    {
        if (player->letters[i] == '\0')
        {
            if (bag->remaining > 0)
            {
                int index = rand() % bag->remaining;
                player->letters[i] = bag->letters[index];
                // Remove the selected letter from the bag
                bag->letters[index] = bag->letters[bag->remaining - 1];
                bag->remaining--;
            }
            else
            {
                player->letters[i] = '\0';
            }
        }
    }
}

// Counts the number of letters in a player's hand
int countPlayerLetters(const Player* player)
{
    int count = 0;
    for (int i = 0; i < MAX_LETTERS; i++)
    {
        if (player->letters[i] != '\0')
        {
            count++;
        }
    }
    return count;
}

// --------------------
// Game State Functions
// --------------------

// Initializes the entire game
void initGame(Game* game)
{
    // Initialize the game board
    initBoard(game->board);

    // Initialize the letter bag
    initLetterBag(&game->bag);

    // Set the initial turn to Player 1
    game->turn = Player1;

    // Clear players' letters
    memset(game->player1.letters, '\0', MAX_LETTERS);
    memset(game->player2.letters, '\0', MAX_LETTERS);

    // Refill players' letters from the bag
    refillPlayerLetters(&game->bag, &game->player1);
    refillPlayerLetters(&game->bag, &game->player2);

    // Initialize player scores and game state
    game->player1.score = 0;
    game->player2.score = 0;
    game->gameOver = false;
    game->player1WantsToEnd = false;
    game->player2WantsToEnd = false;

    // Load valid words into the dictionary BST
    loadValidWords("palabras.txt");
}

// Switches the turn to the next player
void switchTurn(Game* game)
{
    game->turn = (game->turn == Player1) ? Player2 : Player1;
}

// Checks if the game is over based on the letter bag and players' letters
bool isGameOver(const Game* game)
{
    return game->bag.remaining == 0 &&
           countPlayerLetters(&game->player1) == 0 &&
           countPlayerLetters(&game->player2) == 0;
}

// Clones the current game state to another game instance
void cloneGame(const Game* original, Game* clone)
{
    if (original == NULL || clone == NULL)
    {
        return;
    }
    *clone = *original;
    linkPieces(clone->board);
}

// --------------------
// Word Validation and Scoring
// --------------------

// Validates if a player has the necessary letters to form a word
bool playerHasLetters(const Player* player, const char* word)
{
    char tempLetters[MAX_LETTERS];
    memcpy(tempLetters, player->letters, MAX_LETTERS);

    int wordLength = strlen(word);
    for (int i = 0; i < wordLength; i++)
    {
        bool found = false;
        for (int j = 0; j < MAX_LETTERS; j++)
        {
            if (tempLetters[j] == word[i])
            {
                tempLetters[j] = '\0';
                found = true;
                break;
            }
        }
        if (!found)
        {
            // Player does not have the required letter
            return false;
        }
    }
    return true;
}

// Removes used letters from the player's hand after a valid move
void removeLettersFromPlayer(Player* player, const char* word)
{
    int wordLength = strlen(word);
    for (int i = 0; i < wordLength; i++)
    {
        for (int j = 0; j < MAX_LETTERS; j++)
        {
            if (player->letters[j] == word[i])
            {
                player->letters[j] = '\0';
                break;
            }
        }
    }
}

// Determines the score of a single word placement
int calculateWordScore(Game* game, const char* word, int x, int y, bool horizontal)
{
    int wordScore = 0;
    int wordMultiplier = 1;
    int wordLength = strlen(word);

    for (int i = 0; i < wordLength; i++)
    {
        int xi = horizontal ? x + i : x;
        int yi = horizontal ? y : y + i;

        char letter = game->board[yi][xi].letter;
        int letterScore = getLetterScore(letter);

        Multiplier multiplier = game->board[yi][xi].multiplier;

        // Apply multipliers only for newly placed letters
        switch (multiplier)
        {
        case Double_Letter:
            letterScore *= 2;
            break;
        case Triple_Letter:
            letterScore *= 3;
            break;
        case Double_Word:
            wordMultiplier *= 2;
            break;
        case Triple_Word:
            wordMultiplier *= 3;
            break;
        case Center:
        default:
            break;
        }

        wordScore += letterScore;
    }

    wordScore *= wordMultiplier;

    return wordScore;
}

// Validates word placements and calculates the total score for the move
int validateAndScoreWords(Game* game, Coordinate placedLetters[], int numPlacedLetters)
{
    int totalScore = 0;
    Piece (*board)[15] = game->board;
    bool validMove = true;

    // Keep track of words already scored to avoid duplicates
    char scoredWords[100][MAX_WORD_LENGTH_LOCAL];
    int scoredWordCount = 0;

    for (int i = 0; i < numPlacedLetters; i++)
    {
        int x = placedLetters[i].x;
        int y = placedLetters[i].y;

        // Check horizontally
        int xStart = x, xEnd = x;

        // Move to the leftmost character
        while (xStart > 0 && board[y][xStart - 1].is_placed)
        {
            xStart--;
        }
        // Move to the rightmost character
        while (xEnd < LENGTH - 1 && board[y][xEnd + 1].is_placed)
        {
            xEnd++;
        }

        int wordLength = xEnd - xStart + 1;
        if (wordLength > 1)
        {
            // Extract the word
            char word[MAX_WORD_LENGTH_LOCAL];
            for (int xi = xStart, idx = 0; xi <= xEnd; xi++, idx++)
            {
                word[idx] = board[y][xi].letter;
            }
            word[wordLength] = '\0';

            // Check if the word has already been scored
            bool alreadyScored = false;
            for (int s = 0; s < scoredWordCount; s++)
            {
                if (strcmp(scoredWords[s], word) == 0)
                {
                    alreadyScored = true;
                    break;
                }
            }

            if (!alreadyScored)
            {
                // Convert word to lowercase for dictionary lookup
                char lowerWord[MAX_WORD_LENGTH_LOCAL];
                for (int k = 0; k < wordLength; k++)
                {
                    lowerWord[k] = tolower(word[k]);
                }
                lowerWord[wordLength] = '\0';

                // Validate the word
                if (!isValidWord(lowerWord))
                {
                    validMove = false;
                    break;
                }

                // Calculate the score
                int wordScore = calculateWordScore(game, word, xStart, y, true);
                totalScore += wordScore;

                // Add the word to the scoredWords array
                strcpy(scoredWords[scoredWordCount++], word);
            }
        }

        // Check vertically
        int yStart = y, yEnd = y;
        // Move to the topmost character
        while (yStart > 0 && board[yStart - 1][x].is_placed)
        {
            yStart--;
        }
        // Move to the bottommost character
        while (yEnd < LENGTH - 1 && board[yEnd + 1][x].is_placed)
        {
            yEnd++;
        }

        wordLength = yEnd - yStart + 1;
        if (wordLength > 1)
        {
            // Extract the word
            char word[MAX_WORD_LENGTH_LOCAL];
            for (int yi = yStart, idx = 0; yi <= yEnd; yi++, idx++)
            {
                word[idx] = board[yi][x].letter;
            }
            word[wordLength] = '\0';

            // Check if the word has already been scored
            bool alreadyScored = false;
            for (int s = 0; s < scoredWordCount; s++)
            {
                if (strcmp(scoredWords[s], word) == 0)
                {
                    alreadyScored = true;
                    break;
                }
            }

            if (!alreadyScored)
            {
                // Convert word to lowercase for dictionary lookup
                char lowerWord[MAX_WORD_LENGTH_LOCAL];
                for (int k = 0; k < wordLength; k++)
                {
                    lowerWord[k] = tolower((unsigned char)word[k]);
                }
                lowerWord[wordLength] = '\0';

                // Validate the word
                if (!isValidWord(lowerWord))
                {
                    validMove = false;
                    break;
                }

                // Calculate the score
                int wordScore = calculateWordScore(game, word, x, yStart, false);
                totalScore += wordScore;

                // Add the word to the scoredWords array
                strcpy(scoredWords[scoredWordCount++], word);
            }
        }
    }

    if (!validMove)
    {
        return -1; // Indicate that the move is invalid
    }

    return totalScore;
}

// --------------------
// Letter Scoring
// --------------------

// Helper function to get the score of a letter
int getLetterScore(char letter)
{
    switch (toupper(letter))
    {
    case 'A':
    case 'E':
    case 'I':
    case 'O':
    case 'U':
    case 'L':
    case 'N':
    case 'S':
    case 'T':
    case 'R':
        return 1;
    case 'D':
    case 'G':
        return 2;
    case 'B':
    case 'C':
    case 'M':
    case 'P':
        return 3;
    case 'F':
    case 'H':
    case 'V':
    case 'W':
    case 'Y':
        return 4;
    case 'K':
        return 5;
    case 'J':
    case 'X':
        return 8;
    case 'Q':
    case 'Z':
        return 10;
    default:
        return 0;
    }
}
