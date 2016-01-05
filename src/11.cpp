#include <aoc.h>

#define PWD_LEN 8

struct password
{
	char Buf[PWD_LEN+1];
};

static void
PrintPassword(password Pwd)
{
	for (int It = 0; It < PWD_LEN; ++It)
	{
		printf("%c", Pwd.Buf[It] + 'a');
	}
	printf("\n");
}

static bool
IsValidChar(char c)
{
	char d = c + 'a';
	bool Valid = !(d == 'i' || d == 'o' || d == 'l');
	return Valid;
}

static bool
IsValid(password Pwd)
{
	bool HasStraight = false;
	bool CharsAreValid = true;
	int PairCount = 0;
	bool PrevWasPair = false;
	for (int It = 0; It < PWD_LEN; ++It)
	{
		char c = Pwd.Buf[It];
		CharsAreValid &= IsValidChar(c);

		if (It > 0)
		{
			if (c == Pwd.Buf[It-1] && !PrevWasPair)
			{
				++PairCount;
				PrevWasPair = true;
			}
			else
			{
				PrevWasPair = false;
			}
		}

		if (It > 1)
		{
			if (Pwd.Buf[It-2] == c - 2 &&
				Pwd.Buf[It-1] == c - 1)
			{
				HasStraight = true;
			}
		}
	}
	
	bool Result = HasStraight && CharsAreValid && PairCount >= 2;
	return Result;
}

static char
Increment(char c)
{
	++c;
	if (!IsValidChar(c))
	{
		++c;
	}
	return c;
}

static password
GetNext(password OldPwd)
{
	password Pwd = OldPwd;
	do
	{
		Pwd.Buf[PWD_LEN-1] = Increment(Pwd.Buf[PWD_LEN-1]);
		for (int It = PWD_LEN-1; It >= 0; --It)
		{
			if (Pwd.Buf[It] > 25)
			{
				Pwd.Buf[It] -= 26;
				if (It > 0)
				{
					Pwd.Buf[It-1] = Increment(Pwd.Buf[It-1]);
				}
			}
			else
			{
				break;
			}
		}
	} while(!IsValid(Pwd));
	
	return Pwd;
}

static output
Solve(input Input)
{
	password Pwd = {};
	char *PwStr = strtok(Input.Contents, "\n ");
	Assert(strlen(PwStr) == PWD_LEN);
	for (int It = 0; It < PWD_LEN; ++It)
	{
		Pwd.Buf[It] = PwStr[It] - 'a';
	}

	output Output;

	Pwd = GetNext(Pwd);
	Output.a = *(u64 *)&Pwd;
	Pwd = GetNext(Pwd);
	Output.b = *(u64 *)&Pwd;
	return Output;
}
