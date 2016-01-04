#include <aoc.h>
#include <math.h>

static int
PresentCountForHouse(int HouseIndex, int HousesPerElf, int PresentsPerElf)
{
	int SqrtHouseIndex = (int)sqrt((double)HouseIndex);
	int Count = 0;
	for (int ElfIndex = 1; ElfIndex <= SqrtHouseIndex; ++ElfIndex)
	{
		if (HouseIndex % ElfIndex == 0)
		{
			int ElfBuddyIndex = HouseIndex / ElfIndex;
			if (HousesPerElf == -1 || ElfBuddyIndex <= HousesPerElf)
			{
				Count += ElfIndex;
			}
			if (ElfBuddyIndex != ElfIndex && (HousesPerElf == -1 || ElfIndex <= HousesPerElf))
			{
				Count += ElfBuddyIndex;
			}
		}
	}

	return Count * PresentsPerElf;
}

static int
FindMinHouseIndex(int PresentCountLimit, int HousesPerElf, int PresentsPerElf)
{
	int HouseIndex;
	int PresentCount;
	for (HouseIndex = 1, PresentCount = 0; PresentCount <= PresentCountLimit; ++HouseIndex)
	{
		PresentCount = PresentCountForHouse(HouseIndex, HousesPerElf, PresentsPerElf);
	}
	return HouseIndex - 1;
}

static void
Solve(input_file Input)
{
	char *Token = strtok(Input.Contents, "\r\n ");
	int PresentCountLimit = atoi(Token);

	printf("%d\n", FindMinHouseIndex(PresentCountLimit, -1, 10));
	printf("%d\n", FindMinHouseIndex(PresentCountLimit, 50, 11));
}
