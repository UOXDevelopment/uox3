//Copyright © 2022 Charles Kerr. All rights reserved.

#include "packet.hpp"

#include <iostream>
#include <array>

//====================================================================================================
using namespace std::string_literals;
//=========================================================
// Constants
//=========================================================
const std::unordered_map<std::uint8_t,std::pair<int,std::string> > packet_t::uopackets{
	// {packet number,{size,name} }  // -1 for size means variable
	// Client Packets
	{0x00,{104,"Create Character"s}} , 			{0x01,{5,"Disconnect Notification"s}} ,
	{0x02,{7,"Move Request"s}} , 				{0x03,{-1,"Talk Request"s}} ,
	{0x04,{2,"Request God Mode (God Client)"s}} , 	{0x05,{5,"Request Attack"s}} ,
	{0x06,{5,"Double Click"s}} , 				{0x07,{7,"Pick Up Items"s}} ,
	{0x08,{15,"Drop Item"s}} , 				{0x09,{5,"Single Click"s}} ,
	{0x0A,{11,"Edit (God Client)"s}} , 			{0x12,{-1,"Request Skill etc use"s}} ,
	{0x13,{10,"Drop/Wear Item"s}} , 			{0x14,{6,"Send Elevation (God Client)"s}} ,
	{0x1E,{4,"Control Animation"s}} , 			{0x34,{10,"Get Player Status"s}} ,
	{0x35,{653,"Add Resource (God Client)"s}} , 	{0x37,{8,"Move Item (God Client)"s}} ,
	{0x38,{7,"Pathfinding in Client"s}} , 		{0x3B,{-1,"Buy Item(s)"s}} ,
	{0x45,{5,"Version OK"s}} , 				{0x46,{-1,"New Artwork"s}} ,
	{0x47,{11,"New Terrain"s}} , 				{0x48,{73,"New Animation"s}} ,
	{0x49,{93,"New Hues"s}} , 				{0x4A,{5,"Delete Art"s}} ,
	{0x4B,{9,"Check Client Version"s}} , 		{0x4C,{-1,"Script Names"s}} ,
	{0x4D,{-1,"Edit Script File"s}} , 			{0x50,{-1,"Board Header"s}} ,
	{0x51,{-1,"Board Message"s}} , 			{0x52,{-1,"Board Post Message"s}} ,
	{0x57,{110,"Update Regions"s}} , 			{0x58,{106,"Add Region"s}} ,
	{0x59,{-1,"New Context FX"s}} , 			{0x5A,{-1,"Update Context FX"s}} ,
	{0x5C,{2,"Restart Version"s}} , 			{0x5D,{73,"Login Character"s}} ,
	{0x5E,{-1,"Server Listing"s}} , 			{0x5F,{49,"Server List Add Entry"s}} ,
	{0x60,{5,"Server List Remove Entry"s}} , 		{0x61,{9,"Remove Static Object"s}} ,
	{0x62,{15,"Move Static Object"s}} , 		{0x63,{13,"Load Area"s}} ,
	{0x64,{15,"Load Area Request"s}} , 			{0x69,{5,"Change Text/Emote Colors"s}} ,
	{0x75,{35,"Rename Character"s}} , 			{0x7D,{13,"Response To Dialog Box"s}} ,
	{0x80,{62,"Login Request"s}} , 			{0x83,{39,"Delete Character"s}} ,
	{0x8D,{146,"Character Creation (KR/SA 3D"s}} , 	{0x91,{65,"Game Server Login"s}} ,
	{0x9B,{258,"Request Help"s}} , 			{0x9F,{-1,"Sell List Reply"s}} ,
	{0xA0,{3,"Select Server"s}} , 			{0xA4,{-1,"Client Spy"s}} ,
	{0xA7,{4,"Request Tip/Notice Window"s}} , 	{0xAC,{-1,"Gump Text Entry Dialog Reply"s}} ,
	{0xAD,{-1,"Unicode/Ascii Speech Request"s}} , 	{0xB1,{-1,"Gump Menu Selection"s}} ,
	{0xB3,{-1,"Chat Text"s}} , 				{0xB5,{64,"Open Chat Window"s}} ,
	{0xB6,{9,"Send Help/Tip Request"s}} , 		{0xC5,{1,"Invalid Map"s}} ,
	{0xD9,{-1,"Spy On Client"s}} , 			{0xE0,{-1,"Bug Report (KR)"s}} ,
	{0xE1,{9,"Client Type (KR/SA)"s}} , 		{0xEC,{-1,"Equip Macro (KR)"s}} ,
	{0xED,{-1,"Unequip Item Macro (KR)"s}} , 		{0xEF,{21,"KR/2D Client Login/Seed"s}} ,
	{0xF8,{106,"Character Creation (7.0.16.0)"s}} ,	{0xFA,{1,"Open UO Store"s}} ,
	{0xFB,{2,"Update View Public House Contents"s}},
	
	// Server packets
	{0x0B,{7,"Damage"s}} ,					{0x11,{-1,"Status Bar Info"s}} ,
	{0x16,{-1,"Health Bar Status Update (SA)"s}},	{0x17,{12,"Health Bar Status Update (KR)"s}} ,
	{0x1A,{-1,"Object Info"s}},				{0x1B,{32,"Char Locale and Body"s}} ,
	{0x1C,{-1,"Send Speech"s}},				{0x1D,{5,"Delete Object"s}} ,
	{0x1F,{8,"Explosion"s}},				{0x20,{19,"Draw Game Player"s}} ,
	{0x21,{8,"Char Move Rejection"s}},			{0x23,{26,"Dragging Of Item"s}} ,
	{0x24,{7,"Draw Container"s}},				{0x25,{21,"Add Item To Container"s}} ,
	{0x26,{5,"Kick Player"s}},				{0x27,{2,"Reject Move Item Request"s}} ,
	{0x28,{5,"Drop Item Failed/Clear Square"s}},	{0x29,{1,"Drop Item Approved"s}} ,
	{0x2A,{5,"Blood"s}},					{0x2B,{2,"God Mode (God Client)"s}} ,
	{0x2D,{17,"MOB Attributes"s}},			{0x2E,{15,"Worn Item"s}} ,
	{0x2F,{10,"Fight Occuring"s}},			{0x30,{5,"Attack OK"s}} ,
	{0x31,{1,"Attack Ended"s}},				{0x32,{2,"Unknown (fixed)"s}} ,
	{0x33,{2,"Pause Client"s}},				{0x36,{-1,"Resource Tile Data (God Client)"s}} ,
	{0x3C,{-1,"Add Multiple Items In Container"s}},	{0x3E,{37,"Versions (God Client)"s}} ,
	{0x3F,{-1,"Update Statics (God Client)"s}},	{0x4E,{6,"Personal Light Level"s}} ,
	{0x4F,{2,"Overall Light Level"s}},			{0x53,{2,"Reject Character Logon"s}} ,
	{0x54,{12,"Play Sound Effect"s}},			{0x55,{1,"Login Complete"s}} ,
	{0x5B,{4,"Time"s}},					{0x65,{4,"Set Weather"s}} ,
	{0x6D,{3,"Play MIDI Music"s}},			{0x6E,{14,"Character Animation"s}} ,
	{0x70,{28,"Graphical Effect"s}},			{0x74,{-1,"Open Buy Window"s}} ,
	{0x76,{16,"New Subserver"s}},				{0x77,{17,"Update Player"s}} ,
	{0x78,{-1,"Draw Object"s}},				{0x7C,{-1,"Open Dialog Box"s}} ,
	{0x82,{2,"Login Denied"s}},				{0x86,{304,"Resend Characters After Delete"s}} ,
	{0x88,{66,"Open Paperdoll"s}},			{0x89,{-1,"Corpse Clothing"s}} ,
	{0x8C,{11,"Connect To Game Server"s}},		{0x90,{19,"Map Message"s}} ,
	{0x97,{2,"Move Player"s}},				{0x9C,{53,"Request Assistance"s}} ,
	{0x9E,{-1,"Sell List"s}},				{0xA1,{9,"Update Current Health"s}} ,
	{0xA2,{9,"Update Current Mana"s}},			{0xA3,{9,"Update Current Stamina"s}} ,
	{0xA5,{-1,"Open Web Browser"s}},			{0xA6,{-1,"Tip/Notice Window"s}} ,
	{0xA8,{-1,"Game Server List"s}},			{0xA9,{-1,"Character/Starting Locations"s}} ,
	{0xAA,{5,"Allow/Refuse Attack"s}},			{0xAB,{-1,"Gump Text Entry Dialog"s}} ,
	{0xAE,{-1,"Unicode Speech Message"s}},		{0xAF,{13,"Display Death Action"s}} ,
	{0xB0,{-1,"Send Gump Menu Dialog"s}},		{0xB2,{-1,"Chat Message"s}} ,
	{0xB7,{-1,"Help/Tip Data"s}},				{0xB9,{5,"Enable Locked Client Features"s}} ,
	{0xBA,{10,"Quest Arrow"s}},				{0xBC,{3,"Seasonal Information"s}} ,
	{0xC0,{36,"Graphical Effect"s}},			{0xC1,{-1,"Cliloc Message"s}} ,
	{0xC4,{6,"Semivisible"s}},				{0xC6,{-1,"Invalid Map Enable"s}} ,
	{0xC7,{49,"3D Particle Effect"s}},			{0xCB,{7,"Global Queue Count"s}} ,
	{0xCC,{-1,"Cliloc Message Affix"s}},		{0xD2,{25,"Extended 0x20"s}} ,
	{0xD3,{-1,"Extended 0x78"s}},				{0xD8,{-1,"Send Custom House"s}} ,
	{0xD8,{-1,"Character Transfer Log"s}},		{0xDC,{9,"SE Introduced Revision"s}} ,
	{0xDD,{-1,"Compressed Gump"s}},			{0xDE,{-1,"Update Mobile Status"s}} ,
	{0xDF,{-1,"Buff/Debuff System"s}},			{0xE2,{10,"New Character Animation"s}} ,
	{0xE2,{77,"KR Encryption Response"s}},		{0xF0,{-1,"Krrios Client Special"s}} ,
	{0xF3,{24,"Object Information (SA)"s}},		{0xF5,{21,"New Map Message"s}} ,
	
	// Bidirectional packets
	{0x0C,{-1,"Edit Tile Data (God Client)"s}},	{0x15,{9,"Follow"s}} ,
	{0x22,{3,"Character Move ACK/Resync Request"s}},{0x2C,{2,"Resurrection menu"s}} ,
	{0x39,{9,"Remove (Group)"s}},				{0x3A,{-1,"Send Skills"s}} ,
	{0x56,{11,"Map (Cartography/Treasure)"s}},	{0x66,{-1,"Books (Pages)"s}} ,
	{0x6C,{19,"Target Cursor Commands"s}},		{0x6F,{-1,"Secure Trading"s}} ,
	{0x71,{-1,"Bulletin Board Message"s}},		{0x72,{5,"Request War Mode"s}} ,
	{0x73,{2,"Ping"s}},					{0x93,{99,"Old Book Header"s}} ,
	{0x95,{9,"Dye Window"s}},				{0x98,{-1,"All Names (3D Client Only)"s}} ,
	{0x99,{26,"Give Boat/House Placement View"s}},	{0x9A,{-1,"Console Entry Prompt)"s}} ,
	{0xB8,{-1,"Request/Char Profile"s}},		{0xBB,{9,"Ultmia Messenger"s}} ,
	{0xBD,{-1,"Client Version"s}},			{0xBE,{-1,"Assist Version"s}} ,
	{0xBF,{-1,"General Information"s}},			{0xC2,{-1,"Unicode Text Entry"s}} ,
	{0xC8,{2,"Client View Range"s}},			{0xC9,{6,"Get Area Server Ping (God Client)"s}} ,
	{0xCA,{6,"Get User Server Ping (God Client)"s}},{0xD0,{-1,"Configuration File"s}} ,
	{0xD1,{2,"Logout Status"s}},				{0xD4,{-1,"New Book Header"s}} ,
	{0xD6,{-1,"Mega Cliloc"s}},				{0xD7,{-1,"Generic AOS Commands"s}} ,
	{0xF1,{-1,"Freeshard List"s}},			{0xFF,{4,"Initial Packet, Seed Key"s}}
	
};


