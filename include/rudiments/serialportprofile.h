// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORTPROFILE_H
#define RUDIMENTS_SERIALPORTPROFILE_H

#include <rudiments/private/serialportprofileincludes.h>

// cfmakeraw, cfsetspeed???
// wraps struct termios

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class serialportprofileprivate;

class DLLSPEC serialportprofile {
	public:

		enum	inputmode_t {
			cannonical=0,
			raw
		};

		enum	flowcontrol_t {
			fc_none=0,
			fc_software,
			fc_hardware
		};

		enum	baudrate_t {
			baud_0=0,
			baud_50,
			baud_75,
			baud_110,
			baud_134,
			baud_150,
			baud_200,
			baud_300,
			baud_600,
			baud_1200,
			baud_1800,
			baud_2400,
			baud_4800,
			baud_9600,
			baud_19200,
			exta,
			baud_38400,
			extb,
			baud_57600,
			baud_76800,
			baud_115200,
			baud_230400,
			baud_460800,
			baud_500000,
			baud_576000,
			baud_921600,
			baud_1000000,
			baud_1152000,
			baud_1500000,
			baud_2000000,
			baud_2500000,
			baud_3000000,
			baud_3500000,
			baud_4000000
		};

		enum	charsize_t {
			cs_5=0,
			cs_6,
			cs_7,
			cs_8
		};

		enum	newlinedelay_t {
			nl_none=0,
			nl_100
		};

		enum	carriagereturndelay_t {
			cr_none=0,
			cr_depends,
			cr_100,
			cr_150
		};

		enum	tabdelay_t {
			td_0=0,
			td_1,
			td_2,
			td_xtabs
		};

		enum	backspacedelay_t {
			bs_none=0,
			bs_50
		};

		enum	verticaltabdelay_t {
			vt_none=0,
			vt_2
		};

		enum	formfeeddelay_t {
			ff_none=0,
			ff_2
		};

			serialportprofile();
			~serialportprofile();

		void	defaultControlOptions();
			// set the control options to system defaults
		void	defaultLocalOptions();
			// set the local options to system defaults
		void	defaultInputOptions();
			// set the input options to system defaults
		void	defaultOutputOptions();
			// set the output options to system defaults
		void	defaultControlCharacters();
			// set the control characters to system defaults
		void	defaultOptions();
			// set all options and control characters to
			// system defaults

		void	setControlCharacters(const unsigned char *c_cc);
			// Set control characters to the values pointed to by
			// "c_cc".  Note that c_cc must be an array of length
			// NCCS.

		void	evalOptionsString(const char *string);
			// Serial port configuration requirements are often
			// given as a 3 character string representing the
			// number of bits-per-char, the parity and the number
			// of stop bits such as "8n1", "7e2", etc.
			// This methods parses such a string and sets the
			// profile to match those values.

		void		inputMode(inputmode_t inputmode);
				// set input mode to "inputmode"
		inputmode_t	inputMode();
				// get the current input mode setting

		void		flowControl(flowcontrol_t flowcontrol);
				// set flow control to "flowcontrol"
		flowcontrol_t	flowControl();
				// get the current flow control setting

		// control options

