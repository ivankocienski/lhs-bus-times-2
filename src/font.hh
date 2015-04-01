
#ifndef _IK_GLFONT_INC_
#define _IK_GLFONT_INC_

#include <GL/gl.h>

class Font {
private:

  GLuint m_font_surface;

	int m_charListID;

	int m_baseChar;
	int m_charWidth;
	int m_charHeight;
	int m_numChars;
	
public:

	Font();
	~Font();

	void load(char*, int, int, int, int);

	void draw(const char*, int);
	void draw_slow(int, int, const char*);
	void draw_fast(const char*, int);
	
	int width();
	int strWidth(char*);
	int height();
	int centerOffset(char*);
};

#endif //_IK_GLFONT_INC_
