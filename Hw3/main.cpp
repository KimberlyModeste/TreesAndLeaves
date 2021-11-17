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
int gid = 1; //1-4 Spring, Summer, Fall, Winter
float trunk[] = { 0.16, 0.13, 0.07 };
float leafBase[] = { 0.82, 0.54, 0.65 };
float leafTop[] = { 0.91, 0.8, 0.82 };
float grass[] = { 0.61, 0.72, 0.19 };
float grassBench[] = { 0.44, 0.56, 0.0 };

//string production rules
char atomNL[100] = "F";
char FstrNL[100] = "FF-[-F+F+F]+[+F-F-F]";
char XstrNL[100] = "";                       //X production rule


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
    
    glPointSize(6);
    glBegin(GL_POINTS);
        glColor3fv(leafBase);
        glVertex2f(CP.x, CP.y);
    glEnd();
    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3fv(leafTop);
        glVertex2f(CP.x, CP.y+1);
    glEnd();
    glColor3fv(trunk);
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
        {
            if(gid == 4)
                produceStringThicc(FstrNL, order - 1, draw, thicc);
            else 
                produceStringThicc(Fstr, order - 1, draw, thicc);
        }
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
        {
            if(gid == 4)
            produceStringThicc(XstrNL, order - 1, draw, thicc);
            else 
               produceStringThicc(Xstr, order - 1, draw, thicc);
        }
           break;
        case 'Y': if (order > 0)
            produceStringThicc(Ystr, order - 1, draw, thicc); break;
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

    trunk[0] = 1;       trunk[1] = 1;       trunk[2] = 0;
    leafBase[0] = 0;    leafBase[1] = 1;    leafBase[2] = 0;
    leafTop[0] = 0.8;  leafTop[1] = 0.8;  leafTop[2] = 0.8;


    glColor3fv(trunk);
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

void makeTree(int x, int y, int thicc, int newAngle, int t) 
{
    angle = newAngle;
    trueThicc = thicc;

    //setup initial turtle position
    //run through once to determine window coordinates
    glColor3fv(trunk);
    moveTo(x, y);
    turnTo(90);
    if(gid == 4)
        produceStringThicc(atomNL, 4, t, thicc);
    else
    produceStringThicc(atom, 4, t, thicc);
}

void seasonMenu() {

    if (gid == 1)
    {
        leafBase[0] = 0.82;    leafBase[1] = 0.54;    leafBase[2] = 0.65;
        leafTop[0] = 0.91;  leafTop[1] = 0.8;  leafTop[2] = 0.82;
        grass[0] = 0.61; grass[1] = 0.72; grass[2] = 0.19;
        grassBench[0] = 0.44; grassBench[1] = 0.56; grassBench[2] = 0.0;
    }
    else if (gid == 2)
    {
        leafBase[0] = 0.2;    leafBase[1] = 0.55;    leafBase[2] = 0.03;
        leafTop[0] = 0.96;  leafTop[1] = 0.98;  leafTop[2] = 0.3;
        grass[0] = 0.61; grass[1] = 0.72; grass[2] = 0.19;
        grassBench[0] = 0.44; grassBench[1] = 0.56; grassBench[2] = 0.0;
    }
    else if (gid == 3)
    {
        leafBase[0] = 0.82;    leafBase[1] = 0.31;    leafBase[2] = 0.03;
        leafTop[0] = 1.0;  leafTop[1] = 0.75;  leafTop[2] = 0.35;
        grass[0] =  0.61 ; grass[1] =  0.72 ; grass[2] =  0.19 ;
        grassBench[0] = 0.44; grassBench[1] = 0.56; grassBench[2] = 0.0;
    }
    else if (gid == 4)
    {
        glColor3f(0.61, 0.72, 0.19);
        grass[0] =  0.9 ; grass[1] =  0.9; grass[2] =  0.9 ;
        grassBench[0] = 0.6; grassBench[1] = 0.6; grassBench[2] = 0.6;
    }

}


