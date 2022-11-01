#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Defines a string variable 'name' and asks the user to input their name
    string name = get_string("What's your name? ");

    //Returns 'hello' and then their name
    printf("hello, %s\n", name);
}