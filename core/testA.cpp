#include<bits/stdc++.h> 
using namespace std; 
#define ROW 20
#define COL 20 

typedef pair<int, int> Pair; 
typedef pair<int, pair<int, int>> pPair; 
struct cell {	int parent_i, parent_j, f, g, h; 	}; 
//static int mx[8]{1, -1, 1, -1, 0, 0, -1, 1 }, my[8]{ 1, -1, -1, 1, 1, -1, 0, 0 };
static int mx[4]{ 0, 0, -1, 1 }, my[4]{ 1, -1, 0, 0 };
vector< deque< Pair > > paths;

void tracePath(cell cellDetails[][COL], Pair dest) 
{ 
	int row = dest.first, col = dest.second, temp_row, temp_col;	deque< Pair > temp;
	while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col )) 
	{ 
		temp.push_front(make_pair(row, col)); 
		temp_row = cellDetails[row][col].parent_i; row = temp_row; 
		temp_col = cellDetails[row][col].parent_j; col = temp_col; 
	} 
	temp.push_front(make_pair (row, col)); 
	paths.push_back(temp);
} 

bool aStarSearch(int grid[][COL], Pair src, Pair dest) 
{ 
	int i, j; bool closedList[ROW][COL]; cell cellDetails[ROW][COL]; 
	memset(closedList, false, sizeof (closedList)); 	
	for (i=0; i<ROW; i++) 
		for (j=0; j<COL; j++) 
		{ 
			cellDetails[i][j].f = cellDetails[i][j].g = cellDetails[i][j].h = INT_MAX; 
			cellDetails[i][j].parent_i = cellDetails[i][j].parent_j = -1; 
		} 	
	i = src.first, j = src.second; 
	cellDetails[i][j].parent_i = i; cellDetails[i][j].parent_j = j; 
	cellDetails[i][j].f = cellDetails[i][j].g = cellDetails[i][j].h = 0.0; 
	set<pPair> openList; 
	openList.insert(make_pair (0, make_pair (i, j))); 
	bool foundDest = false;	int x, y, gNew, hNew, fNew; 
	while (!openList.empty()) 
	{ 		
		pPair p = *openList.begin(); openList.erase(openList.begin());
		closedList[x = p.second.first][y = p.second.second] = true; 
		for (int k = 0; k < 4; k++) // < 4 para movimientos sin diagonal
		{
			i=mx[k]+x; j=my[k]+y;
			if (i < 0 || i >= ROW || j < 0 || j >= COL) 
				continue;
			if (i == dest.first && j == dest.second) 
			{ 
				cellDetails[i][j].parent_i = x; 
				cellDetails[i][j].parent_j = y; 
				printf ("The destination cell is found\n"); 
				tracePath (cellDetails, dest); 
				return foundDest = true;
			}
			else if (!closedList[i][j] && grid[i][j] != 0) 
			{ 
				gNew = cellDetails[i][j].g + 1; 
				//hNew = sqrt(pow(i-dest.first,2)+pow(j-dest.second,2));//i+j-(dest.first+dest.second); // Distancia euclidiana
				hNew = abs((i - dest.first) + (j - dest.second)); // Distancia manhattan
				fNew = gNew + hNew; 
				if (cellDetails[i][j].f == FLT_MAX || cellDetails[i][j].f > fNew) 
				{ 
					openList.insert( make_pair(fNew, make_pair(i, j))); 
					cellDetails[i][j].f = fNew; cellDetails[i][j].g = gNew; cellDetails[i][j].h = hNew; 
					cellDetails[i][j].parent_i = x; cellDetails[i][j].parent_j = y; 
				} 
			}			
		}
	} 
	if (!foundDest) 
		printf("Failed to find the Destination Cell\n"); 
	return foundDest;
} 
Pair sortFrom(Pair o,set< Pair > &sp)
{
	int imin=-1, vmin=INT_MAX;
	Pair x;
	for (auto s:sp)
		if (vmin>sqrt(pow(o.first-s.first,2)+pow(o.second-s.second,2)))
		{
			vmin=sqrt(pow(o.first-s.first,2)+pow(o.second-s.second,2));
			x=s;
		}	
	sp.erase(x);	
	return x;
}
int main() 
{
	int grid[ROW][COL] = 
	{
		{3,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,3,1,1,1,1,1,3,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,1,1,2,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,0,3,1,1,1,1,1,1,1,1,3,1,1,1,1},
		{1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};	
	set< Pair > sp, sb; // puntos, pilas
	// escanear matriz
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			if(grid[i][j]==3)
				sp.insert(make_pair(i,j));
			else if(grid[i][j]==2)
				sb.insert(make_pair(i,j));
	// crea los caminos
	Pair start=make_pair(19,0);
	do
	{
		Pair dest=sortFrom(start,sp); // ordena todo con respecto a un punto y lo borra del conjunto
		if(!aStarSearch(grid, start, dest))
		{
			printf("Imposible\n");
			return 0;
		}
		start=dest;					  // el destino se vuelve el origen
	}while(sp.size()!=1);
	// imprime primer camino
	for (int i = 0; i < paths.size(); i++,cout<<"\n")
		for (int j = 0; j < paths[i].size(); j++)
			cout<<paths[i][j].first<<" "<<paths[i][j].second<<"\n";
	return 0; 
}