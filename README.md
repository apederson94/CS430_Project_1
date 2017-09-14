# cs430_1_amp845

**Contact Information:**
  name: Austin Pederson
  email: amp845@nau.edu
  
**Usage:** 
  1. Run the command "make" in your console/terminal.
  2. Run the program with the command "./ppmrw # inputfile.ppm outputfile.ppm"
     # should be replaced with either 3 or 6 depending on whether you want to convert to P3 or P6 respectivley.
     inputfile.ppm should be replaced with your chosen input file with the extension .ppm
     outputfile.ppm should be replaced with your chosen output file with the extension .ppm
     
**Known Issues:**
  1. Cannot handle maximum color values over 3 digits (i.e. maxColor could be 999, but not higher).
  2. No handling of errors for negative values anywhere, except in the passed arguments.
  
