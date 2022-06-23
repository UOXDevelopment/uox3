//Copyright © 2022 Charles Kerr. All rights reserved.

#include "dictionary.hpp"

#include "strutil.hpp"

#include <iostream>
#include <stdexcept>
using namespace std::string_literals;


const std::unordered_map<std::string, DistinctLanguage> dictionary::stringtolanguage {
	{"ZRO"s,DL_DEFAULT},
	{"ENG"s,DL_ENGLISH},
	{"GER"s,DL_GERMAN},
	{"SPA"s,DL_SPANISH},
	{"FRE"s,DL_FRENCH},
	{"PTG"s,DL_PORTUGUESE},
	{"ITA"s,DL_ITALIAN},
	{"CSY"s,DL_CZECH},
	{"POL"s,DL_POLISH},
	{"RUS"s,DL_RUSSIAN},
	{"JPN"s,DL_JAPANESE}
};
const std::unordered_map<UnicodeTypes, DistinctLanguage> dictionary::unicode_to_language{
	{UT_CSY,DL_CZECH},	// Czech Czech Republic
	{UT_DEU,DL_GERMAN},	// German Germany
	{UT_DES,DL_GERMAN},	// German Switzerland
	{UT_DEA,DL_GERMAN},	// German Austria
	{UT_DEL,DL_GERMAN},	// German Luxembourg
	{UT_DEC,DL_GERMAN},	// German Liechtenstein
	{UT_ENU,DL_ENGLISH},	// English United States
	{UT_ENG,DL_ENGLISH},	// English United Kingdom
	{UT_ENA,DL_ENGLISH},	// English Australia
	{UT_ENC,DL_ENGLISH},	// English Canada
	{UT_ENZ,DL_ENGLISH},	// English New Zealand
	{UT_ENI,DL_ENGLISH},	// English Ireland
	{UT_ENS,DL_ENGLISH},	// English South Africa
	{UT_ENJ,DL_ENGLISH},	// English Jamaica
	{UT_ENB,DL_ENGLISH},	// English Caribbean
	{UT_ENL,DL_ENGLISH},	// English Belize
	{UT_ENT,DL_ENGLISH},	// English Trinidad
	{UT_ENW,DL_ENGLISH},	// English Zimbabwe
	{UT_ENP,DL_ENGLISH},	// English Philippines
	{UT_ESP,DL_SPANISH},	// Spanish Spain (Traditional Sort)
	{UT_ESM,DL_SPANISH},	// Spanish Mexico
	{UT_ESN,DL_SPANISH},	// Spanish Spain (International Sort)
	{UT_ESG,DL_SPANISH},	// Spanish Guatemala
	{UT_ESC,DL_SPANISH},	// Spanish Costa Rica
	{UT_ESA,DL_SPANISH},	// Spanish Panama
	{UT_ESD,DL_SPANISH},	// Spanish Dominican Republic
	{UT_ESV,DL_SPANISH},	// Spanish Venezuela
	{UT_ESO,DL_SPANISH},	// Spanish Colombia
	{UT_ESR,DL_SPANISH},	// Spanish Peru
	{UT_ESS,DL_SPANISH},	// Spanish Argentina
	{UT_ESF,DL_SPANISH},	// Spanish Ecuador
	{UT_ESL,DL_SPANISH},	// Spanish Chile
	{UT_ESY,DL_SPANISH},	// Spanish Uruguay
	{UT_ESZ,DL_SPANISH},	// Spanish Paraguay
	{UT_ESB,DL_SPANISH},	// Spanish Bolivia
	{UT_ESE,DL_SPANISH},	// Spanish El Salvador
	{UT_ESH,DL_SPANISH},	// Spanish Honduras
	{UT_ESI,DL_SPANISH},	// Spanish Nicaragua
	{UT_ESU,DL_SPANISH},	// Spanish Puerto Rico
	{UT_FRA,DL_FRENCH},	// French France
	{UT_FRB,DL_FRENCH},	// French Belgium
	{UT_FRC,DL_FRENCH},	// French Canada
	{UT_FRS,DL_FRENCH},	// French Switzerland
	{UT_FRL,DL_FRENCH},	// French Luxembourg
	{UT_FRM,DL_FRENCH},	// French Monaco
	{UT_ITA,DL_ITALIAN},	// Italian Italy
	{UT_ITS,DL_ITALIAN},	// Italian Switzerland
	{UT_JPN,DL_JAPANESE},	// Japanese Japan
	{UT_PLK,DL_POLISH},	// Polish Poland
	{UT_PTB,DL_POLISH},	// Portuguese Brazil
	{UT_PTG,DL_POLISH},	// Portuguese Portugal
};




