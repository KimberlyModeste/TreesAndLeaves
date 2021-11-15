#pragma once
#ifndef CLASSES_H // include guard
#define CLASSES_H
#include <GL/glut.h>
#include <math.h>

// << Support Classes for Canvas >>
class Point2   //single point w/ floating point coordinates
{
public:
    Point2() { x = y = 0.0f; } //constructor 1
    Point2(float xx, float yy) { x = xx; y = yy; } //constructor 2
    void set(float xx, float yy) { x = xx; y = yy; }
    float getX() { return x; }
    float getY() { return y; }
    void draw(void) {
        glBegin(GL_POINTS); //draw this point
        glVertex2f((GLfloat)x, (GLfloat)y);
        glEnd();
    }
private:
    float x, y;
};

class IntRect {
    //aligned rectangle with integer coordinates, used for viewport
public:
    IntRect() { l = 0; r = 100; b = 0; t = 100; } //constructors
    IntRect(int left, int right, int bottom, int top)
    {
        l = left; r = right; b = bottom; t = top;
    }
    void set(int left, int right, int bottom, int top) {
        l = left; r = right; b = bottom; t = top;
    }
    void draw(void) { //draw this rectangle using OpenGL
        glBegin(GL_LINE_STRIP); //draw this polygon
        glVertex2i(l, b);
        glVertex2i(r, b);
        glVertex2i(r, t);
        glVertex2i(l, t);
        glVertex2i(l, b);
        glEnd();
    }
    int getL(void) { return l; }
    int getR(void) { return r; }
    int getT(void) { return t; }
    int getB(void) { return b; }

private:
    int l, r, b, t;
};

class RealRect {
    //simlar to IntRect but w/ floating points & used for world window
public:
    RealRect() { l = 0; r = 100; b = 0; t = 100; } //constructors
    RealRect(float left, float right, float bottom, float top)
    {
        l = left; r = right; b = bottom; t = top;
    }
    void set(float left, float right, float bottom, float top)
    {
        l = left; r = right; b = bottom; t = top;
    }
    float getL(void) { return l; }
    float getR(void) { return r; }
    float getT(void) { return t; }
    float getB(void) { return b; }
    void draw(void) { //draw this rectangle using OpenGL
        glBegin(GL_LINE_STRIP); //draw this polygon
        glVertex2f(l, b);
        glVertex2f(r, b);
        glVertex2f(r, t);
        glVertex2f(l, t);
        glVertex2f(l, b);
        glEnd();
    }
private:
    float l, r, b, t;
};


class Canvas {
public:
    Canvas(int width, int height, char* windowTitle); //constructor
    void setWindow(float l, float r, float b, float t);
    void setViewport(int l, int r, int b, int t);
    IntRect getViewport(void); //divulge the viewport data
    RealRect getWindow(void); // divulge the window data
    float getWindowAspectRatio(void);
    void clearScreen();
    void setBackgroundColor(float r, float g, float b);
    void setColor(float r, float g, float b);
    void lineTo(float x, float y);
    void lineTo(Point2 p);
    void lineRel(float dx, float dy);
    void moveTo(float x, float y);
    void moveTo(Point2 p);
    void moveRel(float dx, float dy);

private:
    Point2 CP;         //current position in the world
    IntRect viewport;  //the current viewport
    RealRect window;   //the current window

};



//changes current point
void Canvas::moveTo(float x, float y)
{
    CP.set(x, y);
}

//draws a line from current point to new point
void Canvas::lineTo(float x, float y) {
    glBegin(GL_LINES);
    glVertex2f((GLfloat)CP.getX(), (GLfloat)CP.getY());
    glVertex2f((GLfloat)x, (GLfloat)y); //draw the line
    glEnd();
    CP.set(x, y); //update current point to new point
    glFlush();
}

void Canvas::setWindow(float l, float r, float b, float t) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D((GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t);
    window.set(l, r, b, t);
}

void Canvas::setViewport(int l, int r, int b, int t) {
    glViewport((GLint)l, (GLint)b, (GLint)r - l, (GLint)t - b);
    // viewport.set(l, r, b, t);
}

IntRect Canvas::getViewport(void)
{
    return viewport;
}

RealRect Canvas::getWindow(void)
{
    return window;
}

void Canvas::clearScreen(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Canvas::setBackgroundColor(float r, float g, float b)
{
    glClearColor(r, g, b, 0.0);
}

void Canvas::setColor(float r, float g, float b)
{
    glColor3f(r, g, b);
}

void Canvas::lineTo(Point2 p) {
    glBegin(GL_LINES);
    glVertex2f((GLfloat)CP.getX(), (GLfloat)CP.getY());
    glVertex2f((GLfloat)p.getX(), (GLfloat)p.getY());
    glEnd();
    CP.set(p.getX(), p.getY());
    glFlush();
}

Canvas::Canvas(int width, int height, char* windowTitle) {
    char* argv[1];  //dummy argument list for glutinit()
    char dummyString[16];
    argv[0] = dummyString;  //hook up the pointer
    int argc = 1;

    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //glutInitWindowSize(width, height);
    //glutInitWindowPosition(20, 20);
    //glutCreateWindow(windowTitle);
    //setWindow(0, (float)width, 0, (float)height); // default world window
    //setViewport(0, width, 0, height); //default viewport
    //CP.set(0.0f, 0.0f); //initialize the CP to (0,0)
}

void Canvas::moveTo(Point2 p)  //moves current point CP to point p object
{
    CP.set(p.getX(), p.getY());
}

float Canvas::getWindowAspectRatio(void) {
    //calculates aspect ratio of world window
    float width, height, aspectRatio;
    width = window.getR() - window.getL();
    height = window.getT() - window.getB();
    aspectRatio = width / height;
    return aspectRatio;
}

void Canvas::moveRel(float dx, float dy) {
    CP.set(CP.getX() + dx, CP.getY() + dy);
}

void Canvas::lineRel(float dx, float dy) {
    float x = CP.getX() + dx, y = CP.getY() + dy;
    lineTo(x, y);
    CP.set(x, y);
}

#endif