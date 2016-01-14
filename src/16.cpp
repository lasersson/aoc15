#include <aoc.h>

#define PROPERTY_COUNT 10

static char PropertyIndexMap[PROPERTY_COUNT][64] = {
	"children", "cats", "samoyeds", "pomeranians", "akitas",
	"vizslas", "goldfish", "trees", "cars", "perfumes" };

enum property_comparison
{
	Comp_Eq,
	Comp_Lt,
	Comp_Gt,
};

static property_comparison PropertyComparison[PROPERTY_COUNT] = {
	Comp_Eq, Comp_Gt, Comp_Eq, Comp_Lt, Comp_Eq,
	Comp_Eq, Comp_Lt, Comp_Gt, Comp_Eq, Comp_Eq };

static int
GetPropertyIndex(char *Name)
{
	int Index = -1;
	for (; Index < PROPERTY_COUNT; ++Index)
	{
		if (strcmp(Name, PropertyIndexMap[Index]) == 0)
		{
			break;
		}
	}
	Assert(Index >= 0);
	return Index;
}

struct sue
{
	int Properties[PROPERTY_COUNT];
};

static sue
InitSue()
{
	sue Sue;
	for (int It = 0; It < PROPERTY_COUNT; ++It)
	{
		Sue.Properties[It] = -1;
	}
	return Sue;
}

static int
FindMatch(sue *Sues, sue MatchSue, property_comparison *Comparisons)
{
	int MatchIndex = -1;
	int MaxMatchCount = 0;
	for (int SueIt = 0; SueIt < GACount(Sues); ++SueIt)
	{
		sue *Sue = Sues + SueIt;
		int MatchCount = 0;
		for (int PropIt = 0; PropIt < PROPERTY_COUNT; ++PropIt)
		{
			int PropValue = Sue->Properties[PropIt];
			int MatchValue = MatchSue.Properties[PropIt];
			if (PropValue >= 0)
			{
				bool IsMatch = false;
				switch (Comparisons[PropIt])
				{
					case Comp_Eq: IsMatch = PropValue == MatchValue; break;
					case Comp_Lt: IsMatch = PropValue < MatchValue; break;
					case Comp_Gt: IsMatch = PropValue > MatchValue; break;
				}

				MatchCount += (int)IsMatch;
			}
		}

		if (MatchCount > MaxMatchCount)
		{
			MaxMatchCount = MatchCount;
			MatchIndex = SueIt;
		}
	}

	return MatchIndex;
}

static output
Solve(input Input)
{
	sue *Sues = nullptr;
	const char *Delim = ": ,\n\r";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		sue Sue = InitSue();

		Token = strtok(nullptr, Delim);
		Token = strtok(nullptr, Delim);
		while (Token && strcmp(Token, "Sue") != 0)
		{
			int PropIndex = GetPropertyIndex(Token);
			Token = strtok(nullptr, Delim);
			int PropValue = atoi(Token);
			Token = strtok(nullptr, Delim);

			Sue.Properties[PropIndex] = PropValue;
		}

		GAPush(Sues, Sue);
	}

	sue MatchSue = { 3, 7, 2, 3, 0, 0, 5, 3, 2 ,1 };
	property_comparison ComparisonsAllEq[PROPERTY_COUNT] = {};

	output Output = {};
	Output.a = FindMatch(Sues, MatchSue, ComparisonsAllEq) + 1;
	Output.b = FindMatch(Sues, MatchSue, PropertyComparison) + 1;

	GAFree(Sues);
	return Output;
}
