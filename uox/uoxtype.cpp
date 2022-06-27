//Copyright © 2022 Charles Kerr. All rights reserved.

#include "uoxtype.hpp"

#include "strutil.hpp"

#include <iostream>

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

