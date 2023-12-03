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
#define GRID_CELL_IS_EMPTY			-1
#define GRID_CELL_IS_GEAR_CANDIDATE	-2	// '*'
#define GRID_CELL_IS_SYMBOL			-3	// any symbol other than '*'

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

		if(*c == '*')
		{
			g_grid[x++][y] = GRID_CELL_IS_GEAR_CANDIDATE;
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

// returns -1 if given cell is not a number or is out of bounds
static int TryGetNumberInGridCell(int x, int y)
{
	if(x < 0)
		return -1;

	if(y < 0)
		return -1;

	if(x >= GRID_WIDTH)
		return -1;

	if(y >= GRID_WIDTH)
		return -1;

	if(g_grid[x][y] < 0)
		return -1;

	return g_numberTable[g_grid[x][y]];
}

// returns -1 if not exactly 2 neighboring numbers are found
static int ComputeGearRatio(int x, int y)
{
	static const struct
	{
		int x, y;
	} neighborOffsets[8] =
	{
		{ +1, +1 },
		{ +1, +0 },
		{ +1, -1 },

		{ +0, +1 },
		{ +0, -1 },

		{ -1, +1 },
		{ -1, +0 },
		{ -1, -1 },
	};

	assert(g_grid[x][y] == GRID_CELL_IS_GEAR_CANDIDATE);

	int neighboringNumbers[2];		// exactly 2 unique numbers as neighbors is the criteria
	int neighboringNumbersCount = 0;
	int n, i;

	for(n = 0; n < 8; n++)
	{
		int potentialNumber = TryGetNumberInGridCell(x + neighborOffsets[n].x, y + neighborOffsets[n].y);
		bool alreadyTracked = false;

		if(potentialNumber == -1)
			continue;

		// check if already tracked
		for(i = 0; i < neighboringNumbersCount; i++)
		{
			if(neighboringNumbers[i] == potentialNumber)
			{
				alreadyTracked = true;
				break;
			}
		}

		if(alreadyTracked)
			continue;

		// found new number => check if already already reached the max allowed neighboring numbers
		if(neighboringNumbersCount == 2)
			return -1;

		// track the new number
		neighboringNumbers[neighboringNumbersCount++] = potentialNumber;
	}

	return (neighboringNumbersCount == 2) ? neighboringNumbers[0] * neighboringNumbers[1] : -1;
}

int main()
{
	FILE *f = fopen("3a_input.txt", "r");
	int x, y = 0;
	int gearRatioSum = 0;

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

	// check for potential gears and sum their ratio
	for(x = 0; x < GRID_WIDTH; x++)
	{
		for(y = 0; y < GRID_HEIGHT; y++)
		{
			if(g_grid[x][y] == GRID_CELL_IS_GEAR_CANDIDATE)
			{
				int ratio = ComputeGearRatio(x, y);
				if(ratio != -1)
					gearRatioSum += ratio;
			}
		}
	}

	printf("gearRatioSum: %i\n", gearRatioSum);
	return 0;
}
