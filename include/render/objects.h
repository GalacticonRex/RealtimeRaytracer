#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#define SDL_MAIN_HANDLED
#include "sdl/sdl.h"

#include "core/vector.h"
#include "core/fvector.h"

namespace render {
	
	typedef core::vec3 Vec3;
	typedef core::vec4 Vec4;
	
	struct World;
	struct Scene;
	struct DepthBuffer;
	struct Camera;
	struct Ray;
	struct Renderable;
	struct Light;
	struct Material;
	struct Sphere;
	struct Plane;
	struct Cube;
	struct Mesh;
	
}

#endif//__OBJECTS_H__