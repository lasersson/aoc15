#include <aoc.h>
#include <stdint.h>

typedef uint8_t u8;

static u8 *
LookSay(u8 *In, u8 *Out)
{
	u8 Cur = In[0];
	u8 Count = 1;
	for (int It = 1; It < GACount(In); ++It)
	{
		u8 i = In[It];
		if (i != Cur)
		{
			GAPush(Out, Count);
			GAPush(Out, Cur);
			Cur = i;
			Count = 1;
		}
		else
		{
			Assert(Count < 9);
			++Count;
		}
	}
	Assert(Count > 0);
	GAPush(Out, Count);
	GAPush(Out, Cur);

	return Out;
}

static void
Solve(input_file Input)
{
	u8 *Buf1 = 0;
	u8 *Buf2 = 0;

	char *Token = strtok(Input.Contents, "\n ");
	int Len = (int)strlen(Token);
	for (int It = 0; It < Len; ++It)
	{
		GAPush(Buf1, Token[It] - '0');
	}

	u8 *In = Buf1;
	u8 *Out = Buf2;
	for (int It = 0; It < 50; ++It)
	{
		Out = LookSay(In, Out);
		if (It == 39 || It == 49)
		{
			printf("%d\n", GACount(Out));
		}
		u8 *Tmp = In;
		In = Out;
		Out = Tmp;
		GACount(Out) = 0;
	}

	GAFree(In);
	GAFree(Out);
}
