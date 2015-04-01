
#include <GL/gl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "teletext-window.hh"
#include "third-party/png++/png.hpp"

static const char *teletext_font_file = "data/font.png";

using namespace std;

TeleTextWindow::TeleTextWindow() {
  m_main_window  = NULL;
  m_font_texture = 0;
  m_cursor_pos   = 0;
}

TeleTextWindow::~TeleTextWindow() {
  if( m_font_texture ) 
    glDeleteTextures(1, &m_font_texture);

  if( m_main_window )
    glfwTerminate();
}

bool TeleTextWindow::open( bool fullscreen ) {

  if(!glfwInit())
    return false;

  m_main_window = glfwCreateWindow(800, 600, "Teletext Window", NULL, NULL);

  if(!m_main_window) return false;

  glfwMakeContextCurrent(m_main_window);

  
  glViewport(0, 0, 800, 600);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0, 800, 600, 0.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //glEnable(GL_TEXTURE_2D);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor( 0, 0, 1, 1 );

  //if( !load_font_texture() ) return false;

  //m_font.load( "data/font.png", 12, 20, 0, 255 );

  m_cells.resize( 40 * 25 );

  m_char_width  = 1 / 12.0;
  m_char_height = 1 / 20.0;

  return true;
}

bool TeleTextWindow::load_font_texture() {


  png::image< png::rgba_pixel >image;
  image.read(teletext_font_file);

  int w = image.get_width();
  int h = image.get_height();

  unsigned char *data = NULL;
  data = (unsigned char*)calloc( w * h * 4, sizeof(unsigned char));
  if(!data) return false;

  unsigned char *d = data;

  for( int y = 0; y < h; y++ ) {

    png::image< png::rgba_pixel >::row_type const & row = image.get_row(y);

    for(int x = 0; x < w; x++ ) {

      *d = row.at(x).red;   d++;
      *d = row.at(x).green; d++;
      *d = row.at(x).blue;  d++;
      *d = row.at(x).alpha; d++;
    }
  }

  glGenTextures( 1, &m_font_texture);
  glBindTexture( GL_TEXTURE_2D, m_font_texture);

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    w,
    h,
    0, // border
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    data 
  );

  free(data);

  return true;
}

bool TeleTextWindow::is_active() {

  return !glfwWindowShouldClose(m_main_window);
}

void TeleTextWindow::tick() {
  
  struct timespec ts;

  ts.tv_sec  = 0;
  //ts.tv_nsec = ( 1.0 / (float)10.0) * 1000;
  ts.tv_nsec = 500 * 1000000;

  //glBindTexture( GL_TEXTURE_2D, m_font_texture);
//  glPushMatrix();
  //glLoadIdentity();

  glLoadIdentity();

  glBegin(GL_QUADS);

  vector<T_CELL>::iterator it;

  int xpos = 160;
  int ypos = 50;

//  for( it = m_cells.begin(); it != m_cells.end(); it++ ) {
  for( int it = 0; it < 400; it++ ) {

    glColor3f( xpos / 800.0, 0, 0 );

//    float tu = float(it->letter & 15) / 16.0 * m_char_width;
//    float tv = float(it->letter >> 4) / 16.0 * m_char_height;

//    glTexCoord2f( tu, tv ); 
    glVertex2f( xpos, ypos);

//    glTexCoord2f( tu + m_char_width, tv );
    glVertex2f( xpos + 12, ypos );

//    glTexCoord2f( tu + m_char_width, tv + m_char_height );
    glVertex2f( xpos + 12, ypos + 20 );

//    glTexCoord2f( tu, tv + m_char_height);
    glVertex2f( xpos, ypos + 20);
    
    xpos += 12;
    if( xpos >= 640 ) {
      xpos = 160;
      ypos += 20;
    }
  }
  
  glEnd();
//  glPopMatrix();

  glfwSwapBuffers(m_main_window);
  glfwPollEvents();

  glClear( GL_COLOR_BUFFER_BIT );

  nanosleep( &ts, NULL ); 
}

void TeleTextWindow::draw_font( int x, int y, char *t ) {
//  m_font.draw_slow( x, y, t );
}

void TeleTextWindow::move_to( int x, int y ) {
  m_cursor_pos = y * 40 + x;
}

void TeleTextWindow::puts( char *s ) {

  while(*s) {
    m_cells[m_cursor_pos].letter = *s;

    s++;
    m_cursor_pos++;
    if(m_cursor_pos >= (40 * 25)) break;
  }
}
