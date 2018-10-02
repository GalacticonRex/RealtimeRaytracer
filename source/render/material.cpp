#include "render/material.h"

namespace render {
	
	Material::Material(
		const core::fvec& v,
		float32 amb,
		float32 dif,
		float32 spec,
		float32 refl,
		float32 refr,
		float32 ior
	) :
		_color( v ),
		_ambient( amb ),
		_diffuse( dif ),
		_specular( spec ),
		_translucence( refl ),
		_reflection( refr ),
		_ior( ior ),
		_magic( NULL ) { ; }
		
	Material::Material(
		const core::fvec& v,
		const Scene& sc,
		float32 amb,
		float32 dif,
		float32 spec,
		float32 refl,
		float32 refr,
		float32 ior
	) :
		_color( v ),
		_ambient( amb ),
		_diffuse( dif ),
		_specular( spec ),
		_translucence( refl ),
		_reflection( refr ),
		_ior( ior ),
		_magic( &sc ) { ; }
	
	const core::fvec& Material::getColor() const {
		return _color;
	}
	float32 Material::getAmbient() const {
		return _ambient;
	}
	float32 Material::getDiffuse() const {
		return _diffuse;
	}
	float32 Material::getSpecular() const {
		return _specular;
	}
	float32 Material::getTransparency() const {
		return _translucence;
	}
	float32 Material::getReflection() const {
		return _reflection;
	}
	float32 Material::getIOR() const {
		return _ior;
	}
	const Scene* Material::getMagic() const {
		return _magic;
	}
	
	void Material::setColor( const core::fvec& col ) {
		_color = col;
	}
	void Material::setAmbient( float32 amb ) {
		_ambient = amb;
	}
	void Material::setDiffuse( float32 dif ) {
		_diffuse = dif;
	}
	void Material::setSpecular( float32 spec ) {
		_specular = spec;
	}
	void Material::setTransparency( float32 trans ) {
		_translucence = trans;
	}
	void Material::setReflection( float32 refl ) {
		_reflection = refl;
	}
	void Material::setIOR( float32 ior ) {
		_ior = ior;
	}
	void Material::setMagic( const Scene* sc ) {
		_magic = sc;
	}
	
}