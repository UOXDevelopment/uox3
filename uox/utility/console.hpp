//

#ifndef console_hpp
#define console_hpp

#include <cstdint>
#include <cstdio>
#include <utility>
#include <string>
#include <iostream>
#include <memory>

//=========================================================
class console {
protected:
	bool is_initialized;
	static const std::string ESC ;
	static const std::string CSI ;
	static const std::string BEL ;
	//====================================== control sequences =========================================================================
	// most (windows anyway) control sequences can be found here: https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	// Cursor movement
	/*
	 Sequence		Code	Description	Behavior
	 ESC [ <n> A	CUU	Cursor Up	Cursor up by <n>
	 ESC [ <n> B	CUD	Cursor Down	Cursor down by <n>
	 ESC [ <n> C	CUF	Cursor Forward	Cursor forward (Right) by <n>
	 ESC [ <n> D	CUB	Cursor Backward	Cursor backward (Left) by <n>
	 ESC [ <n> E	CNL	Cursor Next Line	Cursor down <n> lines from current position
	 ESC [ <n> F	CPL	Cursor Previous Line	Cursor up <n> lines from current position
	 ESC [ <n> G	CHA	Cursor Horizontal Absolute	Cursor moves to <n>th position horizontally in the current line
	 ESC [ <n> d	VPA	Vertical Line Position Absolute	Cursor moves to the <n>th position vertically in the current column
	 ESC [ <y> ; <x> H	CUP	Cursor Position	*Cursor moves to <x>; <y> coordinate within the viewport, where <x> is the column of the <y> line
	 ESC [ <y> ; <x> f	HVP	Horizontal Vertical Position	*Cursor moves to <x>; <y> coordinate within the viewport, where <x> is the column of the <y> line
	 ESC [ s	ANSISYSSC	Save Cursor – Ansi.sys emulation	**With no parameters, performs a save cursor operation like DECSC
	 ESC [ u	ANSISYSSC	Restore Cursor – Ansi.sys emulation	**With no parameters, performs a restore cursor operation like DECRC
	 */
	static const std::string CUU ;
	static const std::string CUD ;
	static const std::string CUF ;
	static const std::string CUB ;
	static const std::string CNL ;
	static const std::string CPL ;
	static const std::string CHA ;
	static const std::string VPA ;
	static const std::string CUP ;
	static const std::string HVP;
	static const std::string SAVECUR ;
	static const std::string RESTORECUR ;
	// Cursor visibility
	/*
	 Sequence		Code		Description	Behavior
	 ESC [ ? 12 h	ATT160	Text Cursor Enable Blinking	Start the cursor blinking
	 ESC [ ? 12 l	ATT160	Text Cursor Disable Blinking	Stop blinking the cursor
	 ESC [ ? 25 h	DECTCEM	Text Cursor Enable Mode Show	Show the cursor
	 ESC [ ? 25 l	DECTCEM	Text Cursor Enable Mode Hide	Hide the cursor
	 */
	static const std::string BLINKCUR ;
	static const std::string NOBLINKCUR ;
	static const std::string SHOWCUR ;
	static const std::string HIDECUR ;
	
	// Viewport positioning
	/*
	 Sequence		Code	Description	Behavior
	 ESC [ <n> S	SU	Scroll Up	Scroll text up by <n>. Also known as pan down, new lines fill in from the bottom of the screen
	 ESC [ <n> T	SD	Scroll Down	Scroll down by <n>. Also known as pan up, new lines fill in from the top of the screen
	 */
	static const std::string SU ;
	static const std::string SD ;
	
	// Text Modification
	/*
	 Sequence		Code	Description	Behavior
	 ESC [ <n> @	ICH	Insert Character	Insert <n> spaces at the current cursor position, shifting all existing text to the right. Text exiting the screen to the right is removed.
	 ESC [ <n> P	DCH	Delete Character	Delete <n> characters at the current cursor position, shifting in space characters from the right edge of the screen.
	 ESC [ <n> X	ECH	Erase Character	Erase <n> characters from the current cursor position by overwriting them with a space character.
	 ESC [ <n> L	IL	Insert Line	Inserts <n> lines into the buffer at the cursor position. The line the cursor is on, and lines below it, will be shifted downwards.
	 ESC [ <n> M	DL	Delete Line	Deletes <n> lines from the buffer, starting with the row the cursor is on.
	 
	 For the following commands, the parameter <n> has 3 valid values:
	 0 erases from the current cursor position (inclusive) to the end of the line/display
	 1 erases from the beginning of the line/display up to and including the current cursor position
	 2 erases the entire line/display
	 Sequence	Code	Description	Behavior
	 ESC [ <n> J	ED	Erase in Display	Replace all text in the current viewport/screen specified by <n> with space characters
	 ESC [ <n> K	EL	Erase in Line
	 */
	static const std::string ICH ;
	static const std::string DCH ;
	static const std::string ECH ;
	static const std::string IL;
	static const std::string DL ;
	
	
	static const std::string ED ;
	static const std::string EL ;
	
