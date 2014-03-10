//Fifteen is easy logic game
//								~have fun
//Author: Andrzej Jackowski
//e-mail: andrzejjac@gmail.com


#ifndef _Fifteen_Fifteen_h
#define _Fifteen_Fifteen_h

#include <CtrlLib/CtrlLib.h> 
#include <Painter/Painter.h>

//Size of SIZExSIZE gamearea
#define SIZE 4

//Refreshing of game graphics in ms 
#define REFRESH -100

//Second from ms
#define SEC 1000

//Some new Colors
#define Silver() Color(83,101,138)
#define DarkGray() Color(51,63,81)
#define LightGray() Color(121,138,174)
#define LightRed() Color(255,48,48)
#define LightGreen() Color(127,255,0)

//Some game messages
#define WINMSG "Gratulacje, wygrałeś. Udało Ci się ułożyć zagadkę w %d ruchów i %d sekund" 
#define HELPMSG "Piętnastka to prosta gra logiczna. Twoim celem jest przesunięcie klocków, "\
				"tak by w prawym dolnym rogu było wolne miejsce, a numery na klockach czytane wierszami "\
				"od lewego górnego rogu były ułożone kolejno. Jeśli się znudzisz komputer rozwiąże zagadkę "\
				"za Ciebie - wystarczy kliknąć guzik 'Pokaż rozwiązanie'. "
#define SOLUTIONMSG "Poczekaj aż pokazywanie rozwiazania skończy działanie"

using namespace Upp;

//Upgraded Picture class with some new functions
class GameArea : public Picture{
	public:
		Point movepos;
		Point clickpos;
		bool mouseOn;
		bool newClick;
		
		void LeftUp(Point p, dword keyflags);
		void MouseMove(Point p, dword keyflags);
		void MouseLeave();	
		void MouseEnter(Point p, dword keyflags);
		void operator=(const Painting& p);

};

#define LAYOUTFILE <Fifteen/Fifteen.lay>
#include <CtrlCore/lay.h>

//Main class of this game
class Fifteen : public WithFifteenLayout<TopWindow> 
{		
	typedef Fifteen CLASSNAME;	
		
	int zeroX;
	int zeroY;
	int points;
	int time;
	bool usedSolution;
	bool endOfGame;
	
	public:
		typedef int TGameArray[SIZE][SIZE];
		TGameArray GameArray;
		TGameArray GameStartArray;
		TGameArray Solution[SIZE*SIZE*SIZE*SIZE];
		int solutionLength;
		
		//Class of Set for A* solution finder
		class Set
		{			
			public: 
				int move[SIZE][SIZE];
				int G;
				int H;
				int items;
				Set* next;
				Set* prev;
				
				Set();
				void DeleteAll(Set* s);
				bool onPlace(int x, TGameArray t);
				bool onPlaceArray(int arr[], int length, TGameArray t);
				int setInto(int tile, int number, int x, int y, int zX, int zY);
				
				int oneTile(
					int tile, int number, int x, int y, int zX, int zY, int i, int j,
					TGameArray t, int arr[], int length
				);
				
				bool inSecondPart(int x, char part, TGameArray t);
				bool inSecondPartArray(int arr[], int length, char part, TGameArray t);
				
				int doubleTile
				(
					int tile, int number, int x, int y, int zX, int zY, int i, int j,
					TGameArray t, int arr[], int length, int arr2[], int length2, char c
				);

				int CountH(TGameArray newArray, TGameArray oldArray);
				void best(Set* s, Set* &b);
				void add(Set* s, int nG, TGameArray newArray, Set* prev, Set* &current);
				bool isAlready(Set* s, TGameArray t);
				bool isSame(TGameArray x, TGameArray y);
				bool isLower(TGameArray x, TGameArray y);
				void remove(Set* s, TGameArray t);
				bool empty();					
		};
		
		Fifteen();
		
		int GetTilePosX(int x);
		int GetTilePosY(int y);
		int GetTileSizeX();
		int GetTileSizeY();
		static void CopyTGameArray(TGameArray t1, TGameArray t2);
		static void FindZero(int &x, int &y, TGameArray t);
		static int intToX(int x);
		static int intToY(int x);
	
		int Moveable(int i, int j);
		bool Correct(int x, int y);
		String GetTileString(int x);
		
		void SetGameAreaBackground();
		int CountInversions(int permutation[]);
		void RandomGameArray(TGameArray &t);
		void SetGameArray();
		void DrawGameArea();
		void CountTime();
		void MoveTile(int x, int y);
		void SetSolution(Set* set);
		void ShowSolution();
		void ClickTile();
		void FindSolution();
		bool CorrectGameArea();
		void GameOver();
		
		void HelpClick();
		void SolutionClick();
		bool Key(dword key, int count);
};

#endif
