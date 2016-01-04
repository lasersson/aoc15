#include "aoc.h"

static void
Solve(input_file Input)
{
	int Floor = 0;

	int BasementEntryStep = 0;
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
		if (BasementEntryStep == 0 && Floor < 0)
		{
			BasementEntryStep = It + 1;
		}
	}

	printf("%d\n", Floor);
	printf("%d\n", BasementEntryStep);
}

