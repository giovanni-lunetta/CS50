#include <cs50.h>
#include <stdio.h>

//i is the row
//j is the column
//n is height

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
        //This for loop repeats the pattern needed. It starts from 0 and prints the number of columns needed to print.
        for (int j = 0; j < n + i + 3; j++)
        {

            //This if statement deals with another pattern.
            //If j(Column) is equal to n(height), if it is equal to n+1 or if the row number plus the column is less than n-1 we print a space.
            if (j == n || j == n + 1 || i + j < n - 1)
            {
                printf(" ");
            }

            //Otherwise, if we no a space is not needed we print a hash.
            else
            {
                printf("#");
            }
        }

        //Just like mario-less we print a new line
        printf("\n");
    }
}