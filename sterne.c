#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL.h>
#include <GL/gl.h>

// Defaultwerte, in diesem Fall bedeutet 0 "nicht gesetzt".
int h=0,w=0;
int n=0;

GLfloat speed = .1f;

// Ein paar Einstellungen
#define mostdist 40
#define radius 25

GLfloat rnd()
{
  return (GLfloat)rand() / (GLfloat)RAND_MAX;
}

typedef struct
{
  GLfloat x;
  GLfloat y;
  GLfloat z;
} Position;

Position *stars;

void bright(GLfloat b)
{
  glColor3f(b, b, b);
}

void draw()
{
  int i;
  Position pos;

  glBegin( GL_LINES );

  for( i=0 ; i<n ; i++ )
  {
    pos=stars[i];
    bright(1-pos.z/40);
    glVertex3fv((GLfloat*)&pos);
    pos.z-=speed*5;
    bright(1-pos.z/40);
    glVertex3fv((GLfloat*)&pos);
  }

  glEnd();

  SDL_GL_SwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void initgl()
{
  GLdouble ratio=(GLdouble)h / (GLdouble)w;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -ratio, ratio, 1, 40);
  glMatrixMode(GL_MODELVIEW);
  glRotatef(180.f, 0.f, 1.f, 0.f);
  glTranslatef(0.f, 0.f, 1.f);
  glEnable(GL_DEPTH_TEST);
}

void move()
{
  int i;
  GLfloat *z;

  for( i=0 ; i<n ; i++ )
  {
    z=&stars[i].z;
    if( *z<0 )
      *z+=mostdist;

    if( *z>mostdist )
      *z-=mostdist;

    *z-=speed;
  }
}

void help(char *name)
{
    printf("Options :\n"
         "-w width\n"
         "-h height\n"
         "-n number of stars\n"
         "\nExample: %s -w 640 -h 480 -n 2000\n",name);
}

void initstars()
{
  int i;

  stars=(Position*)malloc(sizeof(Position)*n);

  for(i=0;i<n;i++)
  {
    stars[i].z=rnd()*mostdist;
    stars[i].x=(rnd()*radius*2)-radius;
    stars[i].y=(rnd()*radius*2)-radius;
  }
}

int main(int argc, char **argv)
{
  int option;
  SDL_Event event;
  char done=1;
  Uint8 *keys;
  SDL_Surface *gut;
  int start, left; // Zeitmessung

  if( argc==1 )
  {
    help(argv[0]);
    return 0;
  }

  while(1)
  {
    option = getopt( argc , argv , "w:h:n:" );

    if( option == -1 )
      break;

    switch(option)
    {
      case 'w' : w=atoi( optarg );
                 printf("Setting Width to %d\n",w);
                 break;
      case 'h' : h=atoi( optarg );
                 printf("Setting Height to %d\n",h);
                 break;
      case 'n' : n=atoi( optarg );
                 printf("Number of stars: %d\n",n);
                 break;
      case '?' : return 0;
    }
  }

  if(h*w==0)
  {
    printf("Error: Height and Width not set.\n");
    return 0;
  }

  if(n==0)
  {
    printf("Error: Number of stars not set.\n");
    return 0;
  }

  SDL_Init(SDL_INIT_VIDEO);

  gut=SDL_SetVideoMode( w , h , 16 , SDL_OPENGL );

  initgl();
  initstars();

  while(done)
  {
    start = SDL_GetTicks();

    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT : 
          done=0;
          break;

        case SDL_KEYDOWN :
          if(event.key.state==SDL_PRESSED)
            switch(event.key.keysym.sym)
            {
              case SDLK_ESCAPE : done=0; break;
            }
          break;
      }
    }

    keys=SDL_GetKeyState(NULL);

    if(keys[SDLK_UP])
      speed+=0.01;

    if(keys[SDLK_DOWN])
      speed-=0.01;

    move();
    draw();

    left = 20-SDL_GetTicks()+start;

    if( left < 0 )
    {
      left = 0;
      printf("CPU too slow!\n");
    }

    SDL_Delay( left );
  }

  SDL_Quit();
  free(stars);

  return 0;
}
