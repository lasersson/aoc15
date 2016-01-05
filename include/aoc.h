#ifndef __AOC_H__
#define __AOC_H__
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

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

static uint32_t
Fletcher32(void *Data, uint32_t ByteCount)
{
	uint32_t s1 = 0xffff, s2 = 0xffff;
	uint32_t tLen;
 
	Assert((ByteCount & 3) == 0);
	uint16_t *Words = (uint16_t *)Data;
	uint32_t WordCount = ByteCount >> 2;
	while (WordCount)
	{
		tLen = WordCount >= 359 ? 359 : WordCount;
		WordCount -= tLen;
		do 
		{
			s2 += s1 += *Words++;
		} while (--tLen);
		s1 = (s1 & 0xffff) + (s1 >> 16);
		s2 = (s2 & 0xffff) + (s2 >> 16);
	}

	s1 = (s1 & 0xffff) + (s1 >> 16);
	s2 = (s2 & 0xffff) + (s2 >> 16);
	uint32_t Hash = s2 << 16 | s1;
	return  Hash;
}

struct hash_table
{
	u32 **Buckets;
	int BucketCount;
};

static hash_table
InitHashTable(int BucketCount)
{
	hash_table Table;
	Table.Buckets = (u32 **)malloc(sizeof(u32*) * BucketCount);
	memset(Table.Buckets, 0, sizeof(u32 *) * BucketCount);
	Table.BucketCount = BucketCount;
	return Table;
}

static void
Clear(hash_table *Table)
{
	for (int It = 0; It < Table->BucketCount; ++It)
	{
		u32 *Bucket = Table->Buckets[It];
		if (Bucket)
		{
			GAClear(Bucket);
		}
	}
}

static void
FreeHashTable(hash_table *Table)
{
	for (int It = 0; It < Table->BucketCount; ++It)
	{
		u32 *Bucket = Table->Buckets[It];
		if (Bucket)
		{
			GAFree(Bucket);
		}
	}
	free(Table->Buckets);
	*Table = {};
}

static bool
InBucket(u32 *Bucket, u32 Hash)
{
	bool Result = false;
	for (int It = 0; It < GACount(Bucket); ++It)
	{
		if (Bucket[It] == Hash)
		{
			Result = true;
			break;
		}
	}
	return Result;
}

inline bool
InTable(hash_table *Table, u32 Hash)
{
	bool Result = InBucket(Table->Buckets[Hash % Table->BucketCount], Hash);
	return Result;
}

static bool
InsertHash(hash_table *Table, u32 Hash)
{
	int BucketIndex = Hash % Table->BucketCount;
	u32 *Bucket = Table->Buckets[BucketIndex];
	bool Result = InBucket(Bucket, Hash);
	if (!Result)
	{
		GAPush(Bucket, Hash);
		Table->Buckets[BucketIndex] = Bucket;
	}
	return Result;
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
