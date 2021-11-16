//CSC470 HW3
//Kimberly Modeste
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>   // use as needed for your system
#include <math.h>
#include <time.h>
#include "classes.h"


using namespace std;
char title[] = "Trees And Leaves";
Canvas cvs(600, 600, title);


Point2 CP;
int getrandom(int min, int max) {
    srand(time(0));
    return rand() % (int)(((max)+1) - (min)) + (min);
}

//string production rules
char atom[25] = "F";                      //starting string
char Fstr[25] = "FF-[-F+F+F]+[+F-F-F]";   //F production rule
char Xstr[25] = "";                       //X production rule
char Ystr[25] = "";                       //Y production rule
int angle = getrandom(15, 22);            //turn angle
int length = 20;                          //forward length


//min and max extremes for the window size
float xmin = 0.0, xmax = 0.0, ymin = 0.0, ymax = 0.0;

void produceString(char* st, int order, int draw)
{
    float sx, sy, sa;
    glLineWidth(order);
    
    while (*st != '\0')		// scan through each character
    {
        switch (*st)
        {
        case '[': cvs.saveTurtle(); break;
        case ']': cvs.restoreTurtle(); break;
        case '+': cvs.turn(-angle); break;	// right turn
        case '-': cvs.turn(angle); break;  // left turn
        case 'F': if (order > 0)
            produceString(Fstr, order - 1, draw);
                else
        {
            cvs.forward(length, draw);
            if (!draw)
            {
                if (CP.getX() < xmin)
                    xmin = CP.getX();
                if (CP.getX() > xmax)
                    xmax = CP.getX();
                if (CP.getY() < ymin)
                    ymin = CP.getY();
                if (CP.getY() > ymax)
                    ymax = CP.getY();
            }
        }
                break;
        case 'X': if (order > 0)
            produceString(Xstr, order - 1, draw); break;
        case 'Y': if (order > 0)
            produceString(Ystr, order - 1, draw); break;
        }
        st++;
    }
}

void myDisplay(void)
{
   
    CP.set(0, 600);
    cvs.setBackgroundColor(0.0, 0.0, 0.0);
    cvs.clearScreen();
    cvs.setColor(1, 1, 0);
   

    //setup initial turtle position
    //run through once to determine window coordinates
    cvs.moveTo(0.0, 0.0);
    cvs.turnTo(90);
    produceString(atom, 4, 0);
    cvs.setWindow(xmin - 600, xmax + 600, ymin - 600, ymax + 600);
    //this time draw the curve
    cvs.moveTo(0.0, 0.0);
    cvs.turnTo(90);
     produceString(atom, 4, 1);
    glFlush();		                 // send all output to display
}

void main(int argc, char** argv)
{
    glLineWidth(30);
    glutDisplayFunc(myDisplay);
    glutMainLoop();

}