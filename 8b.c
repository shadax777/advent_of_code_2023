// https://adventofcode.com/2023/day/8

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


#define MAX_INSTRUCTIONS	300
#define MAX_NODES			800


typedef enum
{
	INS_LEFT,
	INS_RIGHT
} instruction_t;


typedef struct node_s
{
	char name[4];
	const struct node_s *neighbors[2];	// indexed by instruction_t
} node_t;


typedef struct
{
	const node_t *curNodes[MAX_NODES];
	int numStepsPerNodeSoFar[MAX_NODES];
	int numNodes;
} parallelSimulation_t;


static instruction_t	g_instructions[MAX_INSTRUCTIONS];
static int				g_numInstructions;

static node_t			g_nodes[MAX_NODES];
static int				g_numNodes;


static node_t *FindOrCreateNode(const char *nodeName)
{
	int i;

	assert(strlen(nodeName) == 3);

	for(i = 0; i < g_numNodes; i++)
	{
		if(!strcmp(g_nodes[i].name, nodeName))
			return g_nodes + i;
	}

	assert(g_numNodes < MAX_NODES);

	strcpy(g_nodes[g_numNodes].name, nodeName);
	return g_nodes + g_numNodes++;
}

static void ParseInstructions(const char *line)
{
	const char *c;

	for(c = line; *c; c++)
	{
		assert(g_numInstructions < MAX_INSTRUCTIONS);

		switch(*c)
		{
			case 'L':
				g_instructions[g_numInstructions++] = INS_LEFT;
				break;

			case 'R':
				g_instructions[g_numInstructions++] = INS_RIGHT;
				break;
		}
	}
}

static void ParseNode(const char *line)
{
	char nodeName[4];
	char leftNodeName[4];
	char rightNodeName[4];
	node_t *node;
	int n;

	n = sscanf(line, "%s = ( %s , %s )", nodeName, leftNodeName, rightNodeName);
	assert(n == 3);

	node = FindOrCreateNode(nodeName);
	node->neighbors[INS_LEFT] = FindOrCreateNode(leftNodeName);
	node->neighbors[INS_RIGHT] = FindOrCreateNode(rightNodeName);
}

static void InitParallelSimulation(parallelSimulation_t *ps)
{
	int i;

	ps->numNodes = 0;

	for(i = 0; i < g_numNodes; i++)
	{
		if(g_nodes[i].name[2] == 'A')
		{
			ps->curNodes[ps->numNodes] = g_nodes + i;
			ps->numStepsPerNodeSoFar[ps->numNodes] = 0;
			ps->numNodes++;
		}
	}
}

static int Min(int a, int b)
{
	return a < b ? a : b;
}

static size_t ParallelSimulation(parallelSimulation_t *ps)
{
	int i;
	int shortestPath;
	size_t mul = 1;

	for(i = 0; i < ps->numNodes; i++)
	{
		while(ps->curNodes[i]->name[2] != 'Z')
		{
			ps->curNodes[i] = ps->curNodes[i]->neighbors[g_instructions[ps->numStepsPerNodeSoFar[i] % g_numInstructions]];
			ps->numStepsPerNodeSoFar[i]++;
		}
	}

	shortestPath = ps->numStepsPerNodeSoFar[0];
	for(i = 1; i < ps->numNodes; i++)
	{
		shortestPath = Min(shortestPath, ps->numStepsPerNodeSoFar[i]);
	}

	while(1)
	{
		size_t test = shortestPath * mul;
		bool found = true;

		for(i = 0; i < ps->numNodes; i++)
		{
			if(test % ps->numStepsPerNodeSoFar[i])
			{
				found = false;
				break;
			}
		}

		if(found)
			return test;

		mul++;
	}

	assert(0);
	return 0;
}

int main()
{
	char line[512];
	FILE *f = fopen("8a_input_patched.txt", "r");
	parallelSimulation_t ps;
	size_t numSteps;

	// 1st line: instructions
	fgets(line, 512, f);
	ParseInstructions(line);

	// 2nd line: empty (skip)
	fgets(line, 512, f);

	// 3rd+ lines: nodes
	while(1)
	{
		fgets(line, 512, f);

		if(feof(f))
			break;

		ParseNode(line);
	}
	fclose(f);

	InitParallelSimulation(&ps);

	numSteps = ParallelSimulation(&ps);

	printf("numSteps: %zu\n", numSteps);

	return 0;
}