		// setters...
		void	baud(const char *baudrate);
		void	baud(baudrate_t baudrate);
			// set the (input and output) baud rate
			// default is 0
		bool	inputBaud(const char *baudrate);
		bool	inputBaud(baudrate_t baudrate);
			// set the input baud rate
			// default is 0
		bool	outputBaud(const char *baudrate);
		bool	outputBaud(baudrate_t baudrate);
			// set the output baud rate
			// default is 0
		void	characterSize(charsize_t size);
			// set the character size
			// default is 5 bits
			// termios flag: CSIZE
		void	twoStopBits(bool truefalse);
			// use two stop bits if "truefalse" is true
			// or one stop bit if "truefalse" is false
			// default is one stop bit
			// termios flag: CSTOPB
		void	receiverOn(bool truefalse);
			// turn the receiver on/off
			// (enables serial port to read incoming data)
			// default is off
			// (you probably always want to set this on)
			// termios flag: CREAD
		void	parityCheck(bool truefalse);
			// turn parity checking on/off
			// default is off
			// termios flag: CPARENB
		void	oddParity(bool truefalse);
			// use odd parity if "truefalse" is true
			// or even parity if "truefalse" is false
			// default is even parity
			// termios flag: PARODD
		void	hangupOnClose(bool truefalse);
			// lower modem control lines after
			// last process closes the device (hang up)
			// default is not to hang up on close
			// termios flag: HUPCL
		void	ignoreModemControlLines(bool truefalse);
			// ignore (possibly sporadic) job control and hangup
			// signals that could terminate or otherwise control
			// the program
			// default is not to ignore
			// (this should be set to true when communicating
			// with a device connected directly to the computer as
			// opposed to over a modem)
			// termios flag: CLOCAL
		void	blockJobControlOutput(bool truefalse);
			// block output from a nonconcurrent shell layer
			// default is not to block
			// termios flag: LOBLK
		void	hardwareFlowControl(bool truefalse);
			// use RTS/CTS flow control
			// default is not to use RTS/CTS flow control
			// termios flag: CRTSCTS/NEW_CRTSCTS/
			//			CRTS_IFLOW/CRTS_OFLOW

		// getters...
		baudrate_t	baud();
				// returns current (input and output) baud
		baudrate_t	inputBaud();
				// returns current input baud
		baudrate_t	outputBaud();
				// returns current output baud
		charsize_t	characterSize();
				// returns current character size
		bool		twoStopBits();
				// returns true if two stop bits are being used
		bool		receiverOn();
				// returns true if the receiver is on
		bool		parityCheck();
				// returns true if parity check is enabled
		bool		oddParity();
				// returns true if odd parity is being used
		bool		hangupOnClose();
				// returns true if the modem lines will be
				// lowered when the last device closes the
				// device
		bool		ignoreModemControlLines();
				// returns true if (possibly sporadic) job
				// control and hangup signals are being ignored
		bool		blockJobControlOutput();
				// returns true if output from a nonconcurrent
				// shell layer is being blocked
		bool		hardwareFlowControl();
				// returns true if RTS/CTS hardware flow
				// control is being used



		// local options
		// setters...
		void	generateSignals(bool truefalse);
			// cause corresponding signals to be raised when any of
			// the INTR, QUIT, SUSP or DSUSP characters are received
			// default is not to raise signals
			// termios flag: ISIG
		void	canonicalInput(bool truefalse);
			// enable special characters EOF, EOL, EOL2, ERASE,
			// KILL, LNEXT, REPRINT, STATUS and WERASE, buffer by
			// lines
			// default is not to enable the special characters and
			// not buffer by lines
			// termios flag: ICANON
		void	escapedUpperCase(bool truefalse);
			// If cannonicalInput() is also set true, converts
			// characters from input to lower case unless they are
			// preceeded by a \.  On output, preceeds each upper
			// case character with a \ and converts all lower case
			// characters to uppercase.
			// default is not to use escaped upper case
			// (this is used when communicating with terminals that
			// can display upper or lower case, but only transmit/
			// receive upper case)
			// termios flag: XCASE
		void	echoInput(bool truefalse);
			// If cannonicalInput() is also set true then echo
			// input.
			// termios flag: ECHO
		void	eraseCharactersOn(bool truefalse);
			// If cannonicalInput() is also set true, character
			// set using eraseCharacter() erases the preceeding
			// character and character set using
			// wordEraseCharacter() erases the preceeding word.
			// termios flag: ECHOE
		void	killCharacterOn(bool truefalse);
			// If cannonicalInput() is also set true, character
			// set using killCharacter() erases the current line.
			// termios flag: ECHOK
		void	echoNewLine(bool truefalse);
			// If cannonicalInput() is also set true, new line
			// characters are echoed even if echoInput() is set
			// false.
			// termios flag: ECHONL
		void	extendedFunctions(bool truefalse);
			// If cannonicalInput() is also set true, enables
			// characters set by secondEndOfLineCharacter(),
			// reprintCharacter() and wordEraseCharacter().  Also
			// enables lowerCase().
			// termios flag: IEXTEN
		void	echoControlCharacters(bool truefalse);
			// If echoInput() is also set true, control characters
			// are echoed as ^X where X is the ascii code for the
			// character plus 0x40.
			// termios flag: ECHOCTL
		void	echoErasedCharacter(bool truefalse);
			// If cannonicalInput() and echoInput() are also set
			// true, characters are printed as they are erased.
			// termios flag: ECHOPRT
		void	emulateKill(bool truefalse);
			// If cannonicalInput() is also set, the character set
			// using killCharacter() causes the line to be erased
			// by erasing each character on the line.
			// (useful when a terminal doesn't support the KILL
			// character but does support the ERASE character)
			// termios flag: ECHOKE
		void	noFlushAfterInterruptOrQuit(bool truefalse);
			// Disables flushing of the input/output queues when
			// generating SIGINT, SIGQUIT or SIGSUSP signals.
			// termios flag: NOFLSH
		void	retypePendingCharacters(bool truefalse);
			// All characters in the input queue are reprinted when
			// the next character is read.
			// termios flag: PENDIN
		void	sendSignalForBackgroundOutput(bool truefalse);
			// Send the SIGTTOU signal to the process group of a
			// background process which tries to write to its
			// controlling terminal.
			// termios flag: TOSTOP

