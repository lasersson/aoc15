#ifndef __AOC_H__
#define __AOC_H__
#ifdef _WINDOWS
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#if defined(_WINDOWS)
#include <Windows.h>
#define AOC_WINDOWS 1
#elif defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L
#include <signal.h>
#define DebugBreak() raise(SIGTRAP)
#define RotateLeft32(x, y) _rotl(x, y)
#define AOC_POSIX 1
#else
#error Unsupported platform
#endif

#if defined(AOC_DEBUG)
#define Assert(x) do { if (!(x)) DebugBreak(); } while(0)
#else
#define Assert(...)
#endif

#define ArrayCount(x) (sizeof(x)/sizeof((x)[0]))

inline int
Min(s64 a, s64 b)
{
	int Result = a < b ? a : b;
	return Result;
}

inline int
Max(s64 a, s64 b)
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

static u16
Fletcher16(const void *Data, u32 ByteCount)
{
	u8 *Bytes = (u8 *)Data;
	u16 s1 = 0;
	u16 s2 = 0;
	for (u32 It = 0; It < ByteCount; ++It)
	{
		s1 = (s1 + Bytes[It]) % 255;
		s2 = (s2 + s1) % 255;
	}
	
	u16 Hash = (s2 << 8) | (s1);
	return Hash;
}

static u32
Fletcher32(const void *Data, u32 ByteCount)
{
	u32 s1 = 0xffff, s2 = 0xffff;
	u32 tLen;
 
	Assert((ByteCount & 3) == 0);
	u16 *Words = (uint16_t *)Data;
	u32 WordCount = ByteCount >> 2;
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
	u32 Hash = s2 << 16 | s1;
	return  Hash;
}

struct hash_table
{
	u32 **Buckets;
	int BucketCount;
	int Count;
};

static hash_table
InitHashTable(int BucketCount)
{
	hash_table Table;
	Table.Buckets = (u32 **)malloc(sizeof(u32*) * BucketCount);
	memset(Table.Buckets, 0, sizeof(u32 *) * BucketCount);
	Table.BucketCount = BucketCount;
	Table.Count = 0;
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
	Table->Count = 0;
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
		++Table->Count;
	}
	return Result;
}

struct input
{
	char *Contents;
	int Length;
	bool Ok;
};

#if defined(AOC_WINDOWS)
typedef u64 timestamp;
#elif defined(AOC_POSIX)
#include <time.h>
typedef timespec timestamp;
#endif

static timestamp
GetTimestamp()
{
#if defined(AOC_WINDOWS)
	LARGE_INTEGER Counts;
	QueryPerformanceCounter(&Counts);
	return Counts.QuadPart;
#elif defined(AOC_POSIX)
	timespec TimeSpec;
	clock_gettime(CLOCK_MONOTONIC, &TimeSpec);
	return TimeSpec;
#endif
}

static double
GetElapsedMilliseconds(timestamp Begin, timestamp End)
{
#if defined(AOC_WINDOWS)
	LARGE_INTEGER CountsPerSecond;
	QueryPerformanceFrequency(&CountsPerSecond);
	u64 ElapsedCounts = End - Begin;
	double ElapsedMilliseconds = (double)ElapsedCounts / (double)CountsPerSecond.QuadPart * 1000.0;
	return ElapsedMilliseconds;
#elif defined(AOC_POSIX)
	timestamp ElapsedTime;
	time_t ElapsedSeconds = End.tv_sec - Begin.tv_sec;
	long ElapsedNanoseconds = End.tv_nsec - Begin.tv_nsec;
	if (ElapsedNanoseconds < 0)
	{
		--ElapsedSeconds;
		ElapsedNanoseconds = 1000000000L + ElapsedNanoseconds;
	}
	double ElapsedMilliseconds = 1000.0 * (double)ElapsedSeconds + (double)ElapsedNanoseconds / 1000000.0;
	return ElapsedMilliseconds;
#endif
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
		s64 a;
		s64 b;
	};
	s64 v[2];
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

			timestamp BeginTime = GetTimestamp();
			output Output = Solve(RunInput);
			timestamp EndTime = GetTimestamp();
			double ElapsedMilliseconds = GetElapsedMilliseconds(BeginTime, EndTime);

			if (RunCount == 0)
			{
				printf("%ld\n%ld\n", Output.a, Output.b);
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
