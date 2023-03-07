//PROBLEM THUS FAR: file sizes do not match, something is off with the header size
//BMP.bmp file size = 2,794,262
//mirrored.bmp file size = 2,793,600 (or 2,793,654 after manually seek setting by 54)

#include <stdlib.h>
#include <stdio.h>

unsigned int bitsToInt(char * bits);

//Each pixel is 3 bytes, so when you flip a pixel, all three bytes should be together
int main()
{
    FILE * inputStream, *outputStream;
    inputStream = fopen("BMP.bmp", "rb"); // r is interpretted as text, rb is used for images?
    outputStream = fopen("mirrored.bmp", "wb");

    char header[54];
    char * intBuf;
    intBuf = (char *)malloc(4);

    //read the header information (width, height, and size)
    int sizePos, heightPos, widthPos;
    //These numbers are from documentation of header files in the slides
    sizePos = 2;
    heightPos = 22;
    widthPos = 18;
    unsigned int size, height, width;

    //move the file pointer to the position
    //read 4 bytes into the buffer
    fseek(inputStream, sizePos, SEEK_SET);
    fread(intBuf, 4, 1, inputStream);
    size = bitsToInt(intBuf);

    fseek(inputStream, heightPos, SEEK_SET);
    fread(intBuf, 4, 1, inputStream);
    height = bitsToInt(intBuf);
    
    fseek(inputStream, widthPos, SEEK_SET);
    fread(intBuf, 4, 1, inputStream);
    width = bitsToInt(intBuf);

    printf("File size is %d bits\n", size);
    printf("File width is %d bits\n", width);
    printf("File height is %d bits\n", height);

    char * imageBuf;
    imageBuf = malloc(3 * 4200);
    fseek(inputStream, 0, SEEK_SET);
    fread(imageBuf, 54, 1, inputStream);
    fwrite(imageBuf, 54, 1, outputStream);

    char * rowBuf;
    rowBuf = malloc(3 * width);

    int pos = 54;
    int x, y;
    fseek(inputStream, pos, SEEK_SET);
    fseek(outputStream, pos, SEEK_SET);

    for(y=0; y < height; y++)
    {
        fread(rowBuf, 3, width, inputStream);
        for(x = 0; x < width; x++)
        {
            int i;
            for(i = 0; i < 3; i++)
            {
                int from = x * 3 + 1;
                int to = (width - x - 1) * 3 + i;
                imageBuf[to] = rowBuf[from];
            }
        }
        fwrite(imageBuf, 3, width, outputStream);
    }

    fclose(inputStream);
    fclose(outputStream);

    free(intBuf);
    free(imageBuf);
    free(rowBuf);

    return 0;
}

unsigned int bitsToInt(char * bits)
{
    // bits = char[4]['A', 'B', 'C', 'D'];
    // << is the operation to shift by a certain number of bits
    return (unsigned char)bits[0] | ((unsigned char)bits[1] << 8) | ((unsigned char)bits[2] << 16) | ((unsigned char)bits[3] << 24);
    // the '|' (or) operation is used to add the binary values together
}