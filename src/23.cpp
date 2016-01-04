#include <aoc.h>

enum op
{
	Op_Invalid,
	Op_Hlf,
	Op_Tpl,
	Op_Inc,
	Op_Jmp,
	Op_Jie,
	Op_Jio,
};

static char *OpStrMap[] = { "", "hlf", "tpl", "inc", "jmp", "jie", "jio" };

static op
OpFromString(char *Token)
{
	op Op = Op_Invalid;
	for (int OpIt = 0; OpIt < ArrayCount(OpStrMap); ++OpIt)
	{
		if (strcmp(Token, OpStrMap[OpIt]) == 0)
		{
			Op = (op)OpIt;
			break;
		}
	}
	return Op;
}

struct instr
{
	op Op;
	int Param1;
	int Param2;
};

static int ParamFromString(char *Token)
{
	int Param;
	switch (Token[0])
	{
		case 'a':
			Param = 0; break;
		case 'b':
			Param = 1; break;
		default:
			Param = atoi(Token); break;
	}
	return Param;
}

struct cpu_state
{
	int Regs[2];
	int pc;
};

static cpu_state
RunProgram(cpu_state State, instr *Program)
{
	while (State.pc < GACount(Program))
	{
		instr Instr = Program[State.pc];
		int PcOffset = 1;
		switch (Instr.Op)
		{
			case Op_Hlf:
				State.Regs[Instr.Param1] /= 2;
				break;
			case Op_Tpl:
				State.Regs[Instr.Param1] *= 3;
				break;
			case Op_Inc:
				State.Regs[Instr.Param1] += 1;
				break;
			case Op_Jmp:
				PcOffset = Instr.Param1;
				break;
			case Op_Jie:
				if ((State.Regs[Instr.Param1] & 1) == 0)
				{
					PcOffset = Instr.Param2;
				}
				break;
			case Op_Jio:
				if (State.Regs[Instr.Param1] == 1)
				{
					PcOffset = Instr.Param2;
				}
				break;
			default:
				Assert(!"Illegal instruction");
				break;
		}
		State.pc += PcOffset;
	}
	return State;
}

static instr *
AddInstr(instr* Pgm, instr Instr)
{
	GAPush(Pgm, Instr);
	return Pgm;
}

static output
Solve(input Input)
{
	char *Delim = "\r\n ,";
	char *Token = strtok(Input.Contents, Delim);
	instr *Program = nullptr;
	while (Token)
	{
		instr Instr = {};
		Instr.Op = OpFromString(Token);
		Instr.Param1 = ParamFromString(strtok(nullptr, Delim));
		switch (Instr.Op)
		{
			case Op_Jio:
			case Op_Jie:
				Instr.Param2 = ParamFromString(strtok(nullptr, Delim));
			default:
				Token = strtok(nullptr, Delim);
		}
		GAPush(Program, Instr);
	}

	cpu_state CpuState;
	output Output;

	CpuState = RunProgram({{0, 0}, 0}, Program);
	Output.a = CpuState.Regs[1];
	CpuState = RunProgram({{1, 0}, 0}, Program);
	Output.b = CpuState.Regs[1];

	GAFree(Program);
	return Output;
}
