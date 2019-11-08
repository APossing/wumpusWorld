// Agent.cc

#include <iostream>
#include "Agent.h"

using namespace std;

Agent::Agent()
{
	InitWorldState();
}

Agent::~Agent()
{
	printf("Agent Destroyed!");
	//RemoveOldWorldFile();
}

void Agent::Initialize()
{
}
void Agent::InitWorldState()
{
	worldState.agentLocation = Location(1, 1);
	worldState.agentOrientation = RIGHT;
	worldState.worldSize = 99999999;
	worldState.wumpusLocation;
	worldState.goldLocation;
	worldState.agentAlive = true;
	worldState.agentHasArrow = true;
	worldState.agentHasGold = false;
	worldState.agentInCave = false;
	worldState.wumpusAlive = true;
	IdentifySquare(SAFE, 1, 1);
	returningToLadder = false;
	if (FileExists())
	{
		LoadPath();
		goingToGold = true;
	}
	else
	{
		goingToGold = false;
	}
}
Action Agent::TurnRight()
{
	if (worldState.agentOrientation == RIGHT)
		worldState.agentOrientation = DOWN;
	else if (worldState.agentOrientation == UP)
		worldState.agentOrientation = RIGHT;
	else if (worldState.agentOrientation == LEFT)
		worldState.agentOrientation = UP;
	else if (worldState.agentOrientation == DOWN)
		worldState.agentOrientation = LEFT;
	return TURNRIGHT;
}

Action Agent::TurnLeft()
{
	if (worldState.agentOrientation == RIGHT)
		worldState.agentOrientation = UP;
	else if (worldState.agentOrientation == UP)
		worldState.agentOrientation = LEFT;
	else if (worldState.agentOrientation == LEFT)
		worldState.agentOrientation = DOWN;
	else if (worldState.agentOrientation == DOWN)
		worldState.agentOrientation = RIGHT;
	return TURNLEFT;
}

Action Agent::Climb()
{
	return CLIMB;
}

Action Agent::Grab()
{
	worldState.agentHasGold = true;
	return GRAB;
}

Action Agent::Shoot()
{
	worldState.agentHasArrow = false;
	return SHOOT;
}

void Agent::PrintMyBoard()
{
	for (int i = Board.size() - 1; i >= 0; i--)
	{
		for (int j = 0; j < Board[i].size(); j++)
		{
			printf("%d,", Board[i][j]);
		}
		printf("\n");
	}
}

Action Agent::GoForward()
{
	if (worldState.agentOrientation == UP && worldState.agentLocation.Y < worldState.worldSize)
		worldState.agentLocation.Y++;
	else if (worldState.agentOrientation == RIGHT && worldState.agentLocation.X < worldState.worldSize)
		worldState.agentLocation.X++;
	else if (worldState.agentOrientation == LEFT && worldState.agentLocation.X > 1)
		worldState.agentLocation.X--;
	else if (worldState.agentOrientation == DOWN && worldState.agentLocation.Y > 1)
		worldState.agentLocation.Y--;
	return GOFORWARD;
}


void Agent::IdentifySquare(SquareType squareType, int x, int y)
{
	if (x > Board.size() || y > Board.size())
	{
		int bigger = 0;
		if (x > y)
			bigger = x + 1;
		else
			bigger = y + 1;
		while (bigger > Board.size())
		{
			vector<SquareType> l = vector<SquareType>();
			l.push_back(UNKNOWN);
			Board.push_back(l);
		}
		for (int i = 0; i < bigger; i++)
		{
			while (Board[i].size() < bigger)
			{
				Board[i].push_back(UNKNOWN);
			}
		}
	}
	if (Board[y - 1][x - 1] != SAFE && Board[y - 1][x - 1] != WUMPUS)
	{
		if (Board[y - 1][x - 1] == DANGER1 && squareType == DANGER1)
			Board[y - 1][x - 1] == DANGER2;
		else
			Board[y - 1][x - 1] = squareType;
	}
}

