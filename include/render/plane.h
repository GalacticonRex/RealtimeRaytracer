#ifndef __PLANE_H__
#define __PLANE_H__

#include "render/renderable.h"
#include "render/ray.h"
#include "render/material.h"

namespace render {
	
	// Flat, infinitely sized plane
	struct Plane : public Renderable {
		Plane( const core::fvec&, const core::fvec&, uint32 );
		
		float Cast( const Ray& ) const;
		float CastInternal( const Ray& ) const;
		void Normal( const core::fvec&, core::fvec& ) const;
		
		const core::fvec& center() const;
		core::fvec& center();
		
		const core::fvec& normal() const;
		core::fvec& normal();
		
	private:
		core::fvec _center;
		core::fvec _normal;
		core::fvec _inverse;
	};
	
}

#endif//__PLANE_H__