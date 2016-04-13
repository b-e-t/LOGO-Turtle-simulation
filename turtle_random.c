/* PROGRAM:	turtle_random.c
   AUTHOR:	Brian Thompson
   DATE:	Feb 3, 2016
   PURPOSE:	Create a PostScript file that makes a random line
   USAGE:	gcc -o turtle_random turtle-random.c, ./turtle_random > postscriptfile.ps
*/

/*INCLUDE*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
void pen_set_rgb(pen_t *, color_t);
int verify_rgb(color_t);
void turtle_walk(turtle_t *, float);
void turtle_turn(turtle_t *, float);
void set_ps_header(int, int);
void set_ps_display();
int roll_dice(int, int);
void turtle_random_walk(turtle_t *, pen_t *, int);

/*************************M*A*I*N*************************/
int main(void) {
	turtle_t boo;
	pen_t pen;
	srand(time(NULL));

	set_ps_header(HEIGHT, WIDTH);
	turtle_goto(&boo, 297.5, 420.5);
	turtle_random_walk(&boo, &pen, 20000);
	set_ps_display();

	return 0;
}
/*************************M*A*I*N*************************/

/*GOTO SPECIFIC LOCATION*/
void turtle_goto(turtle_t *t, float x, float y){
	if((x<595 || x>0) && (y<841 || y>0)){
		t->xPos = x;
		t->yPos = y;
	}
	fprintf(stdout, "%f %f moveto\n", x, y);
	return;
}/*END turtle_goto*/

/*CHANGE PEN COLOR*/
void pen_set_rgb(pen_t *p, color_t c){
	p->color.r = c.r;
	p->color.g = c.g;
	p->color.b = c.b;
	fprintf(stdout, "%.2f %.2f %.2f setrgbcolor\n", c.r, c.g, c.b);
	return;
}/*END pen_set_rgb*/

/*VERIFY COLOR*/
int verify_rgb(color_t c){
	if(c.r<0 || c.r>1 || c.g<0 || c.g>1 || c.b<0 || c.b>1){return 0;}
	else{return 1;}
}/*END verify_rgb*/

/*DRAW LINE IN DIRECTION TURTLE IS FACING*/
void turtle_walk(turtle_t *t, float r){
	if((t->xPos+(r*cos(t->dir*PI/180)))<595 && (t->xPos+(r*cos(t->dir*PI/180)))>0){
		t->xPos = t->xPos+(r*cos(t->dir*PI/180));
	}
	if((t->yPos+(r*sin(t->dir*PI/180)))<841 && (t->yPos+(r*sin(t->dir*PI/180)))>0){
		t->yPos = t->yPos+(r*sin(t->dir*PI/180));
	}
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

/*GET RANDOM NUMBER*/
int roll_dice(int min, int max){
	return min+(rand()%max);
}/*END roll_dice*/

/*TRACE A RANDOM PATH AROUND THE SCREEN*/
void turtle_random_walk(turtle_t *t, pen_t *p, int steps){
	int i;
	color_t cc;
	for(i=0; i<steps; i++){
		int rndm = roll_dice(1, 4);
		if(rndm==1){
			turtle_walk(t, 10);
		}
		else if(rndm==2){
			turtle_turn(t, 90);
		}
		else if(rndm==3){
			turtle_turn(t, -90);
		}
		else{
			cc.r = (rand()%10)*0.1;
			cc.g = (rand()%10)*0.1;
			cc.b = (rand()%10)*0.1;
			pen_set_rgb(p, cc);
		}
	}
	return;
}/*END turtle_random_walk*/
