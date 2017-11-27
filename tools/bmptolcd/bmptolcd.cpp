/**
  BMP to LCD converter
  Modified from the Arduino lcd_test_parrot_bmp example for
  use on a PC.

  Chris Martin, 2016
*/

/***************************************************
  This is an example sketch for the Adafruit 1.8" SPI display.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <cstdio>
#include <stdint.h>

uint16_t read16(FILE* f) {
  uint16_t result;
  fread(&result, 2, 1, f);
  return result;
}

uint32_t read32(FILE* f) {
  uint32_t result;
  fread(&result, 4, 1, f);
  return result;
}

// Source: Adafruit_ST7735.cpp
// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpToLcd(FILE* bmpFile, FILE* dest) {
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  bool     goodBmp = false;       // Set to true on valid header parse
  bool     flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  long     pos = 0;

  printf("Converting image ...\n");

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    printf("File size: %d bytes\n", read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    printf("Image Offset: %d\n", bmpImageoffset);
    // Read DIB header
    printf("Header size: %d bytes\n", read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      printf("Bit Depth: %dbpp\n", bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        printf("Image size: %dx%d\n", bmpWidth, bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(ftell(bmpFile) != pos) { // Need seek?
            fseek(bmpFile, pos, SEEK_SET);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              fread(sdbuffer, sizeof(sdbuffer), 1, bmpFile);
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];

            uint16_t col565 = Color565(r,g,b);
            // For some reason the lcd_image library assumes the value is
            // big-endian ...
            col565 = (col565 << 8) | (col565 >> 8);
            fwrite(&col565, 2, 1, dest);
          } // end pixel
        } // end scanline
      } // end goodBmp
    }
  }

  if(!goodBmp) printf("BMP format not recognized.\n");
  else printf("Done.\n");
}


int main(int argc, char **argv) {
  if (argc != 3) {
    printf("\n");
    printf("Converts a .bmp image to a .lcd image for use with the ST7735 display.\n");
    printf("Derived from the Adafruit BMP example code.\n");
    printf("\n");
    printf("Usage: %s SRC.bmp DST.lcd\n", argv[0]);
    printf("\n");
    return 0;
  }

  FILE *bmpFile = fopen(argv[1], "rb");
  if (!bmpFile) {
    printf("Unable to open '%s' for reading.\n", argv[1]);
    return 0;
  }

  FILE *dest = fopen(argv[2], "wb");
  if (!dest) {
    printf("Unable to open '%s' for writing.\n", argv[2]);
    fclose(bmpFile);
    return 0;
  }

  bmpToLcd(bmpFile, dest);

  fclose(bmpFile);
  fclose(dest);
  return 0;
}