bool Agent::IsSafeOnBoard(int x, int y)
{
	if (x < 1 || y < 1)
		return false;
	if (Board.size() >= y && Board[y].size() >= x)
		return (Board[y - 1][x - 1] != DANGER1 && Board[y - 1][x - 1] != DANGER2);
	else
	{
		if (x < worldState.worldSize && y < worldState.worldSize)
			return true;
	}
}

bool Agent::isValid(int row, int col)
{
	// return true if row number and column number 
	// is in range 
	return (row >= 0
		&& (row < Board.size())
		&& (col >= 0)
		&& (col < Board.size())
		&& (Board[col][row] == SAFE
			|| Board[col][row] == GOLD));
}
Action Agent::GoingToGold()
{
	tuple<int, int> nextPos;
	for (list<tuple<int, int>>::iterator it = mainReturnPath.begin(); it != mainReturnPath.end(); it++)
	{
		if (get<0>(*it) == worldState.agentLocation.X - 1 && get<1>(*it) == worldState.agentLocation.Y - 1)
		{
			it++;
			nextPos = { get<0>(*it), get<1>(*it) };
			break;
		}

	}
	if (worldState.agentOrientation == UP)
	{
		if (get<0>(nextPos) == worldState.agentLocation.X - 1 && get<1>(nextPos) == worldState.agentLocation.Y)
			return GoForward();
		return TurnLeft();
	}
	if (worldState.agentOrientation == LEFT)
	{
		if (get<0>(nextPos) == worldState.agentLocation.X - 2 && get<1>(nextPos) == worldState.agentLocation.Y - 1)
			return GoForward();
		return TurnLeft();
	}
	if (worldState.agentOrientation == RIGHT)
	{
		if (get<0>(nextPos) == worldState.agentLocation.X && get<1>(nextPos) == worldState.agentLocation.Y - 1)
			return GoForward();
		return TurnLeft();
	}
	if (worldState.agentOrientation == DOWN)
	{
		if (get<0>(nextPos) == worldState.agentLocation.X - 1 && get<1>(nextPos) == worldState.agentLocation.Y - 2)
			return GoForward();
		return TurnLeft();
	}
}
Action Agent::ReturnToLadder()
{
	tuple<int, int> nextPos;
	for (list<tuple<int, int>>::iterator it = mainReturnPath.begin(); it != mainReturnPath.end(); it++)
	{
		if (get<0>(*it) == worldState.agentLocation.X - 1 && get<1>(*it) == worldState.agentLocation.Y - 1)
		{
			it--;
			nextPos = { get<0>(*it), get<1>(*it) };
			break;
		}

	}
	if (worldState.agentOrientation == UP)
	{
		if (get<0>(nextPos) == worldState.agentLocation.X - 1 && get<1>(nextPos) == worldState.agentLocation.Y)
			return GoForward();
		return TurnLeft();
	}
	if (worldState.agentOrientation == LEFT)
	{
		if (get<0>(nextPos) == worldState.agentLocation.X - 2 && get<1>(nextPos) == worldState.agentLocation.Y - 1)
			return GoForward();
		return TurnLeft();
	}
	if (worldState.agentOrientation == RIGHT)
	{
		if (get<0>(nextPos) == worldState.agentLocation.X && get<1>(nextPos) == worldState.agentLocation.Y - 1)
			return GoForward();
		return TurnLeft();
	}
	if (worldState.agentOrientation == DOWN)
	{
		if (get<0>(nextPos) == worldState.agentLocation.X - 1 && get<1>(nextPos) == worldState.agentLocation.Y - 2)
			return GoForward();
		return TurnLeft();
	}
}

