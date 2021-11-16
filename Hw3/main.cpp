//CSC470 HW3
//Kimberly Modeste
#include <stdio.h>   // use as needed for your system
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include <iostream>

using namespace std;
class Point2   //single point w/ floating point coordinates
{
public:
    float x, y;
    Point2() { x = y = 0.0f; } //constructor 1
    Point2(float xx, float yy) { x = xx; y = yy; } //constructor 2
    void set(float xx, float yy) { x = xx; y = yy; }
    float getX() { return x; }
    float getY() { return y; }
    void draw(void)
    {
        glBegin(GL_POINTS); //draw this point
        glVertex2f((GLfloat)x, (GLfloat)y);
        glEnd();
    }

};

int getrandom(int min, int max) {
    srand(time(0));
    return (rand() % (int)(((max)+1) - (min))) + (min);
}

Point2 CP;      //current turtle point
float CD = 90;  //current turtle direction
int trueThicc;
//string production rules


char atom[100] = "F";                      //starting string
char Fstr[100] = "FF-[-FX+FX+FX]+[+FX-FX-FX]";   //F production rule
char Xstr[100] = "XX-[FL+FL+FL]+[+FL-FL-FL]";                       //X production rule
char Ystr[100] = "";                       //Y production rule
float angle = getrandom(15,22);                         //turn angle
int length = 20;                          //forward length


//array of current turtle locations
//better to use a linked list for these :)
//These arrays will allow a maximum of 100 nested [] pairs.   Should be heaps!!
Point2 saveTut[100];
float saveAng[100];
int curr = 0;

//min and max extremes for the window size
float xmin = 0.0, xmax = 0.0, ymin = 0.0, ymax = 0.0;

void moveTo(float x, float y)
{
    CP.set(x, y);
}

void lineTo(float x, float y)
{
    glBegin(GL_LINES);
    glVertex2f((GLfloat)CP.getX(), (GLfloat)CP.getY());
    glVertex2f((GLfloat)x, (GLfloat)y); //draw the line
    glEnd();
    CP.set(x, y); //update current point to new point
    glFlush();
}

void turnTo(float angle) { CD = angle; }
void turn(float angle) { CD += angle; }
void forward(float dist, int isVisible)
{
    const float rpd = 0.017453393;
    float x1 = CP.x + dist * cos(rpd * CD);
    float y1 = CP.y + dist * sin(rpd * CD);
    if (isVisible)
        lineTo(x1, y1);
    else
        moveTo(x1, y1);
}

void saveTurtle()
{
    saveTut[curr].set(CP.x, CP.y);
    saveAng[curr] = CD;
    curr++;
}

void restoreTurtle()
{
    curr--;
    moveTo(saveTut[curr].getX(), saveTut[curr].getY());
    turnTo(saveAng[curr]);
}

void drawLeaf() {
    glColor3f(0, 1, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(CP.x, CP.y);
    glEnd();
    glColor3f(1, 1, 0);
}

void produceStringThicc(char* st, int order, int draw, float thicc)
{
    float sx, sy, sa;
    glLineWidth(thicc);

    while (*st != '\0')		// scan through each character
    {
       // cout << thicc << " ";
        switch (*st)
        {
        case '[': if(thicc >= 1) thicc/=3; saveTurtle(); break;
        case ']': if (thicc <= trueThicc) thicc*=3; restoreTurtle(); break;
        case '+': turn(-angle); break;	// right turn
        case '-': turn(angle); break;  // left turn
        case 'L': if (order <= 0) { drawLeaf(); }break;
        case 'F': if (order > 0)
            produceStringThicc(Fstr, order - 1, draw, thicc);
                else
        {
            forward(length, draw);
            if (!draw)
            {
                if (CP.x < xmin)
                    xmin = CP.x;
                if (CP.x > xmax)
                    xmax = CP.x;
                if (CP.y < ymin)
                    ymin = CP.y;
                if (CP.y > ymax)
                    ymax = CP.y;
            }
        }
                break;
        case 'X': if (order > 0)

            produceStringThicc(Xstr, order - 1, draw, thicc); break;
        case 'Y': if (order > 0)
            produceStringThicc(Ystr, order - 1, draw, thicc); break;
        }
        st++;
    }
    //cout << endl;
}

void produceString(char* st, int order, int draw)
{
    float sx, sy, sa;
    glLineWidth(order);

    while (*st != '\0')		// scan through each character
    {
        switch (*st)
        {
        case '[': saveTurtle(); break;
        case ']': restoreTurtle(); break;
        case '+': turn(-angle); break;	// right turn
        case '-': turn(angle); break;  // left turn
        case 'L': if (order <= 0) { drawLeaf(); }break;
        case 'F': if (order > 0)
            produceString(Fstr, order - 1, draw);
                else
        {
            forward(length, draw);
            if (!draw)
            {
                if (CP.x < xmin)
                    xmin = CP.x;
                if (CP.x > xmax)
                    xmax = CP.x;
                if (CP.y < ymin)
                    ymin = CP.y;
                if (CP.y > ymax)
                    ymax = CP.y;
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
    CP.set(0, 0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 0);
    glLineWidth(10.0f);
    int thicc = 20;
    trueThicc = thicc;
  
    //setup initial turtle position
    //run through once to determine window coordinates
    moveTo(0.0, 0.0);
    turnTo(90);
    produceStringThicc(atom, 4, 0, thicc);
    gluOrtho2D(xmin - 10, xmax + 10, ymin - 10, ymax + 10);

    //this time draw the curve
    moveTo(0.0, 0.0);
    turnTo(90);
    produceStringThicc(atom, 4, 1, thicc);
    glFlush();		                 // send all output to display
}

void main(int argc, char** argv)
{
    glutInit(&argc, argv);          // initialize the toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
    glutInitWindowSize(600, 600);     // set window size
    glutInitWindowPosition(100, 150); // set window position on screen
    glutCreateWindow("Trees and Leaves"); // open the screen window and set the name
    glutDisplayFunc(myDisplay);     // register redraw function
    glutMainLoop();
}