void parkDisplay()
{

    CP.set(0, 0);  
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    seasonMenu();

    glColor3fv(trunk);
    int thicc = getrandom(20,30);
    trueThicc = thicc;

    makeTree(-600, 0, thicc, angle, 0);
    gluOrtho2D(-1200, 1200, -1200, 1200);
    
    //Sky
    glBegin(GL_POLYGON);
        glColor3f(0.89, 0.93, 0.97);
        glVertex2i(-1200, 1200);
        glVertex2i(1200, 1200);
        glColor3f(0.56, 0.54, 1.0);
        glVertex2i(1200, 300);
        glVertex2i(-1200, 300);
    glEnd();

    //Grass
    glBegin(GL_POLYGON);
    glColor3fv(grass);
    glVertex2i(-1200, -1200);
    glVertex2i(1200, -1200);
    glVertex2i(1200, 300);
    glVertex2i(-1200, 300);
    glEnd();

    //Tree 1B
    makeTree(-600, 0, thicc, angle, 1);
    //Tree2B
    makeTree(200, -50, getrandom(20, 30), getrandom(15, 25), 1);
    //Tree 3B
    makeTree(1000, -100, getrandom(20, 30), getrandom(15, 25), 1);
    //Tree 4M
    makeTree(-800, -300, getrandom(20, 30), getrandom(15, 25), 1);
    //Tree 5M
    makeTree(-200, -200, getrandom(20, 30), getrandom(15, 25), 1);
    //Tree 6M
    makeTree(700, -250, getrandom(20, 30), getrandom(15, 25), 1);

    //Seat shadow 
    glColor3fv(grassBench);
    glBegin(GL_POLYGON);
    glVertex2i(-950, -675);
    glVertex2i(-550, -675);
    glVertex2i(-500, -750);
    glVertex2i(-1000, -750);
    glEnd();

     //Path
    glColor3f(0.59, 0.52, 0.34);
    glRecti(-1200, -1000, 1200, -1200);

    glColor3f(0.0, 0.0, 0.0);
    //Black straight rods
    glRecti(-950, -325, -925, -575);
    glRecti(-575, -325, -550, -575);
    //Chair Legs
    glRecti(-950, -625, -925, -675);
    glRecti(-575, -625, -550, -675);
    glRecti(-1000, -625, -975, -750);
    glRecti(-500, -625, -525, -750);

    glColor3f(0.84, 0.67, 0.32);
    //Back wood
    glRecti(-1000, -350, -500, -400);
    glRecti(-1000, -425, -500, -475);
    glRecti(-1000, -500, -500, -550);
    //Seat wood
    glBegin(GL_POLYGON);
        glVertex2i(-1000, -575);
        glVertex2i(-500, -575);
        glVertex2i(-480, -625);
        glVertex2i(-1020, -625);
    glEnd();

    //Back wood outline
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex2i(-1000, -350);
    glVertex2i(-500, -350);
    glVertex2i(-500, -400);
    glVertex2i(-1000, -400);
    glVertex2i(-1000, -350);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2i(-1000, -425);
    glVertex2i(-500, -425);
    glVertex2i(-500, -475);
    glVertex2i(-1000, -475);
    glVertex2i(-1000, -425);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2i(-1000, -500);
    glVertex2i(-500, -500);
    glVertex2i(-500, -550);
    glVertex2i(-1000, -550);
    glVertex2i(-1000, -500);
    glEnd();

    //Seat wood outline
    glBegin(GL_LINE_STRIP);
    glVertex2i(-1000, -575);
    glVertex2i(-500, -575);
    glVertex2i(-480, -625);
    glVertex2i(-1020, -625);
    glVertex2i(-1000, -575);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2i(-1200, -1000);
    glVertex2i(1200, -1000);
    glEnd();

    glFlush();
    glutSwapBuffers();

}

void main(int argc, char** argv)
{
    glutInit(&argc, argv);          // initialize the toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
    glutInitWindowPosition(30, 30); // set window position on screen
    glutInitWindowSize(600, 600);     // set window size
    glutCreateWindow("Trees and Leaves"); // open the screen window and set the name

    //Basic Drawing
    glutDisplayFunc(myDisplay); 
    //Park display 
    //glutDisplayFunc(parkDisplay);    
    glutMainLoop();
}
