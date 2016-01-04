#include "aoc.h"

static output
Solve(input Input)
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

	output Output = { Floor, BasementEntryStep };
	return Output;
}

