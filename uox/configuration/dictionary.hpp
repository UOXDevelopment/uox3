//Copyright © 2022 Charles Kerr. All rights reserved.

#ifndef dictionary_hpp
#define dictionary_hpp

#include <cstdint>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "secparser.hpp"
#include "dictsec.hpp"

//====================================================================================================
enum DistinctLanguage {
	DL_DEFAULT = 0,
	DL_ENGLISH,
	DL_GERMAN,
	DL_SPANISH,
	DL_FRENCH,
	DL_PORTUGUESE,
	DL_ITALIAN,
	DL_CZECH,
	DL_POLISH,
	DL_RUSSIAN,
	DL_JAPANESE,
	DL_COUNT
};

enum UnicodeTypes {
	ZERO = 0,
	UT_ARA,
	UT_ARI,
	UT_ARE,
	UT_ARL,
	UT_ARG,
	UT_ARM,
	UT_ART,
	UT_ARO,
	UT_ARY,
	UT_ARS,
	UT_ARJ,
	UT_ARB,
	UT_ARK,
	UT_ARU,
	UT_ARH,
	UT_ARQ,
	UT_BGR,
	UT_CAT,
	UT_CHT,
	UT_CHS,
	UT_ZHH,
	UT_ZHI,
	UT_ZHM,
	UT_CSY,
	UT_DAN,
	UT_DEU,
	UT_DES,
	UT_DEA,
	UT_DEL,
	UT_DEC,
	UT_ELL,
	UT_ENU,
	UT_ENG,
	UT_ENA,
	UT_ENC,
	UT_ENZ,
	UT_ENI,
	UT_ENS,
	UT_ENJ,
	UT_ENB,
	UT_ENL,
	UT_ENT,
	UT_ENW,
	UT_ENP,
	UT_ESP,
	UT_ESM,
	UT_ESN,
	UT_ESG,
	UT_ESC,
	UT_ESA,
	UT_ESD,
	UT_ESV,
	UT_ESO,
	UT_ESR,
	UT_ESS,
	UT_ESF,
	UT_ESL,
	UT_ESY,
	UT_ESZ,
	UT_ESB,
	UT_ESE,
	UT_ESH,
	UT_ESI,
	UT_ESU,
	UT_FIN,
	UT_FRA,
	UT_FRB,
	UT_FRC,
	UT_FRS,
	UT_FRL,
	UT_FRM,
	UT_HEB,
	UT_HUN,
	UT_ISL,
	UT_ITA,
	UT_ITS,
	UT_JPN,
	UT_KOR,
	UT_NLD,
	UT_NLB,
	UT_NOR,
	UT_NON,
	UT_PLK,
	UT_PTB,
	UT_PTG,
	UT_ROM,
	UT_RUS,
	UT_HRV,
	UT_SRL,
	UT_SRB,
	UT_SKY,
	UT_SQI,
	UT_SVE,
	UT_SVF,
	UT_THA,
	UT_TRK,
	UT_URP,
	UT_IND,
	UT_UKR,
	UT_BEL,
	UT_SLV,
	UT_ETI,
	UT_LVI,
	UT_LTH,
	UT_LTC,
	UT_FAR,
	UT_VIT,
	UT_HYE,
	UT_AZE,
	UT_EUQ,
	UT_MKI,
	UT_AFK,
	UT_KAT,
	UT_FOS,
	UT_HIN,
	UT_MSL,
	UT_MSB,
	UT_KAZ,
	UT_SWK,
	UT_UZB,
	UT_TAT,
	UT_BEN,
	UT_PAN,
	UT_GUJ,
	UT_ORI,
	UT_TAM,
	UT_TEL,
	UT_KAN,
	UT_MAL,
	UT_ASM,
	UT_MAR,
	UT_SAN,
	UT_KOK,
	TOTAL_LANGUAGES
};



//====================================================================================================
class dictionary : public secparser {
	// virtual overriden functions;
	auto startSection(const std::string &secline)->void final ;
	auto keyvalue(const std::string &key, const std::string &value)->void final;
	auto endSection() ->void final;
	dictsec currentdictionary ;
	// language lookup
	static const std::unordered_map<std::string, DistinctLanguage> stringtolanguage;
	static const std::unordered_map<UnicodeTypes, DistinctLanguage> unicode_to_language;
	static auto getLanguage(UnicodeTypes code) ->DistinctLanguage ;
	DistinctLanguage default_language ;
public:
	static auto getLanguage(const std::string &lang)->DistinctLanguage ;
	std::unordered_map<DistinctLanguage, dictsec> contents ;
	auto setDefault(DistinctLanguage lang) ->void ;
	
	dictionary(const std::filesystem::path &basepath = std::filesystem::path());
	auto load(const std::filesystem::path &basepath)->bool ;
	auto size() const ->size_t ;
	auto clear() ->void ;
	auto operator[](DistinctLanguage lang) const ->const dictsec& ;
	auto operator[](DistinctLanguage lang) ->dictsec& ;

	auto entry(int index, const std::string &lang ) const ->const std::string & ;
	auto entry(int index, DistinctLanguage lang) const ->const std::string & ;
	auto entry(int index, UnicodeTypes type) const ->const std::string & ;
};
#endif /* dictionary_hpp */
