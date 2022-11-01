#include "helpers.h"
#include <math.h>

//Throughout this code, i refers to rows and j refers to columns.

// Convert image to grayscale
//In order to impliment grayscale we have to look at the three integer values for R G and B and take set all three of them to the average of thos three numbers on the 0 to 255 scale.
//These two for loops (one nested) take each pixel in the array, first converts them to a float, then takes the average, then sets each pixel to the average of the RGB values in that pixel.
//In order to find the average, we first declare three floats and then we use the round function in the math.h library to round those floats to the nearest integer.
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float RED = image[i][j].rgbtRed;
            float GREEN = image[i][j].rgbtGreen;
            float BLUE = image[i][j].rgbtBlue;

            int avg = round((RED + GREEN + BLUE) / 3);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
// A similar process is done here althrough slight more complex. In order to convert a picture to sepia we need to use the algorithm provided to us and apply it to all pixels.
//Therefore, we see the for loop for the rows and then the columns just like above. Except this time, we define the pixels as original red green and blue.
//Then we impliment the equation which is given to us. We run into a problem if the equation outputs a value greater than 255, therefore we have 3 simple if statements that cap the color at 255.
//FInally we update the arrays to be the sepia colors we computed.
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float originalRed = image[i][j].rgbtRed;
            float originalGreen = image[i][j].rgbtGreen;
            float originalBlue = image[i][j].rgbtBlue;

            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

//Reflect image horizontally
//This function I had some trouble with. In pseduo code, what we need to do is somewhat similar to the last but with different implementation. We have the same two for loops, one of them nested to run through and hit each pixel in the array.
//The reason though, that j only runs until width / 2 is because if we do not do this, after we flip flop pixel values, they will put themselves back after half way through the loop.
//Next, we declare a temperary variable with type RGBTRIPLE as defined in the other files given to us, that allows us to store the pixel we want to swap, swap its opposite, then place the pixel in the temp val into its opposite spot.
//We do so by first defining the temp and setting it equal to the pixel. Then we set the first index of the image array as its opposite value.
//Then we set the temp_var which holds the pixel we need to move over equal to the one we just switched.
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp_var = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp_var;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //RGBTRIPLE copy[i][j];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //copy[i][i] = image[i][j];
        }
    }
    return;
}

//Help from Google
//Help from Anvea on Youtube