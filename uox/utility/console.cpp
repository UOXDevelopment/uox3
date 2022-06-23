//

#include "console.hpp"

#include <iostream>
#include <string_view>
#include <cstdio>

#if !defined(_WIN32)
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
struct termios initial_terminal_state;
#else

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
DWORD initial_terminal_state;
#endif

using namespace std::string_literals ;

const std::string console::ESC = "\x1b"s; //escape character
const std::string console::CSI = console::ESC + "["s;
const std::string console::BEL = "\x07"s;


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
const std::string console::CUU = console::CSI+"%iA"s ;
const std::string console::CUD = console::CSI+"%iB"s ;
const std::string console::CUF = console::CSI+"%iC"s;
const std::string console::CUB = console::CSI+"%iD"s;
const std::string console::CNL = console::CSI+"%iE"s;
const std::string console::CPL = console::CSI+"%iF"s;
const std::string console::CHA = console::CSI+"%iG"s;
const std::string console::VPA = console::CSI+"%id"s;
const std::string console::CUP = console::CSI+"%i:%iH"s;
const std::string console::HVP = console::CSI+"%i:%if";
const std::string console::SAVECUR = console::CSI+"s";
const std::string console::RESTORECUR = console::CSI+"u" ;
// Cursor visibility
/*
 Sequence		Code		Description	Behavior
 ESC [ ? 12 h	ATT160	Text Cursor Enable Blinking	Start the cursor blinking
 ESC [ ? 12 l	ATT160	Text Cursor Disable Blinking	Stop blinking the cursor
 ESC [ ? 25 h	DECTCEM	Text Cursor Enable Mode Show	Show the cursor
 ESC [ ? 25 l	DECTCEM	Text Cursor Enable Mode Hide	Hide the cursor
 */
const std::string console::BLINKCUR  = console::CSI +"?12h"s;
const std::string console::NOBLINKCUR = console::CSI + "?12l"s;
const std::string console::SHOWCUR = console::CSI + "?25h"s;
const std::string console::HIDECUR = console::CSI + "?25l"s;

// Viewport positioning
/*
 Sequence		Code	Description	Behavior
 ESC [ <n> S	SU	Scroll Up	Scroll text up by <n>. Also known as pan down, new lines fill in from the bottom of the screen
 ESC [ <n> T	SD	Scroll Down	Scroll down by <n>. Also known as pan up, new lines fill in from the top of the screen
 */
const std::string console::SU = console::CSI +"%iS"s;
const std::string console::SD = console::CSI +"%iT"s;
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
const std::string console::ICH = console::CSI+"%i@"s;
const std::string console::DCH = console::CSI+"%iP"s;
const std::string console::ECH = console::CSI+"%iX"s;
const std::string console::IL = console::CSI+"%iL"s;
const std::string console::DL = console::CSI+"%iM"s;


const std::string console::ED = console::CSI +"%iJ"s ;
const std::string console::EL = console::CSI +"%iK"s;
// Text formatting
/*
 Sequence	Code	Description	Behavior
 ESC [ <n> m	SGR	Set Graphics Rendition	Set the format of the screen and text as specified by <n>
 */
const std::string console::SGR = console::CSI +"%im"s;

// Query state
/*
 Sequence	Code		Description	Behavior
 ESC [ 6 n	DECXCPR	Report Cursor Position	Emit the cursor position as: ESC [ <r> ; <c> R Where <r> = cursor row and <c> = cursor column
 ESC [ 0 c	DA		Device Attributes	Report the terminal identity. Will emit “\x1b[?1;0c”, indicating "VT101 with No Options".
 */
const std::string console::DECXCPR  = console::CSI + "6n"s ;

// Tabs

// Designate Character Set
/*
 Sequence	Description	Behavior
 ESC ( 0	Designate Character Set – DEC Line Drawing	Enables DEC Line Drawing Mode
 ESC ( B	Designate Character Set – US ASCII	Enables ASCII Mode (Default)
 */
const std::string console::GRAPHICON = console::ESC + "(0"s ;
const std::string console::GRAPHICOFF = console::ESC + "(B"s;

