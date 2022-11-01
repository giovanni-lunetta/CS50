#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    //This first step is the same as code we have done before. If the user does not input only one command line arguement we print this error.
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //This line of code opens the file that the user provides in the command line.
    FILE *input = fopen(argv[1], "r");

    //If that file name is invalid, aka input == null, we tell the person.
    if (input == NULL)
    {
        printf("Image cannot be opened for reading.\n");
        return 1;
    }

    //Here, I am used the defined byte of 8 bits and creating a buffer every 512 bytes in order based on the size of the JPEG.
    BYTE buffer[BLOCK_SIZE];

    //In order to keep track of the number of images that we are generating, we must have some sort of counter, which is exactly what this variable is. It will also be used as the file name for the images that we end up recovering.
    int counter = 0;

    //As we go through the imput files to find JPEGS we will have to write them into an output file. We are using pointers here because we want to use the adress of the input file.
    //It is declared though, as NULL because we have not done anything yet.
    FILE *output = NULL;

    //We need some way to make space for the chars we are creating. Therefore, we use MALLOC to give us space.
    //The paramaters for malloc here are 8 * sizeof(char) because 1 byte = 8 bits aka the size of a char multiplied by the size of the unassigned char.
    //In other words, depending on how large the JPEG file is that is how much space we are going to need.
    char *fname = malloc(8 * sizeof(char));

    //From hints
    while (fread(buffer, sizeof(char), BLOCK_SIZE, input))
    {
        //From hints based on what the beginning of a JPG looks like. If we see the beginning of a JPEG we go through until we see the beginning of another. We increase the counter by 1, and allocate enough memory to fit the previous.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            sprintf(fname, "%03i.jpg", counter);

            output = fopen(fname, "w");

            counter++;
        }

        if (output != NULL)
        {
            fwrite(buffer, sizeof(char), BLOCK_SIZE, output);
        }
    }

    //We must close the the input and output files along with free the allocated space after we use it.
    free(fname);
    fclose(output);
    fclose(input);
}

//Help from Google
//Help from Anvea on Youtube