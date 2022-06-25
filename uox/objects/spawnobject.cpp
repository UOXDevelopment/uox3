//Copyright © 2022 Charles Kerr. All rights reserved.

#include "spawnobject.hpp"

#include <iostream>

//====================================================================================================
spawnobject_t::spawnobject_t() : baseobject_t(){
	type = objtype_t::spawn;
}
