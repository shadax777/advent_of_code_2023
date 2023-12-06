// https://adventofcode.com/2023/day/6

#include <stdio.h>
#include <stdint.h>
#include <assert.h>


typedef struct
{
	unsigned int duration;
	uint64_t recordDistance;
} race_t;


static unsigned int GetNumWaysToWinRace(const race_t *race, unsigned int minTime)
{
	unsigned int time;
	unsigned int numWaysToWin = 0;

	for(time = minTime; time <= race->duration; time++)
	{
		uint64_t speed = time;
		uint64_t remainingTime = race->duration - time;
		uint64_t distanceCovered = speed * remainingTime;

		if(distanceCovered > race->recordDistance)
			numWaysToWin++;
	}

	return numWaysToWin;
}

int main()
{
	race_t race = { 42686985, 284100511221341 };
	unsigned int n = GetNumWaysToWinRace(&race, 14);

	printf("n: %u\n", n);

	return 0;
}
