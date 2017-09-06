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
    return 0;
  }
}

int replaceP3() {
  FILE *filePointer
}


//main function to run helper functions
int main(int argc, char const *argv[]) {
  int result;

  result = determine_magic_number();

  printf("%d\n", result);
  return 0;
}