//=========================================================
// huffman encoding
//=========================================================

//=========================================================
auto packet_t::pack(const std::vector<std::uint8_t> &data,std::vector<std::uint8_t> &result) const ->void {
	
	// what we do is first make a buffer twice the size, then we shrink it later
	result.resize(data.size()*2,0) ;
	
	auto packet_length = std::uint32_t(0) ;
	auto bit_byte = std::int32_t(0) ;
	auto nr_bits = std::int32_t(0) ;
	auto value = std::uint32_t(0) ;
	auto input_index = 0 ;
	auto len = static_cast<std::int32_t>(result.size()) ;
	while (len--){
		nr_bits = _huffman_table[data[input_index]][0];
		value = _huffman_table[data[input_index++]][1] ;
		while (nr_bits--){
			
			result[packet_length] = static_cast<std::uint8_t>((result[packet_length]<<1)) | (static_cast<std::uint8_t>((value >> nr_bits))&0x1) ;
			bit_byte = (bit_byte + 1) & 0x07 ;
			if (!bit_byte){
				++packet_length;
			}
		}
	}
	nr_bits = _huffman_table[256][0] ;
	value = _huffman_table[256][1] ;
	while (nr_bits--) {
		result[packet_length] = static_cast<std::uint8_t>((result[packet_length]<<1) | (static_cast<std::uint8_t>(( value >> nr_bits)) & 0x01));
		bit_byte = (bit_byte+1)&0x07 ;
		if (!bit_byte){
			++packet_length ;
		}
	}
	if (bit_byte){
		while (bit_byte < 8){
			result[packet_length] <<=1;
			++bit_byte;
		}
		++packet_length ;
	}
	// resize the packed vector
	result.resize(packet_length);
}

