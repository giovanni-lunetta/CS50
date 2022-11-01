#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    //This just prompts the user to input the text that they want assessed and then prints it out.
    string text = get_string("Text: ");
    printf("%s\n", text);

    //These 6 lines of code call the function and prints out the number of letters, words, and sentences.
    int letters = count_letters(text);
    printf("%i letters\n ", letters);

    int words = count_words(text);
    printf("%i words\n ", words);

    int sentences = count_sentences(text);
    printf("%i sentences\n ", sentences);

    //Uses the formula to get the grade level.
    float calculation = (0.0588 * letters / words * 100) - (0.296 * sentences / words * 100) - 15.8;

    //This rounds the calculation to a whole number. Obviously you cant be reading in grade 5.689394735
    int index = round(calculation);

    if (index < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
        return 0;
    }
    else
    {
        printf("Grade %i\n", index);
    }

}

//This function returns the amount of letters in the sentence given by the user.
//It is almost identical to the for loop in scrabble but this time, it uses the built in function isalpha to determine if the letter given is a valid letter and if so it increments the variable by 1.
int count_letters(string text)
{
    int num_of_letters = 0;
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (isalpha(text[i]))
        {
            num_of_letters++;
        }
    }
    return num_of_letters;
}

//This function does the same thing as the function above except this returns the number of words.
//To determine this we claim that every word is seperated by a space, therefore if we see a space, we incrament the variable by 1
//Because it will always give us a word count 1 short we start the variable at 1.
int count_words(string text)
{
    int num_of_words = 1;
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (text[i] == ' ')
        {
            num_of_words++;
        }
    }
    return num_of_words;
}

//This function like the other two does the same thing except count the number of sentences.
//It uses what we delcare ends a sentence, . ! or ? to increment the variable if we see one of them in the sentence.
int count_sentences(string text)
{
    int num_of_sentences = 0;
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            num_of_sentences++;
        }
    }
    return num_of_sentences;
}

//Help from Google
//Help from Anvea on Youtube