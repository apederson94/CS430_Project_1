#include <stdlib.io>

//opens a .ppm file and determines it's magic number
int determine_magic_number() {
  FILE *filePointer;
  char *buffer;

  fp = fopen("ppmrw_img.ppm", "rf");
  fgets(buffer, 3, fp);

  if (buffer == "P6") {
    return 6;
  } else if (buffer == "P3") {
    return 3;
  } else {
    return 0;
  }
}
