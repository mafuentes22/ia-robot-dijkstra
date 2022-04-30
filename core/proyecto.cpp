#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <deque>
#include <algorithm>
#include <set>
#include <vector>
#include <climits>
#include <cmath> 
using namespace std; 

#define TAB 20
#define NPEL 10
#define ROW 20
#define COL 20
#define BATMIN 20

typedef pair<int, int> Pair;
typedef pair<int, pair<int, int>> pPair;
struct cell {	int parent_i, parent_j, f, g, h; 	};
static int mx[4]{ 0, 0, -1, 1 }, my[4]{ 1, -1, 0, 0 };
static set< Pair > sp;
vector< deque< Pair > > paths;
int grid[ROW][COL];
int c = 0, d=0;
void tracePath(cell cellDetails[][COL], Pair dest)
{
	int row = dest.first, col = dest.second, temp_row, temp_col;	deque< Pair > temp;
	while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col )) 
	{ 
		temp.push_front(make_pair(row, col)); 
		temp_row = cellDetails[row][col].parent_i; row = temp_row; 
		temp_col = cellDetails[row][col].parent_j; col = temp_col; 
	} 
	temp.push_front(make_pair(row, col)); 
	paths.push_back(temp);
}
bool aStarSearch(int grid[][COL], Pair src, Pair dest, int &b) 
{ 
	int i, j, x, y, gNew, hNew, fNew; bool closedList[ROW][COL]; cell cellDetails[ROW][COL]; set<pPair> openList; bool foundDest = false;
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
	openList.insert(make_pair (0, make_pair (i, j)));
	while (!openList.empty())
	{
		pPair p = *openList.begin(); openList.erase(openList.begin());
		closedList[x = p.second.first][y = p.second.second] = true; b--;
		for (int k = 0; k < 4; k++)
		{
			i=mx[k]+x; j=my[k]+y;
			if (i < 0 || i >= ROW || j < 0 || j >= COL||grid[i][j] == 0||b<BATMIN) 
				continue;			
			if (i == dest.first && j == dest.second) 
			{ 
				cellDetails[i][j].parent_i = x; 
				cellDetails[i][j].parent_j = y; 
				tracePath (cellDetails, dest);
				return foundDest = true;
			}
			if (!closedList[i][j]) 
			{
				gNew = cellDetails[i][j].g + 1; 
				hNew = abs(i - dest.first) + abs(j - dest.second); // Distancia manhattan
				fNew = gNew + hNew; 
				if (cellDetails[i][j].f == INT_MAX || cellDetails[i][j].f > fNew) 
				{
					openList.insert( make_pair(fNew, make_pair(i, j))); 
					cellDetails[i][j].f = fNew; cellDetails[i][j].g = gNew; cellDetails[i][j].h = hNew; 
					cellDetails[i][j].parent_i = x; cellDetails[i][j].parent_j = y; 
				}
			}
		}
	}
	return foundDest=false;
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
void Analisis() 
{
	// Generando el mapa
   srand(time(NULL));
   int Tesoro = 3, Pared = 0, Nada = 1, Pila = 2;
   for (int i = 0; i < TAB; i++)//Llenar mundo con nada
       for (int j = 0; j < TAB; j++)
           grid[i][j] = Nada;
   // PAREDES
   for (int i = 0; i < 20; i++)//Poner paredes
       for (int j = 0; j < 20; j++)
           if (((1 + rand() % 23) * 3 / 2) %  4== 0) //4 o 5 para no saturar el mapa o hacerlo imposible
               grid[i][j] = Pared;
   for (int i = 0; i < NPEL; ++i)
      grid[1 + rand() % 19][1 + rand() % 19] = Tesoro; //Posicionar pelota
   grid[1 + rand() % 18][1 + rand() % 18] = Pila; // Posicionar pila
   grid[0][19] = Nada;//Nunca bloquear coordenada inicial
   // Fin de generacion de mapa
	Pair bp; int b=100, t;
	// escanear matriz
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			if(grid[i][j]==3)
				sp.insert(make_pair(i,j));
			else if(grid[i][j]==2)
				bp=make_pair(i,j);
	// crea los caminos
	Pair start=make_pair(0,19); bool findBattery=false;
	do
	{
		Pair dest=sortFrom(start,sp); t=b;
		printf("battery %d%\n",b);
		if(!aStarSearch(grid, start, dest,t))
		{
			printf("Trying to go for the battery\n");
			cout << BATMIN << endl;
			cout << t << endl;
			t = b;
			if(!findBattery&&t>=BATMIN)
			{
				if(!aStarSearch(grid, start, bp,t))
				{
					printf("Killed\n");
					break;
				}
				findBattery=true;
				printf("Go for battery\n");
				b=t=100;
				sp.insert(dest);
				dest=bp;
			}
		}
		else
			b=t;
		start=dest;
	}while(!sp.empty());
}
// ************************
// * Codigo para graficar *
// ************************

void cuadradoC(float x, float y, float w, float h)
{
   glBegin(GL_QUADS);
      glColor3f(1.0f,0.0f,0.0f);
      glVertex2f(x-w/2.0f,y-h/2.0f);
      glVertex2f(x+w/2.0f,y-h/2.0f);
      glVertex2f(x+w/2.0f,y+h/2.0f);
      glVertex2f(x-w/2.0f,y+h/2.0f);
   glEnd();
}
void cuadrado(float x, float y, float w, float h, float r, float g, float b)
{
   glBegin(GL_QUADS);
      glColor3f(r,g,b);
      glVertex2f(x,y);
      glVertex2f(x+w,y);
      glVertex2f(x+w,y+h);
      glVertex2f(x,y+h);
   glEnd();
}

void moveRobot(int x,int y)
{
   glTranslatef(x,y,0);
      cuadrado(0.2f,0.3f,0.5f,0.4f,0.6f,0.6f,0.6f); // Cabeza
      cuadrado(0.3f,0.3f,0.3f,0.1f,0.8f,0.8f,0.8f); // Boca
      cuadrado(0.3f,0.5f,0.1f,0.1f,0.2f,0.2f,0.8f); // Ojo izq
      cuadrado(0.5f,0.5f,0.1f,0.1f,0.2f,0.2f,0.8f); // Ojo der
      cuadrado(0.3f,0.7f,0.1f,0.1f,0.6f,0.6f,0.6f); // antena izq
      cuadrado(0.5f,0.7f,0.1f,0.1f,0.6f,0.6f,0.6f); // antena der
   glTranslatef(-x,-y,0);
}
void moveBrick(int x,int y)
{
   glTranslatef(x,y,0);
   cuadrado(0,0,1,1,0.5f,0.5f,0.5f);

   cuadrado(0,0,1,0.1f,0.2f,0.2f,0.2f);
   cuadrado(0,0.3f,1,0.1f,0.2f,0.2f,0.2f);
   cuadrado(0,0.6,1,0.1f,0.2f,0.2f,0.2f);
   cuadrado(0,0.9f,1,0.1f,0.2f,0.2f,0.2f);

   cuadrado(0.4f,0.3f,0.1f,0.3f,0.2f,0.2f,0.2f);
   cuadrado(0.1f,0.6f,0.1f,0.3f,0.2f,0.2f,0.2f);
   cuadrado(0.8f,0.6f,0.1f,0.3f,0.2f,0.2f,0.2f);

   cuadrado(0.1f,0.1f,0.1f,0.3f,0.2f,0.2f,0.2f);
   cuadrado(0.8f,0.1f,0.1f,0.3f,0.2f,0.2f,0.2f);
   cuadrado(0,0.9f,1,0.1f,0.2f,0.2f,0.2f);
   glTranslatef(-x,-y,0);
}

void DrawBattery(int x, int y)
{
   glTranslatef(x,y,0);
   // Figura de la bateria
   cuadrado(x+0.3f,y,0.4f,0.8f,0,1,0);
   cuadrado(x+0.4f,y+0.8f,0.2f,0.1f,0,1,0);
   // Lineas de la bateria
   cuadrado(x+0.3f,y+0.2f,0.4f,0.05f,0.15f,0.15f,0.15f);
   cuadrado(x+0.3f,y+0.5f,0.4f,0.05f,0.15f,0.15f,0.15f);
   glTranslatef(-x,-y,0);
}
void showMap(int px, int py)
{
   int Tesoro = 3, Pared = 0, Nada = 1, Pila = 2, Encontrado = 4;
	for (int a = 0; a < TAB; a++)
		for (int b = 0; b < TAB; b++)
			if(Pared==grid[a][b])
				moveBrick(a,b);
			else if(Tesoro == grid[a][b] && a == px && b == py)
			{
				grid[a][b] = Encontrado;
			}
			else if(Encontrado == grid[a][b])
				cuadrado(a+0.24f,b+0.24f,0.5f,0.5f,1.0f,0,0);
			else if(Tesoro==grid[a][b])
			{
				cuadrado(a+0.24f,b+0.24f,0.5f,0.5f,0,0,1.0f);
			}
			else if(Pila==grid[a][b])
				cuadrado(a+0.24f,b+0.24f,0.5f,0.5f,0,1,0);
			
}

void move(int x, int y)
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
   for (int i = 0; i < TAB; ++i)
      for (int j = 0; j <  TAB; ++j)
         if((j%2!=0 && i%2!=0)||(i%2==0 && j%2==0))
            cuadrado(i,j, 1,1, 1,1,1);
   showMap(x,y);
   moveRobot(x,y);
   //DrawBattery(2,1);
   //glFlush();  // Render now
   glutSwapBuffers(); // Para renderizado en doble buffer
   //glutPostRedisplay();
}

