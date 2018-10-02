#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "render/objects.h"

namespace render {
	
	// Abstract Light in scene 
	struct Light {
		Light( const core::fvec&, const core::fvec&, float32 = INFINITY );
		
		virtual float32 Cast( const core::fvec& ) const = 0;
		
		const core::fvec& getColor() const;
		const core::fvec& getCenter() const;
		float32 getDistance() const;
		
		void setColor( const core::fvec& );
		void setCenter( const core::fvec& );
		void setDistance( float32 );
		
	private:
		core::fvec _color;
		core::fvec _center;
		float32 _distance;
	};
	
	// unfortunately only 1 inherited light
	struct PointLight : Light {
		PointLight( const core::fvec&, const core::fvec&, float32 );
		float32 Cast( const core::fvec& ) const;
	};
	
}

#endif//__LIGHT_H__