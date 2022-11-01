// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

//Declare variables
unsigned int wordcounter;


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hash_val = hash(word);

    node *cursor = table[hash_val];

    //This while loop will loop all the way until the end of the linked list.
    while (cursor != NULL)
    {
        //Check if the words are the same, aka compare two strings which is what the built in function does.
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }

        //If it does not match, we go to the next node.
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
//For this function, I referenced Aneva on Youtube to help with the solution.
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned long t = 0;
    for (int i = 0, length = strlen(word); i < length; i++)
    {
        t += tolower(word[i]);
    }
    return t % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    //All this line of code does it open the file.
    FILE *d_file = fopen(dictionary, "r");

    //Next, we need to return null if the file cannot be opened.
    if (d_file == NULL)
    {
        printf("File cannot be opened %s\n", dictionary);
        return false;
    }

    //This while loop uses fscanf to scan all files until the end of a file is reached aka EOF which stands for End Of File.
    //The char variable we have here will actually store the word that we are checking.
    //Then we need to allocate memory for additional nodes as we iterate through the while loop.
    //Next, if the malloc returns NULL, we must return false.
    char the_word[LENGTH + 1];

    while (fscanf(d_file, "%s", the_word) != EOF)
    {
        node *temp_var = malloc(sizeof(node));

        if (temp_var == NULL)
        {
            return false;
        }

        //Here we are copying the word into the node using the built in function string copy.
        strcpy(temp_var->word, the_word);

        //This is using the has function that we will impliment later.
        int hash_num = hash(the_word);

        //This if and else statement check to make sure the linked list is working correctly.
        if (table[hash_num] == NULL)
        {
            temp_var->next = NULL;
        }
        else
        {
            temp_var->next = table[hash_num];
        }

        //This points the header back to the temp_var
        table[hash_num] = temp_var;
        wordcounter++;
    }
    fclose(d_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (wordcounter > 0)
    {
        return wordcounter;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
//This function will go through the linked list and move the cursor and then free the bin, one by one.
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor)
        {
            node *temp_var = cursor;
            cursor = cursor->next;
            free(temp_var);
        }
        if (cursor == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}

//Help from Google
//Help from Anvea on Youtube