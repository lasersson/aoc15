#include <aoc.h>

#define ROW_COUNT 100
#define COL_COUNT 100
#define LIGHT_COUNT ((ROW_COUNT)*(COL_COUNT))

static int
GetNeighborOnCount(bool *LightGrid, int Row, int Col)
{
	int OnCount = 0;
	int RowStart = Max(Row - 1, 0);
	int RowEnd = Min(Row + 2, ROW_COUNT);
	int ColStart = Max(Col - 1, 0);
	int ColEnd = Min(Col + 2, COL_COUNT);
	for (int RowIt = RowStart; RowIt < RowEnd; ++RowIt)
	{
		for (int ColIt = ColStart; ColIt < ColEnd; ++ColIt)
		{
			if (!(RowIt == Row && ColIt == Col))
			{
				OnCount += LightGrid[RowIt * COL_COUNT + ColIt];
			}
		}
	}

	return OnCount;
}

static void
Tick(bool *InGrid, bool *OutGrid)
{
	for (int RowIt = 0; RowIt < ROW_COUNT; ++RowIt)
	{
		for (int ColIt = 0; ColIt < COL_COUNT; ++ColIt)
		{
			int OnCount = GetNeighborOnCount(InGrid, RowIt, ColIt);
			bool *IsOn = InGrid + RowIt * COL_COUNT + ColIt;
			bool NewIsOn = *IsOn;
			if (*IsOn)
			{
				if (!(OnCount == 2 || OnCount == 3))
				{
					NewIsOn = false;
				}
			}
			else
			{
				if (OnCount == 3)
				{
					NewIsOn = true;
				}
			}

			OutGrid[RowIt * COL_COUNT + ColIt] = NewIsOn;
		}
	}
}

static bool *
Tick(bool *InGrid, bool *OutGrid, int StepCount, bool CornersAreAlwaysOn)
{
	bool *Current = InGrid;
	bool *Next = OutGrid;
	for (int Step = 0; Step < StepCount; ++Step)
	{
		Tick(Current, Next);
		bool *Tmp = Current;
		Current = Next;
		Next = Tmp;

		if (CornersAreAlwaysOn)
		{
			Current[0] = true;
			Current[COL_COUNT - 1] = true;
			Current[(ROW_COUNT-1) * COL_COUNT + 0] = true;
			Current[(ROW_COUNT-1) * COL_COUNT + (COL_COUNT - 1)] = true;
		}
	}

	return Current;
}

static int
GetOnCount(bool *Grid)
{
	int OnCount = 0;
	for (int RowIt = 0; RowIt < ROW_COUNT; ++RowIt)
	{
		for (int ColIt = 0; ColIt < COL_COUNT; ++ColIt)
		{
			if (Grid[RowIt * COL_COUNT + ColIt])
			{
				++OnCount;
			}
		}
	}
	return OnCount;
}


static output
Solve(input Input)
{
	bool *LightGridOrig = (bool *)malloc(sizeof(bool) * LIGHT_COUNT);
	bool *LightGrid1 = (bool *)malloc(sizeof(bool) * LIGHT_COUNT);
	bool *LightGrid2 = (bool *)malloc(sizeof(bool) * LIGHT_COUNT);

	char *Delim = "\n\r";
	char *Token = strtok(Input.Contents, Delim);
	int RowIt = 0;
	while (Token)
	{
		for (int ColIt = 0; ColIt < COL_COUNT; ++ColIt)
		{
			char c = Token[ColIt];
			Assert(c == '#' || c == '.');
			LightGridOrig[RowIt * COL_COUNT + ColIt] = (c == '#');
		}
		Token = strtok(nullptr, Delim);
		++RowIt;
	}

	output Output = {};

	memcpy(LightGrid1, LightGridOrig, sizeof(bool) * LIGHT_COUNT);
	bool *Result = Tick(LightGrid1, LightGrid2, 100, false);
	Output.a = GetOnCount(Result);

	memcpy(LightGrid1, LightGridOrig, sizeof(bool) * LIGHT_COUNT);
	LightGrid1[0] = true;
	LightGrid1[COL_COUNT - 1] = true;
	LightGrid1[(ROW_COUNT-1) * COL_COUNT + 0] = true;
	LightGrid1[(ROW_COUNT-1) * COL_COUNT + (COL_COUNT - 1)] = true;
	Result = Tick(LightGrid1, LightGrid2, 100, true);
	Output.b = GetOnCount(Result);

	free(LightGridOrig);
	free(LightGrid1);
	free(LightGrid2);

	return Output;
}
