
#pragma once

#include <vector>

#include <GLFW/glfw3.h>
#include "font.hh"

class TeleTextWindow {
private:

  typedef struct _S_CELL {
    unsigned char letter;
    unsigned char foreground;
    unsigned char background;
    int flags;
  } T_CELL, *PT_CELL;

  std::vector<T_CELL> m_cells;

  GLFWwindow* m_main_window = NULL;

//  Font m_font;
  
  GLuint m_font_texture;
  float m_char_width;
  float m_char_height;

  bool load_font_texture();

  int m_cursor_pos;
  
public:

  TeleTextWindow();
  ~TeleTextWindow();

  bool open( bool fullscreen = false );

  bool is_active();

  void tick();

  void move_to( int, int );
  void puts( char* );

  void set_color( int );
  void set_bg( int );
  void draw_chr( int );


  void draw_font( int, int, char* );
};

