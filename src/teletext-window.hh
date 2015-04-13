
#pragma once

#include <SDL/SDL.h>
#include <vector>
#include <string>

enum {
  TC_BLACK,
  TC_RED,
  TC_GREEN,
  TC_YELLOW,
  TC_BLUE,
  TC_MAGENTA,
  TC_CYAN,
  TC_WHITE
};

class TeleTextWindow {
private:

  typedef struct _S_CELL {
    unsigned char code;
    unsigned char foreground;
    unsigned char background;
    int flags;
  } T_CELL, *PT_CELL;

  std::vector<T_CELL> m_cells;

  SDL_Surface *m_font;
  SDL_Surface *m_screen;
  int  m_cursor_pos;
  int  m_cursor_col;
  int  m_cursor_bg;
  bool m_is_running;
  
  void render_cell( int, int, int, int, int );
  void render_cells();
  
public:

  TeleTextWindow();
  ~TeleTextWindow();

  bool open( bool fullscreen = false );

  bool is_active();

  void tick();

  void move_to( int, int );
  void puts( char* );
  void puts( const std::string& );

  void set_color( int );
  void set_bg( int );
  void draw_chr( int );

  void draw_font( int, int, char* );
};

