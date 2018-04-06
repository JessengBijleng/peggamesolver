#pragma once

#include <array>
#include <vector>
#include <iostream>
#include <functional>
#include <stack>
#include <queue>
#include <set>

enum class Move { L, R, UL, UR, DL, DR };
enum class Hole { UNUSED, PEG, EMPTY};
using State = std::array<std::array<Hole, 11>, 5>;

using Vertex = State;
using Path = std::vector<Vertex>;
using Steps = std::set<Vertex>;

class Graph
{
public:
	typename std::vector<Vertex>::const_iterator cbegin(Vertex v, int r, int c, int move) const;
	typename std::vector<Vertex>::const_iterator cend() const;

	bool solve(Vertex v, Path &path, std::function<bool(const Vertex &vertex)> goalTest) const;
	int findMoves(Vertex v, std::queue<std::array<int, 3>>& validMoves) const;
private:
	mutable std::vector<Vertex> adjacents;
};

Path bfs(const Graph &graph, const Vertex &start, std::function<bool(const Vertex &vertex)> goalTest);