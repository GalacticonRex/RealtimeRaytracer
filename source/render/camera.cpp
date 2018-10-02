#include "base/time.h"
#include "core/methods.h"
#include "core/convert.h"
#include "render/camera.h"
#include "render/scene.h"
#include "render/world.h"
#include "render/material.h"
#include "render/renderable.h"

namespace render {
	
	core::fvec Camera::globalForward(0,0,1);
	core::fvec Camera::globalUp(0,1,0);
	core::fvec Camera::globalRight(1,0,0);
	
	void Camera::__build_rays() {
		_iraycount = _parent->height() * _parent->width();
		
		_raycount = _iraycount * _bounces;
		_rayalloc = operator new[] (_raycount*sizeof(Ray)+15);
		_raylist = (Ray*)( (uint8*)_rayalloc + (uint32)((uintptr_t)_rayalloc%16) );
		
		Ray* r = _raylist;
		Ray* e = _raylist + _raycount;
		while (r != e) {
			new (r) Ray();
			++ r;
			for (uint32 j=1;j<_bounces;++j, ++r) {
				new (r) Ray();
				(r-1)->setFollow( r );
			}
		}
		
		__rotate_rays();
		__build_threads();
	}
	void Camera::__build_threads() {
		_alive = true;
		
		_threads = new std::thread[_threadCount];
		_signals = new std::atomic<uint32>[_threadCount];
		_dontshow = new std::atomic<bool>[_threadCount + 1];
		
		std::cerr << "Total rays: " << _iraycount << std::endl;
		
		uint32 sz = _iraycount / (_threadCount+1);
		uint32 s = sz;
		
		_dontshow[0] = false;
		for (uint32 i=0;i<_threadCount;++i ) {
			// for signals:
			// 2 means that the controlling thread has no data
			// 1 means that the controlling thread has given the green light, but the 
			// 		receiving thread has not yet start
			// 0 mean that the receiving thread is currently running
			
			_dontshow[i+1] = false;
			_signals[i] = 2;
			_threads[i] = std::thread(
				&Camera::__render_stream,
				this,
				s,
				std::min( sz, _iraycount - s ),
				i );
			
			s += sz;
		}
		
		_localcount = sz;
	}
	
	Camera::Camera( uint32 threads, uint32 bounces ) {
		_parent = NULL;
		_rayalloc = NULL;
		_clearColor = core::fvec( 0, 0, 0, 1 );
		_fov = core::deg2rad(70);
		_bounces = bounces+1;
		_threadCount = threads - 1;
	}
	Camera::Camera( uint32 threads, uint32 bounces, World& par ) {
		_parent = &par ;
		_rayalloc = NULL;
		_clearColor = core::fvec( 0, 0, 0, 1 );
		_fov = core::deg2rad(70);
		_bounces = bounces+1;
		_threadCount = threads - 1;
		
		__build_rays();
		__rotate_rays();
	}
	Camera::~Camera() {
		_alive = false;
		for ( uint32 i=0;i<_threadCount;++i )
			_threads[i].join();
		delete[] _signals;
		delete[] _threads;
		
		Ray* r = _raylist;
		Ray* e = _raylist + _raycount;
		for (;r != e;++r) r->~Ray();
		operator delete[]( _rayalloc );
	}
	
	void Camera::setWorld( World& par ) {
		_parent = &par;
		if ( _rayalloc != NULL ) {
			for ( uint32 i=0;i<_raycount;++i )
				_raylist[i].~Ray();
			operator delete[]( _rayalloc );
		}
		
		__build_rays();
		__rotate_rays();
	}
	
	const core::fvec& Camera::getPosition() const {
		return _position;
	}
	core::fvec Camera::getForward() const {
		return _direction * globalForward;
	}
	core::fvec Camera::getUp() const {
		return _direction * globalUp;
	}
	core::fvec Camera::getRight() const {
		return _direction * globalRight;
	}
	
	void Camera::move( const core::fvec& npos ) {
		_position += npos;
	}
	void Camera::setPosition( const core::fvec& npos ) {
		_position = npos;
	}

	void Camera::rotate( const core::quaternion_t& nrot ) {
		_direction = core::Integrate( _direction, nrot );
		__rotate_rays( nrot );
	}
	void Camera::setRotation( const core::quaternion_t& nrot ) {
		_direction = nrot;
	}

