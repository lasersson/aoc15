#ifndef __AOC_H__
#define __AOC_H__
#include <Windows.h>
#include <stdio.h>

#if defined(AOC_DEBUG)
#define Assert(x) do { if (!(x)) DebugBreak(); } while(0)
#else
#define Assert(...)
#endif

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

struct input_file
{
	char *Contents;
	int Length;
	bool Ok;
};

static input_file
ReadInfileFromArgs(int Argc, char **Argv)
{
	input_file Result = {};
	if (Argc == 2)
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

static void Solve(input_file Input);

int main(int Argc, char **Argv)
{
	input_file Input = ReadInfileFromArgs(Argc, Argv);
	if (Input.Ok)
	{
		Solve(Input);
	}
}


#endif
