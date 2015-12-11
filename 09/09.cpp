#include <aoc.h>
#include <stdint.h>

typedef uint16_t node;

static node
MakeNodeId(char *Str)
{
	int Len = (int)strlen(Str);
	node Sum1 = 0;
	node Sum2 = 0;
	for (int It = 0; It < Len; ++It)
	{
		Sum1 = (Sum1 + Str[It]) % 255;
		Sum2 = (Sum2 + Sum1) % 255;
	}
	
	node Node = (Sum2 << 8) | (Sum1);
	return Node;
}

struct node_stack
{
	node *Nodes;
	int Cap;
	int Count;
};

static node
HasNode(node_stack *Stack, node Node)
{
	bool Exists = false;
	for (int It = 0; It < Stack->Count; ++It)
	{
		if (Stack->Nodes[It] == Node)
		{
			Exists = true;
			break;
		}
	}
	return Exists;
}

static void
PushNode(node_stack *Stack, node Node)
{
	if (Stack->Count == Stack->Cap)
	{
		Stack->Cap = Max(1, Stack->Cap * 2);
		Stack->Nodes = (node *)realloc(Stack->Nodes, sizeof(node) * Stack->Cap);
		Assert(Stack->Nodes);
	}
	Stack->Nodes[Stack->Count++] = Node;
}

static void
PopNode(node_stack *Stack)
{
	Assert(Stack->Count > 0);
	--Stack->Count;
}

static node
MakeNode(node_stack *Stack, char *NodeName)
{
	node Node = MakeNodeId(NodeName);
	if (!HasNode(Stack, Node))
	{
		PushNode(Stack, Node);
	}
	return Node;
}

struct edge
{
	node Node1;
	node Node2;
	int Dist;
};

struct graph
{
	edge *Edges;
	int Count;
	int Cap;
};

static void
PushEdge(graph *Graph, node Node1, node Node2, int Dist)
{
	if (Graph->Count == Graph->Cap)
	{
		Graph->Cap = Max(1, Graph->Cap * 2);
		Graph->Edges = (edge *)realloc(Graph->Edges, sizeof(edge) * Graph->Cap);
		Assert(Graph->Edges);
	}

	Graph->Edges[Graph->Count] = { Node1, Node2, Dist };
	++Graph->Count;
}

struct path_result
{
	int Shortest;
	int Longest;
};

static path_result
PathMinMax(path_result A, path_result B)
{
	A.Shortest = Min(A.Shortest, B.Shortest);
	A.Longest = Max(A.Longest, B.Longest);
	return A;
}

static path_result
FindPaths(node Node, graph *Graph, node_stack *Visited)
{
	PushNode(Visited, Node);

	path_result Result = { 9999999, 0 };
	for (int It = 0; It < Graph->Count; ++It)
	{
		edge *Edge = Graph->Edges + It;
		if (Edge->Node1 == Node || Edge->Node2 == Node)
		{
			node OtherNode = Edge->Node1 == Node ? Edge->Node2 : Edge->Node1;
			if (!HasNode(Visited, OtherNode))
			{
				path_result EdgeRes = FindPaths(OtherNode, Graph, Visited);
				EdgeRes.Shortest += Edge->Dist;
				EdgeRes.Longest += Edge->Dist;
				Result = PathMinMax(Result, EdgeRes);
			}
		}
	}
	PopNode(Visited);

	if (Result.Shortest == 9999999 && Result.Longest == 0)
	{
		Result = {};
	}

	return Result;
}

static void
Solve(input_file Input)
{
	node_stack AllNodes = {};
	graph Graph = {};

	char *Delim = "\n ";
	char *Token = strtok(Input.Contents, Delim);
	while (Token)
	{
		node Node1 = MakeNode(&AllNodes, Token);
		Token = strtok(nullptr, Delim);
		Assert(strcmp(Token, "to") == 0);
		Token = strtok(nullptr, Delim);
		node Node2 = MakeNode(&AllNodes, Token);
		Token = strtok(nullptr, Delim);
		Assert(Token[0] == '=');
		Token = strtok(nullptr, Delim);
		int Dist = atoi(Token);

		PushEdge(&Graph, Node1, Node2, Dist);

		Token = strtok(nullptr, Delim);
	}

	node_stack VisitedNodes = {};
	path_result Result = { 9999999, 0 };
	for (int It = 0; It < AllNodes.Count; ++It)
	{
		node Node = AllNodes.Nodes[It];
		Result = PathMinMax(Result, FindPaths(Node, &Graph, &VisitedNodes));
		Assert(VisitedNodes.Count == 0);
	}
	printf("%d\n", Result.Shortest);
	printf("%d\n", Result.Longest);

	free(AllNodes.Nodes);
	free(VisitedNodes.Nodes);
	free(Graph.Edges);
}
