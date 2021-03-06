#include "stdafx.h"
#include "PegGame.h"
#include <Chrono>;

//Print a vertex;
void print(Vertex v) {
	std::cout << std::endl;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			if (v[i][j] == Hole::UNUSED) {
				std::cout << " ";
			}

			if (v[i][j] == Hole::EMPTY) {
				std::cout << "0";
			}

			if (v[i][j] == Hole::PEG) {
				std::cout << "1";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// Check if the array bounds for the desired move are valid;
bool validBounds(const Vertex &v, const Move &m, int r, int c) { 
	switch (m) {
		case Move::L: if (c - 4 < 0) return false; break;
		case Move::R: if (c + 4 > 10) return false; break;
		case Move::UL: if (r - 2 < 0 || c - 2 < 0) return false; break;
		case Move::UR: if (r - 2 < 0 || c + 2 > 10) return false; break;
		case Move::DL: if (r + 2 > 4 || c - 2 < 0) return false; break;
		case Move::DR: if (r + 2 > 4 || c + 2 > 10) return false; break;
	}

	return true;
}

// Check if the peg jump can be made and the jump would be within array bounds;
bool canMove(const Vertex &v, const Move &m, int r, int c) {
	switch (m) {
		case Move::L: 
			if (validBounds(v, Move::L, r, c) && v[r][c - 4] == Hole::PEG && v[r][c - 2] == Hole::PEG) return true;
			break;
		case Move::R: 
			if (validBounds(v, Move::R, r, c) && v[r][c + 4] == Hole::PEG && v[r][c + 2] == Hole::PEG) return true;
			break;
		case Move::UL: 
			if (validBounds(v, Move::UL, r, c) && v[r - 2][c - 2] == Hole::PEG && v[r - 1][c - 1] == Hole::PEG) return true;
			break;
		case Move::UR: 
			if (validBounds(v, Move::UR, r, c) && v[r - 2][c + 2] == Hole::PEG && v[r - 1][c + 1] == Hole::PEG) return true;
			break;
		case Move::DL: 
			if (validBounds(v, Move::DL, r, c) && v[r + 2][c - 2] == Hole::PEG && v[r + 1][c - 1] == Hole::PEG) return true;
			break;
		case Move::DR: 
			if (validBounds(v, Move::DR, r, c) && v[r + 2][c + 2] == Hole::PEG && v[r + 1][c + 1] == Hole::PEG) return true;
			break;
	}
	
	return false;
}

// Perfrom a jump, empty the position being jumped from, and empty the jumped peg;
Vertex doMove(const Vertex &v, const Move &m, int r, int c)
{
	Vertex n = v;

	switch (m) {
		case Move::L: std::swap(n[r][c], n[r][c - 4]); n[r][c - 2] = Hole::EMPTY; break;
		case Move::R: std::swap(n[r][c], n[r][c + 4]); n[r][c + 2] = Hole::EMPTY; break;

		case Move::UL: std::swap(n[r][c], n[r - 2][c - 2]); n[r - 1][c - 1] = Hole::EMPTY; break;
		case Move::UR: std::swap(n[r][c], n[r - 2][c + 2]); n[r - 1][c + 1] = Hole::EMPTY; break;
		case Move::DL: std::swap(n[r][c], n[r + 2][c - 2]); n[r + 1][c - 1] = Hole::EMPTY; break;
		case Move::DR: std::swap(n[r][c], n[r + 2][c + 2]); n[r + 1][c + 1] = Hole::EMPTY; break;
	}

	return n;
}

// Adds possible moves to given queue and returns amout of possible moves.
int Graph::findMoves(Vertex v, std::queue<std::array<int, 3>>& validMoves) const {
	int empty = 0;

	for (int row = 0; row < 5; row++) {	
		for (int column = 0; column < 11; column++) {
			if (v[row][column] == Hole::EMPTY) {
				empty++;
				for (int move = 0; move < 6; move++) {
					if (canMove(v, static_cast<Move>(move), row, column)) {
						validMoves.push({row, column, move});
					}
				}
			}
		}
	}

	return empty;
}

// Performs a moves and returns the graphs adjacents
typename std::vector<Vertex>::const_iterator Graph::cbegin(Vertex v, int r, int c, int move) const
{
	adjacents.clear();
	adjacents.push_back(doMove(v, static_cast<Move>(move), r, c));
	return adjacents.cbegin();
}

typename std::vector<Vertex>::const_iterator Graph::cend() const
{
	return adjacents.cend();
}

// Solves the peg game recursively
bool Graph::solve(Vertex v, Path &path, std::function<bool(const Vertex &vertex)> goalTest) const {
	std::queue<std::array<int, 3>> moves;
	bool solved = false;

	int emptyCount = findMoves(v, moves);

	if (emptyCount == 14 && goalTest(v)) {
		return true;
	}

	while (!moves.empty() && !solved) {
		std::array<int, 3> toDo = moves.front();
		moves.pop();

		Vertex newMove = *cbegin(v, toDo[0], toDo[1], toDo[2]);
		path.push_back(newMove);

		// Recursive call
		solved = solve(newMove, path, goalTest);

		if (!solved)
			path.pop_back();
	}

	if (solved == true)
		return true;
	
	return false;
}

Path bfs(const Graph &graph, const Vertex &start, std::function<bool(const Vertex &vertex)> goalTest)
{
	Path path;

	if (graph.solve(start, path, goalTest)) {
		return path;
	}

	return Path();
}

int main()
{
	Graph graph;

	Vertex start = { {
		{ { Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::EMPTY, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED } },
		{ { Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED,   Hole::PEG, Hole::UNUSED,   Hole::PEG, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED } },
		{ { Hole::UNUSED, Hole::UNUSED, Hole::UNUSED,   Hole::PEG, Hole::UNUSED,   Hole::PEG, Hole::UNUSED,   Hole::PEG, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED } },
		{ { Hole::UNUSED, Hole::UNUSED,   Hole::PEG, Hole::UNUSED,   Hole::PEG, Hole::UNUSED,   Hole::PEG, Hole::UNUSED,   Hole::PEG, Hole::UNUSED, Hole::UNUSED } },
		{ { Hole::UNUSED, Hole::PEG, Hole::UNUSED,   Hole::PEG, Hole::UNUSED,  Hole::PEG, Hole::UNUSED,   Hole::PEG, Hole::UNUSED,   Hole::PEG, Hole::UNUSED } },
		} };


	Vertex goal = { {
		{ { Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::EMPTY, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED } },
		{ { Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED } },
		{ { Hole::UNUSED, Hole::UNUSED, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED, Hole::UNUSED, Hole::UNUSED } },
		{ { Hole::UNUSED, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED, Hole::UNUSED } },
		{ { Hole::UNUSED, Hole::EMPTY, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED,  Hole::PEG, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED,   Hole::EMPTY, Hole::UNUSED } },
		} };


	Path path = bfs(graph, start, [&](Vertex v) { return (v == goal); });

	print(start);

	for (int i = 0; i < path.size(); i++) {
		print(path[i]);
	}



	do {
		std::cout << "Waiting for input";
	} while (std::cin.get() != '\n');

	return 0;
}