		// getters...
		bool	generateSignals();
			// returns true if corresponding signals will be raised
			// when any of the INTR, QUIT, SUSP or DSUSP characters
			// are received default is not to raise signals
		bool	canonicalInput();
			// returns true if special characters EOF, EOL, EOL2,
			// ERASE, KILL, LNEXT, REPRINT, STATUS and WERASE are
			// enabled and buffering is being done by lines
		bool	escapedUpperCase();
			// Returns true if on input, characters are being
			// converted from upper to lower case unless they are
			// preceeded by a \ and on output lower case characters
			// are being converted to upper case and upper case
			// characters are being preceeded with a \.
		bool	echoInput();
			// returns true if input characters are being echoed
		bool	eraseCharactersOn();
			// returns true if the character set using
			// eraseCharacter() erases the preceeding
			// character and the character set using
			// wordEraseCharacter() erases the preceeding word
		bool	killCharacterOn();
			// returns true if the character set using
			// killCharacter() erases the current line
		bool	echoNewLine();
			// returns true if new line characters are being echoed
			// even if echoInput() is set false
		bool	extendedFunctions();
			// returns true if characters set by
			// secondEndOfLineCharacter(), reprintCharacter() and
			// wordEraseCharacter() are enabled and lowerCase() is
			// enabled
		bool	echoControlCharacters();
			// returns true if control characters are being echoed
			// as ^X where X is the ascii code for the character
			// plus 0x40
		bool	echoErasedCharacter();
			// returns true if characters are being printed as they
			// are erased
		bool	emulateKill();
			// returns true if the character set using
			// killCharacter() causes the line to be erased
			// by erasing each character on the line
		bool	noFlushAfterInterruptOrQuit();
			// returns true if flushing of the input/output queues
			// when generating SIGINT, SIGQUIT or SIGSUSP signals
			// is enabled
		bool	retypePendingCharacters();
			// returns true if all characters in the input queue
			// are reprinted when the next character is read
		bool	sendSignalForBackgroundOutput();
			// returns true if the SIGTTOU signal is being sent
			// to the process group of a background process which
			// tries to write to its controlling terminal