void display() {
   	if(c < paths.size() && d < paths[c].size())
		{
   		move(paths[c][d].first,paths[c][d].second);
   		d++;
   		if(d == paths[c].size()&&c < paths.size())
			{
				c++;
				d = 1;
			}
   	}
   	else{
   		cout << "Hecho por: Armando Morales Miguel Angel Fuentes Daniel Arellano Aaron Porras" << endl;
		}
   Sleep(500); // Esperamos medio segundo
}
/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
   glutInit(&argc, argv);                 // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE);
   glutInitWindowSize(600, 600);   // Set the window's initial width & height
   glutInitWindowPosition(400, 50); // Position the window's initial top-left corner
   glutCreateWindow("Robot IA"); // Create a window with the given title
   glMatrixMode(GL_PROJECTION);              // setup viewing projection 
   glLoadIdentity();                           // start with identity matrix 
   // Tamaño del "mapa" en este caso 10x10 unidades
   glOrtho(0.0, TAB, 0.0, TAB, -1.0, 1.0);   // setup a 10x10x2 viewing world
   Analisis();
   glutDisplayFunc(display); // Register display callback handler for window re-paint
   glutIdleFunc(display);
   glutMainLoop();           // Enter the infinitely event-processing loop
	cout << "Salida del programa" << endl;
   return 0;
}