//Copyright © 2022 Charles Kerr. All rights reserved.

#include "uoxtype.hpp"

#include "strutil.hpp"

#include <iostream>

using namespace std::string_literals;
//====================================================================================================
// point3_t
//====================================================================================================
//====================================================================================================
auto point3_t::operator==(const point3_t &rhs) const ->bool {
	auto rvalue = (this->x == rhs.x) && (this->y == rhs.y) ;
	if (rvalue){
		if ((this->z != invalid_altitude) && (rhs.z != invalid_altitude)){
			rvalue = this->z == rhs.z;
		}
	}
	return rvalue ;
}
//====================================================================================================
auto point3_t::operator==(const point_t &rhs) const ->bool {
	return (this->x == rhs.x) && (this->y == rhs.y) ;
}
//====================================================================================================
auto point3_t::equal(const point3_t &rhs) const ->bool {
	return (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z);

}

//====================================================================================================
// location_t
//====================================================================================================
//====================================================================================================
location_t::location_t(const std::string &value):location_t(0,0,invalid_altitude,invalid_mapnum,invalid_realm) {
	auto values = strutil::parse(value,",");
	switch (values.size()){
		default:
		case 5:
			this->realm = strutil::ston<coord_t>(values[4]);
			[[fallthrough]];
		case 4:
			this->mapnum = strutil::ston<coord_t>(values[3]);
			[[fallthrough]];
		case 3:
			this->z = strutil::ston<coord_t>(values[2]);
			[[fallthrough]];
		case 2:
			this->y = strutil::ston<coord_t>(values[1]);
			[[fallthrough]];
		case 1:
			this->x = strutil::ston<coord_t>(values[0]);
			[[fallthrough]];
		case 0:
			break;
	}
}
//====================================================================================================
location_t::location_t(coord_t x, coord_t y,coord_t z , coord_t mapnum  , coord_t realm) {
	this->x = x ;
	this->y = y ;
	this->z = z ;
	this->mapnum = mapnum ;
	this->realm  = realm ;
}
//====================================================================================================
auto location_t::operator==(const location_t &rhs) const ->bool {
	auto rvalue = (this->x == rhs.x) && (this->y == rhs.y);
	if (rvalue){
		if ((this->z != invalid_altitude) && (rhs.z != invalid_altitude)){
			rvalue = this->z == rhs.z;
		}
	}
	if (rvalue) {
		if ((this->mapnum != invalid_mapnum) && (rhs.mapnum != invalid_mapnum)){
			rvalue = this->mapnum == rhs.mapnum;
		}

	}
	if (rvalue) {
		if ((this->realm != invalid_realm) && (rhs.realm != invalid_realm)){
			rvalue = this->realm == rhs.realm;
		}
		
	}
	return  rvalue ;
}
//====================================================================================================
auto location_t::equal(const  location_t &rhs) const ->bool {
	return (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z) && (this->realm == rhs.realm) && (this->mapnum == rhs.mapnum);
}
//====================================================================================================
auto location_t::operator==(const  point3_t &rhs) const ->bool {
	return (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z);
}
//====================================================================================================
auto location_t::operator==(const point_t &rhs) const ->bool {
	return (this->x == rhs.x) && (this->y == rhs.y) ;
}
//====================================================================================================
auto location_t::description() const ->std::string {
	auto rvalue = std::to_string(x) + ","s+std::to_string(y) ;
	if ((z!=invalid_altitude) || (realm != invalid_realm) || (mapnum != invalid_mapnum)){
		rvalue +=","s+std::to_string(z) ;
	}
	if ((mapnum!= invalid_mapnum) || (realm != invalid_realm)){
		rvalue +=","s+std::to_string(mapnum) ;
	}
	if ( realm != invalid_realm){
		rvalue +=","s+std::to_string(realm) ;
	}
	return rvalue;
}



//====================================================================================================
// advancement_t
//====================================================================================================
//====================================================================================================
advancement_t::advancement_t(const std::string &value): advancement_t() {
	auto values = strutil::parse(value,",");
	switch (values.size()){
		default:
		case 4:
			gain = strutil::ston<int>(values[3]);
			[[fallthrough]];
		case 3:
			failure = strutil::ston<int>(values[2]);
			[[fallthrough]];
		case 2:
			success = strutil::ston<int>(values[1]);
			[[fallthrough]];
		case 1:
			level = strutil::ston<int>(values[0]);
			[[fallthrough]];
		case 0:
			break;
	}
}
//====================================================================================================
auto advancement_t::save(std::ostream &output) const ->void {
	output <<"\tadvance = " <<level<<","<<failure<<","<<success ;
	if (gain != 1){
		output <<","<<gain ;
	}
	output <<"\n";
}