	void Camera::setClearColor( uint32 cc ) {
		_clearColor.data[0] = ( float32( (cc >> 24) & 0xFF ) / 255.0f );
		_clearColor.data[1] = ( float32( (cc >> 16) & 0xFF ) / 255.0f );
		_clearColor.data[2] = ( float32( (cc >>  8) & 0xFF ) / 255.0f );
		_clearColor.data[3] = ( float32( (cc >>  0) & 0xFF ) / 255.0f );
	}
	void Camera::setFOV( float fov ) {
		_fov = fov;
	}
	
	void Camera::show( uint32 i ) {
		_dontshow[i] = false;
	}
	void Camera::hide( uint32 i ) {
		_dontshow[i] = true;
	}
	void Camera::toggle( uint32 i ) {
		_dontshow[i] = !_dontshow[i];
	}
	
	void Camera::configure() {
		__rotate_rays();
	}

	void Camera::rasterize(const Scene& objects, uint32* pixels) {
		_target = &objects;
		_pixels = pixels;
		for ( uint32 i=0;i<_threadCount;++i )
			_signals[i] = 1;
		
		if ( !_dontshow[0] ) {
			Ray* r = _raylist;
			for ( uint32 i=0;i<_localcount;++i, r+=_bounces ) {
				core::fvec output = _clearColor;
				r->cast( output, *_target );
				_pixels[i]= 0xFF000000 |
							((uint8)( output.x() * 255.0f ) << 16 ) |
							((uint8)( output.y() * 255.0f ) << 8 ) |
							((uint8)( output.z() * 255.0f ));
			}
		} else {
			for ( uint32 i=0;i<_localcount;++i )
				_pixels[i] = 0xFF000000;
		}
		
		for ( uint32 i=0;i<_threadCount;++i ) {
			while (_signals[i] == 1);
			_signals[i] = 2;
		}
	}
	
	void Camera::__render_stream( uint32 offset, uint32 size, uint32 index ) {
		std::cerr << index << " : offset " << offset << ", size " << size << std::endl;
		uint32 ss = offset + size;
		Ray* slot = _raylist + offset * _bounces;
		while ( _alive ) {
			while ( _alive && _signals[index] == 2 );
			if ( _alive ) {
				if ( !_dontshow[index+1] ) {
					Ray* r = slot;
					for ( uint32 i=offset;i<ss;++i, r+=_bounces ) {
						core::fvec output = _clearColor;
						r->cast( output, *_target );
						_pixels[i]= 0xFF000000 |
									((uint8)( output.x() * 255.0f ) << 16 ) |
									((uint8)( output.y() * 255.0f ) << 8 ) |
									((uint8)( output.z() * 255.0f ));
					}
				} 	else {
					for ( uint32 i=offset;i<ss;++i )
						_pixels[i] = 0xFF000000;
				}
				
				_signals[index] = 0;
				while ( _signals[index] == 0 );
			}
		}
	}
	
	void Camera::__rotate_rays() {
		Ray* r = _raylist;
		Ray* termin = r + _raycount;
		
		core::fvec forward = _direction * globalForward;
		core::fvec yaxis = _direction * globalUp * std::tan(_fov / 2.0f);
		core::fvec xaxis = _direction * globalRight * std::tan(_fov / 2.0f);
		
		core::fvec onAxis = forward - xaxis / 2.0f - yaxis / 2.0f;
		core::fvec current = onAxis;
		
		xaxis /= (float)(_parent->width());
		yaxis /= (float)(_parent->height());
		
		uint32 onX = 0;
		for ( ; r != termin; r+=_bounces ) {
			r->setPosition( _position );
			r->setDirection( core::fvec::normalize(current) );
			++ onX;
			if ( onX == _parent->width() ) {
				onAxis += yaxis;
				current = onAxis;
				onX = 0;
			} else {
				current += xaxis;
			}
		}
	}
	void Camera::__rotate_rays( const core::quaternion_t& q ) {
		Ray* r = _raylist;
		for ( uint32 i=0;i<_iraycount;++i, r+=_bounces )
			r->rotate( q );
	}
	
}