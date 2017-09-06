#include <stdio.h>
#include <string.h>

//opens a .ppm file and determines it's magic number
int determine_magic_number() {
  FILE *filePointer;
  char buffer[] = "I'm a string.";

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

int convertP3() {
  FILE *filePointer;
  char p6[] = "P6";

  filePointer = fopen("ppmrw_img_out.ppm", "w");
  fputs(p6, filePointer);
  fclose(filePointer);

  return 0;
}

int convertP6() {
  FILE *filePointer;
  char p3[] = "P3";

  filePointer = fopen("ppmrw_img_out.ppm", "w");
  fputs(p3, filePointer);
  fclose(filePointer);

  return 0;
}


//main function to run helper functions
int main(int argc, char const *argv[]) {
  int result;

  result = determine_magic_number();

  if(result == 6) {
    convertP3();
  } else if (result == 3) {
    convertP6();
  }

  printf("%d\n", result);
  return 0;
}
