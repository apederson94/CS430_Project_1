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

/*int conversionFunction(FILE *fh, char *dataArray, int conversionFrom, char *conversionTo,
 int counter) {
  if (conversionFrom == 51) {
    fputs(&dataArray[counter], fh);
    counter++;
    if (counter >= sizeof(dataArray)) {
      return 0;
    }
  }
  conversionFunction(fh, dataArray, conversionFrom, conversionTo, counter);
}*/


//main function that runs the necessary commands to convert a P3/P6 to a P3/P6
int main(int argc, char const *argv[]) {
  //variables
  FILE *fhIn, *fhOut;
  int fileType, character, width, height, widthSet, heightSet, doneLooping, arraySize, pixelValue, x;
  char *convertTo, *conversionData, str[999], maxColor[10], num[4], *w, *h, *wh;

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
  conversionData = (char *) malloc(arraySize);

  //reading in max color
  fgets(maxColor, 4, fhIn);

  //reads all data into an array
  if (fileType == 51) {
    //character = fgetc(fhIn);
    for (int i = 0; i < arraySize; i++) {
     character = fgetc(fhIn);
     conversionData[i] = character;

    }
 } else if (fileType == 54) {
    //TODO: P6 read
 }

//closing in file and opening out file
fclose(fhIn);
fhOut = fopen(argv[3], "w");

//setting up string width and height
w = (char *) malloc(sizeof(width));
h = (char *) malloc(sizeof(height));

//writing to P3 from P3
if (*convertTo == 51) {

  sprintf(w, "%d ", width);
  sprintf(h, "%d", height);

  wh = (char *) malloc(sizeof(w) + sizeof(h));

  strcat(wh, w);
  strcat(wh, h);

  //writing header
  fputs("P3\n", fhOut);
  fputs(wh, fhOut);
  fputs(maxColor, fhOut);


  printf("%s", "array size is: ");
  printf("%d\n", arraySize);
  x = 0;
  fwrite(conversionData, sizeof(char), arraySize, fhOut);
  //conversionFunction(fhOut, conversionData, fileType, convertTo, x);
  fclose(fhOut);
} else if (*convertTo == 54) {
  //TODO: P6 write
}


  return 0;
}