// Scrolling margins
/*
 the following sequences allow a program to configure the “scrolling region” of the screen that is affected by scrolling operations. This is a subset of the rows that are adjusted when the screen would otherwise scroll, for example, on a ‘\n’ or RI. These margins also affect the rows modified by Insert Line (IL) and Delete Line (DL), Scroll Up (SU) and Scroll Down (SD).
 The scrolling margins can be especially useful for having a portion of the screen that doesn’t scroll when the rest of the screen is filled, such as having a title bar at the top or a status bar at the bottom of your application.
 For DECSTBM, there are two optional parameters, <t> and <b>, which are used to specify the rows that represent the top and bottom lines of the scroll region, inclusive. If the parameters are omitted, <t> defaults to 1 and <b> defaults to the current viewport height.
 Scrolling margins are per-buffer, so importantly, the Alternate Buffer and Main Buffer maintain separate scrolling margins settings (so a full screen application in the alternate buffer will not poison the main buffer’s margins).
 Sequence			Code	Description	Behavior
 ESC [ <t> ; <b> r	DECSTBM	Set Scrolling Region	Sets the VT scrolling margins of the viewport.
 */
const std::string console::DECSTBM = console::CSI + "%i;%ir"s;

// Window title
/*
 the following commands allows the application to set the title of the console window to the given <string> parameter. The string must be less than 255 characters to be accepted. This is equivalent to calling SetConsoleTitle with the given string.
 Note that these sequences are OSC “Operating system command” sequences, and not a CSI like many of the other sequences listed, and as such starts with “\x1b]”, not “\x1b[”. As OSC sequences, they are ended with a String Terminator represented as <ST> and transmitted with ESC \ (0x1B 0x5C). BEL (0x7) may be used instead as the terminator, but the longer form is preferred.
 Sequence				Description	Behavior
 ESC ] 0 ; <string> <ST>	Set Window Title	Sets the console window’s title to <string>.
 ESC ] 2 ; <string> <ST>	Set Window Title	Sets the console window’s title to <string>.
 The terminating character here is the “Bell” character, ‘\x07’
 */
// this one works on both os's
const std::string console::SETTITLE = console::ESC +"]2%s"s+console::BEL;

// Alternate screen buffer
/*
 *Nix style applications often utilize an alternate screen buffer, so that they can modify the entire contents of the buffer, without affecting the application that started them. The alternate buffer is exactly the dimensions of the window, without any scrollback region.
 For an example of this behavior, consider when vim is launched from bash. Vim uses the entirety of the screen to edit the file, then returning to bash leaves the original buffer unchanged.
 Sequence			Description	Behavior
 ESC [ ? 1 0 4 9 h	Use Alternate Screen Buffer	Switches to a new alternate screen buffer.
 ESC [ ? 1 0 4 9 l	Use Main Screen Buffer	Switches to the main buffer.
 */
const std::string console::MAINBUFFER = console::CSI +"?1049l"s;
const std::string console::ALTBUFFER = console::CSI +"?1049h"s;
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
const std::string console::RESET = console::CSI + "!p"s;
//=========================================================
auto console::size() ->std::pair<int,int> {
	int row = 0 ;
	int col = 0 ;
#if !defined(_WIN32)
	// Get the window size
	winsize winsz;
	ioctl(STDIN_FILENO,TIOCGWINSZ,&winsz);
	col = winsz.ws_col;
	row = winsz.ws_row;
#else
	HANDLE hco = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo( hco, &csbi );
	col	= csbi.dwSize.X;
	row	= csbi.dwSize.Y;
#endif
	return std::make_pair(row,col);
}
//=========================================================
auto console::fit(const std::string &input,int column,int width) ->std::pair<std::string,std::string> {
	auto size = input.size() ;
	auto truncated = input ;
	auto whatsleft = std::string() ;
	auto remaining = width - column ;
	if (!input.empty()){
		//auto view = std::string_view(input) ;
		if (size > remaining) {
			auto view = std::string_view(input) ;
			auto spot = view.substr(0,remaining).find_last_of(" \t\r\n\f") ;
			if (spot != std::string::npos) {
				// break it there
				// but, maybe not, if the first spot after is a space, it is ok
				auto wait = view.substr(remaining).find_first_of(" \t\r\n\f");
				if (wait != 0) {
					remaining = static_cast<int>(spot)+1 ;
				}
			}
		}
		truncated = input.substr(0,remaining) ;
		if (remaining<size){
			whatsleft = input.substr(remaining) ;
		}
		
	}
	return std::make_pair(truncated, whatsleft);
	
}
//=========================================================
auto console::newline() ->void {
	std::cout << std::endl;
	current_column = 1;
}
//=========================================================
auto console::output(const std::string &line,int fixcol)->std::string {
	auto [row,column] = this->size() ;
	console_width = column ;
	if (current_column >= console_width) {
		this->newline() ;
	}
	if (fixcol>0) {
		setHozPosition(fixcol);
	}
	auto [current,remaining] = this->fit(line,current_column,console_width);
	if (!current.empty()) {
		std::cout << current ;
		current_column += static_cast<int>(current.size()) ;
	}
	return remaining ;
}
//=========================================================
auto console::getCharacter() ->int {
	auto rvalue = 0;
#if defined(_WIN32)
	auto numread = DWORD(0);
	INPUT_RECORD input;
	HANDLE hco = GetStdHandle(STD_INPUT_HANDLE);
	GetNumberOfConsoleInputEvents(hco, &numread);
	while (numread > 0) {
		
		if (ReadConsoleInput(hco, &input, 1, &numread)) {
			if (numread > 0) {
				switch (input.EventType) {
				case KEY_EVENT: {
					if (!input.Event.KeyEvent.bKeyDown) {
						auto repeat = input.Event.KeyEvent.wRepeatCount;
						auto key = input.Event.KeyEvent.uChar.AsciiChar;
						// we are going to "eat" repeated keys
						rvalue = static_cast<int>(key);
					}
					break;
				}
				case MOUSE_EVENT:
				case WINDOW_BUFFER_SIZE_EVENT:
				case FOCUS_EVENT:
				case MENU_EVENT:
				default:
					break;
				}

			}
		}
	}

#else
	char data = 0 ;
	auto a = ::read(0,&data, 1); // This doesn't block on getting a line due to initalization
	if( a > 0 ) {
		rvalue = static_cast<int>(data) ;
	}

#endif
	return rvalue;
}

