#include <aoc.h>

struct container_array
{
	int *Array;
	int Count;
	int Cap;
};

static void
AddContainer(container_array *Containers, int Container)
{
	if (Containers->Count == Containers->Cap)
	{
		Containers->Cap = Max(1, Containers->Cap * 2);
		Containers->Array = (int *)realloc(Containers->Array, Containers->Cap * sizeof(int));
	}
	Containers->Array[Containers->Count++] = Container;
}

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
	container_array Containers = {};
	char *Delim = "\r\n";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		int Container = atoi(Token);
		AddContainer(&Containers, Container);
		Token = strtok(nullptr, Delim);
	}
	Assert(Containers.Count < 32);

	int *HitCounts = (int *)malloc(Containers.Count * sizeof(int));
	memset(HitCounts, 0, Containers.Count * sizeof(int));
	int CombCount = 1 << Containers.Count;
	int HitCountTotal = 0;
	int MinContainerCount = Containers.Count;
	for (int Combination = 0; Combination < CombCount; ++Combination)
	{
		int Capacity = 0;
		int ContainerCount = 0;
		for (int ContainerIt = 0; ContainerIt < Containers.Count; ++ContainerIt)
		{
			if (!!(Combination & (1 << ContainerIt)))
			{
				++ContainerCount;
				Capacity += Containers.Array[ContainerIt];
			}
		}

		if (Capacity == 150)
		{
			++HitCounts[ContainerCount];
			++HitCountTotal;
			MinContainerCount = Min(MinContainerCount, ContainerCount);
		}
	}

	printf("%d\n", HitCountTotal);
	printf("%d\n", HitCounts[MinContainerCount]);

	free(Containers.Array);
	free(HitCounts);
}
