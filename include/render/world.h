#ifndef __WORLD_H__
#define __WORLD_H__

#include "render/objects.h"
#include "render/camera.h"

namespace render {
	
	// handles SDL creation and destruction
	// encapsulates a single window
	struct World {
		
		World( uint32, uint32, const char* );
		~World();
		
		uint32 width() const;
		uint32 height() const;
		
		// call this function to render the specified scene
		// to this window using the specified camera
		void rasterize( Camera&, const Scene&);
		
	private:
		static bool sdlEnabled;
		static uint32 count;
		
		uint32 _width, _height;
		
		SDL_Window* _window;
		SDL_Renderer* _renderer;
		SDL_Texture* _texture;
	};
	
}

#endif//__WORLD_H__