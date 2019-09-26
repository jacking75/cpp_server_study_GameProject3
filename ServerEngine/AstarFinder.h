#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace ServerEngine
{
#define TILESIZE 1			// change this also to reflect tile size. 64x64.
#define MAPDATANOCOPY		//use the pointer for mapdata

	//TileMap의 경우 각 비트는 정사각형을 나타내며 각 비트는 전달할 수없는 경우 1, 0 : 전달 가능한 경우



	class AstarFinder
	{
		struct NODE       // node structure
		{
			long f, h;
			int g, tmpg;
			int x, y;
			int NodeNum;
			NODE* Parent;
			NODE* Child[8];  // a node may have upto 8+(NULL) children.
			NODE* NextNode;  // for filing purposes
		};

		struct STACK        // the stack structure
		{
			NODE* pNode;
			STACK* pNextStack;
		};
	public:
		AstarFinder(void);
		~AstarFinder();

		BOOL	InitAstarMap(BYTE* pMap, INT32 w, INT32 h);
		BOOL	NewPath(int sx, int sy, int dx, int dy);
		BOOL	IsReached(void);
		BOOL	PathNextNode(void);
		INT32	NodeGetX();
		INT32	NodeGetY();
		INT32	GetTileNum(int x, int y);
		INT32	IsTileAviable(int x, int y); ////为真表示不能通过

	private:
		void	FreeNodes(void);
		void	FindPath(int sx, int sy, int dx, int dy);
		NODE* GetBestNode(void);
		void	GenerateSuccessors(NODE* BestNode, int dx, int dy);
		void	GenerateSucc(NODE* BestNode, int x, int y, int dx, int dy);
		NODE* CheckOPEN(int tilenum);
		NODE* CheckCLOSED(int tilenum);
		void	Insert(NODE* Successor);
		void	PropagateDown(NODE* Old);
		void	Push(NODE* Node); // stack functions
		NODE* Pop(void);

		NODE* m_pOpenList;    // the node list pointers
		NODE* m_pClosedList;
		NODE* m_pCurPath;    // pointer to the best path
		STACK* m_pStack;

		int		m_nRowCnt;			// tilemap data members, need to be initialisize
		int		m_nColCnt;			// with current map's width and height
		int		m_nTotalTiles;	// to allocate memory for the
		BYTE* m_pTileMap;		// pointer to the A* own tilemap data array



	};
	   
}