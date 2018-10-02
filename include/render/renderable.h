#ifndef __RENDERABLE_H__
#define __RENDERABLE_H__

#include "render/objects.h"

namespace render {
	
	// Abstract type for visible object in scene
	struct Renderable {
		
		Renderable( uint32 m ) : mat( m ) { ; }
		virtual ~Renderable() { ; }
		
		void setMaterial( uint32 m ) { mat = m; }
		uint32 getMaterial() const { return mat; }
		
		virtual float Cast( const Ray& ) const = 0;
		virtual float CastInternal( const Ray& ) const = 0;
		virtual void Normal( const core::fvec&, core::fvec& ) const = 0;
		
	private:
		
		uint32 mat;
		
	};
	
}

#endif//__RENDERABLE_H__