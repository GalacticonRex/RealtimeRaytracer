#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "render/objects.h"

namespace render {
	
	// Defines how a surface interacts with light
	struct Material {
		
		Material(
			const core::fvec&,
			float32 amb = 0,
			float32 dif = 0,
			float32 spec = 0,
			float32 refl = 0,
			float32 refr = 0,
			float32 ior = 1
		);
		Material(
			const core::fvec&,
			const Scene&,
			float32 amb = 0,
			float32 dif = 0,
			float32 spec = 0,
			float32 refl = 0,
			float32 refr = 0,
			float32 ior = 1
		);
		
		const core::fvec& getColor() const;
		
		float32 getAmbient() const;
		float32 getDiffuse() const;
		float32 getSpecular() const;
		float32 getTransparency() const;
		float32 getReflection() const;
		
		const Scene* getMagic() const;
		
		float32 getIOR() const;
		
		void setColor( const core::fvec& );
		
		void setAmbient( float32 );
		void setDiffuse( float32 );
		void setSpecular( float32 );
		void setTransparency( float32 );
		void setReflection( float32 );
		
		void setIOR( float32 );
		
		void setMagic( const Scene* );
		
	private:
		// color
		core::fvec _color;
		
		// strengths of different light sources
		// these must add up to 1
		float32 _ambient;
		float32 _diffuse;
		float32 _specular;
		float32 _translucence;
		float32 _reflection;
		
		// transparent values
		float32 _ior;
		
		// if not null, cast rays to this scene instead
		// of the current scene
		const Scene* _magic;
	};
	
}

#endif//__MATERIAL_H__