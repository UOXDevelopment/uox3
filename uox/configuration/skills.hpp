//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef skills_hpp
#define skills_hpp

#include <cstdint>
#include <string>
//====================================================================================================
// advance_t
//====================================================================================================
//====================================================================================================
struct advance_t {
	std::int32_t base ;
	std::int32_t success ;
	std::int32_t failure ;
	std::int32_t gain ;
	advance_t() ;
};

//====================================================================================================
// skill_t
//====================================================================================================
//====================================================================================================
struct skill_t {
	std::int32_t strength ;
	std::int32_t dexterity ;
	std::int32_t intelligence ;
	std::int32_t delay ;
	std::string verb ;
	std::string name ;
	skill_t() ;
};
#endif /* skills_hpp */
