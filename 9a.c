// https://adventofcode.com/2023/day/9

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


#define MAX_SEQ_VALUES	21	// extrapolated value if not stored

typedef struct
{
	int values[MAX_SEQ_VALUES];
	int numValues;
} sequence_t;

static bool SequenceIsAllZeroes(const sequence_t *seq)
{
	int i;

	for(i = 0; i < seq->numValues; i++)
	{
		if(seq->values[i] != 0)
			return false;
	}

	return true;
}

// returns the extrapolated value
static int ExtrapolateSequence(const sequence_t *seq)
{
	int i;
	sequence_t diffedSeq;
	int extrapolated;

	assert(seq->numValues > 0);

	if(SequenceIsAllZeroes(seq))
		return seq->values[seq->numValues - 1] + 0;

	for(i = 1; i < seq->numValues; i++)
	{
		int diff = seq->values[i] - seq->values[i - 1];
		diffedSeq.values[i - 1] = diff;
	}

	diffedSeq.numValues = seq->numValues - 1;

	extrapolated = ExtrapolateSequence(&diffedSeq);

	return seq->values[seq->numValues - 1] + extrapolated;
}

static void Test()
{
	sequence_t seq =
	{
		//{ 0, 3, 6, 9, 12, 15 },
		//{ 1, 3, 6, 10, 15, 21 },
		{ 10, 13, 16, 21, 30, 45 },
		6
	};
	int extrapolated = ExtrapolateSequence(&seq);
	printf("%i\n", extrapolated);
}

int main()
{
#if 0
	Test();
	return 0;
#endif

	FILE *f = fopen("9a_input.txt", "r");
	int sumExtrapolated = 0;

	while(1)
	{
		char line[256];
		sequence_t seq;

		fgets(line, 256, f);

		if(feof(f))
			break;

		seq.numValues = sscanf(line, "%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
			&seq.values[0],
			&seq.values[1],
			&seq.values[2],
			&seq.values[3],
			&seq.values[4],
			&seq.values[5],
			&seq.values[6],
			&seq.values[7],
			&seq.values[8],
			&seq.values[9],
			&seq.values[10],
			&seq.values[11],
			&seq.values[12],
			&seq.values[13],
			&seq.values[14],
			&seq.values[15],
			&seq.values[16],
			&seq.values[17],
			&seq.values[18],
			&seq.values[19],
			&seq.values[20]);
		assert(seq.numValues == 21);

		sumExtrapolated += ExtrapolateSequence(&seq);
	}

	fclose(f);
	printf("sumExtrapolated: %i\n", sumExtrapolated);
	return 0;
}
