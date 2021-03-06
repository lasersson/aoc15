#include <aoc.h>

static output
Solve(input Input)
{
	const char *Delim = "\r\n,. column";
	char *Token = strtok(Input.Contents + 80, Delim);
	int RowIndex = atoi(Token) - 1;
	Token = strtok(nullptr, Delim);
	int ColIndex = atoi(Token) - 1;

	s64 Code = 20151125;
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
	
	output Output = { Code, 0 };
	return Output;
}
