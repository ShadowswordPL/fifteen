#include "Fifteen.h"

//Some const for tile moving
#define TILEDOWN 1
#define TILELEFT 2
#define TILETOP 3
#define TILERIGHT 4

//Main game
Fifteen::Fifteen()
{
	SetGameAreaBackground();
	SetGameArray();
	SetTimeCallback(REFRESH, THISBACK(DrawGameArea));
	SetTimeCallback(-SEC, THISBACK(CountTime));
	SetTimeCallback(REFRESH, THISBACK(GameOver));
	SetTimeCallback(REFRESH, THISBACK(ClickTile));
	newgame <<= THISBACK (SetGameArray);
	solve <<= THISBACK (SolutionClick);
	help <<= THISBACK(HelpClick);
	CtrlLayout(*this, "Piętnastka");
}

//Get tile position
int Fifteen::GetTilePosX(int x)
{
	return x*GetTileSizeX();
}


int Fifteen::GetTilePosY(int y)
{
	return y*GetTileSizeY()+1;
}

//Get size of each tile
int Fifteen::GetTileSizeX()
{
	return gamearea.GetSize().cx/SIZE;
}

int Fifteen::GetTileSizeY()
{
	return gamearea.GetSize().cy/SIZE;
}

//Copy TGameArray t2 content into t1
void Fifteen::CopyTGameArray(TGameArray t1, TGameArray t2)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			t1[i][j] = t2[i][j];
		}
	} 
}

//Function finds when in current array is empty field
void Fifteen::FindZero(int &x, int &y, TGameArray t)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if(t[i][j] == 0)
			{
				x = i;
				y = j;
			}
		}
	}		
}

//Function converts int into X pos
int Fifteen::intToX(int x)
{
	return x % SIZE;
}

//Function converts int into X pos
int Fifteen::intToY(int x)
{
	return x / SIZE;
}



//Function return direction to move tile 
int Fifteen::Moveable(int i, int j)
{
	if (i > 0)
	{
		if (GameArray[i-1][j] == 0)
		{
			return TILEDOWN;
		}
	}
	if ((i < SIZE-1))
	{
		if (GameArray[i+1][j] == 0)
		{
			return TILETOP;
		}
	}
	if (j > 0)
	{
		if (GameArray[i][j-1] == 0)
		{
			return TILERIGHT;
		}
	}
	if (j < SIZE-1)
	{
		if (GameArray[i][j+1] == 0)
		{
			return TILELEFT;
		}
	}
	return 0;
}

//Function check if tile is in correct place
bool Fifteen::Correct(int x, int y)
{
		if (GameArray[x][y] == (x+y*SIZE + 1))
		{
			return true;	
		}
		return false;
}

//Function converts tile number into string
String Fifteen::GetTileString(int x)
{
	return Format("%d", x);
}

//Function set background color of GameArea
void Fifteen::SetGameAreaBackground()
{
	gamearea.Background(Silver());	
}

//Function counts number of inversions
int Fifteen::CountInversions(int permutation[])
{
	int inversions = 0;
	for(int i = 0; i < SIZE*SIZE; i++)
	{
    	for(int j = i+1; j < SIZE*SIZE; j++)
    	{
        	if(permutation[i] > permutation[j])
        	{
            	inversions++;
        	}
    	}
	}
	
	return inversions;
}

//Function random correct order of game tiles
void Fifteen::RandomGameArray(TGameArray &t)
{
	bool used[SIZE*SIZE];
	int permutation[SIZE*SIZE];
	int zeroX;
	int zeroY;
	
	for(int i = 0; i < SIZE*SIZE; i++)
	{
		used[i] = false;	
	}

	//Make random permutation
	for (int i = 0; i < SIZE*SIZE; i++)
	{
		bool isSet = false;
		while(!isSet)
		{
			int r = Random(SIZE*SIZE);
			if (!used[r])
			{
				if (r == 0)
				{
					zeroX = intToX(i);
					zeroY = intToY(i);	
				}
				used[r] = true;
				permutation[i] = r;
				isSet = true;	
			}
		}
	}
	
	int inversions = CountInversions(permutation);

	//Check if permutation is solvable
	if((inversions % 2) == ((zeroX % 2) != (zeroY % 2)))
	{
		//Random again
		RandomGameArray(t);
	}
	else
	{
		//Put permutation into TGameArray
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				t[i][j] = permutation[i+j*4];	
			}
		}
	}
}


