// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "WorldState.h"
#include <list>
#include <bits/stdc++.h> 

class Agent
{
public:
	struct queueNode
	{
		int x;
		int y;
		int dist;
	};
	Agent ();
	~Agent ();
	void Initialize ();
	void InitWorldState();
	Action Process (Percept& percept);
	void GameOver (int score);
	Action TurnRight();
	Action TurnLeft();
	Action GoForward();
	Action Grab();
	Action Shoot();
	Action Climb();
	Action Explore();
	Action RandomWalk();
	void MarkPercepts(Percept& percept);
	enum SquareType {SAFE, STENCH, BREEZE, GOLD, WUMPUS, UNKNOWN, DANGER1, DANGER2, EDGE};
	void IdentifySquare(SquareType type, int x, int y);
	bool GoForwardSafe();
	bool IsSafeOnBoard(int x, int y);
	void PrintMyBoard();
	void ClearDangers();
	void MarkWumpus();
	void MarkEdges();
	bool isValid(int row, int col);
	list<tuple<int, int>> ShortestPathBack2(int curX, int curY, list<tuple<int, int>> visited);
	vector<vector<SquareType>> Board;
	void PrintShortestPath(list<tuple<int, int>> l);
	list<tuple<int, int>> mainReturnPath;
	bool returningToLadder;
	bool goingToGold;
	Action ReturnToLadder();
	void SavePath();
	void LoadPath();
	bool FileExists();
	Action GoingToGold();
	void RemoveOldWorldFile();
private:
	WorldState worldState;
};

#endif // AGENT_H
