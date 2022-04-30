/*
 * GL01Hello.cpp: Test OpenGL C/C++ Setup
 */
//#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <chrono>
#include <thread>

#define TAB 20
#define NPEL 10
//#include "Matrix.h"
 
/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
using namespace std;

static char mundo[20][20];
static int a,b;

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
   // Ladrillo
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
   // cout << "imprimir bat " << x << ", " << y << endl;
}
void generateMap()
{
   srand(time(NULL));
   char Tesoro = 'O', Pared = 'X', Nada = '_', Pila = 'P';
   //Random rand = new Random();
   int num;

   for (int i = 0; i < TAB; i++)//Llenar mundo con nada
       for (int j = 0; j < TAB; j++)
           mundo[i][j] = Nada;

   // PAREDES
   /*
   for (int i = 0; i < 20; i++)//Poner paredes
       for (int j = 0; j < 20; j++)
       {
           num = ((1 + rand() % 23) * 3 / 2);
           if (num %  4== 0) //4 o 5 para no saturar el mapa o hacerlo imposible
               mundo[i][j] = Pared;
       }
   */
   //num = 1 + rand() % 19;
   for (int i = 0; i < NPEL; ++i)
   {
      mundo[1 + rand() % 19][1 + rand() % 19] = Tesoro; //Posicionar pelota
   }
   mundo[1 + rand() % 18][1 + rand() % 18] = Pila; // Posicionar pila

   //for (int i = 0; i < TAB; ++i)
   for(int i = TAB-1; i >= 0; i--)
   {
      for (int j = 0; j < TAB; ++j)
         cout << mundo[j][i];
      cout << endl;
   }
   mundo[0][0] = Nada;//Nunca bloquear coordenada inicial
}
void showMap()
{
   char Tesoro = 'O', Pared = 'X', Nada = '_', Pila = 'P';
   //Ver mundo
    for (int a = 0; a < TAB; a++)
    {
        for (int b = 0; b < TAB; b++)
            if(Pared==mundo[a][b])
               moveBrick(a,b);
            else if(Tesoro==mundo[a][b])
               cuadrado(a+0.24f,b+0.24f,0.5f,0.5f,0,0,1.0f);
            else if(Pila==mundo[a][b]){
               //DrawBattery(a,b);   // NO se muestra la bateria
               cuadrado(a+0.24f,b+0.24f,0.5f,0.5f,0,1,0);
            }
    }
}

void move(int x, int y)
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
 
   for (int i = 0; i < TAB; ++i)
      for (int j = 0; j <  TAB; ++j)
         if((j%2!=0 && i%2!=0)||(i%2==0 && j%2==0))
            cuadrado(i,j, 1,1, 1,1,1);
   showMap();
   //moveRobot(x,y);
   moveRobot(x,y);
   //DrawBattery(2,1);
   //glFlush();  // Render now
   glutSwapBuffers(); // Para renderizado en doble buffer
   //glutPostRedisplay();
}

void display() {
   //move(0,0);
   // Agregar aqui los valores de los puntos del arreglo
   move(a,b);
   // Para mover el robot
   Sleep(500); // Esperamos medio segundo
   a++;  // Aumentamos el valor en X
   // Agregar aqui el aumento en el indice de la ruta del arreglo
}
 
/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
   glutInit(&argc, argv);                 // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE);
   glutInitWindowSize(600, 600);   // Set the window's initial width & height
   glutInitWindowPosition(400, 50); // Position the window's initial top-left corner
   glutCreateWindow("Robot IA"); // Create a window with the given title

   a = b = 0; // Iniciando los valores del robot

   glMatrixMode(GL_PROJECTION);              // setup viewing projection 
   glLoadIdentity();                           // start with identity matrix 
   // Tamaño del "mapa" en este caso 10x10 unidades
   glOrtho(0.0, TAB, 0.0, TAB, -1.0, 1.0);   // setup a 10x10x2 viewing world
   generateMap();

   glutDisplayFunc(display); // Register display callback handler for window re-paint
   glutIdleFunc(display);
   //std::this_thread::sleep_for(chrono::milliseconds(500));

   glutMainLoop();           // Enter the infinitely event-processing loop
   return 0;
}