//Function set new game GameArray
void Fifteen::SetGameArray()
{
	if (!usedSolution)
	{
		points = 0;
		result.SetData(points);
		time = 0;
		timeresult.SetData(time);
		SeedRandom(GetSysTime().Get());
		endOfGame = false;
		usedSolution = false;
		RandomGameArray(GameArray);
	
		//Saving GameArray for solution finder
		CopyTGameArray(GameStartArray, GameArray);
	}
	else
	{
		PromptOK(SOLUTIONMSG);	
	}
}

//Function draws game graphics
void Fifteen::DrawGameArea()
{
	PaintingPainter p(gamearea.GetSize());
	
	for (int i=0; i<SIZE; i++)
	{
		for (int j=0; j<SIZE; j++)
		{
			Color colT = White();
			Color colB = DarkGray();
			if (GameArray[i][j] != 0)
			{
				if(!endOfGame)
				{
					if
					(
					 	gamearea.mouseOn &
						(gamearea.movepos.x > GetTilePosX(i)) && (gamearea.movepos.x < GetTilePosX(i) + GetTileSizeX()) &&
						(gamearea.movepos.y > GetTilePosY(j)) && (gamearea.movepos.y < GetTilePosY(j) + GetTileSizeY())
					)
					{
						if (Moveable(i,j))
						{
							colT = LightGreen();
						}
						else
						{
							colT = LightRed();
						}
						colB = Silver();
					}
					p.Rectangle(GetTilePosX(i)+1, GetTilePosY(j)+1, GetTileSizeX()-2, GetTileSizeY()-2)
					.Fill(GetTilePosX(i), GetTilePosY(j), LightGray(), GetTilePosX(i), GetTilePosY(j), GetTileSizeX(), colB, 3);
					p.DrawText(GetTilePosX(i)+3, GetTilePosY(j)+3, GetTileString(GameArray[i][j]), Arial(32).Bold(), DarkGray());
					p.DrawText(GetTilePosX(i)+1, GetTilePosY(j)+1, GetTileString(GameArray[i][j]), Arial(32).Bold(), colT);
				}
				else
				{
					p.Rectangle(GetTilePosX(i)+1, GetTilePosY(j)+1, GetTileSizeX()-2, GetTileSizeY()-2)
					.Fill(GetTilePosX(i), GetTilePosY(j), LightGray(), GetTilePosX(i), GetTilePosY(j), GetTileSizeX(), DarkGray(), 3);
					p.DrawText(GetTilePosX(i)+3, GetTilePosY(j)+3, GetTileString(GameArray[i][j]), Arial(32).Bold(), DarkGray());
					p.DrawText(GetTilePosX(i)+1, GetTilePosY(j)+1, GetTileString(GameArray[i][j]), Arial(32).Bold(), colT);
				}
			}
		}
	}
	result.SetData(points);
	gamearea = p;
}

//Function counts game time
void Fifteen::CountTime()
{
	if(!endOfGame)
	{
		time++;
		timeresult.SetData(time);
	}
	if(usedSolution)
	{
		timeresult.SetData(0);
	}
}

//Function move tile in (x,y) cords
void Fifteen::MoveTile(int x, int y)
{
	if(!endOfGame)
	{
		if(Moveable(x,y))
		{
			points++;
			int pom = GameArray[x][y];
			GameArray[x][y] = 0;
			zeroX = x;
			zeroY = y;
			if (Moveable(x,y) == TILEDOWN)
			{
				GameArray[x-1][y] = pom;
			}
			if (Moveable(x,y) == TILETOP)
			{
				GameArray[x+1][y] = pom;
			}
			if (Moveable(x,y) == TILERIGHT)
			{
				GameArray[x][y-1] = pom;
			}
			if (Moveable(x,y) == TILELEFT)
			{
				GameArray[x][y+1] = pom;
			}
		}
	}
}

