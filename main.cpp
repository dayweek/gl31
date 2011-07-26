#include <iostream>
#include <SDL/SDL.h>
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;

// FPS
Uint32 globalTime = 0;
float frame = 0;
Uint32 timeMile = 0;
float framesToPrint = 0.0;

GLuint vaos[2];
GLfloat interlieved[24] = {
  0,1,0,
  1,1,0,
  0,1,0,
  1,-1,0,
  0,1,0,
  -1,-1,0,
  0,1,0,
  -1,1,0

};

GLfloat vertices[24] = {
  1,1,0,
  1,-1,0,
  -1,-1,0,
  -1,1,0,
  1,2,0,
  1,0,0,
  -1,0,0,
  -1,2,0
};

GLfloat colors[24] = {
  0.5,0.5,0.5,
  0,1,0,
  0,0,1,
  0,1,1,
  0.5,0.5,0.5,
  0,1,0,
  0,0,1,
  0,1,1
};

GLuint indices1[4] = {
  0,1,2,3
};

GLint multifirst[2] = {
  0,4
};

GLuint indices2[4] = {4,5,6,7};

const GLvoid* indices[2] = {indices1, indices2};
GLsizei multicount[2] = {
  4,4
};

class Model {
public:
  Model() {

  }
  void drawPrim() {
    glBegin ( GL_QUADS );
    glColor3fv ( colors );
    glVertex3fv ( vertices );
    //glColor3fv(colors+3);
    glVertex3fv ( vertices+3 );
    //glColor3fv(colors+6);
    glVertex3fv ( vertices+6 );
    //glColor3fv(colors+9);
    glVertex3fv ( vertices+9 );
    glEnd();
  }
  void prepareInterlieved() {
    glInterleavedArrays ( GL_C3F_V3F, 0, interlieved );
  }
  void prepareVAO() {
    glEnableClientState ( GL_VERTEX_ARRAY );
    glGenVertexArrays ( 2,vaos );
    glBindVertexArray ( vaos[0] );
    glVertexPointer ( 3, GL_FLOAT, 0, vertices );
    glBindVertexArray ( vaos[1] );
    glVertexPointer ( 3, GL_FLOAT, 4, vertices );
    glBindVertexArray ( 0 );
  }
  void prepareVA() {
    glEnableClientState ( GL_VERTEX_ARRAY );
    glEnableClientState ( GL_COLOR_ARRAY );
    glColorPointer ( 3, GL_FLOAT, 0, colors );
    glVertexPointer ( 3, GL_FLOAT, 0, vertices );
  }
  void drawVAO() {
    glColor3f ( 1,1,0 );
    glBindVertexArray ( vaos[0] );
    glDrawArrays ( GL_QUADS, 0, 4 );
    glBindVertexArray ( vaos[1] );
    glDrawArrays ( GL_QUADS, 0, 4 );
  }
  void drawVA() {
 glDrawArrays ( GL_QUADS, 0, 4 );
//       glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, indices1);
//       glMultiDrawArrays(GL_QUADS, multifirst, multicount, 2);
// glMultiDrawElements(GL_QUADS, multicount, GL_UNSIGNED_INT, indices, 2);

  }
  ~Model() {
  }

};
Model* model;

string getDigits ( float fps ) {
  int f = fps;
  string digits;
  stringstream str;
  char c;
  while ( f != 0 ) {
    str << ( f % 10 );
    str.get ( c );
    digits += c;
    f /= 10;
  }
  reverse ( digits.begin(), digits.end() );
  return digits;
}


static void quit ( int code ) {
  SDL_Quit();
  exit ( code );
}



static void handle_key_down ( SDL_keysym* keysym ) {

  /*
   * We're only interested if 'Esc' has
   * been presssed.
   *
   * EXERCISE:
   * Handle the arrow keys and have that change the
   * viewing position/angle.
   */
  switch ( keysym->sym ) {
  case SDLK_ESCAPE:
    quit ( 0 );
    break;
  default:
    break;
  }

}


static void process_events ( void ) {
  /* Our SDL event placeholder. */
  SDL_Event event;

  /* Grab all the events off the queue. */
  while ( SDL_PollEvent ( &event ) ) {

    switch ( event.type ) {
    case SDL_KEYDOWN:
      /* Handle key presses. */
      handle_key_down ( &event.key.keysym );
      break;
    case SDL_QUIT:
      /* Handle quit requests (like Ctrl-c). */
      quit ( 0 );
      break;
    }

  }

}

