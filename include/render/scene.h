#ifndef __SCENE_H__
#define __SCENE_H__

#include "core/array.h"
#include "render/objects.h"

namespace render {
	
	// collection of renderables and lights
	struct Scene {
		
		Scene();
		~Scene();
		
		uint32 count() const;
		uint32 lights() const;
		
		const Renderable& operator[] ( uint32 ) const ;
		const Renderable& get( uint32 ) const ;
			
		Renderable& operator[] ( uint32 );
		Renderable& get( uint32 );
		
		uint32 add( Renderable& );
		uint32 add( Light& );
		void set( uint32, Material* );
		
		const Material& material( uint32 ) const;
		Material& material( uint32 );
		
		const Light& light( uint32 ) const;
		Light& light( uint32 );
		
	private:
		uint32 _matcount;
		void* _matraw;
		Material* _materials;
		core::array_t<Light*> _lights;
		core::array_t<Renderable*> _renderables;
	};
	
}

#endif//__SCENE_H__