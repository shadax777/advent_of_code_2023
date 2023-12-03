// https://adventofcode.com/2023/day/2

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


static const char *MyStrtok(const char *str, char delim, char *out)
{
	const char *c;

	if(!*str)
		return NULL;

	c = strchr(str, delim);
	if(c)
	{
		strncpy(out, str, c - str);
		out[c - str] = '\0';
		return c + 1;
	}
	else
	{
		strcpy(out, str);
		return "\0";
	}
}

// "1 red, 5 blue, 10 green"
static void ParseColorCounts(const char *line, int *redCount, int *greenCount, int *blueCount)
{
	const char *next;
	char tok[256];

	*redCount = *greenCount = *blueCount = 0;

	// "1 red"
	// "5 blue"
	// "10 green"
	for(next = MyStrtok(line, ',', tok); next; next = MyStrtok(next, ',', tok))
	{
		int count;
		char colorName[8];
		int n = sscanf(tok, "%i %s", &count, colorName);
		assert(n == 2);

		if(!strcmp(colorName, "red"))
			*redCount = count;
		else if(!strcmp(colorName, "green"))
			*greenCount = count;
		else if(!strcmp(colorName, "blue"))
			*blueCount = count;
		else
			assert(0);
	}
}

static int Max(int a, int b)
{
	return (a > b) ? a : b;
}

// "1 red, 5 blue, 10 green; 5 green, 6 blue, 12 red; 4 red, 10 blue, 4 green"
static void ParseAndAndCountColors(const char *line, int *maxRedCount, int *maxGreenCount, int *maxBlueCount)
{
	const char *next;
	char tok[256];

	*maxRedCount = *maxGreenCount = *maxBlueCount = 0;

	for(next = MyStrtok(line, ';', tok); next; next = MyStrtok(next, ';', tok))
	{
		int redCount, greenCount, blueCount;
		ParseColorCounts(tok, &redCount, &greenCount, &blueCount);

		*maxRedCount   = Max(*maxRedCount, redCount);
		*maxGreenCount = Max(*maxGreenCount, greenCount);
		*maxBlueCount  = Max(*maxBlueCount, blueCount);
	}
}

int main()
{
	FILE *f = fopen("2a_input.txt", "r");
	int powerSum = 0;

	while(1)
	{
		char line[256];
		const char *next;
		char tok1[256], tok2[256];
		int maxRedCount, maxGreenCount, maxBlueCount;

		fgets(line, 256, f);
		if(feof(f))
			break;

		next = MyStrtok(line, ':', tok1);	// not used
		next = MyStrtok(next, ':', tok2);

		ParseAndAndCountColors(tok2, &maxRedCount, &maxGreenCount, &maxBlueCount);
		powerSum += maxRedCount * maxGreenCount * maxBlueCount;
	}

	printf("powerSum = %i\n", powerSum);
	fclose(f);
	return 0;
}
