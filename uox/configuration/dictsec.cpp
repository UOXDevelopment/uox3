//Copyright © 2022 Charles Kerr. All rights reserved.

#include "dictsec.hpp"

#include "strutil.hpp"


//====================================================================================================
auto dictsec::size() const ->size_t {
	return contents.size();
}
//====================================================================================================
auto dictsec::clear() ->void {
	contents.clear();
}
//====================================================================================================
auto dictsec::operator[](int index) const ->const std::string& {
	static const auto empty = std::string() ;
	try {
		return contents.at(index) ;
	}
	catch(...) {
		return empty ;
	}
}
//====================================================================================================
auto dictsec::operator[](int index)  -> std::string& {
	static auto empty = std::string() ;
	try {
		return contents.at(index) ;
	}
	catch(...) {
		return empty ;
	}

}

//====================================================================================================
auto dictsec::add(const std::string &key, const std::string &value) ->void{
	auto index = strutil::ston<int>(key) ;
	contents.insert_or_assign(index, value);
}

//====================================================================================================
dictsec::dictsec(const std::string &lang):language(lang) {
	
}
