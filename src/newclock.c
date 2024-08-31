/* Analog Clock. T. Barake. March 1983. */
/* Translated to C from BASIC Aug 86 */

#include <stdlib.h>
#include <graphics.h>
#include <time.h>
#include <sys\timeb.h>
#include <math.h>

#ifdef UNIX
#include "vgtplot.h"
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/time.h>
#endif

#define GRAPHPATH "\\tclite\\bgi"		/*depends where graphics code is*/

#define ON 1
#define OFF 0
#define NO_OF_COLS (getmaxx())
#define NO_OF_LINS (getmaxy())
        
#define CENTRE_COL     (NO_OF_COLS / 2)
#define CENTRE_ROW     (NO_OF_LINS / 2)

#define R1     (NO_OF_LINS / 2 * 3 / 4)        /*controlling radius*/
#define R2     (105 * R1 / 100)                /*outer radius of dial*/
#define PI     3.14159                         /*more or less*/
#define PIFAC  (2 * PI / 60)                   /*position of ticks*/

#define R_SECS (95 * R1 / 100)
#define R_MINS (90 * R1 / 100)
#define R_HRS  (62 * R1 / 100)
#define T1     (R_SECS + 1)
#define T2     (R1 + 1)
#define MW     (R1/20)              /*minute hand wedge width*/
#define HW     (R1/20)              /*hour hand wedge width*/
#define SS     (R1/10)              /*tick length for seconds*/
#define SM     (R1/12)              /*minutes*/
#define SH     (R1/12)              /*hours*/

#define        ASPECT (yasp/xasp)
#define        fcolor            GREEN

void auto_init_graph(void)
     {
     int graph_driver = (int) DETECT, graph_mode;
     initgraph(&graph_driver,&graph_mode,GRAPHPATH);
     }

void hline(col1,lin1,col2,lin2,on)
unsigned col1,lin1,col2,lin2,on;
{
if (on)
   setcolor(fcolor);
else
    setcolor(BLACK);
line(col1,NO_OF_LINS-lin1,col2,NO_OF_LINS-lin2);
}


int si[60],co[60];               /*sin cos tables*/
int xasp,yasp;                   /*aspect ratios*/

void main()
{
struct tm *local(),*s_time;
long time();
int i,old_secs,old_mins,old_hrs,secs,mins,hrs;

/*set up tables*/

getaspectratio(&xasp,&yasp);

for (i=0 ; i<60 ; i++)
 {
 si[i] = (int)( sin(i*PIFAC)*100*yasp) ;
 co[i] = (int)( cos(i*PIFAC)*100*xasp) ;
 }

#ifdef UNIX
graphics(ON);
clear_gr();
#endif

auto_init_graph();

setcolor(fcolor);
                    
circle (CENTRE_COL,CENTRE_ROW,R1);
circle (CENTRE_COL,CENTRE_ROW,R2);

for (i=0 ; i<60; i++)
   hline ( fnt2(i),fnt3(i),fnt4(i),fnt5(i),ON );

for (i=0; i<=12 ; i++)
   hline (fnt0(i*5),fnt1(i*5),fnt2(i*5),fnt3(i*5),ON);

/*start main loop*/

old_secs=old_mins=old_hrs=0;

while (!kbhit())
 {
 do
  {
  long t = time(0L);
  s_time = localtime(&t);   
  secs=s_time->tm_sec;
  }
 while (secs==old_secs);

 mins = s_time->tm_min;
 hrs  = 5 * s_time->tm_hour + mins/12 ;

 if (hrs>60)
   hrs -= 60;

 /*move seconds hand*/

 hline (secX1(old_secs),secY1(old_secs),secX2(old_secs),secY2(old_secs),OFF);
 hline (secX1(secs),secY1(secs),secX2(secs),secY2(secs),ON);

 /*move mins hand*/

 if (mins!=old_mins)
  {
  hline (minX1(old_mins),minY1(old_mins),minX0(old_mins),minY0(old_mins),OFF);
  hline (fnm4(old_mins),fnm5(old_mins),minX0(old_mins),minY0(old_mins),OFF);
  hline (fnm4(old_mins),fnm5(old_mins),minX1(old_mins),minY1(old_mins),OFF);
  }


 /*move mins hand*/

 if (hrs!=old_hrs)
  {
  hline (fnh2(old_hrs),fnh3(old_hrs),fnh0(old_hrs),fnh1(old_hrs),OFF);
  hline (fnh4(old_hrs),fnh5(old_hrs),fnh0(old_hrs),fnh1(old_hrs),OFF);
  hline (fnh4(old_hrs),fnh5(old_hrs),fnh2(old_hrs),fnh3(old_hrs),OFF);
  }

 /*redraw all hands*/

 hline (minX1(mins),minY1(mins),minX0(mins),minY0(mins),ON);
 hline (fnm4(mins),fnm5(mins),minX0(mins),minY0(mins),ON);
 hline (fnm4(mins),fnm5(mins),minX1(mins),minY1(mins),ON);
 hline (fnh4(hrs),fnh5(hrs),fnh0(hrs),fnh1(hrs),ON);
 hline (fnh2(hrs),fnh3(hrs),fnh0(hrs),fnh1(hrs),ON);
 hline (fnh4(hrs),fnh5(hrs),fnh2(hrs),fnh3(hrs),ON);

 old_secs=secs;
 old_mins=mins;
 old_hrs=hrs;
 circle(CENTRE_COL,CENTRE_ROW,3);  /*centre of clock, really a small circle R1/33*/
 }
 closegraph();
 clrscr();
}

