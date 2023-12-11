// https://adventofcode.com/2023/day/10

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


//#define TEST

#ifdef TEST

	#define GRID_W	5
	#define GRID_H	5

#else

	#define GRID_W	140
	#define GRID_H	140

#endif


typedef struct
{
	int x, y;
} gridIndex_t;


typedef struct
{
	gridIndex_t relativeIndexes[2];
} edge_t;


typedef struct
{
	edge_t cells[GRID_W][GRID_H];
	gridIndex_t startCell;
} grid_t;


static edge_t Char2RelativeEdge(char ch)
{
	static const struct
	{
		char ch;
		edge_t edge;
	} charEdgePairs[6] =
	{
		{ '|', { { {  0, -1 }, {  0, +1 } } } },
		{ '-', { { { -1,  0 }, { +1,  0 } } } },
		{ 'L', { { {  0, -1 }, { +1,  0 } } } },
		{ 'J', { { { -1,  0 }, {  0, -1 } } } },
		{ '7', { { { -1,  0 }, {  0, +1 } } } },
		{ 'F', { { { +1,  0 }, {  0, +1 } } } }
	};

	int i;

	for(i = 0; i < 6; i++)
	{
		if(ch == charEdgePairs[i].ch)
			return charEdgePairs[i].edge;
	}

	assert(0);
	return charEdgePairs[0].edge;
}

static bool EdgeLeadsTo(const edge_t *edge, const gridIndex_t *relativeDest)
{
	int i;

	for(i = 0; i < 2; i++)
	{
		if(edge->relativeIndexes[i].x == relativeDest->x && edge->relativeIndexes[i].y == relativeDest->y)
			return true;
	}

	return false;
}

static grid_t LoadGrid()
{
#ifdef TEST
	FILE *f = fopen("10a_input_test.txt", "r");
#else
	FILE *f = fopen("10a_input.txt", "r");
#endif
	grid_t grid;
	int x, y = 0;
	int numStartCellEdges = 0;
	int i;

	static const gridIndex_t neighborOffsets[4] =
	{
		{ -1, 0 },
		{ +1, 0 },
		{ 0, -1 },
		{ 0, +1 }
	};

	while(1)
	{
		char line[256];

		fgets(line, 256, f);

		if(feof(f))
			break;

		assert(y < GRID_H);

		for(x = 0; x < GRID_W; x++)
		{
			assert(line[x]);

			switch(line[x])
			{
				case 'S':
					grid.startCell.x = x;
					grid.startCell.y = y;
					break;

				case '.':
					break;

				default:
					grid.cells[x][y] = Char2RelativeEdge(line[x]);
					break;
			}
		}

		y++;
	}

	// figure out the start cell's 2 edges
	for(i = 0; i < 4; i++)
	{
		gridIndex_t neighborGridIndex = { grid.startCell.x + neighborOffsets[i].x, grid.startCell.y + neighborOffsets[i].y };
		gridIndex_t relativeDest = { -neighborOffsets[i].x, -neighborOffsets[i].y };
		const edge_t *neighbor = &grid.cells[neighborGridIndex.x][neighborGridIndex.y];

		if(EdgeLeadsTo(neighbor, &relativeDest))
		{
			assert(numStartCellEdges < 2);
			grid.cells[grid.startCell.x][grid.startCell.y].relativeIndexes[numStartCellEdges++] = neighborOffsets[i];
		}
	}

	assert(numStartCellEdges == 2);

	// DEBUG
	for(i = 0; i < 2; i++)
	{
		printf("startCell: edge #%i = %i, %i\n", i, grid.cells[grid.startCell.x][grid.startCell.y].relativeIndexes[i].x, grid.cells[grid.startCell.x][grid.startCell.y].relativeIndexes[i].y);
	}

	fclose(f);
	return grid;
}

static bool GridIndexesEqual(const gridIndex_t *a, const gridIndex_t *b)
{
	return (a->x == b->x && a->y == b->y);
}

// prevCell is NULL when initiating the walk
static gridIndex_t WalkToNextCell(const grid_t *grid, const gridIndex_t *prevCell, const gridIndex_t *curCell)
{
	const edge_t *curEdge = &grid->cells[curCell->x][curCell->y];
	gridIndex_t destCell;

	if(!prevCell)
	{
		static const int edgeIndex = 0;	// could also use 1 (would then start walking in the opposite direction)
		destCell.x = curCell->x + curEdge->relativeIndexes[edgeIndex].x;
		destCell.y = curCell->y + curEdge->relativeIndexes[edgeIndex].y;
	}
	else
	{
		// find an edge such that we don't walk back to where we came from

		int edgeIndex;

		for(edgeIndex = 0; edgeIndex < 2; edgeIndex++)
		{
			destCell.x = curCell->x + curEdge->relativeIndexes[edgeIndex].x;
			destCell.y = curCell->y + curEdge->relativeIndexes[edgeIndex].y;

			if(!GridIndexesEqual(&destCell, prevCell))
				break;
		}

		// bug if both edges led to previous cell
		assert(edgeIndex < 2);
	}

#ifdef TEST
	printf("prevCell: %i %i\n", prevCell ? prevCell->x : -1, prevCell ? prevCell->y : -1);
	printf("curCell:  %i %i\n", curCell->x, curCell->y);
	printf("destCell: %i %i\n\n", destCell.x, destCell.y);
#endif

	return destCell;
}

int main()
{
	grid_t grid = LoadGrid();
	gridIndex_t prevCell, curCell, nextCell;
	int numStepsFullRoundtrip = 1;

	curCell = grid.startCell;
	nextCell = WalkToNextCell(&grid, NULL, &curCell);

	while(!GridIndexesEqual(&nextCell, &grid.startCell))
	{
		numStepsFullRoundtrip++;

		prevCell = curCell;
		curCell = nextCell;

		nextCell = WalkToNextCell(&grid, &prevCell, &curCell);
	}

	printf("numStepsFullRoundtrip: %i\n", numStepsFullRoundtrip);
	printf("numStepsFurthersFromStartCell : %i\n", numStepsFullRoundtrip / 2);
	return 0;
}
