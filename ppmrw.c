#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//recursively cycles through comments
int loopComments(FILE *fh) {
  int character;
  char str[999];

  character = fgetc(fh);

  if (character == 35) {
    fgets(str, 999, fh);
    printf("%s", "comment: ");
    printf("%s\n", str);
    loopComments(fh);
  }
  return 0;
}


//main function that runs the necessary commands to convert a P3/P6 to a P3/P6
int main(int argc, char const *argv[]) {
  //variables
  int fileType, character, width, height, widthSet, heightSet;
  char *convertTo;

  //allocates space for the conversion type argument
  convertTo = (char *) malloc(sizeof(argv[1]));

  //variable initial setup
  FILE *fh = fopen(argv[2], "r");
  convertTo = argv[1];
  character = fgetc(fh);
  widthSet = 0;

  //determining file type (P3/P6)
  if (character == 80) {
    character = fgetc(fh);
    printf("%s", "second char: ");
    printf("%c\n", character);
    if (character == 51 || character - 6 == 0) {
      fileType = character;
      printf("%s", "fileType is: ");
      printf("%c\n", character);
    }
  }

  //moves file pointer to next line
  fgetc(fh);

  //loops through comments and sets width and height variables
  while (widthSet == 0 || heightSet == 0) {
    loopComments(fh);
    character = fgetc(fh);
    printf("%c\n", character);
    if (widthSet == 0) {
      printf("%s", "width is: ");
      width = fscanf(fh, "%s", &width);
      widthSet = 1;
      printf("%d\n", width);
    } else {
      height = fscanf(fh, "%i", &height);
      heightSet = 1;
      printf("%s", "height is: ");
      printf("%d\n", height);
    }
  }

  return 0;
}
