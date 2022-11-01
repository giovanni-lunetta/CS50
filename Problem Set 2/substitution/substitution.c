#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    //Similar to the caesar code, we need to make sure that the second command line input is the last input given.
    //Therefore, if we see a command line input after argc 2 then we print the error message.
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //This for loop checks the second condition that the command line input at index 1 must be an letter in the alphabet.
    //The function isalpha does this easily for us.
    string k = argv[1];
    int length1 = strlen(k);
    for (int i = 0; i < length1; i++)
    {
        if (!isalpha(k[i]))
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }

    //This if statement simply checks if the length of the key, k, is 26 or not. If not, we return the same error message.
    if (length1 != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //The next step here is checking to make sure that there are no dublicates in the input. If Im using two new variables p and c so i do not get confused with the previous for loop.
    //Because we need to check both lower and upper case letters, we cast them all to lower case which will achieve uniformity and ease of coding.
    //The reason we check p+1 is because we need to check the next indexed letter of the one we are checking.
    for (int p = 0; p < length1; p++)
    {
        for (int c = p + 1; c < length1; c++)
        {
            if (tolower(k[p]) == tolower(k[c]))
            {
                printf("Usage: ./substitution key\n");
                return 1;
            }
        }
    }
    string plain_text = get_string("plaintext: ");

    //This for loop runs through the length of the key and makes every letter that is uppercase into lowercase in order to easily handle the command line input.
    //We add 32 to k[i] because A's ASCII value is 65 and a's is 97. Therefore, 65+32=97, and this corresponds to every letter.
    for (int i = 0; i < length1; i++)
    {
        if (isupper(k[i]))
        {
            k[i] = k[i] + ' ';
        }
    }

    printf("ciphertext: ");

    int length2 = strlen(plain_text);

    //This final for loop goes through the final three cases to convert the plain text to cipher text.
    //The first if statement asks if the index of plain_text is upper case. If so, we have to convert it to the new index of the plain text and then print out that letter - 32 because we added 32 to make all of them lower case in the first place.
    //The second if statement does the same thing but for lower case letter, and the final else just returns back the plain text letter if it doesnt need to be changed.
    for (int i = 0; i < length2; i++)
    {
        if (isupper(plain_text[i]))
        {
            int letter = plain_text[i] - 'A';
            printf("%c", k[letter] - ' ');
        }
        else if (islower(plain_text[i]))
        {
            int letter = plain_text[i] - 'a';
            printf("%c", k[letter]);
        }
        else
        {
            printf("%c", plain_text[i]);
        }
    }
    printf("\n");
}

//Help from Google
//Help from Anvea on Youtube