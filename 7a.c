// https://adventofcode.com/2023/day/7

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// A K Q J T 9 8 7 6 5 4 3 2


#define CARDS_STRING	"23456789TJQKA"	// in increasing order of their value


typedef struct
{
	char cards[5];
	int bid;
} hand_t;


static int CountCard(const hand_t *hand, char card)
{
	int n = 0;
	int i;

	for(i = 0; i < 5; i++)
	{
		if(hand->cards[i] == card)
			n++;
	}

	return n;
}

static bool HandType_FiveOfAKind(const hand_t *hand)
{
	int i;

	for(i = 1; i < 5; i++)
	{
		if(hand->cards[i] != hand->cards[0])
			return false;
	}

	return true;
}

static bool HandType_FourOfAKind(const hand_t *hand)
{
	const char *c;

	for(c = CARDS_STRING; *c; c++)
	{
		if(CountCard(hand, *c) == 4)
			return true;
	}

	return false;
}

static bool HandType_FullHouse(const hand_t *hand)
{
	bool found3same = false;
	bool found2same = false;
	const char *c;

	for(c = CARDS_STRING; *c; c++)
	{
		switch(CountCard(hand, *c))
		{
			case 2:
				found2same = true;
				break;

			case 3:
				found3same = true;
				break;
		}
	}

	return found3same && found2same;
}

static bool HandType_ThreeOfAKind(const hand_t *hand)
{
	bool found3same = false;
	bool found1same = false;
	const char *c;

	for(c = CARDS_STRING; *c; c++)
	{
		switch(CountCard(hand, *c))
		{
			case 1:
				found1same = true;
				break;

			case 3:
				found3same = true;
				break;
		}
	}

	return found3same && found1same;
}

static bool HandType_TwoPair(const hand_t *hand)
{
	int found2sameCount = 0;
	const char *c;

	for(c = CARDS_STRING; *c; c++)
	{
		if(CountCard(hand, *c) == 2)
			found2sameCount++;
	}

	return (found2sameCount == 2);
}

static bool HandType_OnePair(const hand_t *hand)
{
	int found2sameCount = 0;
	const char *c;

	for(c = CARDS_STRING; *c; c++)
	{
		if(CountCard(hand, *c) == 2)
			found2sameCount++;
	}

	return (found2sameCount == 1);
}

static bool HandType_HighCard(const hand_t *hand)
{
	int i, k;

	for(i = 0; i < 5; i++)
	{
		for(k = i + 1; k < 5; k++)
		{
			if(hand->cards[i] == hand->cards[k])
				return false;
		}
	}

	return true;
}

static int HandValue(const hand_t *hand)
{
	if(HandType_FiveOfAKind(hand))
		return 7;

	if(HandType_FourOfAKind(hand))
		return 6;

	if(HandType_FullHouse(hand))
		return 5;

	if(HandType_ThreeOfAKind(hand))
		return 4;

	if(HandType_TwoPair(hand))
		return 3;

	if(HandType_OnePair(hand))
		return 2;

	if(HandType_HighCard(hand))
		return 1;

	assert(0);
	return -1;
}

static int CardValue(char card)
{
	const char *c;
	int value = 1;

	for(c = CARDS_STRING; *c; c++, value++)
	{
		if(*c == card)
			return value;
	}

	assert(0);
	return -1;
}

static int HandCompare(const hand_t *hand1, const hand_t *hand2)
{
	int diff = HandValue(hand1) - HandValue(hand2);

	if(diff == 0)
	{
		int i;

		for(i = 0; i < 5; i++)
		{
			diff = CardValue(hand1->cards[i]) - CardValue(hand2->cards[i]);
			if(diff)
				break;
		}
	}

	return diff;
}

static int QSortHandCompare(const void *hand1, const void *hand2)
{
	return HandCompare((const hand_t *)hand1, (const hand_t *)hand2);
}


#define MAX_HANDS	1024

int main()
{
	hand_t hands[MAX_HANDS];
	int numHands = 0;
	int i;
	int finalScore = 0;
	FILE *f = fopen("7a_input.txt", "r");

	while(1)
	{
		char line[256];
		char cards[16];
		int bid;
		int n;

		fgets(line, 256, f);

		if(feof(f))
			break;

		assert(numHands < MAX_HANDS);

		n = sscanf(line, "%s %i", cards, &bid);
		assert(n == 2);

		strncpy(hands[numHands].cards, cards, 5);
		hands[numHands].bid = bid;
		numHands++;
	}
	fclose(f);

	qsort(hands, numHands, sizeof(hand_t), QSortHandCompare);

	for(i = 0; i < numHands; i++)
	{
		finalScore += (i + 1) * hands[i].bid;
	}

	printf("finalScore: %i\n", finalScore);
	return 0;
}
