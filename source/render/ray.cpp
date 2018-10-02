#include "render/ray.h"
#include "render/scene.h"
#include "render/camera.h"
#include "render/material.h"
#include "render/light.h"
#include "render/renderable.h"

namespace render {
	
	Ray::Ray() :
		_location( NULL ),
		_ior( 1.0f ),
		_exclude( -1 ),
		_follow( NULL ) { ; }
	Ray::Ray( const core::fvec& orig, const core::fvec& dir ) :
		_direction( dir ),
		_location( &orig ),
		_ior( 1.0f ),
		_exclude( -1 ),
		_follow( NULL ) { ; }
	
	void Ray::Shade( core::fvec& output,
					   const core::fvec& norm,
					   const Scene& sc ) {
		for ( uint32 i=0;i<sc.lights();++i ) {
			const Light& lt = sc.light( i );

			core::fvec dif = lt.getCenter() - _follow->origin();
			float32 dist = dif.length();
			core::fvec n = core::fvec::normalize( dif );
			float32 dot = core::fvec::dot( norm, n );
			float32 ltd = lt.getDistance();
			if ( dist <= ltd && dot > 0 ) {
				_follow->setDirection( n );
				if ( _follow->castLight( dist, sc ) )
					output += lt.getColor() * dot * (ltd-dist)/ltd;
			}
		}
		output.data[0] = std::max( 0.f, std::min( output.x(), 1.f ) );
		output.data[1] = std::max( 0.f, std::min( output.y(), 1.f ) );
		output.data[2] = std::max( 0.f, std::min( output.z(), 1.f ) );
	}
	void Ray::Reflect( 	core::fvec& output,
						const core::fvec& norm,
						const Scene& sc ) {
		
		core::fvec refl = core::fvec::normalize( core::fvec::reflect( _direction, norm ) );
		_follow->setDirection( refl );
		_follow->cast( output, sc );
		
	}
	void Ray::Refract( 	core::fvec& output,
						const core::fvec& norm,
						float32 ior,
						const Scene& sc,
						uint32 obj ) {
		
		core::fvec refr = core::fvec::normalize( core::fvec::refract( _direction, norm, _ior / ior ) );
		_follow->setDirection( refr );
		_follow->setIOR( ior );
		_follow->castInternal( output, sc, obj );
		
	}
	
	void Ray::cast( core::fvec& output, const Scene& sc ) {
		uint32 count = sc.count();
		
		const Renderable* target = NULL;
		uint32 targID = -1;
		
		float32 distance = INFINITY;
		
		for ( uint32 i=0;i<count;++i ) {
			if ( i == _exclude )
				continue;
			const Renderable& r = sc[i];
			float casted = r.Cast( *this );
			if ( casted < distance ) {
				distance = casted;
				target = &r;
				targID = i;
			}
		}
		
		if ( std::isfinite(distance) ) {
			
			const Material& mat = sc.material( target->getMaterial() );
			
			core::fvec shaded;
			if ( _follow != NULL ) {
				
				core::fvec reflected, refracted;
				core::fvec norm, pt = *_location + _direction * distance;
				float32 reflv, refrv;
				
				target->Normal( pt, norm );
				
				_follow->setPosition( pt );
				_follow->setIOR( 1.0f );
				_follow->excludeObject( targID );
				
				Shade( shaded, norm, sc );
				
				if ( (reflv=mat.getReflection()) != 0 )
					Reflect( reflected, norm, sc );
				if ( (refrv=mat.getTransparency()) != 0 )
					Refract( refracted, norm, mat.getIOR(), sc, targID );
				
				output =
					mat.getAmbient() * mat.getColor() +
					mat.getDiffuse() * shaded +
					mat.getTransparency() * refracted +
					mat.getReflection() * reflected;
				
			} else
				output = (
					mat.getAmbient() * mat.getColor() +
					mat.getDiffuse() * shaded 
				) / ( mat.getAmbient() + mat.getDiffuse() );
		}
	}
	
	void Ray::castInternal( core::fvec& output, const Scene& sc, uint32 objid ) {
		const Renderable& obj = sc[objid];
		float32 distance = obj.CastInternal( *this );
		if ( std::isfinite(distance) ) {
			if ( _follow != NULL ) {
				core::fvec norm, pt = *_location + _direction * distance;
				obj.Normal( pt, norm );
				core::fvec refr = core::fvec::refract( _direction, norm, 1.0 );
				_follow->setPosition( pt );
				_follow->setDirection( refr );
				_follow->setIOR( 1.0f );
				const Scene* nsc = sc.material( obj.getMaterial() ).getMagic();
				if ( nsc != NULL ) {
					_follow->excludeObject( -1 );
					_follow->cast( output, *nsc );
				} else {
					_follow->excludeObject( objid );
					_follow->cast( output, sc );
				}
			}
		}
	}
	bool Ray::castLight( float32 dist, const Scene& sc ) {
		uint32 count = sc.count();
		for ( uint32 i=0;i<count;++i ) {
			if ( i == _exclude )
				continue;
			const Renderable& r = sc[i];
			if ( r.Cast( *this ) < dist )
				return false;
		}
		return true;
	}
	
	void Ray::setPosition( const core::fvec& pos ) {
		_location = &pos;
	}
	void Ray::setDirection( const core::fvec& dir ) {
		_direction = dir;
	}
	void Ray::rotate( const core::quaternion_t& q ) {
		_direction = q * _direction;
	}
	void Ray::setFollow( Ray* r ) {
		_follow = r;
	}
	void Ray::setIOR( float32 ior ) {
		_ior = ior;
	}
	void Ray::excludeObject( uint32 obj ) {
		_exclude = obj;
	}
	
	const core::fvec& Ray::origin() const {
		return *_location;
	}
	const core::fvec& Ray::direction() const {
		return _direction;
	}
}