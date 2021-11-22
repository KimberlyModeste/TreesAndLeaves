//CSC470 HW3
//Kimberly Modeste
#include <stdio.h>
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
int gid; //Global ID, 1-4 Spring, Summer, Fall, Winter
float trunk[] = { 0.16, 0.13, 0.07 };       //Trunk color
float leafBase[] = { 0.82, 0.54, 0.65 };    //The overall color of leaf
float leafTop[] = { 0.91, 0.8, 0.82 };      //The highlight of the leaf
float grass[] = { 0.61, 0.72, 0.19 };       //Grass color (green or white)
float grass2[] = { 0.0, 0.0, 0.0 };       //Grass color (green or white)
float grassBench[] = { 0.44, 0.56, 0.0 };   //Grass under bench  (green or white)
float appleColor[] = { 1.0,0.0,0.0 };
float sky1[]{ 0.0,0.0,0.0 };
float sky2[]{ 0.0,0.0,0.0 };

//string production rules
//char atomNL[100] = "F";                      //No leaves starting string
//char FstrNL[100] = "FF-[-F+F+F]+[+F-F-F]";   //No leaves F or New leaves
//char XstrNL[100] = "";                       //No leaves X


char atomNL[100] = "F";                      //No leaves starting string
char FstrNL[100] = "FF-[-FX+FX+FX]+[+FX-FX-FX]"; //No leaves F 
char XstrNL[100] = "";                       //No leaves X



char atom[100] = "F";                            //starting string
char Fstr[100] = "FF-[-FX+FX+FX]+[+FX-FX-FX]";   //F production rule
char Xstr[100] = "XX-[FL+FL+FL]+[+FL-FL-FL]";    //X production rule
char Ystr[100] = "";                             //Y production rule

char XstrA[100] = "XX-[FN+FNY+FN]+[+FN-FN-FN]";    //X production rule
char YstrA[100] = "YY-[FNW+FN]+[+FNA]";


float angle = getrandom(15,22);                  //turn angle
int length = 20;                                 //forward length


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

void ngon(int n, float cx, float cy, float radius, float rotAngle) {
    if (n < 3) return;
    double angle = rotAngle * 3.14159265 / 180;
    double angleInc = 2 * 3.14159265 / n;
    moveTo(radius + cx, cy);

    for (int k = 0; k < n; k++)
    {
        angle += angleInc;
        lineTo(radius * cos(angle) + cx, radius * sin(angle) + cy);
    }
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

void newLeaf(int t) {
    glBegin(GL_POLYGON);

    glColor3fv(leafBase);
    float theta;
    int r = 10;
    for (int i = 0; i < 360; i++)
    {
        theta = i * 3.14159265 / 180;
        glVertex2f(r * cos(theta) + CP.getX(), r * sin(theta) + CP.getY());
    }
    glEnd();
    glLineWidth(0.5);;
   
    glBegin(GL_LINE_STRIP);
    glColor3fv(leafTop);
    glVertex2i(cos(3.14159265 / 180)+ CP.getX()+10, sin(3.14159265 / 180)+CP.getY());
    glVertex2i(cos(3.14159265 / 180) + CP.getX() -11, sin(3.14159265 / 180) + CP.getY());
    glEnd();

    glLineWidth(t);
    glColor3fv(trunk);
}

void makeSnow() {
    float theta;
    int r = 30;

    glColor3fv(appleColor);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        theta = i * 3.14159265 / 180;
        glVertex2f(r * cos(theta) + CP.getX(), r * sin(theta) + CP.getY());
    }
    glEnd();
    glColor3fv(trunk);
}

void makeApple() {
    float theta;
    int r = 10;
    int root = 10; //Make the apple hang

    glColor3fv(appleColor);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        theta = i * 3.14159265 / 180;
        glVertex2f(r * cos(theta) + CP.getX() - 6, r * sin(theta) + CP.getY() + root);
        glVertex2f(r * cos(theta) + CP.getX() - 6, r * sin(theta) + CP.getY() + root + 5);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        theta = i * 3.14159265 / 180;
        glVertex2f(r * cos(theta) + CP.getX() + 6, r * sin(theta) + CP.getY() + root);
        glVertex2f(r * cos(theta) + CP.getX() + 6, r * sin(theta) + CP.getY() + root + 5);
    }
    glEnd();
    glColor3fv(trunk);
}


