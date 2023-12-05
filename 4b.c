// https://adventofcode.com/2023/day/4

#include <stdio.h>
#include <assert.h>


#define NUM_CARDS	223


int main()
{
	FILE *f = fopen("4a_input.txt", "r");
	int cardInstances[NUM_CARDS];
	int totalCards = 0;
	int i;

	// initially, there is already 1 instance per card
	for(i = 0; i < NUM_CARDS; i++)
	{
		cardInstances[i] = 1;
	}

	while(1)
	{
		char line[256];
		int cardId;
		int winningNumbers[10];
		int myNumbers[25];
		int myMatchingNumbersCount = 0;
		int n, i, k;

		fgets(line, 256, f);
		if(feof(f))
			break;

		// "Card   1: 75 68 35 36 86 83 30 11 14 59 | 86 25 63 57 59 91 68 14 72 32 36 74 66 44 30 28 11 35 75 34 55 83 69 56 38"
		n = sscanf(line, "Card %i: %i %i %i %i %i %i %i %i %i %i | %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
			&cardId,
			&winningNumbers[0],
			&winningNumbers[1],
			&winningNumbers[2],
			&winningNumbers[3],
			&winningNumbers[4],
			&winningNumbers[5],
			&winningNumbers[6],
			&winningNumbers[7],
			&winningNumbers[8],
			&winningNumbers[9],
			&myNumbers[0],
			&myNumbers[1],
			&myNumbers[2],
			&myNumbers[3],
			&myNumbers[4],
			&myNumbers[5],
			&myNumbers[6],
			&myNumbers[7],
			&myNumbers[8],
			&myNumbers[9],
			&myNumbers[10],
			&myNumbers[11],
			&myNumbers[12],
			&myNumbers[13],
			&myNumbers[14],
			&myNumbers[15],
			&myNumbers[16],
			&myNumbers[17],
			&myNumbers[18],
			&myNumbers[19],
			&myNumbers[20],
			&myNumbers[21],
			&myNumbers[22],
			&myNumbers[23],
			&myNumbers[24]);
		assert(n == 36);

		// just patch the cardId to make the code easier to digest (Cards start at 1 in the example file)
		cardId--;

		totalCards += cardInstances[cardId];

		// count matches
		for(i = 0; i < 10; i++)
		{
			for(k = 0; k < 25; k++)
			{
				if(winningNumbers[i] == myNumbers[k])
				{
					myMatchingNumbersCount++;
					break;
				}
			}
		}

		// duplicate all following cards based on the number of matches
		for(i = 0; i < myMatchingNumbersCount; i++)
		{
			int nextCardId = cardId + 1 + i;

			// don't go past the end of the table
			if(nextCardId == NUM_CARDS)
				break;

			cardInstances[nextCardId] += cardInstances[cardId];
		}
	}

	printf("totalCards: %i\n", totalCards);
	fclose(f);
	return 0;
}
