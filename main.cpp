
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include "font.c"

using namespace std;

bool borrar=false;

struct Click {

	int position[2];
	bool color=true;

	Click(int x,int y) {

		position[0] = x;
		position[1] = y;
			
		next = 0;
	}

	Click* next;
};


Click* top=0;


void Push(int x,int y) {

	Click* p = new Click(x,y);
	if(borrar && top)
	{	top->color=false;	
		borrar=false;
	}
	p->next = top;
	
	top = p;
}


void Pop() {
	
	if(!top)
		return;
	
	Click* p = top;
	
	top = top->next;
	
	delete p;
}

void DrawClicks() {
	
	Click* curr = top;
	glShadeModel(GL_FLAT);
	glBegin(GL_LINE_STRIP);
	
	while (curr) {
		if(!curr->color)
			glColor3f(1, 1, 1);
		else
			glColor3f(0, 0, 0);
		
		glVertex2iv(curr->position);
		curr = curr->next;
	}	
	
	glEnd();
	
	curr = top;
	glColor3f(0,0,0);
	glPointSize(4);
	glBegin(GL_POINTS);
	
	while (curr) {
		
		glVertex2iv(curr->position);
		
		curr = curr->next;
	}
	
	glEnd();
	
}

void OnDraw() {
	
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	
	glColor3f(0.4 ,1 ,0.3);
	DrawFont("Dibujando con el clic, presione b para borrar todo",10,50);
	glColor3f(0.4 ,1 ,0.3);
	DrawFont("clic derecho para separar las figuras", 10, 75);
	DrawClicks();
	
	glutSwapBuffers();
}

void OnReshape(int w, int h)
{

	if (h==0)
		h=1;

	glViewport(0,0,w,h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0,w,h,0,-100,100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OnInit() {

	glClearColor(1, 1, 1, 0.5);	
//	glEnable(GL_DEPTH_TEST);
}


void OnExit() {
}


void OnMouseClick(int button,int state,int x,int y) {

	if(state == GLUT_DOWN) {

		if (button == GLUT_LEFT_BUTTON)
			Push(x,y);

		if (button == GLUT_RIGHT_BUTTON)
			borrar=true;
			//Pop();
	}
	glutPostRedisplay();
}

void Key(unsigned char key, int x, int y)
{
	switch (key) {

	case 'b':
		while(top)
			Pop();
		borrar=false;
		glutPostRedisplay();
		break;
		
	case 27:
		cout<<"Programa Finalizado"<<endl;
		exit(0);
	}
}

int main(int argc,char** argv) {
	
	ofstream Myfile;
	Myfile.open("test.csv");
	Myfile.close();
	
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);

	glutInitWindowSize(640,480);
	
	glutInitWindowPosition(250,150);

	glutCreateWindow("Drawing figures");

	glutDisplayFunc(OnDraw);
	
	glutKeyboardFunc(Key);

	glutReshapeFunc(OnReshape);

	glutMouseFunc(OnMouseClick);

	OnInit();

	//atexit(OnExit);

	glutMainLoop();
	
	return 0;
}
