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
	const struct node_s *left, *right;
} node_t;


static instruction_t	g_instructions[MAX_INSTRUCTIONS];
static int				g_numInstructions;

static node_t			g_nodes[MAX_NODES];
static int				g_numNodes;
static node_t *			g_startNode;	// "AAA"
static node_t *			g_endNode;		// "ZZZ"


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
	strcpy(node->name, nodeName);
	node->left = FindOrCreateNode(leftNodeName);
	node->right = FindOrCreateNode(rightNodeName);

	if(!strcmp(node->name, "AAA"))
		g_startNode = node;
	else if(!strcmp(node->name, "ZZZ"))
		g_endNode = node;
}

int main()
{
	char line[512];
	FILE *f = fopen("8a_input_patched.txt", "r");
	int curInstructionIndex = 0;
	const node_t *curNode;
	int numSteps = 0;

	// 1st line: instruction
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

	curNode = g_startNode;

	while(curNode != g_endNode)
	{
		switch(g_instructions[curInstructionIndex])
		{
			case INS_LEFT:
				curNode = curNode->left;
				break;

			case INS_RIGHT:
				curNode = curNode->right;
				break;
		}

		curInstructionIndex = (curInstructionIndex + 1) % g_numInstructions;
		numSteps++;
	}

	printf("numSteps: %i\n", numSteps);
	return 0;
}