	// Text formatting
	/*
	 Sequence	Code	Description	Behavior
	 ESC [ <n> m	SGR	Set Graphics Rendition	Set the format of the screen and text as specified by <n>
	 */
	static const std::string SGR ;
	
	// Attributes for SGR
	/*
	 Value	Description	Behavior
	 0	Default	Returns all attributes to the default state prior to modification
	 1	Bold/Bright	Applies brightness/intensity flag to foreground color
	 22	No bold/bright	Removes brightness/intensity flag from foreground color
	 4	Underline	Adds underline
	 24	No underline	Removes underline
	 7	Negative	Swaps foreground and background colors
	 27	Positive (No negative)	Returns foreground/background to normal
	 30	Foreground Black	Applies non-bold/bright black to foreground
	 31	Foreground Red	Applies non-bold/bright red to foreground
	 32	Foreground Green	Applies non-bold/bright green to foreground
	 33	Foreground Yellow	Applies non-bold/bright yellow to foreground
	 34	Foreground Blue	Applies non-bold/bright blue to foreground
	 35	Foreground Magenta	Applies non-bold/bright magenta to foreground
	 36	Foreground Cyan	Applies non-bold/bright cyan to foreground
	 37	Foreground White	Applies non-bold/bright white to foreground
	 38	Foreground Extended	Applies extended color value to the foreground (see details below)
	 39	Foreground Default	Applies only the foreground portion of the defaults (see 0)
	 40	Background Black	Applies non-bold/bright black to background
	 41	Background Red	Applies non-bold/bright red to background
	 42	Background Green	Applies non-bold/bright green to background
	 43	Background Yellow	Applies non-bold/bright yellow to background
	 44	Background Blue	Applies non-bold/bright blue to background
	 45	Background Magenta	Applies non-bold/bright magenta to background
	 46	Background Cyan	Applies non-bold/bright cyan to background
	 47	Background White	Applies non-bold/bright white to background
	 48	Background Extended	Applies extended color value to the background (see details below)
	 49	Background Default	Applies only the background portion of the defaults (see 0)
	 90	Bright Foreground Black	Applies bold/bright black to foreground
	 91	Bright Foreground Red	Applies bold/bright red to foreground
	 92	Bright Foreground Green	Applies bold/bright green to foreground
	 93	Bright Foreground Yellow	Applies bold/bright yellow to foreground
	 94	Bright Foreground Blue	Applies bold/bright blue to foreground
	 95	Bright Foreground Magenta	Applies bold/bright magenta to foreground
	 96	Bright Foreground Cyan	Applies bold/bright cyan to foreground
	 97	Bright Foreground White	Applies bold/bright white to foreground
	 100	Bright Background Black	Applies bold/bright black to background
	 101	Bright Background Red	Applies bold/bright red to background
	 102	Bright Background Green	Applies bold/bright green to background
	 103	Bright Background Yellow	Applies bold/bright yellow to background
	 104	Bright Background Blue	Applies bold/bright blue to background
	 105	Bright Background Magenta	Applies bold/bright magenta to background
	 106	Bright Background Cyan	Applies bold/bright cyan to background
	 107	Bright Background White	Applies bold/bright white to background
	 */
	
	// Query state
	/*
	 Sequence	Code		Description	Behavior
	 ESC [ 6 n	DECXCPR	Report Cursor Position	Emit the cursor position as: ESC [ <r> ; <c> R Where <r> = cursor row and <c> = cursor column
	 ESC [ 0 c	DA		Device Attributes	Report the terminal identity. Will emit “\x1b[?1;0c”, indicating "VT101 with No Options".
	 */
	static const std::string DECXCPR  ;
	
