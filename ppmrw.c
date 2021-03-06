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

int checkExtension(char *fileName) {
  char character;
  character = fileName[strlen(fileName) - 1];
  if (!(character == 'm')) {
    return -1;
  }
  character = fileName[strlen(fileName) - 2];
  if (!(character == 'p')) {
    return -1;
  }
  character = fileName[strlen(fileName) - 3];
  if (!(character == 'p')) {
    return -1;
  }
  character = fileName[strlen(fileName) - 4];
  if (!(character == '.')) {
    return -1;
  }
}

//main function that runs the necessary commands to convert a P3/P6 to a P3/P6
int main(int argc, char const *argv[]) {
  //variables
  FILE *fhIn, *fhOut;
  int fileType, character, width, height, widthSet, heightSet, doneLooping,
  arraySize, number, bit, divisor, accumulator, tracker, x;
  char *convertTo, *conversionData, *convertedData, slew[4], maxColor[10], *stringNumber, *w, *h, *wh, *outFile, *inFile;

  //check for correct variable count
  if (argc != 4) {
    perror("Error: Incorrect number of parameters provided. Please provide 3 parameters: converted file type, input file, and output file.\n");
    exit(EXIT_FAILURE);
  }

  //allocates space for the conversion type argument
  convertTo = (char *) malloc(sizeof(argv[1]));
  inFile = (char *) malloc(sizeof(argv[2]));
  outFile = (char *) malloc(sizeof(argv[3]));

  inFile = argv[2];
  outFile = argv[3];

  //checks to make sure file has a .ppm extension
  x = checkExtension(inFile);
  if (x == -1) {
    perror("Error: Input filename must be of the type .ppm. Please retry with the correct input.");
    exit(EXIT_FAILURE);
  }
  x = checkExtension(outFile);
  if (x == -1) {
    perror("Error: Output filename must be of the type .ppm. Please retry with the correct input.");
    exit(EXIT_FAILURE);
  }

  //check if input file exists
  if (!fopen(argv[2], "r")) {
    perror("Error: Input file does not exist. Please make sure it exists and try again.\n");
    exit(EXIT_FAILURE);
  }

  //variable initial setup
  fhIn = fopen(argv[2], "r");
  convertTo = argv[1];
  character = fgetc(fhIn);
  widthSet = 0;
  doneLooping = 0;

  if (*convertTo != 51 && *convertTo != 54) {
    perror("Error: Conversion type must be either type 3 or 6. Please try again.\n");
    exit(EXIT_FAILURE);
  }


  //determining file type (P3/P6)
  if (character == 80) {
    character = fgetc(fhIn);
    if (character == 51 || character == 54) {
      fileType = character;
    }
  } else {
    perror("Error: Input file is not a .ppm file. Please reattempt with a .ppm file.");
    exit(EXIT_FAILURE);
  }

  //If P6, open in binary format and toss first 2 chars
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
  conversionData = (char *) malloc(arraySize);

  //reading in max color
  fgets(maxColor, 4, fhIn);
  if (fileType == 54) {
    fgets(maxColor, 4, fhIn);
    strcat(maxColor, "\n");
  } else if (*convertTo == 54) {
    fgets(maxColor, 4, fhIn);
    strcat(maxColor, "\n");
    fgetc(fhIn);
  }

  //reads all data into an array
  if (fileType == 51) {
    for (int i = 0; i < arraySize; i++) {
     character = fgetc(fhIn);
     conversionData[i] = character;
    }
 } else if (fileType == 54) {
   //tossing new line character
   fgetc(fhIn);
   fread(conversionData, sizeof(int), arraySize, fhIn);
 }

//closing in file and opening out file
fclose(fhIn);
fhOut = fopen(argv[3], "wb");

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
    //moving binary ints 4 spaces apart to make room for characters later
    for (int y = (arraySize - sizeof(int)); y >= 0; y-=4) {
      conversionData[y] = conversionData[y/4];
    }
    //converting binary ints and separating them into individual chars
    for (int i = 0; i < arraySize; i+=4) {
      number = conversionData[i] & 0xFF;
      divisor = 100;
      for (int z = 0; z < 3; z++) {
        character = floor(number/divisor);
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
    //TODO: P3 to P6 conversionData
    tracker = 0;
    x = 0;
    character = conversionData[tracker];
    while (tracker < arraySize) {
      number = 0;
      while (character > 47 && character < 58) {
          accumulator = character - '0';
          number *= 10;
          number += accumulator;
          tracker++;
          character = conversionData[tracker];
      }
      conversionData[x] = number;
      //conversionData[tracker] = '\0';
      tracker++;
      x++;
      character = conversionData[tracker];
    }
  }
    fwrite(conversionData, sizeof(int), arraySize, fhOut);
    //free(stringNumber);
  }
free(conversionData);
free(wh);
fclose(fhOut);
  return 0;
}
