// https://adventofcode.com/2023/day/1

#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
			if(isdigit(*firstDigit))
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

			if(isdigit(*c))
				lastDigit = c;

			c++;
		}

		if(*firstDigit)
			number = *firstDigit - '0';

		if(*lastDigit)
			number = number * 10 + *lastDigit - '0';

		printf("%i\n", number);

		sum += number;
	}

	printf("%i\n", sum);
	fclose(f);
	return 0;
}
