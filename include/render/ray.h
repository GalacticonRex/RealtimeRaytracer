#ifndef __RAY_H__
#define __RAY_H__

#include "core/quaternion.h"
#include "render/objects.h"

namespace render {
	
	// A single ray in the scene, which draws to a single pixel
	struct Ray {
	private:
		
		core::fvec _direction;
		const core::fvec* _location;
		
		float32 _ior; // IOR of this ray, for tracing refraction
		
		uint32 _exclude; // which object in the scene to exclude
		Ray* _follow; // if there is a bounce, this ray will take care of it
		
		// get light from lights
		void Shade( core::fvec&, const core::fvec&, const Scene& );
		
		// get light from reflection
		void Reflect( core::fvec&, const core::fvec&, const Scene& );
		
		// get light from refraction
		void Refract( core::fvec&, const core::fvec&, float32, const Scene&, uint32 );
		
	public:
		Ray();
		Ray( const core::fvec&, const core::fvec& );
		
		// cast a ray into the scene
		void cast( core::fvec&, const Scene& );
		
		// cast a ray into an object
		void castInternal( core::fvec&, const Scene&, uint32 );
		
		// determine if light is visible
		bool castLight( float32, const Scene& );
		
		void setPosition( const core::fvec& );
		
		void setDirection( const core::fvec& );
		void rotate( const core::quaternion_t& );
		
		void setFollow( Ray* );
		
		void setIOR( float32 );
		
		void excludeObject( uint32 );
		
		const core::fvec& origin() const;
		const core::fvec& direction() const;
	};
	
}

#endif//__RAY_H__