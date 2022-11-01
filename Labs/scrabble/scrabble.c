#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    //These are simple if statements. If score 1 is greater, then player 1 wins, if its less than score 2, player 2 wins, anything else would mean that the scores are the same resulting in a tie.
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    //Define a variable score to update as we iterate through each index of the input word.
    int score = 0;

    //This interates through the length of the input word one by one and assigns it to its value.
    //In the if statements, because each letter of the alphabet pertains to a certain ASCII value, we must take this into consideration.
    //Because A's ASCII value is 65 in order to map every letter to its respective points in our POINTS array we must subtract 65 from the scored value.
    //For example, B's ASCII value is 66 thereofe to get its corresponding point we subtract B(66) from A(65) and get 1. Thereofre B's SCrabble points correspond with POINTS[1]
    for (int index = 0, length = strlen(word); index < length; index++)
    {
        if (islower(word[index]))
        {
            score += POINTS[word[index] - 'a'];
        }
        else if (isupper(word[index]))
        {
            score += POINTS[word[index] - 'A'];
        }
    }
    return score;
}