	// Tabs
	/*
	 Sequence		Code	Description	Behavior
	 ESC H		HTS	Horizontal Tab Set	Sets a tab stop in the current column the cursor is in.
	 ESC [ <n> I	CHT	Cursor Horizontal (Forward) Tab	Advance the cursor to the next column (in the same row) with a tab stop. If there are no more tab stops, move to the last column in the row. If the cursor is in the last column, move to the first column of the next row.
	 ESC [ <n> Z	CBT	Cursor Backwards Tab	Move the cursor to the previous column (in the same row) with a tab stop. If there are no more tab stops, moves the cursor to the first column. If the cursor is in the first column, doesn’t move the cursor.
	 ESC [ 0 g		TBC	Tab Clear (current column)	Clears the tab stop in the current column, if there is one. Otherwise does nothing.
	 ESC [ 3 g		TBC	Tab Clear (all columns)	Clears all currently set tab stops.
	 For both CHT and CBT, <n> is an optional parameter that (default=1) indicating how many times to advance the cursor in the specified direction.
	 If there are no tab stops set via HTS, CHT and CBT will treat the first and last columns of the window as the only two tab stops.
	 Using HTS to set a tab stop will also cause the console to navigate to the next tab stop on the output of a TAB (0x09, ‘\t’) character, in the same manner as CHT.
	 */
	// Designate Character Set
	/*
	 Sequence	Description	Behavior
	 ESC ( 0	Designate Character Set – DEC Line Drawing	Enables DEC Line Drawing Mode
	 ESC ( B	Designate Character Set – US ASCII	Enables ASCII Mode (Default)
	 */
	static const std::string GRAPHICON ;
	static const std::string GRAPHICOFF ;
	// Graphic symbols
	/*
	 Notably, the DEC Line Drawing mode is used for drawing borders in console applications. The following table shows what ASCII character maps to which line drawing character.
	 Hex	ASCII	DEC Line Drawing
	 0x6a	j	┘
	 0x6b	k	┐
	 0x6c	l	┌
	 0x6d	m	└
	 0x6e	n	┼
	 0x71	q	─
	 0x74	t	├
	 0x75	u	┤
	 0x76	v	┴
	 0x77	w	┬
	 0x78	x	│
	 */
	
	// Scrolling margins
	/*
	 the following sequences allow a program to configure the “scrolling region” of the screen that is affected by scrolling operations. This is a subset of the rows that are adjusted when the screen would otherwise scroll, for example, on a ‘\n’ or RI. These margins also affect the rows modified by Insert Line (IL) and Delete Line (DL), Scroll Up (SU) and Scroll Down (SD).
	 The scrolling margins can be especially useful for having a portion of the screen that doesn’t scroll when the rest of the screen is filled, such as having a title bar at the top or a status bar at the bottom of your application.
	 For DECSTBM, there are two optional parameters, <t> and <b>, which are used to specify the rows that represent the top and bottom lines of the scroll region, inclusive. If the parameters are omitted, <t> defaults to 1 and <b> defaults to the current viewport height.
	 Scrolling margins are per-buffer, so importantly, the Alternate Buffer and Main Buffer maintain separate scrolling margins settings (so a full screen application in the alternate buffer will not poison the main buffer’s margins).
	 Sequence			Code	Description	Behavior
	 ESC [ <t> ; <b> r	DECSTBM	Set Scrolling Region	Sets the VT scrolling margins of the viewport.
	 */
	static const std::string DECSTBM ;
	
	// Window title
	/*
	 the following commands allows the application to set the title of the console window to the given <string> parameter. The string must be less than 255 characters to be accepted. This is equivalent to calling SetConsoleTitle with the given string.
	 Note that these sequences are OSC “Operating system command” sequences, and not a CSI like many of the other sequences listed, and as such starts with “\x1b]”, not “\x1b[”. As OSC sequences, they are ended with a String Terminator represented as <ST> and transmitted with ESC \ (0x1B 0x5C). BEL (0x7) may be used instead as the terminator, but the longer form is preferred.
	 Sequence				Description	Behavior
	 ESC ] 0 ; <string> <ST>	Set Window Title	Sets the console window’s title to <string>.
	 ESC ] 2 ; <string> <ST>	Set Window Title	Sets the console window’s title to <string>.
	 The terminating character here is the “Bell” character, ‘\x07’
	 */
	static const std::string SETTITLE;
	