void produceStringThicc(char* st, int order, int draw, float thicc)
{
    float sx, sy, sa;
    glLineWidth(thicc);

    while (*st != '\0')		// scan through each character
    {
        switch (*st)
        {
        case '[': if(thicc >= 1) thicc/=3; saveTurtle(); break;
        case ']': if (thicc <= trueThicc) thicc*=3; restoreTurtle(); break;
        case '+': turn(-angle); break;	// right turn
        case '-': turn(angle); break;  // left turn
        case 'L': if (order <= 0) { drawLeaf(); }break;
        case 'N': if (order <= 0) { if(gid >= 5 && gid < 8)newLeaf(thicc); } break;
        case 'A': if (order <= 0) { if (gid != 8)  makeApple(); } break;
        case 'W': if (order <= 0) { if (gid == 8) makeSnow(); } break;
        case 'F': if (order > 0)
        {
            if(gid == 4 )
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

            else if (gid >= 5)
            produceStringThicc(XstrA, order - 1, draw, thicc);

            else 
               produceStringThicc(Xstr, order - 1, draw, thicc);
        }
           break;
        case 'Y': if (order > 0) {
            if (gid >= 6)
                produceStringThicc(YstrA, order - 1, draw, thicc);

            else
                produceStringThicc(Ystr, order - 1, draw, thicc);
        }break;
        }
        st++;
    }

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

void makeChair(int x, int y)
{
    //Seat shadow 
    glColor3fv(grassBench);
    glBegin(GL_POLYGON);
    glVertex2i(-950 + x, -675 + y);
    glVertex2i(-550 + x, -675 + y);
    glVertex2i(-500 + x, -750 + y);
    glVertex2i(-1000 + x, -750 + y);
    glEnd();


    glColor3f(0.0, 0.0, 0.0);
    //Black straight rods
    glRecti(-950 + x, -325 + y, -925 + x, -575 + y);
    glRecti(-575 + x, -325 + y, -550 + x, -575 + y);
    //Chair Legs
    glRecti(-950 + x, -625 + y, -925 + x, -675 + y);
    glRecti(-575 + x, -625 + y, -550 + x, -675 + y);
    glRecti(-1000 + x, -625 + y, -975 + x, -750 + y);
    glRecti(-500 + x, -625 + y, -525 + x, -750 + y);

    glColor3f(0.84, 0.67, 0.32);
    //Back wood
    glRecti(-1000 + x, -350 + y, -500 + x, -400 + y);
    glRecti(-1000 + x, -425 + y, -500 + x, -475 + y);
    glRecti(-1000 + x, -500 + y, -500 + x, -550 + y);
    //Seat wood
    glBegin(GL_POLYGON);
    glVertex2i(-1000 + x, -575 + y);
    glVertex2i(-500 + x, -575 + y);
    glVertex2i(-480 + x, -625 + y);
    glVertex2i(-1020 + x, -625 + y);
    glEnd();

    //Back wood outline
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex2i(-1000 + x, -350 + y);
    glVertex2i(-500 + x, -350 + y);
    glVertex2i(-500 + x, -400 + y);
    glVertex2i(-1000 + x, -400 + y);
    glVertex2i(-1000 + x, -350 + y);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2i(-1000 + x, -425 + y);
    glVertex2i(-500 + x, -425 + y);
    glVertex2i(-500 + x, -475 + y);
    glVertex2i(-1000 + x, -475 + y);
    glVertex2i(-1000 + x, -425 + y);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2i(-1000 + x, -500 + y);
    glVertex2i(-500 + x, -500 + y);
    glVertex2i(-500 + x, -550 + y);
    glVertex2i(-1000 + x, -550 + y);
    glVertex2i(-1000 + x, -500 + y);
    glEnd();

    //Seat wood outline
    glBegin(GL_LINE_STRIP);
    glVertex2i(-1000 + x, -575 + y);
    glVertex2i(-500 + x, -575 + y);
    glVertex2i(-480 + x, -625 + y);
    glVertex2i(-1020 + x, -625 + y);
    glVertex2i(-1000 + x, -575 + y);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2i(-1200 + x, -1000 + y);
    glVertex2i(1200 + x, -1000 + y);
    glEnd();
}

void makeRocks(int x, int y, int r)
{
    
    float theta;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        theta = i * 3.14159265 / 180;
        glVertex2f(r * cos(theta) + x, r * sin(theta) + y);
    }
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    ngon(50, x, y, r, 0);

}

