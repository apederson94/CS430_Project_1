#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//recursively cycles through comments
int loopComments(FILE *fh, int done) {
  int character;
  char str[999];

  character = fgetc(fh);

  if (character == 35) {
    fgets(str, 999, fh);
    loopComments(fh, done);
  } else {
    done = 1;
  }

  if (done == 1) {
    ungetc(character, fh);
  }
  return 0;
}

//main function that runs the necessary commands to convert a P3/P6 to a P3/P6
int main(int argc, char const *argv[]) {
  //variables
  FILE *fhIn, *fhOut;
  int fileType, character, width, height, widthSet, heightSet, doneLooping,
  arraySize, number, bit, divisor, tracker;
  char *convertTo, *conversionData, maxColor[10], *w, *h, *wh;

  //allocates space for the conversion type argument
  convertTo = (char *) malloc(sizeof(argv[1]));

  //variable initial setup
  fhIn = fopen(argv[2], "r");
  convertTo = argv[1];
  character = fgetc(fhIn);
  widthSet = 0;
  doneLooping = 0;

  //determining file type (P3/P6)
  if (character == 80) {
    character = fgetc(fhIn);
    if (character == 51 || character == 54) {
      fileType = character;
    }
  }

  if (fileType == 54) {
    fclose(fhIn);
    fhIn = fopen(argv[2], "rb");
    fgetc(fhIn);
    fgetc(fhIn);
  }

  //moves file pointer to next line
  fgetc(fhIn);

  //reads header and extracts width and height data
  while (widthSet == 0 || heightSet == 0) {
    loopComments(fhIn, doneLooping);

    if (widthSet == 0) {

      fscanf(fhIn, "%d", &width);
      widthSet = 1;

      fgetc(fhIn);
    } else if (heightSet == 0) {
      fscanf(fhIn, "%d", &height);
      heightSet = 1;

    }
  }

  //allocating memory for all of the data in the ppm
  arraySize = 3 * width * height * sizeof(int);
  if (fileType == 51) {
    arraySize *= sizeof(int);
  }
  conversionData = (char *) malloc(arraySize);

  //reading in max color
  fgets(maxColor, 4, fhIn);
  if (fileType == 54) {
    fgets(maxColor, 4, fhIn);
    strcat(maxColor, "\n");
  }

  //reads all data into an array
  if (fileType == 51) {
    //character = fgetc(fhIn);
    for (int i = 0; i < arraySize; i++) {
     character = fgetc(fhIn);
     conversionData[i] = character;

    }
 } else if (fileType == 54) {
   fgetc(fhIn);
    fread(conversionData, sizeof(int), arraySize, fhIn);
 }

//closing in file and opening out file
fclose(fhIn);
fhOut = fopen(argv[3], "w");

//setting up string width and height
w = (char *) malloc(sizeof(width));
h = (char *) malloc(sizeof(height));

sprintf(w, "%d ", width);
sprintf(h, "%d\n", height);

wh = (char *) malloc(sizeof(w) + sizeof(h));

strcat(wh, w);
strcat(wh, h);

free(w);
free(h);
printf("Array size is: %d\n", arraySize);
//writing to P3 from P3
if (*convertTo == 51) {
  //writing header
  fputs("P3\n", fhOut);
  fputs(wh, fhOut);
  fputs(maxColor, fhOut);
  //P3 to P3
  if (fileType == 51) {
    fwrite(conversionData, sizeof(char), arraySize, fhOut);
    //P6 to P3
  } else if (fileType == 54) {
    //TODO: P6 to P3 conversion
    //moving binary ints 4 digits apart to make room for characters later
    for (int y = (arraySize - sizeof(int)); y >= 0; y-=4) {
      conversionData[y] = conversionData[y/4];
    }
    //converting binary ints and separating them into individual chars
    for (int i = 0; i < arraySize; i+=4) {
      number = conversionData[i] & 0xFF;
      divisor = 100;
      for (int z = 0; z < 3; z++) {
        character = floor(number/divisor);
        if (i == 0 || i == 4) {
          printf("-----------------------------\nI is: %d   Z is: %d\n"
          "-----------------------------\n"
          "Character is: %c\nNumber is: %d\nDivisor is: %d\n"
          "Number Character is: %c\n"
          , i, z, character, number, divisor, character +'0');
        }
        number -= character * divisor;
        divisor /= 10;
        conversionData[i+z] = character + '0';

      }
      conversionData[i + 3] = '\n';
    }
    fwrite(conversionData, sizeof(int), arraySize, fhOut);
  }
} else if (*convertTo == 54) {
  //writing header
  fputs("P6\n", fhOut);
  fputs(wh, fhOut);
  fputs(maxColor, fhOut);
  //P6 to P6
  if (fileType == 54) {
    fwrite(conversionData, sizeof(int), arraySize, fhOut);
    //P3 to P6
  } else if (fileType == 51) {
    //TODO: P3 to P6 conversion
  }

}
free(conversionData);
free(wh);
fclose(fhOut);
  return 0;
}
