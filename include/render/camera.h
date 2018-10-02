#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <thread>
#include <atomic>
#include "core/contiguous.h"
#include "core/quaternion.h"
#include "render/objects.h"
#include "render/ray.h"

namespace render {
	
	// Camera class defines a collection of rays and associated pixels
	struct Camera {
		
		// globally accessible base vectors for coordinate system
		static core::fvec globalForward;
		static core::fvec globalUp;
		static core::fvec globalRight;
		
		Camera( uint32 threads, uint32 bounces );
		Camera( uint32 threads, uint32 bounces, World& );
		Camera( const Camera& );
		~Camera();
		
		void setWorld( World& );
		
		const core::fvec& getPosition() const;
		core::fvec getForward() const;
		core::fvec getUp() const;
		core::fvec getRight() const;
		
		void move( const core::fvec& );
		void setPosition( const core::fvec& );
		
		void rotate( const core::quaternion_t& );
		void setRotation( const core::quaternion_t& );
		
		void setFOV( float32 );
		void setClearColor( uint32 );
		
		void show( uint32 );
		void hide( uint32 );
		void toggle( uint32 );
		
		// configure the rays based on the camera's current rotation
		void configure();
		
		// export image of scene into array of uint32 pixels
		void rasterize( const Scene&, uint32* );
	
	private:
		
		// construct the rays
		void __build_rays();
		
		// align rays with camera
		void __rotate_rays();
		
		// rotate all rays by set amount
		void __rotate_rays( const core::quaternion_t& );
		
		// construct threads for ray tracing
		void __build_threads();
		
		// call this function in each thread
		void __render_stream( uint32 offset,  // start ray
							  uint32 size,    // how many rays to render
							  uint32 index ); // which thread am I?
		
		core::fvec _clearColor;
		core::fvec _position;
		
		uint32 _threadCount; // how many threads
		
		std::atomic<bool> _alive; // signal to threads to keep going
		std::atomic<uint32>* _signals; // for synchronization
		std::atomic<bool>* _dontshow; // whether or not to render on a specific thread
		
		std::thread* _threads; // here are the threads
		
		uint32 _localcount; // number of rays per thread
		
		const Scene* _target; // what scene is being rendered
		uint32* _pixels; // location of the pixel data
		
		World* _parent;
		float32 _fov;
		uint32 _bounces; // max number of allowed bounces
		uint32 _iraycount; // number of initial rays
		uint32 _raycount; // total number of rays (including all possible bounces)
		
		void* _rayalloc; // need this because __m128 must be aligned to the nearest 16 bytes
						 // and the heap allocator only aligns to the nearest 8 bytes
		Ray* _raylist; // location in _rayalloc that is the actual start of the Rays
		
		core::quaternion_t _direction;
	};
	
}

#endif//__CAMERA_H__