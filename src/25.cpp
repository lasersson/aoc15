#include <aoc.h>
#include <stdint.h>

static void
Solve(input_file Input)
{
	char *Delim = "\r\n,. column";
	char *Token = strtok(Input.Contents + 80, Delim);
	int RowIndex = atoi(Token) - 1;
	Token = strtok(nullptr, Delim);
	int ColIndex = atoi(Token) - 1;

	uint64_t Code = 20151125;
	for(int ColIt = 0, RowIt = 1;
		!(RowIt == RowIndex + ColIndex && ColIt == ColIndex + 1);
		++ColIt)
	{
		Code = (Code * 252533ll) % 33554393ll;
		if (ColIt == RowIt + 1)
		{
			ColIt = 0;
			++RowIt;
		}
	}
	printf("%d\n", Code);
}