list<tuple<int, int>> Agent::ShortestPathBack2(int curX, int curY, list<tuple<int, int>> visited)
{
	list<tuple<int, int>> returnList = list<tuple<int, int>>{ {curX, curY} };
	if (curX == 0 && curY == 0)
	{
		return returnList;
	}
	if (isValid(curX, curY))
	{
		list<tuple<int, int>>::iterator it = visited.begin();
		while (it != visited.end())
		{
			if (get<0>(*it) == curX && get<1>(*it) == curY)
				return {};
			it++;
		}
		visited.push_back(tuple<int, int>{curX, curY});
		returnList = ShortestPathBack2(curX - 1, curY, visited);
		if (!returnList.empty())
		{
			returnList.push_back(tuple<int, int>{curX, curY});
			return returnList;
		}
		returnList = ShortestPathBack2(curX, curY - 1, visited);
		if (!returnList.empty())
		{
			returnList.push_back(tuple<int, int>{curX, curY});
			return returnList;
		}
		returnList = ShortestPathBack2(curX + 1, curY, visited);
		if (!returnList.empty())
		{
			returnList.push_back(tuple<int, int>{curX, curY});
			return returnList;
		}
		returnList = ShortestPathBack2(curX, curY + 1, visited);
		if (!returnList.empty())
		{
			returnList.push_back(tuple<int, int>{curX, curY});
			return returnList;
		}
	}
	return {};
}

bool Agent::GoForwardSafe()
{
	if (worldState.agentOrientation == UP && worldState.agentLocation.Y < worldState.worldSize && IsSafeOnBoard(worldState.agentLocation.X, worldState.agentLocation.Y + 1))
		return true;
	else if (worldState.agentOrientation == RIGHT && worldState.agentLocation.X < worldState.worldSize && IsSafeOnBoard(worldState.agentLocation.X + 1, worldState.agentLocation.Y))
		return true;
	else if (worldState.agentOrientation == LEFT && worldState.agentLocation.X > 1 && IsSafeOnBoard(worldState.agentLocation.X - 1, worldState.agentLocation.Y))
		return true;
	else if (worldState.agentOrientation == DOWN && worldState.agentLocation.Y > 1 && IsSafeOnBoard(worldState.agentLocation.X, worldState.agentLocation.Y - 1))
		return true;
	return false;
}

Action Agent::Explore()
{
	int agentX = worldState.agentLocation.X;
	int agentY = worldState.agentLocation.Y;

	if (worldState.worldSize == 0)
	{

	}
	return GOFORWARD;
}

void Agent::MarkPercepts(Percept& percept)
{
	if (percept.Breeze)
		IdentifySquare(BREEZE, worldState.agentLocation.X, worldState.agentLocation.Y);
	if (percept.Stench && worldState.wumpusAlive)
	{
		IdentifySquare(STENCH, worldState.agentLocation.X, worldState.agentLocation.Y);
		if (worldState.agentOrientation != UP && worldState.agentLocation.Y != 1)
			IdentifySquare(DANGER1, worldState.agentLocation.X, worldState.agentLocation.Y - 1);
		if (worldState.agentOrientation != DOWN)
			IdentifySquare(DANGER1, worldState.agentLocation.X, worldState.agentLocation.Y + 1);
		if (worldState.agentOrientation != RIGHT && worldState.agentLocation.X != 1)
			IdentifySquare(DANGER1, worldState.agentLocation.X - 1, worldState.agentLocation.Y);
		if (worldState.agentOrientation != LEFT)
			IdentifySquare(DANGER1, worldState.agentLocation.X + 1, worldState.agentLocation.Y);
	}
	if (percept.Glitter)
		IdentifySquare(GOLD, worldState.agentLocation.X, worldState.agentLocation.Y);
	else
		IdentifySquare(SAFE, worldState.agentLocation.X, worldState.agentLocation.Y);

}

Action Agent::RandomWalk()
{
	int move = rand() % 3;
	if (move == 0)
	{
		if (GoForwardSafe())
		{
			return GoForward();
		}
		else
			return RandomWalk();
	}
	else if (move == 1)
	{
		return TurnRight();
	}
	else
	{
		return TurnLeft();
	}
}

