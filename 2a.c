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

// "1 red, 5 blue, 10 green; 5 green, 6 blue, 12 red; 4 red, 10 blue, 4 green"
// returns true if valid counts, false otherwise
static bool ParseAndCheckColorCounts(const char *line)
{
	const char *next;
	char tok[256];
	int redCount, greenCount, blueCount;

	for(next = MyStrtok(line, ';', tok); next; next = MyStrtok(next, ';', tok))
	{
		ParseColorCounts(tok, &redCount, &greenCount, &blueCount);
		if(redCount > 12 || greenCount > 13 || blueCount > 14)
			return false;
	}

	return true;
}

int main()
{
	FILE *f = fopen("2a_input.txt", "r");
	int idSum = 0;

	while(1)
	{
		char line[256];
		const char *next;
		char tok1[256], tok2[256];
		int id, n;

		fgets(line, 256, f);
		if(feof(f))
			break;

		next = MyStrtok(line, ':', tok1);
		next = MyStrtok(next, ':', tok2);
		n = sscanf(tok1, "Game %i", &id);
		assert(n == 1);

		if(ParseAndCheckColorCounts(tok2))
			idSum += id;
	}

	printf("idSum = %i\n", idSum);
	fclose(f);
	return 0;
}
