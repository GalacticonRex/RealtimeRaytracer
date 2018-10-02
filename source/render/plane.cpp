#include "render/plane.h"

namespace render {
	
	Plane::Plane( const core::fvec& c, const core::fvec& n, uint32 m ) :
		Renderable( m ),
		_center( c ),
		_normal( n ),
		_inverse( -n ) { ; }
		
	float Plane::Cast( const Ray& ray ) const {
		float dot = core::fvec::dot( ray.direction(), _inverse );
		if ( dot > 0.0f ) {
			core::fvec distance = _normal * core::fvec::dot( _center - ray.origin(), _normal );
			core::fvec decomp = _inverse * dot;
			return distance.length() / decomp.length();
		}
		return INFINITY;
	}
	float Plane::CastInternal( const Ray& ray ) const {
		return INFINITY;
	}
	void Plane::Normal( const core::fvec& pt, core::fvec& norm ) const {
		norm = _normal;
	}
	
	const core::fvec& Plane::center() const {
		return _center;
	}
	core::fvec& Plane::center() {
		return _center;
	}
	
	const core::fvec& Plane::normal() const {
		return _normal;
	}
	core::fvec& Plane::normal() {
		return _normal;
	}
	
}