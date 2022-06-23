//

#ifndef tiledata_hpp
#define tiledata_hpp

#include "tileinfo.hpp"

#include <cstdint>
#include <vector>
#include <filesystem>

//=========================================================
//===============================================================================================
// tiledata
//===============================================================================================
//===============================================================================================
class tiledata {
	constexpr static auto hssize = 3188736;
	std::vector<CLand > terrain_data ;
	std::vector<CTile> art_data;
	auto processTerrain(std::ifstream &input) ->void ;
	auto processArt(std::ifstream &input) ->void ;
	bool isHS_format ;
public:
	tiledata(const std::string &filename = "");
	auto load(const std::string &filename) ->bool ;
	auto load(const std::filesystem::path &filepath) ->bool ;
	auto terrain(std::uint16_t tileid) const ->const CLand &;
	auto terrain(std::uint16_t tileid) ->CLand & ;
	auto art(std::uint16_t tileid) ->CTile &;
	auto art(std::uint16_t tileid) const ->const CTile & ;
	
	auto sizeTerrain() const -> size_t ;
	auto sizeArt() const -> size_t ;
	
	auto collectionTerrain() const -> const std::vector<CLand>&;
	auto collectionTerrain() ->std::vector<CLand> &;
	auto collectionArt() const -> const std::vector<CTile>& ;
	auto collectionArt() ->std::vector<CTile>& ;
	auto totalMemory() const ->size_t ;
	
};

#endif /* tiledata_hpp */