		// input options
		void	inputParityCheck(bool truefalse);
			// enable parity checking on input
			// termios flag: INPCK
		void	ignoreParityErrors(bool truefalse);
			// ignore parity errors (ie. if a character has a
			// parity error, just return what we got, rather than
			// marking or converting it)
			// termios flag: IGNPAR
		void	markParityErrors(bool truefalse);
			// Unless ignoreParityErrors() is set true, prefix a
			// character with a parity error with \337 \0.  The
			// default is to convert it to \0 unless
			// ignoreParityErrors() is set true.
			// termios flag: PARMRK
		void	stripParityBits(bool truefalse);
			// set the 8th bit of each character (the parity bit)
			// to 0
			// termios flag: ISTRIP
		void	softwareFlowControlOnOutput(bool truefalse);
			// enable XON/XOFF flow control on output.
			// termios flag: IXON
		void	softwareFlowControlOnInput(bool truefalse);
			// enable XON/XOFF flow control on input.
			// termios flag: IXOFF
		void	anyCharacterStartsFlow(bool truefalse);
			// enable any character to restart output
			// termios flag: IXANY
		void	ignoreBreak(bool truefalse);
			// ignore BREAK character
			// termios flag: IGNBRK
		void	sendSignalOnBreak(bool truefalse);
			// If ignoreBreak() isn't set true and a BREAK character
			// is received, flush input and output queues and send a
			// SIGINT if the serial port is the controlling terminal
			// of the process group.  If ignoreBreak() is not set,
			// the default is to return a \0 character or \377 \0
			// if markParityErrors() is set true.
			// termios flag: BRKINT
		void	mapNewLineToCarriageReturnOnInput(bool truefalse);
			// translate new line to carriage return on input
			// termios flag: INLCR
		void	discardEndOfTransmission(bool truefalse);
			// termios flat: ONOEOT
		void	ignoreCarriageReturn(bool truefalse);
			// ignore carriage return on input
			// termios flag: IGNCR
		void	mapCarriageReturnToNewLineOnInput(bool truefalse);
			// translate carriage return to new line on input
			// termios flag: ICRNL
		void	lowerCase(bool truefalse);
			// map uppercase characters to lowercase on input
			// termios flag: IUCLC
		void	bellIfLineTooLong(bool truefalse);
			// ring bell when input queue is full
			// termios flag: IMAXBEL

		// getters...
		bool	inputParityCheck();
			// returns true if input parity checking is enabled
		bool	ignoreParityErrors();
			// returns true if parity errors are being ignored
		bool	markParityErrors();
			// returns true if characters with parity errors are
			// prefixed with \377 \0
		bool	stripParityBits();
			// returns true if the 8th bit (the parity bit) of
			// each character is being set to 0
		bool	softwareFlowControlOnOutput();
			// returns true if XON/XOFF flow control is enabled on
			// output
		bool	softwareFlowControlOnInput();
			// returns true if XON/XOFF flow control is enabled on
			// input
		bool	anyCharacterStartsFlow();
			// returns true if any character will restart output
		bool	ignoreBreak();
			// returns true if the BREAK character is being ignored	
		bool	sendSignalOnBreak();
			// returns true if input and output queues are flushed
			// and a SIGINT is sent to the process group if the
			// serial port is the controlling terminal when a BREAK
			// character is received
		bool	mapNewLineToCarriageReturnOnInput();
			// returns true if new lines are mapped to carriage
			// returns on input
		bool	discardEndOfTransmission();
		bool	ignoreCarriageReturn();
			// returns true if carriage returns are ignored on input
		bool	mapCarriageReturnToNewLineOnInput();
			// returns true if carriage returns are mapped to new
			// lines on input
		bool	lowerCase();
			// returns true if uppercase characters are mapped to
			// lowercase on input
		bool	bellIfLineTooLong();
			// returns true if the bell will be rung when the input
			// queue is full




		// output options
		void	postProcessOutput(bool truefalse);
			// enables implementation-defined output processing
			// termios flag: OPOST
		void	outputUpperCase(bool truefalse);
			// map lowercase characters to uppercase on output
			// termios flag: OLCUC
		void	mapNewLineToCarriageReturnNewLineOnOutput(
							bool truefalse);
			// map new line to carriage return/new line on
			// output
			// termios flag: ONLCR
		void	mapCarriageReturnToNewLineOnOutput(bool truefalse);
			// map carriage return to new line on output
			// termios flag: OCRNL
		void	dontOutputCarriageReturnAtColumnZero(bool truefalse);
			// don't output carriage return at column 0
			// termios flag: ONOCR
		void	mapNewLineToCarriageReturnOnOutput(bool truefalse);
			// map new line to carriage return on output
			// termios flag: ONLRET
		void	useFillCharactersForDelay(bool truefalse);
			// send fill characters for delay instead of using a
			// timed delay
			// termios flag: OFILL
		void	useDelForFill(bool truefalse);
			// use the DEL character instead of NULL for the fill
			// character
			// termios flag: OFDEL
		void	expandTabToSpaces(bool truefalse);
			// map tabs to spaces
			// termios flag: XTAGS/OXTABS/TAB3

