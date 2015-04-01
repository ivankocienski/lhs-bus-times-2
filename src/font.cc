
#include <GL/gl.h>
#include <string.h>
#include <stdio.h>

#include "third-party/png++/png.hpp"
#include "font.hh"

Font::Font() {}

void Font::load( char* from, int cw, int ch, int start, int end ) {

  /* setup vars */
	m_charListID = 0;
  m_font_surface = 0;
	
	m_baseChar   = 0;
	m_charWidth  = 0;
	m_charHeight = 0;
	m_numChars   = 0;

  png::image< png::rgba_pixel >image;
  image.read(from);

  int w = image.get_width();
  int h = image.get_height();

  unsigned char *data = NULL;
  data = (unsigned char*)calloc( w * h * 4, sizeof(unsigned char));
  if(!data) return;

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

  glGenTextures( 1, &m_font_surface);
  glBindTexture( GL_TEXTURE_2D, m_font_surface);

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

  float image_w = (float)w;
  float image_h = (float)h;

  /* do magic */
	float x1;
	float x2;
	float y1;
	float y2;
	
	int ix;
	int iy;
	int n;
	
	m_numChars   = end - start;
	m_baseChar   = start;
	m_charWidth  = cw;
	m_charHeight = ch;
	
	m_charListID = glGenLists(m_numChars);
	
	ix = 0;
	iy = 0;

	for(n = 0; n < m_numChars; n++) {
		
		x1 = (float)ix / image_w;
		y1 = (float)iy / image_h;
		x2 = (float)(ix + m_charWidth) / image_w;
		y2 = (float)(iy + m_charHeight) / image_h;
		
		glNewList(m_charListID + n, GL_COMPILE);
		
		  glBegin(GL_QUADS);
			
			glTexCoord2f(x1, y1); glVertex2i(0, 0);
      glTexCoord2f(x2, y1); glVertex2i(m_charWidth, 0);
			glTexCoord2f(x2, y2); glVertex2i(m_charWidth, m_charHeight);
			glTexCoord2f(x1, y2); glVertex2i(0, m_charHeight);
			
		  glEnd();
		  
		  glTranslated(m_charWidth, 0, 0);
		  
		glEndList();
		
		ix += m_charWidth;
		if(ix >= image_w) {
			ix = 0;
			iy += m_charHeight;
			
			if(iy >= image_h) break;
		}
	}
}

Font::~Font() {

  if(m_font_surface) glDeleteTextures(1, &m_font_surface);
	if(m_charListID) glDeleteLists(m_charListID, m_numChars);
}

void Font::draw_fast(const char* s, int l) {
	glListBase(m_charListID - m_baseChar);
	glCallLists(l, GL_BYTE, s);
}

void Font::draw(const char* s, int len) {
  glBindTexture( GL_TEXTURE_2D, m_font_surface);
  glPushMatrix();

	glListBase(m_charListID - m_baseChar);
	glCallLists(len, GL_BYTE, s);

  glPopMatrix();
}

void Font::draw_slow(int x, int y, const char* s) {
  glBindTexture( GL_TEXTURE_2D, m_font_surface);

  glPushMatrix();
	glLoadIdentity();
	glTranslated(x, y, 0);
	glListBase(m_charListID - m_baseChar);
	
	glCallLists(strlen(s), GL_BYTE, s);
  glPopMatrix();
}

int Font::width() {

	return m_charWidth;
}

int Font::strWidth(char* s) {
	
	return m_charWidth * strlen(s);
}

int Font::height() {

	return m_charHeight;
}

int Font::centerOffset(char* s) {
	
	return (640 - (m_charWidth * strlen(s))) / 2.0;
}

