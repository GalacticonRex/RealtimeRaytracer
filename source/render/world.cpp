#include "base/time.h"
#include "render/world.h"
#include "render/camera.h"

namespace render {
	
	bool World::sdlEnabled = false;
	uint32 World::count = 0;
	
	World::World( uint32 w, uint32 h, const char* name ) {
		++ count;
		if ( !sdlEnabled )
			SDL_Init( SDL_INIT_EVERYTHING );
		
		_window = SDL_CreateWindow(
			name,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			w, h, SDL_WINDOW_RESIZABLE );
		
		_renderer = SDL_CreateRenderer(
			_window,
			-1, 
			SDL_RENDERER_ACCELERATED);
		
		_texture = SDL_CreateTexture(
			_renderer,
			SDL_PIXELFORMAT_RGB888,
			SDL_TEXTUREACCESS_STREAMING,
			w,
			h );
		
		SDL_QueryTexture(_texture, NULL, NULL, (int*) &_width, (int*) &_height);
		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
	}
	World::~World() {
		SDL_DestroyRenderer( _renderer );
		SDL_DestroyWindow( _window );
		if ( -- count = 0 )
			SDL_Quit();
	}
	
	uint32 World::width() const {
		return _width;
	}
	uint32 World::height() const {
		return _height;
	}
	
	void World::rasterize( Camera& cam, const Scene& sc ) {
		int pitch;
    	void *pixels;

		SDL_LockTexture(_texture, NULL, &pixels, &pitch);
		uint32 *upixels = (uint32*) pixels;
		
		//Time.setTimer();
		try {
			cam.rasterize(sc, upixels);
		} catch ( ERROR_TYPE e ) {
			error_message_t err;
			Error.get(err);
			std::cerr << err << std::endl;
		}
		//std::cerr << Time.readTimer() << std::endl;

		SDL_UnlockTexture(_texture);
		
		SDL_RenderCopy( _renderer, _texture, NULL, NULL );
		SDL_RenderPresent(_renderer);
	}
	
}