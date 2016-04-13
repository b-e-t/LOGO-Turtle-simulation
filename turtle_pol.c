/* PROGRAM:	turtle_pol.c
   AUTHOR:	Brian Thompson
   DATE:	Feb 3, 2016
   PURPOSE:	Create a PostScript file that makes a polygon
   USAGE:	gcc -o turtle_polygon turtle-pol.c, ./turtle_polygon > postscriptfile.ps
*/

/*INCLUDE*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define HEIGHT 595
#define WIDTH 841

/*color_t holds info about the pen color*/
typedef struct {
	float r, g, b;
}color_t;

/*turtle_t holds info on the turtle position and direction*/
typedef struct {
	double xPos;
	double yPos;
	double dir;
}turtle_t;

/*pen_t holds info about the pen color, if it is down, and if it is a filled object*/
typedef struct {
	color_t color;
	int down;
	int filled;
}pen_t;

/*FUNCTION PROTOTYPES*/
void turtle_goto(turtle_t *, float, float);
void pen_set_rgb(pen_t *, float, float, float);
void turtle_walk(turtle_t *, float);
void turtle_turn(turtle_t *, float);
void set_ps_header(int, int);
void set_ps_display();
void pen_fill(pen_t *);
void turtle_polygon(turtle_t *, pen_t *, int, int);

/*************************M*A*I*N*************************/
int main(void) {
	turtle_t boo;
	pen_t pen;

	set_ps_header(HEIGHT, WIDTH);
	pen_fill(&pen);
	turtle_goto(&boo, 45, 450);
	pen_set_rgb(&pen, 0.8, 0.1, 0.1);
	turtle_polygon(&boo, &pen, 3, 100);
	turtle_goto(&boo, 245, 250);
	pen_set_rgb(&pen, 0.1, 0.8, 0.1);
	turtle_polygon(&boo, &pen, 6, 200);
	turtle_goto(&boo, 145, 50);
	pen_set_rgb(&pen, 0.1, 0.1, 0.8);
	turtle_polygon(&boo, &pen, 12, 50);
	set_ps_display();

	return 0;
}
/*************************M*A*I*N*************************/

/*GOTO SPECIFIC LOCATION*/
void turtle_goto(turtle_t *t, float x, float y){
	if((x<595 && x>0) && (y<841 && y>0)){
		t->xPos = x;
		t->yPos = y;
	}
	fprintf(stdout, "%f %f moveto\n", x, y);
	return;
}/*END turtle_goto*/

/*CHANGE PEN COLOR*/
void pen_set_rgb(pen_t *p, float r, float g, float b){
	p->color.r = r;
	p->color.g = g;
	p->color.b = b;
	fprintf(stdout, "%.2f %.2f %.2f setrgbcolor\n", r, g, b);
	return;
}/*END pen_set_rgb*/


/*DRAW LINE IN DIRECTION TURTLE IS FACING*/
void turtle_walk(turtle_t *t, float r){
	if((t->xPos+(r*cos(t->dir*PI/180)))<595 && (t->xPos+(r*cos(t->dir*PI/180)))>0)
	t->xPos = t->xPos+(r*cos(t->dir*PI/180));
	if((t->yPos+(r*sin(t->dir*PI/180)))<841 && (t->yPos+(r*sin(t->dir*PI/180)))>0)
	t->yPos = t->yPos+(r*sin(t->dir*PI/180));
	fprintf(stdout, "%f %f lineto\n", t->xPos, t->yPos);
	return;
}/*END turtle_walk*/

/*TURN TURTLE COUNTER CLOCKWISE*/
void turtle_turn(turtle_t *t, float d){
	while(d>360){
		d = d-360;
	}
	while(d<(-360)){
		d = d+360;
	}
	if(t->dir+d>360){
		t->dir = d-(360-t->dir);
	}
	else if(t->dir+d<0){
		t->dir = 360+(t->dir+d);
	}
	else{t->dir += d;}
	return;
}/*END turtle_turn*/

/*SET HEADER FOR PS FILE*/
void set_ps_header(int w, int h){
	fprintf(stdout, "%%!PS-Adobe-3.0 EPSF-3.0\n");
	fprintf(stdout, "%%%%BoundingBox: 0 0 %d %d\n\n", w, h);
	fprintf(stdout, "newpath\n");
	return;
}/*END set_ps_header*/

/*SET FOOTER FOR PS FILE*/
void set_ps_display(){
	fprintf(stdout, "\nstroke\nshowpage");
	return;
}/*END set_ps_display*/

/*TOGGLE PEN FILLED/UNFILLED*/
void pen_fill(pen_t *p){
	if(p->filled==1){
		p->filled = 0;
	}
	else{
		p->filled = 1;
	}
	return;
}/*END pen_fill*/

/*DRAW A POLYGON WITH X SIDES*/
void turtle_polygon(turtle_t *t, pen_t *p, int count, int size){
	if(size>2){
		int i;
		for(i=0;i<count;i++){
			turtle_walk(t, size);
			turtle_turn(t, 360/count);
		}
		fprintf(stdout, "closepath\n");
		if(p->filled==1){
			fprintf(stdout, "fill\n");
		}
	}
	return;
}/*END turtle_polygon*/
