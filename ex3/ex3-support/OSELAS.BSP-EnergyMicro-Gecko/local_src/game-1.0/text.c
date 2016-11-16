#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

typedef uint16_t color;

/*
Block width and hight in screen pixels.
Used to control image size.
*/
static const uint32_t blockW = 3;
static const uint32_t blockH = 3;

/*
Store letter bitmaps. Letters are 5x5 blocks.
Whether a block should be colored or black is stored as a bit.
Each uint16_t stores the bits for the corresponding block.

Bit Symbol
----------
0   I
1   !
2   E
3   N
4   W
5   R

Bits higher than 5 is 0.
*/
static const uint16_t letters[25] = {
  63, 36, 52, 32, 56,
  63, 8, 16, 0, 56,
  63, 36, 56, 32, 24,
  61, 0, 16, 8, 56,
  47, 20, 4, 16, 40
};

/*
Draws the symbol corresponding to num, into the framebuffer file.
Starting at pixel x, y and using the specified color and black.
If num is more than 5, it draws symbol 5.
*/
void
drawLetter (uint32_t x, uint32_t y, color numColor, uint16_t num,
	    FILE * framebuffer)
{
  // Find what bits to use.
  uint16_t shiftAmount;
  if (num <= 5)
    shiftAmount = num;
  else
    shiftAmount = 5;

  // Stores a single vertical line of pixels.
  color colorArray[5 * blockW];

  // Iterate over all vertical pixels.
  for (uint32_t i = 0; i < 5; i++)
    {
      for (uint32_t k = 0; k < blockH; k++)
	{

	  // Find position of vertical pixel line in file.
	  fseek (framebuffer,
		 sizeof (color) * (320 * (y + i * blockH + k) + x), SEEK_SET);

	  // Iterate over horizontal blocks.
	  for (uint32_t j = 0; j < 5; j++)
	    {

	      // Color is numColor if bit is 1, black otherwise.
	      color tmpColor =
		numColor * ((letters[i * 5 + j] >> shiftAmount) & 1);

	      // Write block to pixel array.
	      for (uint32_t l = 0; l < blockW; l++)
		{
		  colorArray[j * blockW + l] = tmpColor;
		}
	    }

	  // Write pixel array to file.
	  fwrite (colorArray, sizeof (color), 5 * blockW, framebuffer);
	}
    }
}

/*
Draws the word "WINNER!", into the framebuffer file.
Starting at pixel x, y and using the specified color and black.
Letters has 1 block spacing.
*/
void
drawWinner (uint32_t x, uint32_t y, color numColor, FILE * framebuffer)
{
  drawLetter (x, y, numColor, 4, framebuffer);
  drawLetter (x + 6 * blockW, y, numColor, 0, framebuffer);
  drawLetter (x + 8 * blockW, y, numColor, 3, framebuffer);
  drawLetter (x + 14 * blockW, y, numColor, 3, framebuffer);
  drawLetter (x + 20 * blockW, y, numColor, 2, framebuffer);
  drawLetter (x + 24 * blockW, y, numColor, 5, framebuffer);
  drawLetter (x + 30 * blockW, y, numColor, 1, framebuffer);
}