//Function moves clicked tiles
void Fifteen::ClickTile()
{	
	if (gamearea.newClick)
	{
		int x = gamearea.clickpos.x / GetTileSizeX();
		int y = gamearea.clickpos.y / GetTileSizeY();	
		MoveTile(x,y);
		gamearea.newClick = false;
	}
}

//Function set values in Solution class
void Fifteen::SetSolution(Set* set)
{
	solutionLength = 0;
	while(set->prev != NULL)
	{
		CopyTGameArray(Solution[solutionLength], set->move);			
		solutionLength++;
		set = set->prev;
	}
}

//Function show solution of game
void Fifteen::ShowSolution()
{
	solutionLength--;
	CopyTGameArray(GameArray, Solution[solutionLength]);
	if (solutionLength > 0)
	{
		SetTimeCallback(REFRESH*(-2), THISBACK(ShowSolution));	
	}
	else
	{
		usedSolution = false;	
	}
}

//Beware - FindSolution works only for 4x4 game!!!
void Fifteen::FindSolution()
{
	CopyTGameArray(GameArray, GameStartArray);			
	points = 0;
	Set* nothing;
	Set* closedSet = new Set();
	Set* openSet = new Set();
	Set* best = NULL;
	
	openSet->add(openSet, 0, GameArray, openSet, nothing);
	
	while (!openSet->empty())
	{
		openSet->best(openSet, best);		
		
		closedSet->add(closedSet, best->G, best->move, best->prev, best);			
		openSet->remove(openSet, best->move);			 
		CopyTGameArray(GameArray, best->move);
		
		if(best->H < 1)
		{
			SetSolution(best);
			ShowSolution();

			closedSet->DeleteAll(closedSet);
			openSet->DeleteAll(openSet);
			return;	
		}
		int zeroX;
		int zeroY;
		FindZero(zeroX, zeroY, best->move);
		
		for (int i = 0; i < 4; i++)
		{
			int dx = (-1) * ((zeroX > 0) && (i == 0)) + ((zeroX < SIZE - 1) && (i == 1));	
			int dy = (-1) * ((zeroY > 0) && (i == 2)) + ((zeroY < SIZE - 1) && (i == 3));
			
			TGameArray newArray;
			CopyTGameArray(newArray, best->move);
			newArray[zeroX][zeroY] = best->move[zeroX+dx][zeroY+dy];
			newArray[zeroX+dx][zeroY+dy] = 0; 
			if(!closedSet->isAlready(closedSet, newArray))
			{				
				openSet->add(openSet, best->G + 1, newArray, best, nothing);		
			}	
		}
	} 
}

//Function checks if all tiles are in correct position
bool Fifteen::CorrectGameArea()
{
	int correct = 0;
	
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (Correct(i,j))
			{
				correct++;	
			}
		}
	}
	return correct >= SIZE*SIZE - 1;	
}

//Function checks if it's end of game
void Fifteen::GameOver()
{
	if(!endOfGame)
	{	
		if (CorrectGameArea())
		{
			endOfGame = true;
			if (!usedSolution)
			{
		   		PromptOK(Format(WINMSG, points, time)); 
			}
		}
	}
}

//What to do if Help button is clicked
void Fifteen::HelpClick()
{		
	PromptOK(HELPMSG); 
}

//What to do if Solution button is clicked
void Fifteen::SolutionClick()
{
	if (!usedSolution)
	{
		usedSolution = true;
		endOfGame = true;
		FindSolution();
	}
	else
	{
		PromptOK(SOLUTIONMSG);	
	}
}

//Move tiles with keyboard arrows
bool Fifteen::Key(dword key, int count)
{
	
	if (key == K_DOWN)
	{
		if(zeroY > 0)
		{
			MoveTile(zeroX, zeroY - 1);
		}
	}
	
	else if (key == K_LEFT)
	{
		if(zeroX < SIZE - 1)
		{
			MoveTile(zeroX + 1, zeroY);
		}
	}
	
	else if (key == K_UP)
	{
		if(zeroY < SIZE - 1)
		{
			MoveTile(zeroX, zeroY + 1);
		}
	}
	
	else if (key == K_RIGHT)
	{
		if(zeroX > 0)
		{
			MoveTile(zeroX - 1, zeroY);
		}
	}
	
	return false;

}



GUI_APP_MAIN
{
	Fifteen().Sizeable().Run();
}
