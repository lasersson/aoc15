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
		FILE *File = fopen(Argv[1], "r");
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