void seasonMenu() {

    //Spring
    if (gid == 1)
    {
        leafBase[0] = 0.82;    leafBase[1] = 0.54;    leafBase[2] = 0.65;
        leafTop[0] = 0.91;  leafTop[1] = 0.8;  leafTop[2] = 0.82;
        grass[0] = 0.61; grass[1] = 0.72; grass[2] = 0.19;
        grassBench[0] = 0.44; grassBench[1] = 0.56; grassBench[2] = 0.0;
    }

    //Summer
    else if (gid == 2)
    {
        leafBase[0] = 0.2;    leafBase[1] = 0.55;    leafBase[2] = 0.03;
        leafTop[0] = 0.96;  leafTop[1] = 0.98;  leafTop[2] = 0.3;
        grass[0] = 0.61; grass[1] = 0.72; grass[2] = 0.19;
        grassBench[0] = 0.44; grassBench[1] = 0.56; grassBench[2] = 0.0;
    }

    //Fall  
    else if (gid == 3)
    {
        leafBase[0] = 0.82;    leafBase[1] = 0.31;    leafBase[2] = 0.03;
        leafTop[0] = 1.0;  leafTop[1] = 0.75;  leafTop[2] = 0.35;
        grass[0] =  0.61 ; grass[1] =  0.72 ; grass[2] =  0.19 ;
        grassBench[0] = 0.44; grassBench[1] = 0.56; grassBench[2] = 0.0;
    }

    //Winter
    else if (gid == 4)
    {
        glColor3f(0.61, 0.72, 0.19);
        grass[0] =  0.9 ; grass[1] =  0.9; grass[2] =  0.9 ;
        grassBench[0] = 0.6; grassBench[1] = 0.6; grassBench[2] = 0.6;
    }

    //Spring colors
    else if(gid == 5)
    {
        leafBase[0] = 0.82;    leafBase[1] = 0.54;    leafBase[2] = 0.65;
        leafTop[0] = 0.91;  leafTop[1] = 0.8;  leafTop[2] = 0.82;
        grass[0] = 0.71; grass[1] = 0.87; grass[2] = 0.12;
        grass2[0] = 0.82; grass2[1] = 0.94; grass2[2] = 0.31; 
        grassBench[0] = 0.53; grassBench[1] = 0.63; grassBench[2] = 0.19;
        sky1[0] = 0.07; sky1[1] = 0.48; sky1[2] = 0.93;
        sky2[0] = 0.23; sky2[1] = 0.68; sky2[2] = 0.94;
    }

    //Summer colors
    else if (gid == 6)
    {
        appleColor[0] = 1.0;  appleColor[1] = 0.0; appleColor[2] = 0.0;
        leafBase[0] = 0.2;    leafBase[1] = 0.55;    leafBase[2] = 0.03;
        leafTop[0] = 0.96;  leafTop[1] = 0.98;  leafTop[2] = 0.3;
        grass[0] = 0.62; grass[1] = 0.86; grass[2] = 0.08;
        grass2[0] = 0.32; grass2[1] = 0.45; grass2[2] = 0.0;
        grassBench[0] = 0.23; grassBench[1] = 0.33; grassBench[2] = 0.0;
        sky1[0] = 0.23; sky1[1] = 0.68; sky1[2] = 0.94;
        sky2[0] = 0.07; sky2[1] = 0.48; sky2[2] = 0.93;
    }
    //Fall colors
    else if (gid == 7)
    {
        appleColor[0] = 1.0;  appleColor[1] = 1.0; appleColor[2] = 0.0;
        leafBase[0] = 0.82;    leafBase[1] = 0.31;    leafBase[2] = 0.03;
        leafTop[0] = 1.0;  leafTop[1] = 0.75;  leafTop[2] = 0.35;
        grass[0] = 0.53; grass[1] = 0.54; grass[2] = 0.11;
        grass2[0] = 0.57; grass2[1] = 0.63; grass2[2] = 0.18;
        grassBench[0] = 0.44; grassBench[1] = 0.56; grassBench[2] = 0.0;
        sky1[0] = 1.0; sky1[1] = 0.63; sky1[2] = 0.27;
        sky2[0] = 0.96; sky2[1] = 0.79; sky2[2] = 0.70;
    }
    //Winter
    else if (gid == 8)
    {
        appleColor[0] = 0.9;  appleColor[1] = 0.9; appleColor[2] = 1.0;
        grass[0] = 0.8; grass[1] = 0.8; grass[2] = 0.8;
        grass2[0] = 0.9; grass2[1] = 0.9; grass2[2] = 0.9;
        grassBench[0] = 0.6; grassBench[1] = 0.6; grassBench[2] = 0.6;
        sky1[0] = 0.41; sky1[1] = 0.43; sky1[2] = 0.55;
        sky2[0] = 0.92; sky2[1] = 0.91; sky2[2] = 0.90;
    }
}

