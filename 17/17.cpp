#include <aoc.h>

static int
CompareContainers(const void *a, const void *b)
{
	int ContainerA = *(int *)a;
	int ContainerB = *(int *)b;
	int Result = ContainerB - ContainerA;
	return Result;
}

static void
Solve(input_file Input)
{
	int *Containers = nullptr;
	char *Delim = "\r\n";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		int Container = atoi(Token);
		GAPush(Containers, Container);
		Token = strtok(nullptr, Delim);
	}
	int ContainerCount = GACount(Containers);
	Assert(ContainerCount < 32);

	int *HitCounts = (int *)malloc(ContainerCount * sizeof(int));
	memset(HitCounts, 0, ContainerCount * sizeof(int));
	int CombCount = 1 << ContainerCount;
	int HitCountTotal = 0;
	int MinContainerCount = ContainerCount;
	for (int Combination = 0; Combination < CombCount; ++Combination)
	{
		int Capacity = 0;
		int CombContainerCount = 0;
		for (int ContainerIt = 0; ContainerIt < ContainerCount; ++ContainerIt)
		{
			if (!!(Combination & (1 << ContainerIt)))
			{
				++CombContainerCount;
				Capacity += Containers[ContainerIt];
			}
		}

		if (Capacity == 150)
		{
			++HitCounts[CombContainerCount];
			++HitCountTotal;
			MinContainerCount = Min(MinContainerCount, CombContainerCount);
		}
	}

	printf("%d\n", HitCountTotal);
	printf("%d\n", HitCounts[MinContainerCount]);

	GAFree(Containers);
	free(HitCounts);
}
