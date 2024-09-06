#include <GL/glut.h>
#include <GL/glu.h> 
#include<GL/gl.h>
#include <stdio.h>
#include<stdlib.h>
#include<math.h>

#define PI 3.1415926535
#define PI2 PI/2 
#define PI3 3*PI/2 
#define DR 0.0174533


float px, py, pdx, pdy, pa; // player positions

float dist(float ax, float ay, float bx, float by, float ang){
    return (sqrt((bx-ax)*(bx-ax) + (by-ay)*(by-ay))) ;
}


void drawPlayer(){
    glColor3f(1,1,0);
    glPointSize(8) ;
    glBegin(GL_POINTS) ;
    glVertex2i(px, py)  ;
    glEnd() ; 

    glLineWidth(3) ;
    glBegin(GL_LINES) ;
    glVertex2i(px, py ) ;
    glVertex2i(px+pdx*5, py+pdy*5) ;
    glEnd() ;

}

int mapx = 8, mapy = 8, maps = 64 ; 

int map[] = 
{
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,1,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,1,0,0,1, 
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
} ;

void drawMap2D(){
    int x, y, xo, yo ; 
    for(y = 0; y < mapy; y++){
        for(x = 0; x < mapx; x++){

            xo = x * maps;  // Calculate x offset
            yo = y * maps;  // Calculate y offset
            if(map[y*mapx + x] == 1){
                glColor3f(1,1,1) ;
            }
            else{
                glColor3f(0,0,0) ;    
            }

            glBegin(GL_QUADS) ;
            glVertex2i(xo  +1 ,  yo     +1) ;
            glVertex2i(xo  +1 , yo+maps -1) ;
            glVertex2i(xo+maps-1, yo+maps-1) ;
            glVertex2i(xo+maps-1, yo+1) ;
            glEnd() ;
        }
    }
}

void buttons(unsigned char key, int x, int y){
    if(key == 'a') {pa -= 0.1; if(pa < 0){pa+=2*PI;} pdx = 5*cos(pa) ; pdy = 5*sin(pa) ;}
    if(key == 'd') {pa += 0.1; if(pa > 2*PI){pa-=2*PI;} pdx = 5*cos(pa) ; pdy = 5*sin(pa) ;}
    if(key == 'w') {px+=pdx ; py+=pdy ; }
    if(key == 's') {px-= pdx; py-=pdy ; }  

    glutPostRedisplay() ;
}

void drawRays3D(){
    int r, mx, my, mp, dof; float rx, ry, ra, xo, yo, disT  ;
    ra = pa - DR*30; if(ra < 0){ra+=2*PI ;}  if(ra > 2*PI){ra-=2*PI ;} ; // ray has the same angle as the player
    // check horizontal
    for(r = 0; r < 120; r++){

        float disH = 1000000 , hx = px , hy = py     ; 
        dof = 0 ;
        float aTan = -1/tan(ra) ;
        if(ra>PI){ry = (((int)py >> 6)<<6)- 0.0001 ; rx = (py-ry)*aTan+px ; yo = -64; xo = -yo*aTan; } // looking down
        if(ra<PI){ry = (((int)py >> 6)<<6) +    64 ; rx = (py-ry)*aTan+px ; yo = 64 ; xo = -yo*aTan ; } // looking up

        if((ra==0 || ra==PI)){rx = px ; ry = py; dof = 8 ;}

        while(dof<8){
            mx=(int)(rx)>>6 ; my = (int)(ry)>>6 ; mp=my*mapx+mx ;
            if(mp>0 && mp < mapx*mapy && map[mp] == 1){ hx = rx ; hy=ry; disH = dist(px, py, hx, hy, ra)  ; dof=8 ;} 
            else{rx+=xo; ry+=yo; dof+=1;}// next line lmao
        }

        // glColor3f(1,0,0) ;
        // glLineWidth(10) ;
        // glBegin(GL_LINES) ; 
        // glVertex2i(px, py) ;
        // glVertex2i(rx, ry) ;
        // glEnd() ;

       
        // vertical line checks
        
        float disV = 1000000 , vx = px , vy = py     ; 
        dof = 0 ;
        float nTan = -tan(ra) ;
        if(ra>PI2 && ra<PI3){rx = (((int)px >> 6)<<6)- 0.0001 ; ry = (px-rx)*nTan+py ; xo = -64; yo = -xo*nTan; } // looking right
        if( ra<PI2 || ra > PI3){rx = (((int)px >> 6)<<6) +    64 ; ry = (px-rx)*nTan+py ; xo = 64 ; yo = -xo*nTan ; } // looking left

        if((ra==0 || ra==PI)){rx = px ; ry = py; dof = 8 ;}

        while(dof<8){
            mx=(int)(rx)>>6 ; my = (int)(ry)>>6 ; mp=my*mapx+mx ;
            if(mp > 0 && mp < mapx*mapy && map[mp] == 1){ vx = rx; vy = ry ; disV = dist(px, py, vx, vy, ra) ; dof=8 ;} 
            else{rx+=xo; ry+=yo; dof+=1;}// next line lmao
        }

        if(disV < disH) rx = vx, ry = vy, disT=disV, glColor3f(0.7, 0,0) ;
        else rx = hx, ry = hy, disT = disH , glColor3f(0.5,0,0);

        // glColor3f(1,0,0) ;
        glLineWidth(1)     ;
        glBegin(GL_LINES)  ; 
        glVertex2i(px, py) ;
        glVertex2i(rx, ry) ;
        glEnd()            ;

        // draw the 3d walls
        float ca = pa-ra ; if(ca<0){ca+=2*PI ;} if(ca > 2*PI){ca -= 2*PI ;} ; disT = disT*cos(ca) ;
        float lineH = (maps*320)/disT ; if(lineH > 320){lineH = 320 ;}

        float lineO = 160 - lineH/2 ; 

        glLineWidth(15) ; glBegin(GL_LINES) ; glVertex2i(r*8+530, lineO) ; glVertex2i(r*8+530, lineH+lineO) ; glEnd() ;

        ra+=0.5*DR ; if(ra < 0){ ra+=2*PI ;} ; if(ra > 2*PI){ra-=2*PI ;} ;

    }
 
    
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
    
    drawMap2D() ;   
    drawPlayer() ;
    drawRays3D() ; 
    glutSwapBuffers() ;
}

void init(){
    glClearColor(0.3, 0.3, 0.3, 0) ;
    gluOrtho2D(0, 1024, 512, 0) ;
    px = 200 ; py = 345 ; pdx = 5*cos(pa) ; pdy = 5*sin(pa) ; 
    
}


int main(int argc, char* argv[]){
    glutInit(&argc, argv) ;
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE) ;
    glutInitWindowSize(1024, 512) ;
    glutCreateWindow("blyat") ;
    init() ;
    glutDisplayFunc(display) ;
    glutKeyboardFunc(buttons) ;
    glutMainLoop() ;

}