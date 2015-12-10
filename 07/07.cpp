#include <aoc.h>
#include <stdint.h>

typedef uint16_t u16;
typedef uint32_t u32;

enum op
{
	Op_And,
	Op_Or,
	Op_ShiftR,
	Op_ShiftL,
	Op_Not,
	Op_Set,
};

typedef u32 id;

static id
MakeId(char *Name)
{
	id Id;
	if (Name[0] >= '0' && Name[0] <= '9')
	{
		int Value = atoi(Name);
		Assert((Value & 0xffff) == Value);
		Id = (id)Value;
	}
	else
	{
		size_t NameLen = strlen(Name);
		Assert(NameLen <= 3);
		Id = 
			(0x80 << 24) |
			(Name[0] << 16) |
			((NameLen > 1 ? Name[1] : 0) << 8) |
			((NameLen > 2 ? Name[2] : 0) << 0);
	}

	return Id;
}

struct wire
{
	id Id;

	op Op;
	id Operand1;
	id Operand2;
};

struct circuit
{
	wire *Wires;
	int WireCount;
	int WireCap;
};

static wire *
PushWire(circuit *Circuit)
{
	if (Circuit->WireCount == Circuit->WireCap)
	{
		Circuit->WireCap = Max(1, Circuit->WireCap * 2);
		Circuit->Wires = (wire *)realloc(Circuit->Wires, sizeof(wire) * Circuit->WireCap);
	}
	
	wire *Wire = Circuit->Wires + Circuit->WireCount;
	++Circuit->WireCount;
	return Wire;
}

static circuit
CloneCircuit(circuit *Src)
{
	circuit Dst = {};
	Dst.WireCount = Dst.WireCap = Src->WireCount;
	Dst.Wires = (wire *)malloc(Dst.WireCount * sizeof(wire));
	memcpy(Dst.Wires, Src->Wires, sizeof(wire) * Src->WireCount);
	return Dst;
}

static wire *
FindWire(id Id, circuit *Circuit)
{
	wire *Result = nullptr;
	for (int It = 0; It < Circuit->WireCount; ++It)
	{
		wire *Wire = Circuit->Wires + It;
		if (Wire->Id == Id)
		{
			Result = Wire;
			break;
		}
	}

	Assert(Result);
	return Result;
}

struct cached_value
{
	id Id;
	u16 Value;
};

static u16
GetValue(id Id, circuit *Circuit)
{
	u16 Value;
	if (Id & (1 << 31))
	{
		wire *Wire = FindWire(Id, Circuit);
		u16 Value1 = GetValue(Wire->Operand1, Circuit);
		if (Wire->Op == Op_Set)
		{
			Value = Value1;
		}
		else if (Wire->Op == Op_Not)
		{
			Value = ~Value1;
		}
		else
		{
			u16 Value2 = GetValue(Wire->Operand2, Circuit);
			switch (Wire->Op)
			{
				case Op_And: Value = Value1 & Value2; break;
				case Op_Or: Value = Value1 | Value2; break;
				case Op_ShiftR: Value = Value1 >> Value2; break;
				case Op_ShiftL: Value = Value1 << Value2; break;
				default: Assert(!"invalid op"); Value = 0; break;
			}
		}

		Wire->Op = Op_Set;
		Wire->Operand1 = Value;
	}
	else
	{
		Value = (u16)Id;
	}
	
	return Value;
}


static void
Solve(input_file Input)
{
	circuit Circuit = {};
	char *Delim = "\n ";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		wire *Wire = PushWire(&Circuit);

		if (strcmp(Token, "NOT") == 0)
		{
			Wire->Op = Op_Not;
			Token = strtok(nullptr, Delim);
			Wire->Operand1 = MakeId(Token);
			Token = strtok(nullptr, Delim);
		}
		else
		{
			Wire->Operand1 = MakeId(Token);

			Token = strtok(nullptr, Delim);
			if (Token[0] == '-')
			{
				Wire->Op = Op_Set;
			}
			else
			{
				if (Token[0] == 'A')
					Wire->Op = Op_And;
				else if (Token[0] == 'O')
					Wire->Op = Op_Or;
				else if (Token[0] == 'R')
					Wire->Op = Op_ShiftR;
				else if (Token[0] == 'L')
					Wire->Op = Op_ShiftL;

				Token = strtok(nullptr, Delim);
				Wire->Operand2 = MakeId(Token);
				Token = strtok(nullptr, Delim);
			}
		}

		Assert(Token[0] == '-' && Token[1] == '>');

		Token = strtok(nullptr, Delim);
		Wire->Id = MakeId(Token);

		Token = strtok(nullptr, Delim);
	}

	circuit SolveCircuit = CloneCircuit(&Circuit);
	id WireAId = MakeId("a");
	u16 Value = GetValue(WireAId, &SolveCircuit);
	printf("%d\n", Value);

	SolveCircuit = CloneCircuit(&Circuit);
	wire *WireB = FindWire(MakeId("b"), &SolveCircuit);
	WireB->Op = Op_Set;
	WireB->Operand1 = Value;
	WireB->Operand2 = 0;
	Value = GetValue(WireAId, &SolveCircuit);
	printf("%d\n", Value);

	free(Circuit.Wires);
	free(SolveCircuit.Wires);
}