	// Alternate screen buffer
	/*
	 *Nix style applications often utilize an alternate screen buffer, so that they can modify the entire contents of the buffer, without affecting the application that started them. The alternate buffer is exactly the dimensions of the window, without any scrollback region.
	 For an example of this behavior, consider when vim is launched from bash. Vim uses the entirety of the screen to edit the file, then returning to bash leaves the original buffer unchanged.
	 Sequence			Description	Behavior
	 ESC [ ? 1 0 4 9 h	Use Alternate Screen Buffer	Switches to a new alternate screen buffer.
	 ESC [ ? 1 0 4 9 l	Use Main Screen Buffer	Switches to the main buffer.
	 */
	static const std::string MAINBUFFER ;
	static const std::string ALTBUFFER ;
	
	
	// Soft reset
	/*
	 The following sequence can be used to reset certain properties to their default values.The following properties are reset to the following default values (also listed are the sequences that control those properties):
	 Cursor visibility: visible (DECTEM)
	 Numeric Keypad: Numeric Mode (DECNKM)
	 Cursor Keys Mode: Normal Mode (DECCKM)
	 Top and Bottom Margins: Top=1, Bottom=Console height (DECSTBM)
	 Character Set: US ASCII
	 Graphics Rendition: Default/Off (SGR)
	 Save cursor state: Home position (0,0) (DECSC)
	 Sequence	Code		Description	Behavior
	 ESC [ ! p	DECSTR	Soft Reset	Reset certain terminal settings to their defaults.
	 */
	static const std::string RESET ;
	//==========================================================
	// The source for this was found on StackOverflow at:
	// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
	//
	template<typename ... Args>
	std::string format( const std::string& format_str, Args ... args ) const {
		int size_s = std::snprintf( nullptr, 0, format_str.c_str(), args ... ) + 1; // Extra space for '\0'
		if (size_s > 0){
			auto size = static_cast<size_t>( size_s );
			auto buf = std::make_unique<char[]>( size );
			std::snprintf( buf.get(), size, format_str.c_str(), args ... );
			return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
			
		}
		return format_str ; // We take the same approach as std library, and just fail silently with
		// best answer
	}
	// We keep track of what column we are on
	int current_column ;
	int console_width  ;
public:
	static constexpr auto CURTOEND = 0 ;
	static constexpr auto BEGTOCUR = 1 ;
	static constexpr auto ENTIRE = 2 ;

	static constexpr auto DEFAULT = 0 ;
	static constexpr auto BOLD = 1 ;
	static constexpr auto UNDERLINE = 4;
	static constexpr auto INVERSE = 7;
	static constexpr auto NO = 20 ;
	
	static constexpr auto BLACK = 0 ;
	static constexpr auto RED = 1 ;
	static constexpr auto GREEN = 2 ;
	static constexpr auto YELLOW = 3 ;
	static constexpr auto BLUE = 4 ;
	static constexpr auto MAGENTA = 5 ;
	static constexpr auto CYAN = 6 ;
	static constexpr auto WHITE = 7 ;
	static constexpr auto FOREGROUND = 30 ;
	static constexpr auto BACKGROUND = 40 ;
	static constexpr auto BRIGHT = 60 ;
protected:
	// returns row,column
	auto size() ->std::pair<int,int> ;
	auto fit(const std::string &input,int column,int width) ->std::pair<std::string,std::string>;
	virtual auto newline() ->void ;
	auto output(const std::string &line,int fixcol)->std::string ;
	auto getCharacter() ->int;
	auto reset() ->void ;

public:
	console() ;
	~console() ;
	console(const console&) = delete ;
	auto operator=(const console &) ->console& = delete ;

	auto initialize() ->void ;
	// basic
	auto currentColumn() const ->int ;
	auto width() const ->int ;
	
	auto setColor(int color) ->console& ;
	auto setPosition(int col,int row) ->console& ;
	auto setHozPosition(int column) ->console& ;
	auto clear() ->console& ;
	auto setTitle(const std::string &title) ->console& ;
	auto write(const std::string & line,int fixcol = 0) ->console& ;
	auto center(const std::string &line) ->console& ;
};
#endif /* cConsole_hpp */
