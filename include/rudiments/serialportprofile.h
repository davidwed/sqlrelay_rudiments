// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORTPROFILE_H
#define RUDIMENTS_SERIALPORTPROFILE_H

#include <rudiments/private/serialportprofileincludes.h>

// cfmakeraw, cfsetspeed???
// wraps struct termios

class serialportprofile {
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
			baud_0=B0,
			baud_50=B50,
			baud_75=B75,
			baud_110=B110,
			baud_134=B134,
			baud_150=B150,
			baud_200=B200,
			baud_300=B300,
			baud_600=B600,
			baud_1200=B1200,
			baud_1800=B1800,
			baud_2400=B2400,
			baud_4800=B4800,
			baud_9600=B9600,
			#if defined(B57600)
			baud_19200=B19200,
			#elif defined(EXTA)
			baud_19200=EXTA,
			#endif
			#if defined(B38400)
			baud_38400=B38400,
			#elif defined(EXTB)
			baud_38400=EXTB,
			#endif
			#ifdef B57600
			baud_57600=B57600,
			#endif
			#ifdef B76800
			baud_76800=B76800,
			#endif
			#ifdef B115200
			baud_115200=B115200,
			#endif
			#ifdef B230400
			baud_230400=B230400,
			#endif
			#ifdef B460800
			baud_460800=B460800,
			#endif
			#ifdef B500000
			baud_500000=B500000,
			#endif
			#ifdef B576000
			baud_576000=B576000,
			#endif
			#ifdef B921600
			baud_921600=B921600,
			#endif
			#ifdef B1000000
			baud_1000000=B1000000,
			#endif
			#ifdef B1142000
			baud_1152000=B1152000,
			#endif
			#ifdef B1500000
			baud_1500000=B1500000,
			#endif
			#ifdef B2000000
			baud_2000000=B2000000,
			#endif
			#ifdef B2500000
			baud_2500000=B2500000,
			#endif
			#ifdef B3000000
			baud_3000000=B3000000,
			#endif
			#ifdef B3500000
			baud_3500000=B3500000,
			#endif
			#ifdef B4000000
			baud_4000000=B4000000,
			#endif
		};

		enum	charsize_t {
			cs_5=CS5,
			cs_6=CS6,
			cs_7=CS7,
			cs_8=CS8
		};

		#ifdef NLDLY
		enum	newlinedelay_t {
			nl_none=NL0,
			nl_100=NL1
		};
		#endif

		#ifdef CRDLY
		enum	carriagereturndelay_t {
			cr_none=CR0,
			cr_depends=CR1,
			cr_100=CR2,
			cr_150=CR3
		};
		#endif

		#ifdef TABDLY
		enum	tabdelay_t {
			td_none=TAB0,
			td_depends=TAB1,
			td_100=TAB2
		};
		#endif

		#ifdef BSDLY
		enum	backspacedelay_t {
			bs_none=BS0,
			bs_50=BS1
		};
		#endif

		#ifdef VTDLY
		enum	verticaltabdelay_t {
			vt_none=VT0,
			vt_2=VT1
		};
		#endif

		#ifdef FFDLY
		enum	formfeeddelay_t {
			ff_none=FF0,
			ff_2=FF1
		};
		#endif

			serialportprofile();
			~serialportprofile();

		void	setControlOptions(tcflag_t flags);
			// set control options to "flags"
		void	setLocalOptions(tcflag_t flags);
			// set local options to "flags"
		void	setInputOptions(tcflag_t flags);
			// set input options to "flags"
		void	setOutputOptions(tcflag_t flags);
			// set output options to "flags"
		void	setControlCharacters(cc_t *c_cc);
			// Set control characters to the values pointed to by
			// "c_cc".  Note that c_cc must be an array of length
			// NCCS.
		void	setOptions(termios *newtio);
			// set all options and control characters to
			// the values set in "newtio"

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
		#ifdef LOBLK
		void	blockJobControlOutput(bool truefalse);
			// block output from a nonconcurrent shell layer
			// default is not to block
			// termios flag: LOBLK
		#endif
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
		#ifdef XCASE
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
		#endif
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
		#ifdef ECHOPRT
		void	echoErasedCharacter(bool truefalse);
			// If cannonicalInput() and echoInput() are also set
			// true, characters are printed as they are erased.
			// termios flag: ECHOPRT
		#endif
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
		#ifdef PENDIN
		void	retypePendingCharacters(bool truefalse);
			// All characters in the input queue are reprinted when
			// the next character is read.
			// termios flag: PENDIN
		#endif
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
		#ifdef XCASE
		bool	escapedUpperCase();
			// Returns true if on input, characters are being
			// converted from upper to lower case unless they are
			// preceeded by a \ and on output lower case characters
			// are being converted to upper case and upper case
			// characters are being preceeded with a \.
		#endif
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
		#ifdef ECHOPRT
		bool	echoErasedCharacter();
			// returns true if characters are being printed as they
			// are erased
		#endif
		bool	emulateKill();
			// returns true if the character set using
			// killCharacter() causes the line to be erased
			// by erasing each character on the line
		bool	noFlushAfterInterruptOrQuit();
			// returns true if flushing of the input/output queues
			// when generating SIGINT, SIGQUIT or SIGSUSP signals
			// is enabled
		#ifdef PENDIN
		bool	retypePendingCharacters();
			// returns true if all characters in the input queue
			// are reprinted when the next character is read
		#endif
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
		#ifdef ONOEOT
		void	discardEndOfTransmission(bool truefalse);
			// termios flat: ONOEOT
		#endif
		void	ignoreCarriageReturn(bool truefalse);
			// ignore carriage return on input
			// termios flag: IGNCR
		void	mapCarriageReturnToNewLineOnInput(bool truefalse);
			// translate carriage return to new line on input
			// termios flag: ICRNL
		#ifdef IUCLC
		void	lowerCase(bool truefalse);
			// map uppercase characters to lowercase on input
			// termios flag: IUCLC
		#endif
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
		#ifdef ONOEOT
		bool	discardEndOfTransmission();
		#endif
		bool	ignoreCarriageReturn();
			// returns true if carriage returns are ignored on input
		bool	mapCarriageReturnToNewLineOnInput();
			// returns true if carriage returns are mapped to new
			// lines on input
		#ifdef IUCLC
		bool	lowerCase();
			// returns true if uppercase characters are mapped to
			// lowercase on input
		#endif
		bool	bellIfLineTooLong();
			// returns true if the bell will be rung when the input
			// queue is full




		// output options
		void	postProcessOutput(bool truefalse);
			// enables implementation-defined output processing
			// termios flag: OPOST
		#ifdef OLCUC
		void	outputUpperCase(bool truefalse);
			// map lowercase characters to uppercase on output
			// termios flag: OLCUC
		#endif
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
		#ifdef OFILL
		void	useFillCharactersForDelay(bool truefalse);
			// send fill characters for delay instead of using a
			// timed delay
			// termios flag: OFILL
		#endif
		#ifdef OFDEL
		void	useDelForFill(bool truefalse);
			// use the DEL character instead of NULL for the fill
			// character
			// termios flag: OFDEL
		#endif
		void	expandTabToSpaces(bool truefalse);
			// map tabs to spaces
			// termios flag: XTAGS/OXTABS/TAB3

		#ifdef NLDLY
		void	delayAfterNewLine(newlinedelay_t nldelay);
			// send a delay after each new line character
			// termios flag: NLDLY
		#endif
		#ifdef CRDLY
		void	delayAfterCarriageReturn(carriagereturndelay_t crdelay);
			// send a delay after each carriage return character
			// termios flag: CRDLY
		#endif
		#ifdef TABDLY
		void	delayAfterTab(tabdelay_t tabdelay);
			// send a delay after each tab character
			// termios flag: TABDLY
		#endif
		#ifdef BSDLY
		void	delayAfterBackSpace(backspacedelay_t bsdelay);
			// send a delay after each backspace character
			// termios flag: BSDLY
		#endif
		#ifdef VTDLY
		void	delayAfterVerticalTab(verticaltabdelay_t vtdelay);
			// send a delay after each vertical tab character
			// termios flag: VTDLY
		#endif
		#ifdef FFDLY
		void	delayAfterFormFeed(formfeeddelay_t ffdelay);
			// send a delay after each form feed character
			// termios flag: FFDLY
		#endif


		// getters...
		bool	postProcessOutput();
			// returns true if implementation-defined output
			// processing is enabled
		#ifdef OLCUC
		bool	outputUpperCase();
			// returns true if lowercase characters are mapped to
			// uppercase on output
		#endif
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
		#ifdef OFILL
		bool	useFillCharactersForDelay();
			// returns true if fill characters are sent for delay
			// instead of using a timed delay
		#endif
		#ifdef OFDEL
		bool	useDelForFill();
			// returns true if the DEL character is used instead of
			// NULL for the fill character
		#endif
		bool	expandTabToSpaces();
			// returns true if the tabs are mapped to spaces
		#ifdef NLDLY
		newlinedelay_t		delayAfterNewLine();
			// returns the delay that is sent after new line
			// characters
		#endif
		#ifdef CRDLY
		carriagereturndelay_t	delayAfterCarriageReturn();
			// returns the delay that is sent after carriage return
			// characters
		#endif
		#ifdef TABDLY
		tabdelay_t		delayAfterTab();
			// returns the delay that is sent after tab characters
		#endif
		#ifdef BSDLY
		backspacedelay_t	delayAfterBackSpace();
			// returns the delay that is sent after backspace
			// characters
		#endif
		#ifdef VTDLY
		verticaltabdelay_t	delayAfterVerticalTab();
			// returns the delay that is sent after vertical tab
			// characters
		#endif
		#ifdef FFDLY
		formfeeddelay_t		delayAfterFormFeed();
			// returns the delay that is sent after form feed
			// characters
		#endif



		// control characters
		void	interruptCharacter(cc_t character);
			// set the character that will cause a SIGINT to be
			// sent to the process when generateSignals() is set
			// true
			// termios flag: VINTR
		void	quitCharacter(cc_t character);
			// set the character that will cause a SIGQUIT to be
			// sent to the process when generateSignals() is set
			// true
			// termios flag: VQUIT
		void	eraseCharacter(cc_t character);
			// set the character that will cause a character erase
			// when canonicalInput() is set to true
			// termios flag: VERASE
		void	killCharacter(cc_t character);
			// set the character that will cause a line erase
			// when canonicalInput() is set to true
			// termios flag: VKILL
		void	endOfFileCharacter(cc_t character);
			// set the character that will cause the pending tty
			// buffer to be sent to the program without waiting for
			// end-of-line and read()'s to return 0 when
			// canonicalInput() is set to true
			// termios flag: VEOF
		void	endOfLineCharacter(cc_t character);
			// set the end-of-line character, recognized when 
			// canonicalInput() is set to true
			// termios flag: VEOL
		void	secondEndOfLineCharacter(cc_t character);
			// set the "other" end-of-line character, recognized
			// when canonicalInput() is set to true
			// termios flag: VEOL2
		void	switchCharacer(cc_t character);
			// set the switch character
			// termios flag: VSWTCH/VSWTC
		void	startCharacter(cc_t character);
			// set the start character for XON/XOFF flow control
			// termios flag: VSTART
		void	stopCharacter(cc_t character);
			// set the stop character for XON/XOFF flow control
			// termios flag: VSTOP
		void	suspendCharacter(cc_t character);
			// set the character that will cause a SIGSUSP to be
			// sent to the process when generateSignals() is set
			// true
			// termios flag: VSUSP
		#ifdef VDSUSP
		void	delayedSuspendCharacter(cc_t character);
			// set the character that will cause a SIGTSTP to be
			// sent to the process when generateSignals() and
			// extendedFunctions() are set true
			// termios flag: VDSUSP
		#endif
		void	literalNextCharcter(cc_t character);
			// set the character that "quotes" the next character,
			// depriving it of special meaning, recognized when
			// extendedFunctions() is set true
			// termios flag: VLNEXT
		void	wordEraseCharcter(cc_t character);
			// set the word erase character, recognized when
			// canonicalInput() and extendedFunctions() are set true
			// termios flag: VWERASE

		void	reprintCharacter(cc_t character);
			// set the character that causes unread characters to
			// be reprinted, recognized when canonicalInput() and
			// extendedFunctions() are set true
			// termios flag: VREPRINT
		void	discardPendingOutputCharacter(cc_t character);
			// set the character that toggles discarding pending
			// output, recognized when extendedFunctions() is set
			// true
			// termios flag: VDISCARD

		#ifdef VSTATUS
		void	statusRequestCharacter(cc_t character);
			// set the status request character
			// termios flag: VSTATUS
		#endif

		void	readThreshold(cc_t count);
			// set the number of characters that must be read
			// before a read() will begin waiting for readTimeout()
			// deciseconds before falling through
			// termios flag: VMIN
		void	readTimeout(cc_t deciseconds);
			// set the number of deciseconds that a read() will
			// wait after reading readThreshold() characters before
			// falling through
			// termios flag: VTIME


		// getters...
		cc_t	interruptCharacter();
			// returns the character that will cause a SIGINT to be
			// sent to the process when generateSignals() is set
			// true
		cc_t	quitCharacter();
			// returns the character that will cause a SIGQUIT to be
			// sent to the process when generateSignals() is set
			// true
		cc_t	eraseCharacter();
			// returns the character that will cause a character
			// erase when canonicalInput() is set to true
		cc_t	killCharacter();
			// returns the character that will cause a line erase
			// when canonicalInput() is set to true
		cc_t	endOfFileCharacter();
			// returns the character that will cause the pending tty
			// buffer to be sent to the program without waiting for
			// end-of-line and read()'s to return 0 when
			// canonicalInput() is set to true
		cc_t	endOfLineCharacter();
			// returns the end-of-line character, recognized when 
			// canonicalInput() is set to true
		cc_t	secondEndOfLineCharacter();
			// returns the "other" end-of-line character, recognized
			// when canonicalInput() is set to true
		cc_t	switchCharacer();
			// returns the switch character
		cc_t	startCharacter();
			// returns the start character for XON/XOFF flow control
		cc_t	stopCharacter();
			// returns the stop character for XON/XOFF flow control
		cc_t	suspendCharacter();
			// returns the character that will cause a SIGSUSP to be
			// sent to the process when generateSignals() is set
			// true
		#ifdef VDSUSP
		cc_t	delayedSuspendCharacter();
			// returns the character that will cause a SIGTSTP to be
			// sent to the process when generateSignals() and
			// extendedFunctions() are set true
		#endif
		cc_t	literalNextCharcter();
			// returns the character that "quotes" the next
			// character, depriving it of special meaning,
			// recognized when extendedFunctions() is set true
		cc_t	wordEraseCharcter();
			// returns the word erase character, recognized when
			// canonicalInput() and extendedFunctions() are set true


		cc_t	reprintCharacter();
			// returns the character that causes unread characters
			// to be reprinted, recognized when canonicalInput() and
			// extendedFunctions() are set true
		cc_t	discardPendingOutputCharacter();
			// returns the character that toggles discarding pending
			// output, recognized when extendedFunctions() is set
			// true

		cc_t	statusRequestCharacter();
			// returns the status request character

		cc_t	readThreshold();
			// returns the number of characters that must be read
			// before a read() will begin waiting for readTimeout()
			// deciseconds before falling through
		cc_t	readTimeout();
			// returns the number of deciseconds that a read() will
			// wait after reading readThreshold() characters before
			// falling through




		termios	*getTermios();
			// returns the struct termios used internally

	#include <rudiments/private/serialportprofile.h>
};

#endif