static void draw_screen ( void ) {

  globalTime = SDL_GetTicks();
  if ( globalTime - timeMile > 1000 ) {
    framesToPrint = frame/ ( globalTime - timeMile ) *1000;
    timeMile = globalTime;
    frame = 0;
    cout << framesToPrint << endl;
  }
  frame++;

  // viewing
  glMatrixMode ( GL_PROJECTION );
  glLoadIdentity();
  glOrtho ( -10,10,-10,10,1,-1 );
  glMatrixMode ( GL_MODELVIEW );
  glLoadIdentity();
  glPolygonMode ( GL_BACK, GL_LINE );

  glClearColor ( 0,0,0,0 );
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  model->drawVA();

  glFlush();

  /*
   * Swap the buffers. This this tells the driver to
   * render the next frame from the contents of the
   * back-buffer, and to set all rendering operations
   * to occur on what was the front-buffer.
   *
   * Double buffering prevents nasty visual tearing
   * from the application drawing on areas of the
   * screen that are being updated at the same time.
   */
  SDL_GL_SwapBuffers( );
}




int main ( int argc, char* argv[] ) {

  srand ( unsigned ( time ( NULL ) ) );
  /* Information about the current video settings. */
  const SDL_VideoInfo* info = NULL;
  /* Dimensions of our window. */
  int width = 0;
  int height = 0;
  /* Color depth in bits of our window. */
  int bpp = 0;
  /* Flags we will pass into SDL_SetVideoMode. */
  int flags = 0;

  /* First, initialize SDL's video subsystem. */
  if ( SDL_Init ( SDL_INIT_VIDEO ) < 0 ) {
    /* Failed, exit. */
    fprintf ( stderr, "Video initialization failed: %s\n",
              SDL_GetError( ) );
    quit ( 1 );
  }


  /* Let's get some video information. */
  info = SDL_GetVideoInfo( );

  if ( !info ) {
    /* This should probably never happen. */
    fprintf ( stderr, "Video query failed: %s\n",
              SDL_GetError( ) );
    quit ( 1 );
  }

  /*
   * Set our width/height to 640/480 (you would
   * of course let the user decide this in a normal
   * app). We get the bpp we will request from
   * the display. On X11, VidMode can't change
   * resolution, so this is probably being overly
   * safe. Under Win32, ChangeDisplaySettings
   * can change the bpp.
   */
  width = 800;
  height = 800;
  bpp = info->vfmt->BitsPerPixel;

  /*
   * Now, we want to setup our requested
   * window attributes for our OpenGL window.
   * We want *at least* 5 bits of red, green
   * and blue. We also want at least a 16-bit
   * depth buffer.
   *
   * The last thing we do is request a double
   * buffered window. '1' turns on double
   * buffering, '0' turns it off.
   *
   * Note that we do not use SDL_DOUBLEBUF in
   * the flags to SDL_SetVideoMode. That does
   * not affect the GL attribute state, only
   * the standard 2D blitting setup.
   */
  SDL_GL_SetAttribute ( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute ( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute ( SDL_GL_BLUE_SIZE, 5 );
  SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, 16 );
  SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );

  /*
   * We want to request that SDL provide us
   * with an OpenGL window, in a fullscreen
   * video mode.
   */
  flags = SDL_OPENGL;

  /*
   * Set the video mode
   */
  if ( SDL_SetVideoMode ( width, height, bpp, flags ) == 0 ) {
    /*
     * This could happen for a variety of reasons,
     * including DISPLAY not being set, the specified
     * resolution not being available, etc.
     */
    fprintf ( stderr, "Video mode set failed: %s\n",
              SDL_GetError( ) );
    quit ( 1 );
  }

  model = new Model();
  model->prepareVA();


  /*
   * Now we want to begin our normal app process--
   * an event loop with a lot of redrawing.
   */
  while ( 1 ) {
    /* Process incoming events. */
    process_events( );
    /* Draw the screen. */
    draw_screen( );
    SDL_Delay ( 5 );

  }

  /* Never reached. */
  quit ( 0 );
}