void Agent::ClearDangers()
{
	for (int i = Board.size() - 1; i >= 0; i--)
	{
		for (int j = 0; j < Board[i].size(); j++)
		{
			if (Board[i][j] == DANGER1 || Board[i][j] == DANGER2)
				Board[i][j] = SAFE;
		}
	}
}
void Agent::MarkEdges()
{
	for (int i = 1; i <= worldState.worldSize; i++)
	{
		if (i == worldState.worldSize)
		{
			for (int j = 1; j <= worldState.worldSize; j++)
			{
				IdentifySquare(EDGE, i, j);
			}
		}
		IdentifySquare(EDGE, i, worldState.worldSize);
	}
}
void Agent::MarkWumpus()
{
	if (worldState.agentOrientation == UP)
	{
		IdentifySquare(WUMPUS, worldState.agentLocation.X, worldState.agentLocation.Y + 1);
	}
	if (worldState.agentOrientation == RIGHT)
	{
		IdentifySquare(WUMPUS, worldState.agentLocation.X + 1, worldState.agentLocation.Y);
	}
	if (worldState.agentOrientation == LEFT)
	{
		IdentifySquare(WUMPUS, worldState.agentLocation.X - 1, worldState.agentLocation.Y);
	}
	if (worldState.agentOrientation == DOWN)
	{
		IdentifySquare(WUMPUS, worldState.agentLocation.X + 1, worldState.agentLocation.Y);
	}
}
void Agent::PrintShortestPath(list<tuple<int, int>> l)
{
	for (list<tuple<int, int>>::iterator it = l.begin(); it != l.end(); it++)
	{
		printf("%d,%d\n", get<0>(*it), get<1>(*it));
	}
}

Action Agent::Process(Percept& percept)
{
	PrintMyBoard();
	char c;
	Action action;
	if (percept.Bump)
	{
		if (worldState.agentOrientation == RIGHT)
		{
			worldState.worldSize = worldState.agentLocation.X;
			worldState.agentLocation.X--;
			MarkEdges();
		}
		else if (worldState.agentOrientation == UP)
		{
			worldState.worldSize = worldState.agentLocation.Y;
			worldState.agentLocation.Y--;
			MarkEdges();
		}
		else if (worldState.agentOrientation == DOWN)
		{
			worldState.agentLocation.Y++;
		}
		else if (worldState.agentOrientation == LEFT)
		{
			worldState.agentLocation.X++;
		}
	}
	if (percept.Scream)
	{
		MarkWumpus();
		ClearDangers();
		worldState.wumpusAlive = false;
	}
	MarkPercepts(percept);
	printf("\n");
	//PrintMyBoard();
	if (worldState.agentHasGold)
		cout << worldState.agentLocation.X << ", " << worldState.agentLocation.Y << "\n";
	if (percept.Glitter)
	{
		list<tuple<int, int>> shortestPath = ShortestPathBack2(worldState.agentLocation.X - 1, worldState.agentLocation.Y - 1, {});
		PrintShortestPath(shortestPath);
		mainReturnPath = shortestPath;
		returningToLadder = true;
		return Grab();
	}
	else if (worldState.agentLocation.X == 1 && worldState.agentLocation.Y == 1 && worldState.agentHasGold)
	{
		return Climb();
	}
	else if (percept.Stench)
	{
		if (worldState.agentHasArrow)
			return Shoot();
	}
	if (returningToLadder)
		return ReturnToLadder();
	if (goingToGold)
		return GoingToGold();
	return RandomWalk();
}

void Agent::SavePath()
{
	ofstream myfile;
	myfile.open("save.csv");
	for (list<tuple<int, int>>::iterator it = mainReturnPath.begin(); it != mainReturnPath.end(); it++)
	{
		if (it == mainReturnPath.begin())
			myfile << get<0>(*it) << "," << get<1>(*it);
		else
			myfile << "," << get<0>(*it) << "," << get<1>(*it);
	}
	myfile.close();
}

void Agent::LoadPath()
{
	ifstream file("save.csv");
	string value;
	int v1, v2;
	while (file.good())
	{
		getline(file, value, ',');
		stringstream g(value);
		g >> v1;
		getline(file, value, ',');
		stringstream q(value);
		q >> v2;
		mainReturnPath.push_back({ v1,v2 });
	}
	PrintShortestPath(mainReturnPath);
	file.close();
}

bool Agent::FileExists()
{
	ifstream f("save.csv");
	return f.good();
}

void Agent::RemoveOldWorldFile()
{
	remove("save.csv");
}


void Agent::GameOver(int score)
{
	SavePath();
}

