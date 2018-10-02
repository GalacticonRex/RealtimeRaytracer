#include "render/light.h"

namespace render {
	
	Light::Light( const core::fvec& col, const core::fvec& cen, float32 d ) :
		_color( col ),
		_center( cen ),
		_distance( d ) { ; }
	
	const core::fvec& Light::getColor() const {
		return _color;
	}
	const core::fvec& Light::getCenter() const {
		return _center;
	}
	float32 Light::getDistance() const {
		return _distance;
	}
	
	void Light::setColor( const core::fvec& col ) {
		_color = col;
	}
	void Light::setCenter( const core::fvec& cen ) {
		_center = cen;
	}
	void Light::setDistance( float32 d ) {
		_distance = d;
	}
	
	
	PointLight::PointLight( const core::fvec& col, const core::fvec& cen, float32 d ) :
		Light( col, cen, d ) { ; }
	float32 PointLight::Cast( const core::fvec& ) const {
		return INFINITY;
	}
	
}