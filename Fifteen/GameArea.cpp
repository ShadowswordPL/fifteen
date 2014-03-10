#include "Fifteen.h"

void GameArea::LeftUp(Point p, dword keyflags)
{
	clickpos = p;
	newClick = true;
}	 

void GameArea::MouseMove(Point p, dword keyflags)
{
	movepos = p;
}	 
		
void GameArea::MouseLeave()
{
	mouseOn = false;
}

void GameArea::MouseEnter(Point p, dword keyflags)
{
	mouseOn = true;
}
		
void GameArea::operator=(const Painting& p) 
{ 
	Picture::operator=(p); 
}
