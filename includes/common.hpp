#pragma once

using namespace std;
using uint = unsigned int;


typedef	struct  Node
{
	uint x, y, f_score, g_score;

	bool operator==(const Node& n) const
	{
		return (x == n.x && y == n.y);
	} 

	bool operator!=(const Node& n) const
	{
		return (x != n.x || y != n.y);
	}

	friend ostream& operator<<(ostream& out, const Node& n)
	{
		out << '[' << n.x << ", " << n.y << ']';
		return (out);
	}
} Node;
