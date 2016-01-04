#ifndef __AOC_H__
#define __AOC_H__
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

#if defined(AOC_DEBUG)
#define Assert(x) do { if (!(x)) DebugBreak(); } while(0)
#else
#define Assert(...)
#endif

#define ArrayCount(x) (sizeof(x)/sizeof((x)[0]))

inline int
Min(int a, int b)
{
	int Result = a < b ? a : b;
	return Result;
}

inline int
Max(int a, int b)
{
	int Result = a > b ? a : b;
	return Result;
}

inline uint64_t
Min(uint64_t a, uint64_t b)
{
	uint64_t Result = a < b ? a : b;
	return Result;
}

inline uint64_t
Max(uint64_t a, uint64_t b)
{
	uint64_t Result = a > b ? a : b;
	return Result;
}

#define GAHeader(Ary) ((int *)(Ary) - 2)
#define GACapacity(Ary) (GAHeader(Ary)[0])
#define GACount(Ary) ((Ary) ? GAHeader(Ary)[1] : 0)
#define GAClear(Ary) (GAHeader(Ary)[1] = 0)
#define GATop(Ary) ((Ary)[GACount(Ary) - 1])
#define GAGrow(Ary) (Ary = (decltype(Ary))GAAlloc(Ary, sizeof((Ary)[0]), 1))
#define GAPush(Ary, x) (GAGrow(Ary), Ary[GAHeader(Ary)[1]++] = (x))
#define GAInit(Ary, n) ((decltype(Ary))GAAlloc(Ary, sizeof((Ary)[0]), n))
#define GAFree(Ary) free((int *)Ary - 2)

static void *
GAAlloc(void *Ary, int ElemSize, int n)
{
	int *NewAry = (int *)Ary;
	int MinCap = n;
	if (Ary)
	{
		MinCap += GACount(Ary);
	}
	if (Ary == nullptr || MinCap > GACapacity(Ary))
	{
		int Cap = Ary == nullptr ? 0 : GACapacity(Ary);
		Cap = Max(MinCap, Cap * 2);
		int Count = Ary ? GACount(Ary) : 0;
		NewAry = (int *)realloc(Ary ? GAHeader(Ary) : 0, Cap * ElemSize + sizeof(int) * 2);
		Assert(NewAry);
		NewAry[0] = Cap;
		NewAry[1] = Count;

		NewAry += 2;
	}

	return NewAry;
}

static void
GAPop(void *Ary)
{
	Assert(GACount(Ary) > 0);
	--GAHeader(Ary)[1];
}

struct input
{
	char *Contents;
	int Length;
	bool Ok;
};

static uint64_t
GetCounts()
{
	LARGE_INTEGER Counts;
	QueryPerformanceCounter(&Counts);
	return Counts.QuadPart;
}

static double
GetElapsedMilliseconds(uint64_t BeginCounts, uint64_t EndCounts)
{
	LARGE_INTEGER CountsPerSecond;
	QueryPerformanceFrequency(&CountsPerSecond);
	uint64_t ElapsedCounts = EndCounts - BeginCounts;
	double ElapsedMilliseconds = (double)ElapsedCounts / (double)CountsPerSecond.QuadPart * 1000.0;
	return ElapsedMilliseconds;
}

static input
ReadInfileFromArgs(int Argc, char **Argv)
{
	input Result = {};
	if (Argc >= 2)
	{
		FILE *File = fopen(Argv[1], "rb");
		if (File)
		{
			fseek(File, 0, SEEK_END);
			size_t FileSize = ftell(File);
			fseek(File, 0, SEEK_SET);

			Result.Length = (int)FileSize;
			if ((size_t)Result.Length == FileSize)
			{
				Result.Contents = (char *)malloc(Result.Length + 1);
				Result.Contents[Result.Length] = 0;
				if (Result.Contents)
				{
					size_t BytesRead = fread(Result.Contents, 1, Result.Length, File);
					if (BytesRead == Result.Length)
					{
						Result.Ok = true;
					}
					else
					{
						printf("I/O error\n");
						free(Result.Contents);
						Result = {};
					}
				}
				else
				{
					printf("Out of memory\n");
					Result = {};
				}
			}
			else
			{
				printf("File too large\n");
				Result = {};
			}
		}
		else
		{
			printf("Could not open file \"%s\"\n", Argv[1]);
		}
	}
	else
	{
		printf("Expected exactly one argument\n");
	}

	return Result;
}

union output
{
	struct
	{
		uint64_t a;
		uint64_t b;
	};
	uint64_t v[2];
};

static output Solve(input Input);

int main(int Argc, char **Argv)
{
	input Input = ReadInfileFromArgs(Argc, Argv);
	if (Input.Ok)
	{
		bool IsTiming = Argc >= 3 && strcmp(Argv[2], "-time") == 0;
		double TimingLimit = 1000.0;
		if (Argc >= 4)
		{
			TimingLimit = atof(Argv[3]) * 1000.0;
		}
		double ElapsedMillisecondsTotal = 0.0;

		int RunCount = 0;
		input RunInput = Input;
		RunInput.Contents = (char *)malloc(Input.Length+1);
		do
		{
			memcpy(RunInput.Contents, Input.Contents, Input.Length+1);

			uint64_t BeginCounts = GetCounts();
			output Output = Solve(RunInput);
			uint64_t EndCounts = GetCounts();
			double ElapsedMilliseconds = GetElapsedMilliseconds(BeginCounts, EndCounts);

			if (RunCount == 0)
			{
				printf("%llu\n%llu\n", Output.a, Output.b);
			}

			ElapsedMillisecondsTotal += ElapsedMilliseconds;
			++RunCount;
		} while (IsTiming && ElapsedMillisecondsTotal < TimingLimit);

		if (IsTiming)
		{
			printf("%fms\n", ElapsedMillisecondsTotal / (double)RunCount);
		}

		free(RunInput.Contents);
		free(Input.Contents);
	}
}


#endif
