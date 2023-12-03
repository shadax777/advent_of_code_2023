// https://adventofcode.com/2023/day/3

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>


#define GRID_WIDTH	140
#define GRID_HEIGHT	140

// - special value of each grid cell that is not a number
// - non-negative values represent an index into the number table
#define GRID_CELL_IS_EMPTY	-1
#define GRID_CELL_IS_SYMBOL	-2

static int	g_grid[GRID_WIDTH][GRID_HEIGHT];
static int	g_numberTable[GRID_WIDTH * GRID_HEIGHT];
static int	g_numberTableSize;


static void FillGridRow(const char *line, int y)
{
	const char *c = line;
	int x = 0;

	while(1)
	{
		if(!*c)
		{
			break;
		}

		if(isdigit(*c))
		{
			int number = 0;
			while(isdigit(*c))
			{
				number = number * 10 + *c - '0';
				g_grid[x++][y] = g_numberTableSize;
				c++;
			}
			g_numberTable[g_numberTableSize++] = number;
			continue;
		}

		if(*c == '.')
		{
			g_grid[x++][y] = GRID_CELL_IS_EMPTY;
			c++;
			continue;
		}

		if(isgraph(*c))
		{
			g_grid[x++][y] = GRID_CELL_IS_SYMBOL;
		}

		c++;
	}
}

// safe check that handles out-of-bonds parameters
static bool IsGridCellSymbol(int x, int y)
{
	if(x < 0)
		return false;

	if(y < 0)
		return false;

	if(x >= GRID_WIDTH)
		return false;

	if(y >= GRID_HEIGHT)
		return false;

	return g_grid[x][y] == GRID_CELL_IS_SYMBOL;
}

static bool IsGridCellAdjacentToSymbol(int x, int y)
{
	if(IsGridCellSymbol(x + 1, y + 1))
		return true;

	if(IsGridCellSymbol(x + 1, y + 0))
		return true;

	if(IsGridCellSymbol(x + 1, y - 1))
		return true;

	//---

	if(IsGridCellSymbol(x + 0, y + 1))
		return true;

	if(IsGridCellSymbol(x + 0, y - 1))
		return true;

	// ---

	if(IsGridCellSymbol(x - 1, y + 1))
		return true;

	if(IsGridCellSymbol(x - 1, y + 0))
		return true;

	if(IsGridCellSymbol(x - 1, y - 1))
		return true;

	return false;
}

int main()
{
	FILE *f = fopen("3a_input.txt", "r");
	int x, y = 0;
	int numbersSum = 0;

	// fill the grid
	while(1)
	{
		char line[256];

		fgets(line, 256, f);
		if(feof(f))
			break;

		FillGridRow(line, y++);
	}
	fclose(f);

	// count numbers adjacent to symbols
	for(y = 0; y < GRID_HEIGHT; y++)
	{
		for(x = 0; x < GRID_WIDTH; x++)
		{
			int numberTableIndex = g_grid[x][y];
			bool isAdjacentToSymbol = false;

			if(numberTableIndex < 0)
				continue;

			while(x < GRID_WIDTH && g_grid[x][y] == numberTableIndex)
			{
				if(IsGridCellAdjacentToSymbol(x, y))
					isAdjacentToSymbol = true;

				x++;
			}

			if(isAdjacentToSymbol)
				numbersSum += g_numberTable[numberTableIndex];
		}
	}

	printf("numbersSum: %i\n", numbersSum);
	return 0;
}
