//Copyright © 2022 Charles Kerr. All rights reserved.

#include "baseobject.hpp"

#include <iostream>

//====================================================================================================
baseobject::baseobject(serial_t serial ){
	this->type = objtype_t::base ;
	this->serial = serial ;
}
