#include "render/scene.h"
#include "render/material.h"

namespace render {
	
	Scene::Scene() :
		_matcount( 0 ),
		_matraw( NULL ),
		_materials( NULL ) { ; }
		
	Scene::~Scene() {
		if ( _matraw != NULL ) {
			for ( uint32 i=0;i<_matcount;++i )
				_materials[i].~Material();
			operator delete[]( _matraw );
		}
	}

	const Renderable& Scene::operator[] ( uint32 i ) const {
		return this->get( i );
	}
	const Renderable& Scene::get( uint32 i ) const {
		return *(_renderables[i]);
	}
	Renderable& Scene::operator[] ( uint32 i ) {
		return this->get( i );
	}
	Renderable& Scene::get( uint32 i ) {
		return *(_renderables[i]);
	}
	uint32 Scene::add( Renderable& obj ) {
		_renderables.push( &obj );
		return _renderables.size() - 1;
	}
	uint32 Scene::add( Light& lght ) {
		_lights.push( &lght );
		return _lights.size() - 1;
	}
	void Scene::set( uint32 amt, Material* mats ) {
		if ( _matraw != NULL ) {
			for ( uint32 i=0;i<_matcount;++i )
				_materials[i].~Material();
			operator delete[]( _matraw );
		}
		
		_matcount = amt;
		_matraw = operator new[] (_matcount*sizeof(Material)+15);
		_materials = (Material*)( (uint8*)_matraw + (uint32)((uintptr_t)_matraw%16) );
		
		Material* m = _materials;
		Material* e = _materials + _matcount;
		for ( ;m != e; ++ m, ++ mats )
			new (m) Material( *mats );
	}
	uint32 Scene::count() const {
		return _renderables.size();
	}
	uint32 Scene::lights() const {
		return _lights.size();
	}
	const Material& Scene::material( uint32 i ) const {
		return _materials[i];
	}
	Material& Scene::material( uint32 i ) {
		return _materials[i];	
	}
	const Light& Scene::light( uint32 i ) const {
		return *(_lights[i]);
	}
	Light& Scene::light( uint32 i ) {
		return *(_lights[i]);	
	}
	
}