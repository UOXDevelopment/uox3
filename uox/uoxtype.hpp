//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef uoxtype_hpp
#define uoxtype_hpp

#include <cstdint>
#include <cmath>
#include <string>
#include <limits>
#include <ostream>
//====================================================================================================
// These define types we use thoughout the code
// Note, we use int a lot, and expect the translation to occur from uodata/packet.
// We use int, as that is a more natural storage for modern processors
// but we can change it if needed.
using serial_t = std::uint32_t ;
using coord_t = std::int32_t ;

constexpr auto invalid_serial =std::numeric_limits<serial_t>::max();

//=========================================================
using elevation_t = std::int32_t ;
using light_t = std::int32_t ;
using color_t = std::int32_t ;
using tileid_t = std::int32_t ;
using weight_t = std::int32_t ;
using animid_t = std::int32_t ;
using texid_t = std::int32_t ;
using height_t = std::int32_t ;
using flag_t = std::uint64_t ;
using layer_t = std::int32_t ;
using altitude_t = std::int32_t ;
using skillid_t = std::int32_t ;

using coord_t = std::int32_t ;

constexpr auto invalid_tile = 0xFFFF ;
constexpr auto invalid_texture = 0xffff ;

constexpr auto elev_cap = 127 ;
constexpr auto max_weight = 255 ;

//==========================================================================================
// a few other types used throughout the code , perhaps
struct point_t {
	coord_t x;
	coord_t y;
	point_t():x(0),y(0){}
	point_t(coord_t x, coord_t y) : x(x),y(y) {}
};
//==========================================================================================
struct point3_t : public point_t {
	altitude_t z;
	point3_t():point_t(),z(0){}
	point3_t(const point_t &value,coord_t z=0):point_t(value.x,value.y),z(z){}
	point3_t(coord_t x,coord_t y, coord_t z=0):point_t(x,y),z(z) {}
};
//==========================================================================================
struct rpoint3_t {
	float x;
	float y;
	float z ;
	rpoint3_t():x(0.0),y(0.0),z(0.0){}
	rpoint3_t(const point3_t &value):x(static_cast<float>(value.x)),y(static_cast<float>(value.y)),z(static_cast<float>(value.z)){}
	rpoint3_t(coord_t x, coord_t y, coord_t z) :x(static_cast<float>(x)),y(static_cast<float>(y)),z(static_cast<float>(z)){}
	rpoint3_t(const point_t &value):x(static_cast<float>(value.x)),y(static_cast<float>(value.y)),z(0.0){}
	auto magnitude() const ->double { return std::sqrt(magsquare()); }
	auto magsquare() const ->double { return std::pow(x,2) + std::pow(y,2); }
	auto magnitude3() const ->double { return std::sqrt(mag3square()); }
	auto mag3square() const ->double {return std::pow(x,2) + std::pow(y,2)+ std::pow(z,2); }
	auto normalize() ->void { auto temp = magnitude3(); x /= temp; y /= temp; z/=temp;}
	
};
//==========================================================================================
struct location_t : public point3_t {
	coord_t mapnum ;
	coord_t realm ;
	location_t() : point3_t(),mapnum(0),realm(0){}
};

//==========================================================================================
struct srcdeslocation_t {
	location_t source;
	location_t destination ;
};
//==========================================================================================
struct advancement_t {
	int level ;
	int success ;
	int failure;
	int gain ;
	advancement_t():level(0),success(0),failure(0),gain(1) {}
	advancement_t(const std::string &value);
	auto save(std::ostream &output) const ->void ;
};
#endif /* uoxtype_hpp */
