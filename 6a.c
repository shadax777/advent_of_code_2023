// https://adventofcode.com/2023/day/6

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


#define MAX_RACES	20


typedef struct
{
	int duration;
	int recordDistance;
} race_t;


static race_t	g_races[MAX_RACES];
static int		g_numRaces;


static const char *ParseNextInt(const char *str, int *out)
{
	// skip leading spaces
	while(isspace(*str))
		str++;

	if(!*str)
		return NULL;

	*out = 0;

	while(isdigit(*str))
		*out = *out * 10 + *str++ - '0';

	return str;
}

static void LoadFile()
{
	char times[256];
	char distances[256];
	const char *s;
	int number;
	race_t *race;
	FILE *f = fopen("6a_input.txt", "r");

	fscanf(f, "Time: %[0-9 ]\n", times);
	fscanf(f, "Distance: %[0-9 ]\n", distances);

	// parse times
	for(s = ParseNextInt(times, &number), race = g_races; s; s = ParseNextInt(s, &number), race++)
	{
		race->duration = number;
		assert(g_numRaces < MAX_RACES);
		g_numRaces++;
	}

	// parse distances
	for(s = ParseNextInt(distances, &number), race = g_races; s; s = ParseNextInt(s, &number), race++)
	{
		race->recordDistance = number;
	}

	fclose(f);
}

static int GetNumWaysToWinRace(const race_t *race)
{
	int time;
	int numWaysToWin = 0;

	for(time = 0; time <= race->duration; time++)
	{
		int speed = time;
		int remainingTime = race->duration - time;
		int distanceCovered = speed * remainingTime;

		if(distanceCovered > race->recordDistance)
			numWaysToWin++;
	}

	return numWaysToWin;
}

int main()
{
	int marginOfError = 1;
	int i;

	LoadFile();

	for(i = 0; i < g_numRaces; i++)
	{
		int n = GetNumWaysToWinRace(g_races + i);
		marginOfError *= n;
	}

	printf("marginOfError: %i\n", marginOfError);

	return 0;
}
