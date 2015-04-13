
#include <SDL/SDL_image.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "teletext-window.hh"

static const char *s_window_title = "Teletext Window";
static const char *s_teletext_font_file = "data/font.png";

static const int s_xres = 800;
static const int s_yres = 600;
static const int s_char_width  = 12;
static const int s_char_height = 20;
static const int s_term_width  = 40;
static const int s_term_height = 25;

static SDL_Color s_palette[] = {
  {   0,   0,   0 }, // 0 - black
  { 255,   0,   0 }, // 1 - red
  {   0, 240,   0 }, // 2 - green
  { 255, 230,   0 }, // 3 - yellow
  {   0,   0, 255 }, // 4 - blue
  { 255,   0, 255 }, // 5 - magenta
  {   0, 255, 255 }, // 6 - cyan
  { 255, 255, 255 }, // 7 - white
};

using namespace std;

TeleTextWindow::TeleTextWindow() {
  m_cursor_pos = 0;
  m_cursor_col = 15;
  m_cursor_bg  = 0;
  m_is_running = false;
  m_screen     = NULL;

  m_cells.resize( s_term_width * s_term_height );
}

TeleTextWindow::~TeleTextWindow() {
  if(m_screen) SDL_Quit();
}

bool TeleTextWindow::open( bool fullscreen ) {

  m_screen = SDL_SetVideoMode( s_xres, s_yres, 8, SDL_SWSURFACE); 
  if( !m_screen ) return false;

  m_font = IMG_Load( s_teletext_font_file );
  if( !m_font ) return false;

  SDL_LockSurface( m_font );

  SDL_SetPalette( m_screen, SDL_LOGPAL, s_palette, 0, 17 ); 
  SDL_WM_SetCaption( s_window_title, 0 );

  m_is_running = true;
  return true;
}

bool TeleTextWindow::is_active() {

  return m_is_running;
}

void TeleTextWindow::tick() {
  
  SDL_Event event;

  render_cells();

  SDL_Flip( m_screen );
  SDL_Delay( 20 );

  while( SDL_PollEvent(&event) ) {
    switch( event.type ) {

      case SDL_QUIT:
        m_is_running = false;
        break;
    }
  }
}

void TeleTextWindow::render_cells() {

  int term_pixel_width = s_term_width * s_char_width;
  int sx = (s_xres - term_pixel_width) / 2;
  int mx = sx + term_pixel_width - s_char_width;
  int px = sx;
  int py = (s_yres - (s_term_height * s_char_height)) / 2;

  SDL_LockSurface( m_screen );

  for( vector<T_CELL>::iterator it = m_cells.begin(); it != m_cells.end(); it++ ) {

    render_cell( px, py, it->code, it->foreground, it->background );

    px += s_char_width;
    if( px > mx ) {
      px = sx;
      py += s_char_height;
    } 
  }

  SDL_UnlockSurface( m_screen );
}

void TeleTextWindow::render_cell( int px, int py, int code, int fg, int bg ) {

  unsigned char *pixels = (unsigned char *)m_screen->pixels + py * m_screen->pitch + px;
  
  unsigned char *from_pixels = (unsigned char *)m_font->pixels;

  {
    int col = (code & 15) * s_char_width;
    int row = (code / 16) * s_char_height;

    from_pixels += row * m_font->pitch + col;
  }

  for( int ch = 0; ch < s_char_height; ch++ ) {
    for( int cw = 0; cw < s_char_width; cw++ ) {

      if( *from_pixels )
        *pixels = fg;
      else
        *pixels = bg;

      pixels++;
      from_pixels++;
    }

    from_pixels += m_font->pitch - s_char_width;
    pixels      += m_screen->pitch - s_char_width;
  }


}

void TeleTextWindow::draw_font( int x, int y, char *t ) {
}

void TeleTextWindow::set_color( int c ) {
  m_cursor_col = c;
}

void TeleTextWindow::set_bg( int c ) {
  m_cursor_bg = c;
}

void TeleTextWindow::move_to( int x, int y ) {
  m_cursor_pos = y * s_term_width + x;
}

void TeleTextWindow::puts( char *s ) {

  while(*s) {
    m_cells[m_cursor_pos].code = *s;
    m_cells[m_cursor_pos].foreground = m_cursor_col;
    m_cells[m_cursor_pos].background = m_cursor_bg;


    s++;
    m_cursor_pos++;
    if(m_cursor_pos >= (s_term_width * s_term_height)) break;
  }
}

void TeleTextWindow::puts( const string &s ) {
  puts( (char *)s.c_str() );
}
