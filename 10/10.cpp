#include <aoc.h>
#include <stdint.h>

typedef uint8_t u8;

struct buffer
{
	u8 *Array;
	int Cap;
	int Count;
};

static void
BufPush(buffer *Buf, u8 v)
{
	if (Buf->Count == Buf->Cap)
	{
		Buf->Cap = Max(1, Buf->Cap * 2);
		Buf->Array = (u8 *)realloc(Buf->Array, sizeof(Buf->Array[0]) * Buf->Cap);
		Assert(Buf->Array);
	}
	Buf->Array[Buf->Count++] = v;
}

static void
LookSay(buffer *In, buffer *Out)
{
	u8 Cur = In->Array[0];
	u8 Count = 1;
	for (int It = 1; It < In->Count; ++It)
	{
		u8 i = In->Array[It];
		if (i != Cur)
		{
			BufPush(Out, Count);
			BufPush(Out, Cur);
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
	BufPush(Out, Count);
	BufPush(Out, Cur);
}

static void
Solve(input_file Input)
{
	buffer Buf1 = {};
	buffer Buf2 = {};

	char *Token = strtok(Input.Contents, "\n ");
	int Len = (int)strlen(Token);
	for (int It = 0; It < Len; ++It)
	{
		BufPush(&Buf1, Token[It] - '0');
	}

	buffer *In = &Buf1;
	buffer *Out = &Buf2;
	for (int It = 0; It < 50; ++It)
	{
		LookSay(In, Out);
		if (It == 39 || It == 49)
		{
			printf("%d\n", Out->Count);
		}
		buffer *Tmp = In;
		In = Out;
		Out = Tmp;
		Out->Count = 0;
	}

	free(Buf1.Array);
	free(Buf2.Array);
}