/*ticks around dial endpoint calculator*/

fnt0(i)
int i;
{
return ((int)(CENTRE_COL+ASPECT*T1*si[i]/100 ));
}

fnt1(i)
int i;
{
return ((int)(CENTRE_ROW+T1*co[i]/100 ) );
}

fnt2(i)
int i;
{
return((int)(CENTRE_COL+ASPECT*T2*si[i]/100 ) );
}

fnt3(i)
int i;
{
return((int)( CENTRE_ROW+T2*co[i]/100 ) );
}

fnt4(i)
int i;
{
return((int)(CENTRE_COL+ASPECT*(R2-1)*si[i]/100 ) );
}

fnt5(i)
int i;
{
return((int)(CENTRE_ROW+(R2-1)*co[i]/100 ) );
}

/*seconds endpoint calculators*/

secX2(i)
int i;
{
return((int)( CENTRE_COL + ASPECT * R_SECS * si[i]/100 ) );
}

secY2(i)
int i;
{
return((int)( CENTRE_ROW + R_SECS * co[i]/100 ) );
}


secX1(i)
int i;
{
return((int)( CENTRE_COL + -ASPECT * SS * si[i]/100 ) );
}

secY1(i)
int i;
{
return((int)( CENTRE_ROW - SS * co[i]/100 ) );
}

minX0(i)
int i;
{
return((int)( CENTRE_COL + ASPECT * R_MINS * si[i]/100 ) );
}

/*minute hand endpoint calc*/

minY0(i)
int i;
{
return((int)( CENTRE_ROW + R_MINS * co[i]/100 ) );
}

minX1(i)
int i;
{
return( (int)( CENTRE_COL - ASPECT * SM * si[( ( (i-MW) % 60 )+ 60 ) % 60 ]/100 ) );
}

minY1(i)
int i;
{
return( (int)( CENTRE_ROW - SM * co[ ( ( ( i - MW ) % 60 ) + 60 ) % 60 ]/100 ) );
}

fnm4(i)
int i;
{
return( (int) ( CENTRE_COL - ASPECT * SM * si[ ( i + MW ) % 60 ]/100 ) );
}

fnm5(i)
int i;
{
return( (int) ( CENTRE_ROW - SM * co[ ( i + MW ) % 60 ]/100 ) );
}

fnh0(i)
int i;
{
return( (int) ( CENTRE_COL + ASPECT * R_HRS * si[i]/100 ) );
}

fnh1(i)
int i;
{
return( (int) ( CENTRE_ROW + R_HRS * co[i]/100 ) );
}

fnh2(i)
int i;
{
return( (int) ( CENTRE_COL - ASPECT * SH * si[ ( ( (i-HW) % 60 ) + 60 ) % 60 ]/100 ) );
}

fnh3(i)
int i;
{
return( (int) ( CENTRE_ROW - SH * co [ ( ( ( i - HW ) % 60 ) + 60 ) % 60 ]/100 ) );
}

fnh4(i)
int i;
{
return( (int) ( CENTRE_COL - ASPECT * SH * si[ ( i + HW ) % 60 ]/100 ) );
}

fnh5(i)
int i;
{
return( (int) ( CENTRE_ROW - SH * co[ ( i + HW ) %  60 ]/100 ) );
}

#ifdef UNIX
void circle(cc,cl,r)
int cc,cl,r;
{
int i,x,y;
for (i=0 ; i < 360 ; i++)
 {
 x=(int) ( ASPECT * r * cos (2*PI*i/360) + cc );
 y=(int) ( r * sin (2*PI*i/360) + cl );
 plot (x,y);
 }
update_gr();
}
#endif 

#ifdef UNIX /*REPLACED BY TURBO C SPECIFIC CODE*/
/*
Function : hline
Description: plots a line using vgt hardware vector mode
             between col1,lin1 and col2,lin2
Inputs:     col1,lin1 starting coords
            col2,lin2 ending coords  

            0 <= l < NO_OF_LINS
            0 <= c < NO_OF_COLS

            on  : if ON line is plotted, if OFF, line is erased

Notes:      Requires macros defined in "vgtplot.h"
            Origin is in bottom left hand corner        
            Requires that graphics mode be enabled by graphics(ON) call
            update_gr() gets called everytime to refresh
*/


            
void hline(col1,lin1,col2,lin2,on)
unsigned col1,lin1,col2,lin2,on;
{
#define GS (0x1f & ']')         /*vgT100 control*/
putchar(GS);
#undef GS

white(on);              /*set color*/
plot(col1,lin1);        /*plot vector*/
plot(col2,lin2);
update_gr();
graphics(ON);           /*set back to point mode*/
}

#endif
