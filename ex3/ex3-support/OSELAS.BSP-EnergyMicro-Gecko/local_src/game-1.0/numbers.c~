#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

typedef uint16_t color;

/*
Block width and hight in screen pixels.
Used to control image size.
*/
static const uint32_t blockW = 5;
static const uint32_t blockH = 5;

/*
Store number bitmaps. Numbers are 3x5 blocks.
Whether a block should be colored or black is stored as a bit.
Each uint16_t stores the bits for the corresponding block.
Bit 0 stores the bitmap for "0", bit 1 stores the bitmap for "1", etc...
bit 10 stores the bitmap for "!". Bits higher than 10 is 0.
*/
static const uint16_t numbers[15] = {
  1021, 2031, 1021,
  883, 1026, 925,
  893, 2046, 893,
  453, 2, 889,
  1007, 1903, 895
};

/*
Draws the number num, into the framebuffer file.
Starting at pixel x, y and using the specified color and black.
If num is more than 9, it draws a "!" insted.
*/
void
drawNumber (uint32_t x, uint32_t y, color numColor, uint16_t num,
	    FILE * framebuffer)
{
  // Find what bits to use.
  uint16_t shiftAmount;
  if (num <= 10)
    shiftAmount = num;
  else
    shiftAmount = 10;

  // Stores a single vertical line of pixels.
  color colorArray[3 * blockW];

  // Iterate over all vertical pixels.
  for (uint32_t i = 0; i < 5; i++)
    {
      for (uint32_t k = 0; k < blockH; k++)
	{

	  // Find position of vertical pixel line in file.
	  fseek (framebuffer,
		 sizeof (color) * (320 * (y + i * blockH + k) + x), SEEK_SET);

	  // Iterate over horizontal blocks.
	  for (uint32_t j = 0; j < 3; j++)
	    {

	      // Color is numColor if bit is 1, black otherwise.
	      color tmpColor =
		numColor * ((numbers[i * 3 + j] >> shiftAmount) & 1);

	      // Write block to pixel array.
	      for (uint32_t l = 0; l < blockW; l++)
		{
		  colorArray[j * blockW + l] = tmpColor;
		}
	    }

	  // Write pixel array to file.
	  fwrite (colorArray, sizeof (color), 3 * blockW, framebuffer);
	}
    }
}
