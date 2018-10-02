#include "base/time.h"
#include "core/fvector.h"
#include "core/convert.h"
#include "core/methods.h"
#include "render/world.h"
#include "render/camera.h"
#include "render/light.h"
#include "render/scene.h"
#include "render/sphere.h"
#include "render/plane.h"

int main( int argc, char** argv ) {
	
	int w, h, thr, boun;
	
	std::cout << "Dimensions:" << std::endl;
	std::cin >> w >> h;
	std::cout << "# of Threads:" << std::endl;
	std::cin >> thr;
	std::cout << "# of Bounces:" << std::endl;
	std::cin >> boun;
	
	// hacky user input stuff
	bool keyboard[1024];
	bool mouseButtons[8];
	core::vec2i mousePos;
	
	for( int i=0;i<8;++i )
		mouseButtons[i] = false;
	for( int i=0;i<1024;++i )
		keyboard[i] = false;
	
	// Build World
	render::World world( w, h, "Raytracer" );
	render::Scene scene[3];
	render::Camera camera( thr, boun, world );
	
	// Create Materials
	render::Material customMat1( {0,0,1,1} );
	customMat1.setAmbient( 0.2f );
	customMat1.setDiffuse( 0.4f );
	customMat1.setReflection( 0.4f );
	
	render::Material customMat2( {1,0,1,1} );
	customMat2.setAmbient( 0.1f );
	customMat2.setDiffuse( 0.3f );
	customMat2.setTransparency( 0.6f );
	customMat2.setIOR( 1.3333f );
	
	render::Material customMat3( {0.2f,0.9f,0.1f,1} );
	customMat3.setAmbient( 0.2f );
	customMat3.setDiffuse( 0.8f );
	
	render::Material customMat4( {0.5f,0.5f,0,1} );
	customMat4.setAmbient( 0.1f );
	customMat4.setDiffuse( 0.2f );
	customMat4.setReflection( 0.35f );
	customMat4.setTransparency( 0.35f );
	customMat4.setIOR( 2.1f );
	
	render::Material customFloor( {1,1,1,1} );
	customFloor.setAmbient( 0.3f );
	customFloor.setDiffuse( 0.7f );
	
	render::Material pureGlass( {0.5,0,0.2,0} );
	pureGlass.setAmbient( 0.1f );
	pureGlass.setDiffuse( 0.1f );
	pureGlass.setTransparency( 0.8f );
	pureGlass.setIOR( 1.5f );
	
	render::Material magicMaterial( {1,1,1,1}, scene[0] );
	magicMaterial.setAmbient( 0.1f );
	magicMaterial.setTransparency( 0.9f );
	magicMaterial.setIOR( 1.0f );
	
	render::Material customFloor2( {0,0,1,1} );
	customFloor2.setAmbient( 0.2f );
	customFloor2.setDiffuse( 0.2f );
	customFloor2.setReflection( 0.6f );
	
	render::Material customFloor3( {0,0,1,1} );
	customFloor3.setAmbient( 0.2f );
	customFloor3.setDiffuse( 0.8f );
	
	render::Material mats[] = {
		customMat2,
		customMat1,
		customMat3,
		customMat4,
		customFloor,
		pureGlass,
		magicMaterial,
		customFloor2,
		customFloor3
	};
	
	// Scene 0
	float32 lt1height = -6.0f;
	float32 lt2height = -12.0f;
	float32 lt1range = 6.0f;
	float32 lt2range = 12.0f;
	core::fvec lt1orbit(  3, 0, 3 );
	core::fvec lt2orbit( -3, 0,-3 );
	
	render::PointLight lt1( {1,1,1,1}, {0,0,0}, 32.0f );
	render::PointLight lt2( {1,1,1,1}, {0,0,0}, 32.0f );
	
	render::Sphere sp1( { -6, 0, 0 }, 1.5f, 0 );
	render::Sphere sp2( {  0,-1, 3 }, 3, 1 );
	render::Sphere sp3( {  4, 1, 0 }, 1, 2 );
	render::Sphere sp4( {  2, -3, 8 }, 2, 3 );
	
	render::Plane p1( {  0,  4,  0}, { 0,-1, 0}, 4 );
	
	// Scene 1
	render::PointLight lt3( {1,0,1,1}, {-8,0,0}, 32.0f );
	render::PointLight lt4( {0,1,1,1}, { 8,0,0}, 32.0f );
	
	core::fvec vec5( -6,-2,-6 );
	core::fvec vec6( -6,-2, 6 );
	core::fvec vec7( -6, 2,-6 );
	core::fvec vec8( -6, 2, 6 );
	core::fvec vec9(  6,-2,-6 );
	core::fvec vecA(  6,-2, 6 );
	core::fvec vecB(  6, 2,-6 );
	core::fvec vecC(  6, 2, 6 );
	
	render::Sphere sp5( {0,0,0}, 2, 2 );
	render::Sphere sp6( {0,0,0}, 2, 2 );
	render::Sphere sp7( {0,0,0}, 2, 2 );
	render::Sphere sp8( {0,0,0}, 2, 2 );
	render::Sphere sp9( {0,0,0}, 2, 2 );
	render::Sphere spA( {0,0,0}, 2, 2 );
	render::Sphere spB( {0,0,0}, 2, 2 );
	render::Sphere spC( {0,0,0}, 2, 2 );
	
	render::Sphere spD( {0,0,0}, 4, 5 );
	
	render::Plane p2( {  0,  4,  0}, { 0,-1, 0}, 7 );
	
	// Scene 2
	render::PointLight lt5( {1,0,1,1}, {0,-12,0}, 64.0f );
	
	render::Sphere spE( {0,0,-32}, 4, 6 );
	render::Sphere spF( {0,0,-48}, 3, 1 );
	
	render::Plane p3( {  0,  6,  0}, { 0,-1, 0}, 8 );
	
	// Build Scenes
	scene[0].set( 9, mats );
	scene[1].set( 9, mats );
	scene[2].set( 9, mats );
	
	scene[0].add( lt1 );
	scene[0].add( lt2 );
	scene[0].add( sp1 );
	scene[0].add( sp2 );
	scene[0].add( sp3 );
	scene[0].add( sp4 );
	scene[0].add( p1 );
	
	scene[1].add( lt3 );
	scene[1].add( lt4 );
	scene[1].add( sp5 );
	scene[1].add( sp6 );
	scene[1].add( sp7 );
	scene[1].add( sp8 );
	scene[1].add( sp9 );
	scene[1].add( spA );
	scene[1].add( spB );
	scene[1].add( spC );
	scene[1].add( spD );
	scene[2].add( p2 );
	
	scene[2].add( lt5 );
	scene[2].add( spE );
	scene[2].add( spF );
	scene[2].add( p3 );
	
	// Place Camera
	camera.setPosition( {0,0,-16} );
	camera.setClearColor( 0x000000FF );
	
	double t = Time.time();
	double run = 0.0f; // current time from the start of the main loop
	bool running = true;
	uint32 sceneIndex = 0;
	
	while(running) {
		
		// get user input
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT:
					running = false;
					break;
				
				case SDL_MOUSEMOTION:
					mousePos.x = e.motion.x;
					mousePos.y = e.motion.y;
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouseButtons[e.button.button] = true;
					break;
				case SDL_MOUSEBUTTONUP:
					mouseButtons[e.button.button] = false;
					break;
					
				case SDL_KEYDOWN:
					switch( e.key.keysym.sym ) {
						case SDLK_ESCAPE: running = false; break;
						case SDLK_F1: camera.toggle( 0 ); break;
						case SDLK_F2: camera.toggle( 1 ); break;
						case SDLK_F3: camera.toggle( 2 ); break;
						case SDLK_F4: camera.toggle( 3 ); break;
						case SDLK_F5: camera.toggle( 4 ); break;
						case SDLK_F6: camera.toggle( 5 ); break;
						case SDLK_F7: camera.toggle( 6 ); break;
						case SDLK_F8: camera.toggle( 7 ); break;
						case SDLK_SPACE: ++ sceneIndex %= 3; break;
					}
					keyboard[e.key.keysym.scancode] = true;
					break;
				case SDL_KEYUP:
					keyboard[e.key.keysym.scancode] = false;
					break;
			}
		}
		
		// handle user input
		if ( keyboard[SDL_SCANCODE_RIGHT] ) {
			core::axisangle_t aa( render::Camera::globalUp, 0.035f );
			camera.rotate( core::toQuaternion( aa ) );
		}
		if ( keyboard[SDL_SCANCODE_LEFT] ) {
			core::axisangle_t aa( -render::Camera::globalUp, 0.035f );
			camera.rotate( core::toQuaternion( aa ) );
		}
		
		float slow = 8.0f;
		if ( keyboard[SDL_SCANCODE_W] )
			camera.move( camera.getForward() / slow );
		if ( keyboard[SDL_SCANCODE_S] )
			camera.move( -camera.getForward() / slow );
		if ( keyboard[SDL_SCANCODE_D] )
			camera.move( camera.getRight() / slow );
		if ( keyboard[SDL_SCANCODE_A] )
			camera.move( -camera.getRight() / slow );
		if ( keyboard[SDL_SCANCODE_UP] )
			camera.move( -camera.getUp() / slow );
		if ( keyboard[SDL_SCANCODE_DOWN] )
			camera.move( camera.getUp() / slow );
		
		// programmatically manipulate scene
		double t2 = Time.time();
		run += t2 - t;
		t = t2;
		
		if ( sceneIndex == 1 ) {
			sp5.center() = core::fvec( vec5.x() * std::cos(run), vec5.y(), vec5.z() * std::sin(run) );
			sp6.center() = core::fvec( vec6.x() * std::sin(run), vec6.y(), vec6.z() * std::cos(run) );
			sp7.center() = core::fvec( vec7.x() * std::sin(run), vec7.y(), vec7.z() * std::cos(run) );
			sp8.center() = core::fvec( vec8.x() * std::cos(run), vec8.y(), vec8.z() * std::sin(run) );
			sp9.center() = core::fvec( vec9.x() * std::sin(run), vec9.y(), vec9.z() * std::cos(run) );
			spA.center() = core::fvec( vecA.x() * std::cos(run), vecA.y(), vecA.z() * std::sin(run) );
			spB.center() = core::fvec( vecB.x() * std::cos(run), vecB.y(), vecB.z() * std::sin(run) );
			spC.center() = core::fvec( vecC.x() * std::sin(run), vecC.y(), vecC.z() * std::cos(run) );
		} else {
			lt1.setCenter( lt1orbit + core::fvec( lt1range * std::sin(run), lt1height + std::cos(run), lt1range * std::cos(run) ) );
			lt2.setCenter( lt2orbit + core::fvec( lt2range * std::cos(run), lt2height + std::sin(run), lt2range * std::sin(run) ) );
		}
		
		// RENDER DAH WHOLE WIDE WORLD
		world.rasterize( camera, scene[sceneIndex] );
	}
	
	// return 0;
	return 0;
	// return 0;
}