		void	delayAfterNewLine(newlinedelay_t nldelay);
			// send a delay after each new line character
			// termios flag: NLDLY
		void	delayAfterCarriageReturn(carriagereturndelay_t crdelay);
			// send a delay after each carriage return character
			// termios flag: CRDLY
		void	delayAfterTab(tabdelay_t tabdelay);
			// send a delay after each tab character
			// termios flag: TABDLY
		void	delayAfterBackSpace(backspacedelay_t bsdelay);
			// send a delay after each backspace character
			// termios flag: BSDLY
		void	delayAfterVerticalTab(verticaltabdelay_t vtdelay);
			// send a delay after each vertical tab character
			// termios flag: VTDLY
		void	delayAfterFormFeed(formfeeddelay_t ffdelay);
			// send a delay after each form feed character
			// termios flag: FFDLY


		// getters...
		bool	postProcessOutput();
			// returns true if implementation-defined output
			// processing is enabled
		bool	outputUpperCase();
			// returns true if lowercase characters are mapped to
			// uppercase on output
		bool	mapNewLineToCarriageReturnNewLineOnOutput();
			// returns true if new lines are mapped to carriage
			// return/new line on output
		bool	mapCarriageReturnToNewLineOnOutput();
			// returns true if carriage returns are mapped to new
			// lines on output
		bool	dontOutputCarriageReturnAtColumnZero();
			// returns true if carriage returns aren't sent at
			// column 0
		bool	mapNewLineToCarriageReturnOnOutput();
			// returns true if new lines are mapped to carriage
			// returns on output
		bool	useFillCharactersForDelay();
			// returns true if fill characters are sent for delay
			// instead of using a timed delay
		bool	useDelForFill();
			// returns true if the DEL character is used instead of
			// NULL for the fill character
		bool	expandTabToSpaces();
			// returns true if the tabs are mapped to spaces
		newlinedelay_t		delayAfterNewLine();
			// returns the delay that is sent after new line
			// characters
		carriagereturndelay_t	delayAfterCarriageReturn();
			// returns the delay that is sent after carriage return
			// characters
		tabdelay_t		delayAfterTab();
			// returns the delay that is sent after tab characters
		backspacedelay_t	delayAfterBackSpace();
			// returns the delay that is sent after backspace
			// characters
		verticaltabdelay_t	delayAfterVerticalTab();
			// returns the delay that is sent after vertical tab
			// characters
		formfeeddelay_t		delayAfterFormFeed();
			// returns the delay that is sent after form feed
			// characters



		// control characters
		void	interruptCharacter(unsigned char character);
			// set the character that will cause a SIGINT to be
			// sent to the process when generateSignals() is set
			// true
			// termios flag: VINTR
		void	quitCharacter(unsigned char character);
			// set the character that will cause a SIGQUIT to be
			// sent to the process when generateSignals() is set
			// true
			// termios flag: VQUIT
		void	eraseCharacter(unsigned char character);
			// set the character that will cause a character erase
			// when canonicalInput() is set to true
			// termios flag: VERASE
		void	killCharacter(unsigned char character);
			// set the character that will cause a line erase
			// when canonicalInput() is set to true
			// termios flag: VKILL
		void	endOfFileCharacter(unsigned char character);
			// set the character that will cause the pending tty
			// buffer to be sent to the program without waiting for
			// end-of-line and read()'s to return 0 when
			// canonicalInput() is set to true
			// termios flag: VEOF
		void	endOfLineCharacter(unsigned char character);
			// set the end-of-line character, recognized when 
			// canonicalInput() is set to true
			// termios flag: VEOL
		void	secondEndOfLineCharacter(unsigned char character);
			// set the "other" end-of-line character, recognized
			// when canonicalInput() is set to true
			// termios flag: VEOL2
		void	switchCharacer(unsigned char character);
			// set the switch character
			// termios flag: VSWTCH/VSWTC
		void	startCharacter(unsigned char character);
			// set the start character for XON/XOFF flow control
			// termios flag: VSTART
		void	stopCharacter(unsigned char character);
			// set the stop character for XON/XOFF flow control
			// termios flag: VSTOP
		void	suspendCharacter(unsigned char character);
			// set the character that will cause a SIGSUSP to be
			// sent to the process when generateSignals() is set
			// true
			// termios flag: VSUSP
		void	delayedSuspendCharacter(unsigned char character);
			// set the character that will cause a SIGTSTP to be
			// sent to the process when generateSignals() and
			// extendedFunctions() are set true
			// termios flag: VDSUSP
		void	literalNextCharcter(unsigned char character);
			// set the character that "quotes" the next character,
			// depriving it of special meaning, recognized when
			// extendedFunctions() is set true
			// termios flag: VLNEXT
		void	wordEraseCharcter(unsigned char character);
			// set the word erase character, recognized when
			// canonicalInput() and extendedFunctions() are set true
			// termios flag: VWERASE

