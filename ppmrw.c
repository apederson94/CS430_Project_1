#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//opens a .ppm file and determines it's magic number
int determine_magic_number() {
  FILE *filePointer;
  char buffer[] = "I am a string.";

  filePointer = fopen("ppmrw_img.ppm", "rf");
  fgets(buffer, 3, filePointer);
  fclose(filePointer);

  if (strncmp("P6", buffer, 2)) {
    return 6;
  } else if (strncmp("P3", buffer, 2)) {
    return 3;
  } else {
    return 1;
  }
}

int convertFile() {
  FILE *filePointerIn, *filePointerOut;
  char buffer[] = "I am a string.";
  char *bit;
  unsigned int value;
  int c = 0;

  filePointerIn = fopen("ppmrw_img.ppm", "r");
  filePointerOut = fopen("ppmrw_img_out.ppm", "w");
  fgets(buffer, 255, filePointerIn);
  fputs(buffer, filePointerOut);
  fgets(buffer, 255, filePointerIn);
  fputs(buffer, filePointerOut);
  fgets(buffer, 255, filePointerIn);
  fputs(buffer, filePointerOut);
  fgets(buffer, 255, filePointerIn);
  fputs(buffer, filePointerOut);
  printf("%s\n", buffer);

  while(fgets(buffer, 255, filePointerIn) > 0) {

    printf("%s\n", buffer);

    char binary[8];

    while (value != 0) {
      c++;
      sprintf(bit, "%d", value % 2);
      strcat(binary, bit);
      value = value>>1;
    }
    fputs(buffer, filePointerOut);
  }

  fclose(filePointerOut);
  fclose(filePointerIn);

  return 0;
}


//main function to run helper functions
int main(int argc, char const *argv[]) {
  int result;

  result = determine_magic_number();

  convertFile();

  printf("%d\n", result);
  return 0;
}
