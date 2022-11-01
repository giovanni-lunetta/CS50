#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Ask for height and only allow inputs that are positive (and greater than 0)
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n <= 0 || n > 8);

    //Base for loop creating the number of rows
    for (int i = 0; i < n; i++)
    {
        //Prints the blank spaces needed. It prints n-1 blank spaces which will always be all spaces and one hash on the first line.
        for (int j = n - 1; j > i; j--)

        {
            printf(" ");
        }

        //This for loop prints the hashtags.
        for (int j  = 0; j <= i; j++)

        {
            printf("#");
        }
        
        //Next row
        printf("\n");
    }
}