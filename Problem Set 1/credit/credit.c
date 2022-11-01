#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long card_num;
    do
    {
        card_num = get_long("Card number: ");
    }
    while (card_num < 0);

    //We need to start from the second to last digit and go backwards. Therefore, the first variable is the second to last digit, the second variable is the fourth to last digit, etc.
    int c1, c2, c3, c4, c5, c6, c7, c8;

    //Here we are accessing the proper index of card_num and declaring it as c1-c8.
    //We use modulo because the remainder of modulo is always the last digit of the number given.
    //In order to skip to every second number, we must add two 0's to make the jump. We then multiply by 2 as per Luhn’s Algorithm
    c1 = ((card_num % 100) / 10) * 2;
    c2 = ((card_num % 10000) / 1000) * 2;
    c3 = ((card_num % 1000000) / 100000) * 2;
    c4 = ((card_num % 100000000) / 10000000) * 2;
    c5 = ((card_num % 10000000000) / 1000000000) * 2;
    c6 = ((card_num % 1000000000000) / 100000000000) * 2;
    c7 = ((card_num % 100000000000000) / 10000000000000) * 2;
    c8 = ((card_num % 10000000000000000) / 1000000000000000) * 2;

    //The next task of Luhn’s Algorithm is to add together the digits of the previous product. In order to do that we must again use modulo.
    //Here, we take each variable which has been assigned to a number in the above lines of code and seperate the digits.
    //For eample of c5 is 14 then we use modulo to seperate the 1 and the 4 using the same logic as above.
    c1 = (c1 % 100 / 10) + (c1 % 10);
    c2 = (c2 % 100 / 10) + (c2 % 10);
    c3 = (c3 % 100 / 10) + (c3 % 10);
    c4 = (c4 % 100 / 10) + (c4 % 10);
    c5 = (c5 % 100 / 10) + (c5 % 10);
    c6 = (c6 % 100 / 10) + (c6 % 10);
    c7 = (c7 % 100 / 10) + (c7 % 10);
    c8 = (c8 % 100 / 10) + (c8 % 10);

    //As per Luhn’s Algorithm we must now add the indidual digits together.
    //s = sum
    int s1 = c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8;

    //Now we have to deal with the remaining digits. To do so, we have to first declare the remaining 8 numbers in the card.
    int c9, c10, c11, c12, c13, c14, c15, c16;

    //This process is the same as before. In order to properly index through the card number, we have to use modulo.
    //This time, according to Luhn’s Algorithm, we just add the remaining numbers to the first sum that we got, that is why we are not multiplying by 2.
    c9 = (card_num % 10);
    c10 = ((card_num % 1000) / 100);
    c11 = ((card_num % 100000) / 10000);
    c12 = ((card_num % 10000000) / 1000000);
    c13 = ((card_num % 1000000000) / 100000000);
    c14 = ((card_num % 100000000000) / 10000000000);
    c15 = ((card_num % 10000000000000) / 1000000000000);
    c16 = ((card_num % 1000000000000000) / 100000000000000);

    //This step similar to above sums all the remaining digits in the card number, but not to the previous total.
    int s2 = c9 + c10 + c11 + c12 + c13 + c14 + c15 + c16;

    //This sum is the final sum. It sums the two above sums to give us a number that we must check through Luhn’s Algorithm.
    int s3 = s1 + s2;

    //Now we must check if the sum is divisible by 10. If so, it is a valid card, if not it is invalid.
    //This process is simple. Just a if statement that makes this decision for us and stops if invalid is returned, hence the return 0 code.
    //These new variables are the ones we are going to use to identify which type of credit card this is.
    int l = 0;
    long visa = card_num;
    long amex = card_num;
    long master = card_num;

    if (s3 % 10 != 0)
    {
        printf("%s\n", "INVALID");
        return 0;
    }

    //To begin, this while loop will give us the length of the card. This is an important step as different cards have a different number of numbers in them.
    //This while loop works by dividing by 10 and getting incramenting l by 1 until l is no longer divisible by 10 where it will exit the loop, leaving us with the length.
    while (card_num > 0)
    {
        card_num = card_num / 10;
        l++;
    }

    //Each of these three processes are the same but with different restrictions. We know the restrictions on visa are that the card starts with a 4 and has a length of 13 or 14.
    //The while loop does the same thing as the one above except this one outputs the first digit of the card by dividing by 10 until it cannot anymore.
    //The if statement then checks conditions, using || as or and && as and. If these conditions are satisfied we print that it is a VISA on a new line.
    //The reason for return 0 is to stop the code after a output is printer. Without this, the code would run the next if statement which we do not need it to do because we know...
    //it can only be one type of card.
    while (visa >= 10)
    {
        visa /= 10;
    }

    if ((l == 13 || l == 16) && visa == 4)
    {
        printf("%s\n", "VISA");
        return 0;
    }

    while (amex >= 10000000000000)
    {
        amex /= 10000000000000;
    }

    if (l == 15 && (amex == 34 || amex == 37))
    {
        printf("%s\n", "AMEX");
        return 0;
    }

    while (master >= 100000000000000)
    {
        master /= 100000000000000;
    }

    if (l == 16 && (master == 51 || master == 52 || master == 53 || master == 54 || master == 55))
    {
        printf("%s\n", "MASTERCARD");
        return 0;
    }

    else
    {
        printf("%s\n", "INVALID");
        return 0;
    }

}