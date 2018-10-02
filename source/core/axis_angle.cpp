#include "core/axis_angle.h"
#include "core/vector.h"

namespace core {

namespace {
	using namespace std;
}

axisangle_t::axisangle_t() {
	axis = fvec(1,0,0);
	angle = 0;
}
axisangle_t::axisangle_t(fvec v,float32 n) {
	axis = fvec::normalize(v);
	angle = n;
}
axisangle_t::axisangle_t(float32 n,fvec v) {
	axis = fvec::normalize(v);
	angle = n;
}
axisangle_t::axisangle_t(fvec a,fvec b)
{
	axis = fvec::normalize( fvec::cross(a,b) );
	angle = fvec::angle( a,b );
}

fvec axisangle_t::simplify() { return axis * angle; }

ostream& operator<< ( ostream& out, const axisangle_t& aa ) {
	out << "axis" << vec3(aa.axis.x(),aa.axis.y(),aa.axis.z()) << " | angle<" << aa.angle << ">";
	return out;
}

}
