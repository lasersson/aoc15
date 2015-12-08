#include "aoc.h"

static void
Solve(input_file Input)
{
	int Floor = 0;

	for (int It = 0; It < Input.Length; ++It)
	{
		char c = Input.Contents[It]; 
		if (c == '(')
		{
			++Floor;
		}
		else if (c == ')')
		{
			--Floor;
		}
	}

	printf("%d", Floor);
}

