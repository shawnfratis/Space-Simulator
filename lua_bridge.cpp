#include <ogl.hpp>

extern "C" int engine_display_init( int window_width, int window_height, const char *window_title ) {
	return ogl_init::display_init( window_width, window_height, window_title );
}
