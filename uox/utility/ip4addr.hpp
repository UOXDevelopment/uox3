//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef ip4addr_hpp
#define ip4addr_hpp
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <utility>

//=================================================================================
// ip4addr_t
//=================================================================================
struct ip4addr_t {
private:
	static const std::vector<ip4addr_t> lanips ;
	static const std::vector<ip4addr_t> localips ;
	static const std::vector<ip4addr_t> apipaips ;
public:
	enum ip4type_t {invalid,local,lan,apipa,wan};
	std::array<std::string,4> components ;
	
	ip4addr_t(std::uint32_t addr,bool bigendian = true);
	ip4addr_t(const std::string &value = "");
	
	auto ipaddr(bool bigendian = true) const ->std::uint32_t ;
	
	auto exact(const ip4addr_t& value) const ->bool ;
	
	auto match(std::uint32_t value, bool bigendian = true ) const -> int ;
	auto match(const ip4addr_t &value) const ->int;
	
	auto operator==(const ip4addr_t &value) const ->bool ;
	auto operator==(std::uint32_t value) const ->bool ;
	auto description() const ->std::string ;
	auto type() const ->ip4type_t ;
	
};

//=================================================================================
// ip4list_t
//=================================================================================
struct ip4list_t {
	std::vector<ip4addr_t> ipaddresses ;
	
	static auto available() ->ip4list_t;
	
	ip4list_t(const std::string &filename="") ;
	auto load(const std::string &filename) ->bool ;
	
	auto add(const ip4addr_t &value) ->void ;
	auto remove(const ip4addr_t &value) ->void ;
	
	auto size() const ->size_t ;
	auto bestmatch(std::uint32_t value, bool bigendian = true ) const ->std::pair<ip4addr_t,int> ;
	auto bestmatch(const ip4addr_t &value) const ->std::pair<ip4addr_t,int> ;
	
	auto ips() const ->const std::vector<ip4addr_t>& ;
	auto ips() -> std::vector<ip4addr_t>&;
};



#endif /* Iip4addr_hpp */
