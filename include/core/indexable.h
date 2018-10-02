#ifndef __CORE_INDEXABLE_H__
#define __CORE_INDEXABLE_H__

#include "base/types.h"

namespace core {
	
	template<typename T>
	struct indexable_t {
		virtual uint32 size() const = 0;
		virtual uint32 last() const = 0;
		virtual T& operator[] ( uint32 ) = 0;
		virtual const T& operator[] ( uint32 ) const = 0;
	};
	
}

#endif//__CORE_INDEXABLE_H__