		void	reprintCharacter(unsigned char character);
			// set the character that causes unread characters to
			// be reprinted, recognized when canonicalInput() and
			// extendedFunctions() are set true
			// termios flag: VREPRINT
		void	discardPendingOutputCharacter(unsigned char character);
			// set the character that toggles discarding pending
			// output, recognized when extendedFunctions() is set
			// true
			// termios flag: VDISCARD

		void	statusRequestCharacter(unsigned char character);
			// set the status request character
			// termios flag: VSTATUS

		void	readThreshold(unsigned char count);
			// set the number of characters that must be read
			// before a read() will begin waiting for readTimeout()
			// deciseconds before falling through
			// termios flag: VMIN
		void	readTimeout(unsigned char deciseconds);
			// set the number of deciseconds that a read() will
			// wait after reading readThreshold() characters before
			// falling through
			// termios flag: VTIME


		// getters...
		unsigned char	interruptCharacter();
			// returns the character that will cause a SIGINT to be
			// sent to the process when generateSignals() is set
			// true
		unsigned char	quitCharacter();
			// returns the character that will cause a SIGQUIT to be
			// sent to the process when generateSignals() is set
			// true
		unsigned char	eraseCharacter();
			// returns the character that will cause a character
			// erase when canonicalInput() is set to true
		unsigned char	killCharacter();
			// returns the character that will cause a line erase
			// when canonicalInput() is set to true
		unsigned char	endOfFileCharacter();
			// returns the character that will cause the pending tty
			// buffer to be sent to the program without waiting for
			// end-of-line and read()'s to return 0 when
			// canonicalInput() is set to true
		unsigned char	endOfLineCharacter();
			// returns the end-of-line character, recognized when 
			// canonicalInput() is set to true
		unsigned char	secondEndOfLineCharacter();
			// returns the "other" end-of-line character, recognized
			// when canonicalInput() is set to true
		unsigned char	switchCharacer();
			// returns the switch character
		unsigned char	startCharacter();
			// returns the start character for XON/XOFF flow control
		unsigned char	stopCharacter();
			// returns the stop character for XON/XOFF flow control
		unsigned char	suspendCharacter();
			// returns the character that will cause a SIGSUSP to be
			// sent to the process when generateSignals() is set
			// true
		unsigned char	delayedSuspendCharacter();
			// returns the character that will cause a SIGTSTP to be
			// sent to the process when generateSignals() and
			// extendedFunctions() are set true
		unsigned char	literalNextCharcter();
			// returns the character that "quotes" the next
			// character, depriving it of special meaning,
			// recognized when extendedFunctions() is set true
		unsigned char	wordEraseCharcter();
			// returns the word erase character, recognized when
			// canonicalInput() and extendedFunctions() are set true


		unsigned char	reprintCharacter();
			// returns the character that causes unread characters
			// to be reprinted, recognized when canonicalInput() and
			// extendedFunctions() are set true
		unsigned char	discardPendingOutputCharacter();
			// returns the character that toggles discarding pending
			// output, recognized when extendedFunctions() is set
			// true

		unsigned char	statusRequestCharacter();
			// returns the status request character

		unsigned char	readThreshold();
			// returns the number of characters that must be read
			// before a read() will begin waiting for readTimeout()
			// deciseconds before falling through
		unsigned char	readTimeout();
			// returns the number of deciseconds that a read() will
			// wait after reading readThreshold() characters before
			// falling through

	#include <rudiments/private/serialportprofile.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
