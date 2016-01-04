#include <aoc.h>

enum node_type
{
	NodeType_Object,
	NodeType_Array,
	NodeType_Key,
	NodeType_Value,
};

struct node
{
	node_type Type;
	bool IsRed;
	int Sum;
	int RedSum;
};

struct stack
{
	node Array[1024];
	int Count;
};

static node *
Push(stack *Stack, node_type Type)
{
	node *Result = Stack->Array + Stack->Count;
	Result->Type = Type;
	bool IsRed = false;
	if (Stack->Count > 0)
	{
		IsRed = Stack->Array[Stack->Count - 1].IsRed;
	}
	Result->IsRed = IsRed;
	Result->Sum = 0;
	Result->RedSum = 0;
	++Stack->Count;
	return Result;
}

static node *
Pop(stack *Stack)
{
	Assert(Stack->Count > 0);
	--Stack->Count;
	node *Result = nullptr;
	if (Stack->Count > 0)
	{
		Result = Stack->Array + Stack->Count - 1;
	}
	return Result;
}

static stack NodeStack;

static void
Solve(input_file Input)
{
	node *Root = Push(&NodeStack, NodeType_Object);
	node *Node = Root;

	for (char *Cur = Input.Contents; *Cur; ++Cur)
	{
		switch (*Cur)
		{
			case '{':
			{
				Node = Push(&NodeStack, NodeType_Object);
				break;
			}
			case '[':
			{
				Node = Push(&NodeStack, NodeType_Array);
				break;
			}

			case ']':
			case '}':
			{
				node *Parent = Pop(&NodeStack);
				Parent->Sum += Node->Sum;
				if (Node->IsRed)
				{
					Parent->RedSum += Node->Sum;
				}
				else
				{
					Parent->RedSum += Node->RedSum;
				}
				Node = Parent;
				break;
			}

			case '"':
			case ',':
			case ':':
				break;

			default:
				if (Node)
				{
					if (Node->Type == NodeType_Object &&
						Cur[0] == 'r' && Cur[1] == 'e' && Cur[2] == 'd')
					{
						Node->IsRed = true;
						Cur += 3;
					}
					else if (Cur[0] == '-' || (Cur[0] >= '0' && Cur[0] <= '9'))
					{
						int v = strtol(Cur, &Cur, 10);
						Node->Sum += v;
						--Cur;
					}
				}
				break;
		}
	}

	Assert(Node == Root);
	printf("%d\n", Node->Sum);
	printf("%d\n", Node->Sum - Node->RedSum);
}
