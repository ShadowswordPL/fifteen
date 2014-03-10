#include "Fifteen.h"

//Maximal H value
#define MAX 999999999
#define DIVISOR 10

//Constructor makes new Set
Fifteen::Set::Set()
{
	G = MAX;
	H = MAX;
	items = 0;
	next = NULL;
	prev = NULL;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			move[i][j] = 0;		
		}
	}
}

//Function removes all elements of Set s from memory
void Fifteen::Set::DeleteAll(Set* s)
{
	if (s != NULL)
	{
		s->DeleteAll(s->next);
	}
	delete s;
}

//Function checks if tile with number x is on correct place
bool Fifteen::Set::onPlace(int x, TGameArray t)
{
	if(x != 0)
	{
		return x == t[intToX(x-1)][intToY(x-1)];
	}
	else
	{
		return x == t[SIZE-1][SIZE-1];	
	}
}

//Function checks if every number from array is on correct place
bool Fifteen::Set::onPlaceArray(int arr[], int length, TGameArray t)
{
	for (int i = 0; i < length; i++)
	{
		if(!onPlace(arr[i], t))
		{
			return false;	
		}
	}
	return true;
}

//Function checks if tiles with number x is in correct part of GameArea
bool Fifteen::Set::inSecondPart(int x, char part, TGameArray t)
{
	for(int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if((i > 1) && (part == 'x')) 
			{	
				if(t[i][j] == x)
				{
					return true;
				}
			}
			
			if((j > 1) && (part == 'y')) 
			{	
				if(t[i][j] == x)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Fifteen::Set::inSecondPartArray(int arr[], int length, char part, TGameArray t)
{
	for (int i = 0; i < length; i++)
	{
		if(!inSecondPart(arr[i], part, t))
		{
			return false;	
		}
	}
	return true;
}

//Function count H value, best for set x tile in correct place
int Fifteen::Set::setInto(int tile, int number, int x, int y, int zX, int zY)
{
	int kx = x * 15;
	int ky = y * 15;
	kx = kx + abs(zX-x) * 5;
	ky = ky + abs(zY-y) * 5;
	kx = kx + abs(zX-intToX(tile-1)) * 3;
	ky = ky + abs(zY-intToY(tile-1)) * 3;
	return MAX/number + kx + ky;
}

//Function return result of H for one tile
int Fifteen::Set::oneTile
(	
	int tile, int number, int x, int y, int zX, int zY, int i, int j, 
	TGameArray t, int arr[], int length
)
{
	int result;
	if (t[i][j] == tile)
	{
		result = setInto(tile, number, x, y, zX, zY);
		if (!onPlaceArray(arr, length, t))
		{
			result = MAX;
		}
	}
	return result;
}

//Function return result of H when counting for double tile
int Fifteen::Set::doubleTile
(
	int tile, int number, int x, int y, int zX, int zY, int i, int j, 
	TGameArray t, int arr[], int length, int arr2[], int length2, char c
)
{
	int result = 0;
	if (t[i][j] == tile)
	{
		if(!(onPlaceArray(arr, length, t) && inSecondPartArray(arr2, length2, c, t)))
		{
			result = MAX;
		}
		else
		{
			result = setInto(3, number, x, y, zX, zY);
		}
	}			
	return result;
}

//Function count H value for A-star algorithm
int Fifteen::Set::CountH(TGameArray newArray, TGameArray oldArray)
{
	int result = MAX / DIVISOR;
	int zX;
	int zY;
	FindZero(zX, zY, newArray);
	
	//Defining lot of bools for choosing GameArray state
	bool oneIsSet = onPlace(1, oldArray);
	bool twoIsSet = (onPlace(2, oldArray) && oneIsSet);
	bool fiveIsSet = (onPlace(5, oldArray) && twoIsSet);
	
	bool threeIsSet = (inSecondPart(3, 'x', oldArray) && fiveIsSet);
	bool fourIsSet = 
	(
		threeIsSet &&  ((inSecondPart(4, 'x', oldArray) && inSecondPart(0, 'x', oldArray)) || 
		(onPlace(3, oldArray) && onPlace(4, oldArray)))
	);
	bool topIsSet = (onPlace(3, oldArray) && onPlace(4, oldArray) && fourIsSet);
	
	bool nineIsSet = (inSecondPart(9, 'y', oldArray) && topIsSet);
	bool thirteenIsSet = 
	(
		nineIsSet && ((inSecondPart(13, 'y', oldArray) && inSecondPart(0, 'y', oldArray)) || 
		(onPlace(9, oldArray) && onPlace(13, oldArray)))
	);
	
	bool leftIsSet = (onPlace(9, oldArray) && onPlace(13, oldArray) && thirteenIsSet);		
	bool more = (leftIsSet && onPlace(6, oldArray) && onPlace(7, oldArray) && onPlace(8, oldArray));	
	bool helper = false;

	//Count H for each tile
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			int x = abs((newArray[i][j] - 1)%SIZE - i);
			int y = abs((newArray[i][j] - 1)/SIZE - j);
			int kx = 0;
			int ky = 0;
					
			if (!oneIsSet)
			{			
				int arr[0];
				result = oneTile(1, 2, x, y, zX, zY, i, j, newArray, arr, 0);
			}
			
			else if (!twoIsSet)
			{
				int arr[1] = {1};
				result = oneTile(2, 3, x, y, zX, zY, i, j, newArray, arr, 1);
			}
			
			
			else if (!fiveIsSet)
			{
				int arr[2] = {1, 2};
				result = oneTile(5, 4, x, y, zX, zY, i, j, newArray, arr, 2);
			}
			
			
			else if (!threeIsSet)
			{
				int arr[3] = {1, 2, 5};
				result = oneTile(3, 5, x, y, zX, zY, i, j, newArray, arr, 3);
			}
			
			else if (!fourIsSet)
			{
				int arr[3] = {1, 2, 5};
				result = oneTile(4, 6, x, y, zX, zY, i, j, newArray, arr, 3);
			}
		
			else if (!topIsSet)
			{	
				int resultVar = 50;
				int resultCur;
				if (!helper)
				{
					helper = true;
					result = 0;	
				}
				
				int arr[3] = {1, 2, 5};
				int arr2[3] = {3, 4, 0};	
				result = result + doubleTile(3, resultVar, x, y, zX, zY, i, j, newArray, arr, 3, arr2, 3, 'x');
				result = result + doubleTile(4, resultVar, x, y, zX, zY, i, j, newArray, arr, 3, arr2, 3, 'x');
				
			}
			
			else if (!nineIsSet)
			{
				int arr[5] = {1, 2, 5, 3, 4};
				result = oneTile(9, 51, x, y, zX, zY, i, j, newArray, arr, 5);
			}
			
			else if (!thirteenIsSet)
			{
				int arr[5] = {1, 2, 5, 3, 4};
				result = oneTile(13, 52, x, y, zX, zY, i, j, newArray, arr, 5);
			}
			
			else if (!leftIsSet)
			{	
				int resultVar = 1500;
				int resultCur;
					
				if (!helper)
				{
					helper = true;
					result = 0;	
				}
				
				int arr[5] = {1, 2, 5, 3, 4};
				int arr2[3] = {9, 13, 0};	
				result = result + doubleTile(9, resultVar, x, y, zX, zY, i, j, newArray, arr, 5, arr2, 3, 'y');
				result = result + doubleTile(13, resultVar, x, y, zX, zY, i, j, newArray, arr, 5, arr2, 3, 'y');
			}
			else 
			{
				int length = 7;
				int arr[7] = {1, 2, 5, 3, 4, 9, 13};
				if(onPlaceArray(arr, length, newArray))
				{
					if(!helper)
					{
						result = 0;
						helper = true;	
					} 
					if(newArray[i][j] != 0)
					{
						result = result + abs((newArray[i][j]-1)/SIZE-j) + abs((newArray[i][j]-1)%SIZE-i);
					}
					if (more)
					{
						if(!(onPlace(6, newArray) && onPlace(7, newArray) && onPlace(8, newArray)))
						{
							result = MAX;	
						}
					}
				}
				else
				{
					result = MAX;
				}	
			}			
		}			
	}
	return result;
}

//Function finds the best move in Set s
void Fifteen::Set::best(Set* s, Set* &b)
{
	b = s;
	while(s != NULL)
	{
		if ((b->H)+b->G > (s->H)+s->G)
		{
			b = s;	
		}
		s = s->next;	
	}
}

//Function adds object with newArray to Set s
void Fifteen::Set::add(Set* s, int nG, TGameArray newArray, Set* prev, Set* &current)
{
	s->items++;
	while ((s->next != NULL) && (isLower(s->next->move, newArray)))
	{
		s = s->next;	
	}
	Set* dummy = s->next;
	s->next = new Set();
	s->next->G = nG;
	s->next->H = CountH(newArray, prev->move);
	s->next->next = dummy;
	s->next->prev = prev;
	
	CopyTGameArray(s->next->move, newArray);
	current = s->next;
}

//Function checks if TGameArray t is already in Set s
bool Fifteen::Set::isAlready(Set* s, TGameArray t)
{
	while((s != NULL) && (isLower(s->move, t))) 
	{
		int same = 0;
		for (int i = 0; i < SIZE; i++)
		{	
			for (int j = 0; j < SIZE; j++)
			{
				if (s->move[i][j] == t[i][j])
				{
					same++;
				}
			}
		}	
		if (same == SIZE*SIZE)
		{
			return true;
		}
		s = s->next;
	}
	return false;
}

//Function returns true when two TGameArrays are same 
bool Fifteen::Set::isSame(TGameArray x, TGameArray y)
{
	int same = 0;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (x[i][j] == y[i][j])
			{
				same++;
			}
		}
	}
	if (same == SIZE*SIZE)
	{
		return true;
	}	
	return false;
}

//Fumction returns true when x is lower permutation
bool Fifteen::Set::isLower(TGameArray x, TGameArray y)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (x[j][i] > y[j][i])
			{
				return false;
			}	
			if (x[j][i] < y[j][i])
			{
				return true;
			}		  
		}
	}
	return true;
}

//Function removes an object from s, which contains TGameArray t
void Fifteen::Set::remove(Set* s, TGameArray t)
{
	Set* org = s;
	while((s->next != NULL))
	{	
		if (isSame(s->next->move, t))
		{
			Set* pom = s->next;
			s->next = s->next->next;
			org->items--;
			delete pom;
			break;
		}
		else
		{
			s = s->next;
		}
	}
}

//Function checks if set is empty
bool Fifteen::Set::empty()
{
	return items == 0;
}