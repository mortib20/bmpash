#include <stdio.h>      // Standard input/output definitions
#include <stdlib.h>     // Standard library
#include <stdint.h>     // Standard integer types
#include <endian.h>     // Endianness
#include <sys/types.h>  // System data types

struct colorray {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint32_t bfReserved;
    uint32_t bfOffBits;

    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;

    struct colorray* field;
} BMP;

/*
 * Reads a BMP file and returns a pointer to the BMP struct
 */
void readBMP(BMP *bmp, FILE *fp)
{
    fread(&bmp->bfType, sizeof(bmp->bfType), 1, fp);
    bmp->bfType = htobe16(bmp->bfType);

    fread(&bmp->bfSize, sizeof(bmp->bfSize), 1, fp);
    fread(&bmp->bfReserved, sizeof(bmp->bfReserved), 1, fp);
    fread(&bmp->bfOffBits, sizeof(bmp->bfOffBits), 1, fp);

    fread(&bmp->biSize, sizeof(bmp->biSize), 1, fp);
    fread(&bmp->biWidth, sizeof(bmp->biWidth), 1, fp);
    fread(&bmp->biHeight, sizeof(bmp->biHeight), 1, fp);
    fread(&bmp->biPlanes, sizeof(bmp->biPlanes), 1, fp);
    fread(&bmp->biBitCount, sizeof(bmp->biBitCount), 1, fp);
    fread(&bmp->biCompression, sizeof(bmp->biCompression), 1, fp);
    fread(&bmp->biSizeImage, sizeof(bmp->biSizeImage), 1, fp);
    fread(&bmp->biXPelsPerMeter, sizeof(bmp->biXPelsPerMeter), 1, fp);
    fread(&bmp->biYPelsPerMeter, sizeof(bmp->biXPelsPerMeter), 1, fp);
    fread(&bmp->biClrUsed, sizeof(bmp->biClrUsed), 1, fp);
    fread(&bmp->biClrImportant, sizeof(bmp->biClrImportant), 1, fp);

    fseek(fp, bmp->bfOffBits, SEEK_SET);

    size_t RowSize = ((bmp->biBitCount * bmp->biWidth) / 32) * 4;

    printf("Row Size\t%zu\n", RowSize);

    bmp->field = (struct colorray *) malloc(sizeof(struct colorray) * (RowSize * bmp->biHeight));

    for(int i = 0; i < bmp->biHeight * bmp->biWidth; i++) {
        fread(&bmp->field[i].blue, sizeof(uint8_t), 1, fp);
        fread(&bmp->field[i].green, sizeof(uint8_t), 1, fp);
        fread(&bmp->field[i].red, sizeof(uint8_t), 1, fp);
    }
}

/*
 * Prints a BMP file
 */
void printBMPInfo(BMP *bmp)
{
    printf("bfType\t\t%x\n", bmp->bfType);
    printf("bfSize\t\t%i\n", bmp->bfSize);
    printf("bfReserved\t%i\n", bmp->bfReserved);
    printf("bfOffBits\t%i\n", bmp->bfOffBits);

    putchar('\n');

    printf("biSize\t\t%i\n", bmp->biSize);
    printf("biWidth\t\t%i\n", bmp->biWidth);
    printf("biHeight\t%i\n", bmp->biHeight);
    printf("biPlanes\t%i\n", bmp->biPlanes);
    printf("biBitCount\t%i\n", bmp->biBitCount);
    printf("biCompression\t%i\n", bmp->biCompression);
    printf("biSizeImage\t%i\n", bmp->biSizeImage);
    printf("biXPelsPerMeter\t%i\n", bmp->biXPelsPerMeter);
    printf("biYPelsPerMeter\t%i\n", bmp->biYPelsPerMeter);
    printf("biClrUsed\t%i\n", bmp->biClrUsed);
    printf("biClrImportant\t%i\n", bmp->biClrImportant);

    //for(int i = (bmp->biWidth * bmp->biHeight) - 1; i >= 0 ; i--) {
    //    printf("\x1b[38;2;%i;%i;%im█", bmp->field[i].blue, bmp->field[i].green, bmp->field[i].red);
    //
    //    if(i % bmp->biHeight == 0)
    //        putchar('\n');
    //}

    for(int i = ((bmp->biWidth * bmp->biHeight) - 1) - (bmp->biWidth - 1); i >= 0 ; i -= bmp->biWidth) {
        for(int b = i; b < i + bmp->biWidth; b++) {
            printf("\x1b[38;2;%i;%i;%im█", bmp->field[b].red, bmp->field[b].green, bmp->field[b].blue);
        }
        putchar('\n');
    }
}


int main(int argc, char *argv[])
{
    if(argc == 1) {
        printf("Usage: %s 24bmp.bmp\n", argv[0]);
        exit(0);
    }

    FILE *fp = fopen(argv[1], "rb");
    BMP bmp;

    readBMP(&bmp, fp);

    printBMPInfo(&bmp);

    fclose(fp);
    free(bmp.field);

    return 0;
}