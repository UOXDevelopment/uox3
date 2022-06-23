//

#include "tiledata.hpp"

#include <fstream>

//===============================================================================================
// tiledata
//===============================================================================================
//===============================================================================================

//===============================================================================================
tiledata::tiledata(const std::string &filename):isHS_format(false){
	if (!filename.empty()){
		this->load(filename) ;
	}
}
//===============================================================================================
auto tiledata::load(const std::string &filename) ->bool {
	auto path = std::filesystem::path(filename) ;
	return load(path) ;
}
//===============================================================================================
auto tiledata::load(const std::filesystem::path &filepath) ->bool {
	auto rvalue = false ;
	art_data.clear() ;
	terrain_data.clear() ;
	isHS_format =false ;
	if (std::filesystem::exists(filepath)) {
		if (std::filesystem::file_size(filepath)>= hssize) {
			isHS_format = true ;;
		}
		auto input = std::ifstream(filepath.string(),std::ios::binary) ;
		if (input.is_open()){
			rvalue = true ;
			processTerrain(input) ;
			processArt(input) ;
		}
	}
	return rvalue ;

}

//===============================================================================================
auto tiledata::terrain(std::uint16_t tileid) const -> const CLand& {
	return terrain_data[tileid] ;
}
//===============================================================================================
auto tiledata::terrain(std::uint16_t tileid)  ->  CLand& {
	return terrain_data[tileid] ;
}
//===============================================================================================
auto tiledata::art(std::uint16_t tileid) const -> const CTile& {
	return art_data[tileid] ;
}
//===============================================================================================
auto tiledata::art(std::uint16_t tileid)  ->  CTile& {
	return art_data[tileid] ;
}
//===============================================================================================
auto tiledata::sizeTerrain() const -> size_t {
	return terrain_data.size();
}
//===============================================================================================
auto tiledata::sizeArt() const -> size_t {
	return art_data.size() ;
}
//===============================================================================================
auto tiledata::collectionTerrain() const -> const std::vector<CLand>&{
	return terrain_data;
}
//===============================================================================================
auto tiledata::collectionTerrain() ->std::vector<CLand> &{
	return terrain_data;
}

//===============================================================================================
auto tiledata::collectionArt() const -> const std::vector<CTile>& {
	return  art_data;
}
//===============================================================================================
auto tiledata::collectionArt() ->std::vector<CTile>& {
	return  art_data;
}

//===============================================================================================
auto tiledata::processTerrain(std::ifstream &input) ->void {
	terrain_data.reserve(0x4000);
	std::uint32_t value32 = 0 ;
	std::uint64_t value64 =  0;
	std::array<char,20> string_buffer ;
	string_buffer.fill(0);
	for (auto i=0 ; i< 0x4000;i++){
		// We have to get rid of the header on blocks of information
		// HS has the firt entry messed up
		if (isHS_format) {
			if ( (((i & 0x1F)==0) && (i>0)) || (i == 1)){
				input.read(reinterpret_cast<char*>(&value32),4); // read off the group header
			}
			
		}
		else {
			if ( (i & 0x1f) == 0) {
				input.read(reinterpret_cast<char*>(&value32),4); // read off the group header
			}
		}
		// now create the info_t we will use
		auto info = CLand();
		// read off the flags, 32 bit on pre-HS, 64 bit after
		if (isHS_format) {
			input.read(reinterpret_cast<char*>(&value64),8);
			info.flags = std::bitset<64>(value64);
			
		}
		
		else{
			input.read(reinterpret_cast<char*>(&value32),4);
			info.flags = std::bitset<64>(value32);
		}
		// only other thing for terrain is the texture
		// and name
		input.read(reinterpret_cast<char*>(&info.textureID),2);
		input.read(string_buffer.data(),20);
		if (input.gcount()==20){
			// We need to find the "0", if any
			auto iter = std::find(string_buffer.begin(),string_buffer.end(),0);
			info.name = std::string(string_buffer.begin(),iter);
			terrain_data.push_back(std::move(info));
			
		}
		else {
			break;
		}
	}
	
}
//===============================================================================================
auto tiledata::processArt(std::ifstream &input) ->void {
	art_data.reserve(0xFFFF);
	std::uint32_t value32 = 0 ;
	std::uint64_t value64 =  0;
	std::array<char,20> string_buffer ;
	string_buffer.fill(0);
	if ( !(input.eof() || input.bad())){
		auto loopcount = 0 ;
		do {
			if ( (loopcount & 0x1f) == 0) {
				input.read(reinterpret_cast<char*>(&value32),4); // read off the group header
			}
			auto info = CTile();
			if (isHS_format) {
				input.read(reinterpret_cast<char*>(&value64),8);
				info.flags = std::bitset<64>(value64);
				
				
			}
			else {
				input.read(reinterpret_cast<char*>(&value32),4);
				info.flags = std::bitset<64>(value32);
				
			}
			input.read(reinterpret_cast<char*>(&info.weight),1);
			input.read(reinterpret_cast<char*>(&info.layer),1);
			//misc data
			input.read(reinterpret_cast<char*>(&info.unknown1),2);
			//Second unkown
			input.read(reinterpret_cast<char*>(&info.unknown2),1);
			// Quantity
			input.read(reinterpret_cast<char*>(&info.quantity),1);
			// Animation
			input.read(reinterpret_cast<char*>(&info.animation),2);
			
			//Third a byte
			input.read(reinterpret_cast<char*>(&info.unknown3),1);
			
			// Hue/Value
			input.read(reinterpret_cast<char*>(&info.hue),1);
			//stacking offset/value  = 16 bits, but UOX3 doesn't know that, so two eight bit unkowns
			input.read(reinterpret_cast<char*>(&info.unknown4),1);
			input.read(reinterpret_cast<char*>(&info.unknown5),1);
			// Height
			input.read(reinterpret_cast<char*>(&info.height),1);
			// and name
			input.read(string_buffer.data(),20);
			if ((input.gcount()==20) && input.good()){
				// We need to find the "0", if any
				auto iter = std::find(string_buffer.begin(),string_buffer.end(),0);
				info.name = std::string(string_buffer.begin(),iter);
				art_data.push_back(std::move(info));
			}
			loopcount++;
			
		}while( (!input.eof()) && input.good());
		
	}
	
}
//===============================================================================================
auto tiledata::totalMemory() const ->size_t {
	auto flag = isHS_format?8:4 ;
	auto terrain = flag + 20 + 2 ;
	auto art = flag + 20 + 23 ;
	return static_cast<size_t>((terrain*terrain_data.size()) + (art * art_data.size()) );
}