//=====================================================================
auto packet_t::pack()  ->packet_t& {
	pack(_bytedata,packed_data) ;
	return *this ;
}
//======================================================================
// Constructors
//=====================================================================
packet_t::packet_t():bytebuffer_t() {
	is_variable = false ;
}
//=====================================================================
packet_t::packet_t(int size,const std::string &name,bool variable):packet_t() {
	this->name = name ;
	is_variable = variable ;
	if (variable){
		// we want to at least initially size this to handle three bytes
		this->size(3);
	}
	else if (size>0){
		this->size(size) ;
	}
}
//======================================================================
// UO Packet information
//=====================================================================
//=====================================================================
auto packet_t::packetInfo(std::uint8_t packetnum)  ->const std::pair<int,std::string>&{
	return uopackets.at(packetnum);
}
//======================================================================
// finalize a variable packet (take the size and write it into the size field of a variable packet)
//=====================================================================
//=====================================================================
auto packet_t::finalize() ->packet_t&{
	if (is_variable){
		auto size = static_cast<std::uint16_t>(this->size());
		// Length in the packet is big endian
		std::reverse(reinterpret_cast<std::uint8_t*>(&size), reinterpret_cast<std::uint8_t*>(&size)+2);
		// put the length in the packet ;
		std::copy(reinterpret_cast<std::uint8_t*>(&size),reinterpret_cast<std::uint8_t*>(&size)+2, this->raw()+1);
	}
	return *this ;
}
