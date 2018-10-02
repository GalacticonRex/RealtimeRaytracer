#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "render/renderable.h"
#include "render/ray.h"
#include "render/material.h"

namespace render {
	
	// typical sphere
	struct Sphere : public Renderable {
		Sphere( const core::fvec&, float, uint32 );
		
		float Cast( const Ray& ) const;
		float CastInternal( const Ray& ) const;
		void Normal( const core::fvec&, core::fvec& ) const;
		
		const core::fvec& center() const;
		core::fvec& center();
		
		float radius() const;
		
	private:
		core::fvec _center;
		float _radius;
	};
	
}

#endif//__SPHERE_H__