void basicADisplay(void)
{
    CP.set(0, 0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    //Change colors to basic colors
    trunk[0] = 1;       trunk[1] = 1;       trunk[2] = 0;
    leafBase[0] = 0;    leafBase[1] = 1;    leafBase[2] = 0;
    leafTop[0] = 0.0;  leafTop[1] = 1;  leafTop[2] = 0.0;
    

    glColor3fv(trunk);
    int thicc = 20;         //The thicccness of the tree base
    trueThicc = thicc;      //The const to make sure the branches dont get above thicc
  
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
    glFlush();		               
}

void basicBDisplay(void)
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

     //Path
    glColor3f(0.59, 0.52, 0.34);
    glRecti(-1200, -1000, 1200, -1200);

    makeChair(0, 0);

    glFlush();
    glutSwapBuffers();

}

void creativeADisplay() {

    CP.set(0, 0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    seasonMenu();

    glColor3fv(trunk);
    int thicc = getrandom(20, 30);
    trueThicc = thicc;

    makeTree(0.0, 0.0, thicc, angle, 0);
    gluOrtho2D(xmin - 10, xmax + 10, ymin - 10, ymax + 10);
    makeTree(0.0, 0.0, thicc, angle, 1);

    glFlush();
}

void creativeBDisplay() {
    CP.set(0, 0);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    seasonMenu();

    glColor3fv(trunk);
    int thicc = getrandom(20, 30);
    trueThicc = thicc;

    makeTree(-600, 0, thicc, angle, 0);
    gluOrtho2D(-1200, 1200, -1200, 1200);

    //Sky
    glBegin(GL_POLYGON);
    glColor3fv(sky2);
    glVertex2i(-1200, 1200);
    glVertex2i(1200, 1200);
    glColor3fv(sky1);
    glVertex2i(1200, 300);
    glVertex2i(-1200, 300);
    glEnd();

    //Grass
    glBegin(GL_POLYGON);
    glColor3fv(grass2);
    glVertex2i(-1200, -1200);
    glVertex2i(1200, -1200);
    glColor3fv(grass);
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
    
    //Path
    glColor3f(0.59, 0.52, 0.34);
    if(gid != 8)
    glRecti(-1200, -1000, 1200, -1200);

    makeChair(0, 0);
    makeChair(1500, 0);

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2i(-100, -900);
    glVertex2i(-100, 600);
    glVertex2i(-120, 600);
    glVertex2i(-120, -900);
    glEnd();
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_POLYGON);
    glVertex2i(-140, 600);
    glVertex2i(-160, 700);
    glVertex2i(-60, 700);
    glVertex2i(-80, 600);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    glVertex2i(-140, 600);
    glVertex2i(-160, 700);
    glVertex2i(-60, 700);
    glVertex2i(-80, 600);
    glVertex2i(-140, 600);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2i(-60, 700);
    glVertex2i(-110, 750);
    glVertex2i(-160, 700);
    glEnd();
    srand(time(0));
    for (int i = 0; i < 30; i++) 
    {
        if(gid == 8)
        glColor3f(0.9, 0.9, 0.9);
        else
        glColor3f(0.5, 0.5, 0.5);
        makeRocks(-1200 + (i * 100), -950, rand()%50 +50);
    }
    
    glColor3fv(trunk);

    glFlush();
}

void main(int argc, char** argv)
{
    glutInit(&argc, argv);          // initialize the toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
    glutInitWindowPosition(30, 30); // set window position on screen
    glutInitWindowSize(600, 600);     // set window size
    glutCreateWindow("Trees and Leaves"); // open the screen window and set the name
   //1-4 = Basic 5-8 CreativeB
    gid = 1;


    //Basic Drawing A
    glutDisplayFunc(basicADisplay); 

    //Basic Drawing B
    //glutDisplayFunc(basicBDisplay);    

    //Creative Drawing A
    //glutDisplayFunc(creativeADisplay);

    //Creative Drawing B
    //IT TAKES 1 MINUTE AND 16 SECONDS FOR THIS TO FINISH RUNNING
    glutDisplayFunc(creativeBDisplay);

    glutMainLoop();
}
/* TIME TURBO
   Basic Drawing A: 00:09
   Basic Drawing B
   Spring: 00:48
   Summer: 00:48
   Fall:   00:48
   Winter: 00:15

   Creative Drawing A: 00:09
   Creative Drawing B
   Spring: 00:51
   Summer: 00:54
   Fall:   00:54
   Winter: 00:50

   Record Breaker since timing: 01:16
*/