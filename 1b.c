// https://adventofcode.com/2023/day/1

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// returns -1 if not convertible
static int ToDigit(const char *s)
{
	if(isdigit(*s))
		return *s - '0';

	if(!strncmp(s, "one", 3))
		return 1;

	if(!strncmp(s, "two", 3))
		return 2;

	if(!strncmp(s, "three", 5))
		return 3;

	if(!strncmp(s, "four", 4))
		return 4;

	if(!strncmp(s, "five", 4))
		return 5;

	if(!strncmp(s, "six", 3))
		return 6;

	if(!strncmp(s, "seven", 5))
		return 7;

	if(!strncmp(s, "eight", 5))
		return 8;

	if(!strncmp(s, "nine", 4))
		return 9;

	return -1;
}

int main()
{
	FILE *f = fopen("1a_input.txt", "r");
	int sum = 0;

	while(1)
	{
		char line[256];
		int number = 0;
		const char *firstDigit;
		const char *lastDigit;
		const char *c;

		fscanf(f, "%s", line);
		if(feof(f))
			break;

		firstDigit = line;
		while(1)
		{
			if(ToDigit(firstDigit) != -1)
				break;

			if(!*firstDigit)
				break;

			firstDigit++;
		}

		c = lastDigit = firstDigit;
		while(1)
		{
			if(!*c)
				break;

			if(ToDigit(c) != -1)
				lastDigit = c;

			c++;
		}

		if(*firstDigit)
			number = ToDigit(firstDigit);

		if(*lastDigit)
			number = number * 10 + ToDigit(lastDigit);

		printf("%i\n", number);

		sum += number;
	}

	printf("%i\n", sum);
	fclose(f);
	return 0;
}
