#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool only_digits(string s);
char rotate(char p, int k);

int main(int argc, string argv[])
{
    //This first if statement makes sure that the user provides exactly 1 command line arguement.
    //Because the arguement we are looking for is the second arguement, if it is anything but 2 we tell it the correct way.
    //We also have the ||(or) which calls the function we created below and inputs the command line that the user inputed to check if the entirety is a digit.
    if (argc != 2 || !only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int i;
    int key = atoi(argv[1]);
    string plain_text = get_string("plaintext: ");
    int length = strlen(plain_text);
    char cipher[length + 1];

    for (i = 0; i < length; i++)
    {
        cipher[i] = rotate(plain_text[i], key);
    }
    cipher[i] = '\0';

    printf("ciphertext: %s\n", cipher);
    return 0;
}

//This function checks if the input is a digit from 0-9
//It runs through each element in s[i] and checks if each is a digit or not.
//If it is not a digit, hence the !(bang)isdigit which basically means if it is not a digit we print the same error message as before.
bool only_digits(string s)
{
    for (int i = 0, length = strlen(s); i < length; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

//This function uses the algoritm along with the command line input and properly takes each each letter and translates it to the correct letter based on the key.
//The reason that we have to subract and then add the a and A is because the formula checks the algoithm as if it is 0 indexed and then returns it to its proper ASCII index when you add it back.
char rotate(char plain, int key)
{
    char c;
    if (islower(plain))
    {
        c = (plain - 'a' + key) % 26 + 'a';
    }
    else if (isupper(plain))
    {
        c = (plain - 'A' + key) % 26 + 'A';
    }
    else
    {
        c = plain;
    }
    return c;
}

//Help from Google
//Help from Anvea on Youtube