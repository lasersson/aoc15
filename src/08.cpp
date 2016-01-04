#include <aoc.h>

enum state
{
	State_Begin,
	State_Default,
	State_Escaped,
	State_Hex0,
	State_Hex1,
};

struct parser
{
	state State;
	int Length;
};

static parser
Decode(char c, parser Parser)
{
	if (Parser.State == State_Begin)
	{
		Parser.State = State_Default;
	}

	switch (c)
	{
		case '\\':
		{
			if (Parser.State == State_Escaped)
			{
				++Parser.Length;
				Parser.State = State_Default;
			}
			else
			{
				Parser.State = State_Escaped;
			}
			break;
		}
		case '"':
		{
			if (Parser.State == State_Escaped)
			{
				++Parser.Length;
				Parser.State = State_Default;
			}
			break;
		}
		default:
		{
			switch (Parser.State)
			{
				case State_Escaped:
				{
					Assert(c == 'x');
					Parser.State = State_Hex0;
					break;
				}
				case State_Hex0:
				{
					Parser.State = State_Hex1;
					break;
				}
				case State_Hex1:
				{
					Parser.State = State_Default;
					++Parser.Length;
					break;
				}
				default:
				{
					++Parser.Length;
				}
			}
			break;
		}
	}

	return Parser;
}

static parser
Encode(char c, parser Parser)
{
	if (Parser.State == State_Begin)
	{
		Parser.Length += 2;
		Parser.State = State_Default;
	}

	switch (c)
	{
		case '"':
		case '\\':
		{
			Parser.Length += 2;
			break;
		}
		default:
		{
			Parser.Length += 1;
			break;
		}
	}
	return Parser;
}

static parser
Reset(parser Parser)
{
	Parser.State = State_Begin;
	return Parser;
}

static output
Solve(input Input)
{
	int LineLengthTotal = 0;
	char *Line = strtok(Input.Contents, "\n");

	parser Decoder = {};
	parser Encoder = {};

	while (Line)
	{
		int LineLength = (int)strlen(Line);
		LineLengthTotal += LineLength;

		Decoder = Reset(Decoder);
		Encoder = Reset(Encoder);
		for (int It = 0; It < LineLength; ++It)
		{
			int LineLengthDecoded = 0;
			Decoder = Decode(Line[It], Decoder);
			Encoder = Encode(Line[It], Encoder);
		}

		Line = strtok(nullptr, "\n");
	}

	output Output = { LineLengthTotal - Decoder.Length, Encoder.Length - LineLengthTotal };
	return Output;
}
