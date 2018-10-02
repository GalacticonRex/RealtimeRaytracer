#include "render/sphere.h"

namespace render {

	Sphere::Sphere( const core::fvec& pos, float32 r, uint32 mat ) :
		Renderable( mat ),
		_center( pos ),
		_radius( r ) { ; }

	float32 Sphere::Cast( const Ray& ray ) const {
		core::fvec dif = _center - ray.origin();
		float32 dot = core::fvec::dot( dif, ray.direction() );
		if ( dot >= 0.0f ) {
			float32 dist = (dif - dot * ray.direction()).length();
			if ( dist <= _radius )
				return dif.length() - std::sqrt( _radius * _radius - dist * dist );
		}
		return INFINITY;
	}
	float32 Sphere::CastInternal( const Ray& ray ) const {
		core::fvec dif = _center - ray.origin();
		float32 dot = core::fvec::dot( dif, ray.direction() );
		if ( dot >= 0.0f ) {
			core::fvec with = dot * ray.direction();
			return with.length() * 2.0f;
		}
		return INFINITY;
	}
	void Sphere::Normal( const core::fvec& pt, core::fvec& norm ) const {
		norm = core::fvec::normalize( pt - _center );
	}
	
	const core::fvec& Sphere::center() const {
		return _center;
	}
	core::fvec& Sphere::center() {
		return _center;
	}
	
	float32 Sphere::radius() const {
		return _radius;
	}
		
}