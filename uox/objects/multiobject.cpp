//Copyright © 2022 Charles Kerr. All rights reserved.

#include "multiobject.hpp"

#include <iostream>

//====================================================================================================
multiobject_t::multiobject_t() : baseobject_t(){
	type = objtype_t::multi;
}
