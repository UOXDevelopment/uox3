//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef baseobject_hpp
#define baseobject_hpp

#include <cstdint>
#include <string>

#include "uoxtype.hpp"

enum class objtype_t{base,item,mob,multi,boat,spawn} ;
//====================================================================================================
struct baseobject {
	objtype_t type ;
	
	serial_t serial ;
	baseobject(serial_t serial = invalid_serial);
};
#endif /* baseobject_hpp */