//=========================================================
console::console() : is_initialized(false),current_column(1), console_width(80){
	auto [row,col] = this->size() ;
	console_width = col ;
}
//=========================================================
console::~console() {
	reset();
}
//=========================================================
auto console::reset() ->void {
	std::cout.flush();
	if (is_initialized) {
#if !defined(_WIN32)
		tcsetattr(1, TCSAFLUSH, &initial_terminal_state);
		std::cout << std::endl;
#else
		auto hco = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleMode(hco,initial_terminal_state);
#endif
	}
	is_initialized = false ;
}
//=========================================================
auto console::initialize() ->void {
	is_initialized = true ;
#if !defined(_WIN32)
	tcgetattr(1, &initial_terminal_state); // get the current state of the terminal
	auto temp = initial_terminal_state;
	temp.c_lflag = temp.c_lflag & (~ECHO) & (~ICANON); // Disable echo and canonical (line) mode
	temp.c_cc[VMIN] = 0;  		// in non canonical mode we non blocking read
	temp.c_cc[VTIME] = 0; // in non canonical mode we non blocking read
	tcsetattr(1, TCSANOW, &temp);
#else
	// Set the input to non echo and non canonical (line) mode
	auto hco = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hco, &initial_terminal_state);
	auto temp = initial_terminal_state;
	temp = (temp & (~ENABLE_ECHO_INPUT) & (~ENABLE_LINE_INPUT) & (~ENABLE_MOUSE_INPUT)) | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_VIRTUAL_TERMINAL_INPUT;
	SetConsoleMode(hco,initial_terminal_state);
#endif
}
//=========================================================
auto console::currentColumn() const ->int {
	return current_column ;
}
//=========================================================
auto console::width() const ->int {
	return console_width;
}
//=========================================================
auto console::setColor(int color) ->console& {
	auto cmd = this->format(SGR, color) ;
	std::cout << cmd ;
	return *this ;
}
//=========================================================
auto console::setPosition(int col,int row) ->console& {
	auto cmd = this->format(CUP, row,col) ;
	std::cout <<cmd ;
	current_column = col ;
	return *this ;
}
//=========================================================
auto console::setHozPosition(int column) ->console& {
	auto cmd = this->format(CHA, column) ;
	std::cout <<cmd ;
	current_column = column ;
	return *this ;
}
//=========================================================
auto console::clear() ->console& {
	auto cmd = this->format(ED,ENTIRE) ;
	std::cout <<cmd ;
	std::cout.flush();
	setPosition(1, 1);
	current_column = 1 ;
	return *this ;
}

//=========================================================
auto console::setTitle(const std::string &title) ->console& {
	auto cmd = this->format(SETTITLE, title.c_str()) ;
	std::cout <<cmd ;
	std::cout.flush() ;
	return *this ;
}
//==========================================================
auto console::write(const std::string & line,int fixcol) ->console& {
	auto left = output(line,fixcol) ;
	while (!left.empty()) {
		left = output(left,fixcol) ;
	}
	return *this;
}
//==========================================================
auto console::center(const std::string &line) ->console& {
	auto size = static_cast<int>(line.size()) ;
	if (size > console_width) {
		size = console_width;
	}
	size = size/2 ;
	auto start = console_width/2 - size ;
	setHozPosition(start);
	write(line) ;
	
	return *this ;
	
}