//====================================================================================================
auto dictionary::startSection(const std::string &secline)->void  {
	auto [sec,lang] = strutil::split(strutil::simplify(secline)," ");
	if (strutil::upper(sec) == "DICTIONARY"){
		currentdictionary = dictsec(lang) ;
	}

}
//====================================================================================================
auto dictionary::keyvalue(const std::string &key, const std::string &value)->void {
	
	currentdictionary.add(key, value);
}
//====================================================================================================
auto dictionary::endSection() ->void {
	auto iter = stringtolanguage.find(strutil::upper(currentdictionary.language));
	if (iter != stringtolanguage.end()){
		contents.insert_or_assign(iter->second, currentdictionary);
	}
	currentdictionary = dictsec();
}

//====================================================================================================
auto dictionary::getLanguage(const std::string &lang)->DistinctLanguage {
	auto iter = stringtolanguage.find(lang) ;
	if (iter != stringtolanguage.end()){
		return iter->second ;
	}
	return DL_DEFAULT ;
}
//====================================================================================================
auto dictionary::getLanguage(UnicodeTypes code) ->DistinctLanguage {
	auto iter = unicode_to_language.find(code) ;
	if (iter != unicode_to_language.end()){
		return iter->second ;
	}
	return DL_DEFAULT ;

}
//====================================================================================================
auto dictionary::setDefault(DistinctLanguage lang) ->void {
	default_language = lang ;
}

//====================================================================================================
dictionary::dictionary(const std::filesystem::path &basepath) {
	default_language = DL_DEFAULT ;
	if (!basepath.empty()){
		if (!load(basepath)){
			throw std::runtime_error("Error loading dictionarys");
		}
	}
}
//====================================================================================================
auto dictionary::load(const std::filesystem::path &basepath)->bool{
	contents.clear() ;
	auto root = basepath / std::filesystem::path("dictionary");
	for (const auto &[lang,code]:stringtolanguage){
		auto path = root / std::filesystem::path("dictionary."s+lang);
		processFile(path);
	}
	return !contents.empty() ;
}
//====================================================================================================
auto dictionary::size() const ->size_t {
	return contents.size();
}
//====================================================================================================
auto dictionary::clear() ->void {
	contents.clear();
}
//====================================================================================================
auto dictionary::operator[](DistinctLanguage lang) const ->const dictsec& {
	return contents.at(lang);
}
//====================================================================================================
auto dictionary::operator[](DistinctLanguage lang) ->dictsec& {
	return contents.at(lang);
}

//====================================================================================================
auto dictionary::entry(int index, const std::string &lang) const ->const std::string & {
	return entry(index,getLanguage(lang));
}
//====================================================================================================
auto dictionary::entry(int index, UnicodeTypes type) const ->const std::string & {
	return entry(index,getLanguage(type));
}
//====================================================================================================
auto dictionary::entry(int index, DistinctLanguage lang) const ->const std::string & {
	static const auto emptystring = std::string() ;
	if (lang == DL_DEFAULT){
		lang = default_language ;
	}
	
	auto iter = contents.find(lang) ;
	if (iter != contents.end()) {
		return (iter->second)[index] ;
	}
	if (lang != DL_DEFAULT) {
		if (lang != default_language){
			lang = default_language ;
			iter = contents.find(lang);
			if (iter != contents.end()) {
				return (iter->second)[index] ;
			}

		}
	}
	return emptystring ;
}
