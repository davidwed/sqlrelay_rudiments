// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORTPROFILE_H
#define RUDIMENTS_SERIALPORTPROFILE_H

#include <rudiments/private/serialportprofileincludes.h>

// cfmakeraw, cfsetspeed???
// wraps struct termios

class serialportprofile {

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

	public:

			serialportprofile();

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
		void	baud(baudrate_t baud);
			// set the (input and output) baud rate
			// default is 0
		bool	inputBaud(baudrate_t baud);
			// set the input baud rate
			// default is 0
		bool	outputBaud(baudrate_t baud);
			// set the output baud rate
			// default is 0
		void	characterSize(charsize_t size);
			// set the character size
			// default is 5 bits
		void	twoStopBits(bool truefalse);
			// use two stop bits if "truefalse" is true
			// or one stop bit if "truefalse" is false
			// default is one stop bit
		void	receiverOn(bool truefalse);
			// turn the receiver on/off
			// (enables serial port to read incoming data)
			// default is off
			// (you probably always want to set this on)
		void	parityCheck(bool truefalse);
			// turn parity checking on/off
			// default is off
		void	oddParity(bool truefalse);
			// use odd parity if "truefalse" is true
			// or even parity if "truefalse" is false
			// default is even parity
		void	hangupOnClose(bool truefalse);
			// lower modem control lines after
			// last process closes the device (hang up)
			// default is not to hang up on close
		void	ignoreModemControlLines(bool truefalse);
			// ignore (possibly sporadic) job control and hangup
			// signals that could terminate or otherwise control
			// the program
			// default is not to ignore
			// (this should be set to true when communicating
			// with a device connected directly to the computer as
			// opposed to over a modem)
		#ifdef LOBLK
		void	blockJobControlOutput(bool truefalse);
			// block output from a nonconcurrent shell layer
			// default is not to block
		#endif
		void	hardwareFlowControl(bool truefalse);
			// use RTS/CTS flow control
			// default is not to use RTS/CTS flow control

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
		void	canonicalInput(bool truefalse);
			// enable special characters EOF, EOL, EOL2, ERASE,
			// KILL, LNEXT, REPRINT, STATUS and WERASE, buffer by
			// lines
			// default is not to enable the special characters and
			// not buffer by lines
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
		#endif
		void	echoInput(bool truefalse);
			// If cannonicalInput() is also set true then echo
			// input.
		void	eraseCharactersOn(bool truefalse);
			// If cannonicalInput() is also set true, character
			// set using eraseCharacter() erases the preceeding
			// character and character set using
			// wordEraseCharacter() erases the preceeding word.
		void	killCharacterOn(bool truefalse);
			// If cannonicalInput() is also set true, character
			// set using killCharacter() erases the current line.
		void	echoNewLine(bool truefalse);
			// If cannonicalInput() is also set true, new line
			// characters are echoes even if echoInput() is set
			// false.
		void	extendedFunctions(bool truefalse);
			// If cannonicalInput() is also set true, enables
			// characters set by secondEndOfLineCharacter(),
			// reprintCharacter() and wordEraseCharacter().  Also
			// enables lowerCase().
		void	echoControlCharacters(bool truefalse);
			// If echoInput() is also set true, control characters
			// are echoed as ^X where X is the ascii code for the
			// character plus 0x40.
		#ifdef ECHOPRT
		void	echoErasedCharacter(bool truefalse);
			// If cannonicalInput() and echoInput() are also set
			// true, characters are printed as they are erased.
		#endif
		void	emulateKill(bool truefalse);
			// If cannonicalInput() is also set, the character set
			// using killCharacter() causes the line to be erased
			// by erasing each character on the line.
			// (useful when a terminal doesn't support the KILL
			// character but does support the ERASE character)
		void	noFlushAfterInterruptOrQuit(bool truefalse);
			// Disables flushing of the input/output queues when
			// generating SIGINT, SIGQUIT or SIGSUSP signals.
		#ifdef PENDIN
		void	retypePendingCharacters(bool truefalse);
			// All characters in the input queue are reprinted when
			// the next character is read.
		#endif
		void	sendSignalForBackgroundOutput(bool truefalse);
			// Send the SIGTTOU signal to the process group of a
			// background process which tries to write to its
			// controlling terminal.

		// getters...
		bool	generateSignals();
		bool	canonicalInput();
		#ifdef XCASE
		bool	escapedUpperCase();
		#endif
		bool	echoInput();
		bool	eraseCharactersOn();
		bool	killCharacterOn();
		bool	echoNewLine();
		bool	extendedFunctions();
		bool	echoControlCharacters();
		#ifdef ECHOPRT
		bool	echoErasedCharacter();
		#endif
		bool	emulateKill();
		bool	noFlushAfterInterruptOrQuit();
		#ifdef PENDIN
		bool	retypePendingCharacters();
		#endif
		bool	sendSignalForBackgroundOutput();



		// input options
		void	inputParityCheck(bool truefalse);
			// INPCK
		void	markParityErrors(bool truefalse);
			// PARMRK
		void	stripParityBits(bool truefalse);
			// ISTRIP
		void	softwareFlowControlOnOutput(bool truefalse);
			// IXON
		void	softwareFlowControlOnInput(bool truefalse);
			// IXOFF
		void	anyCharacterStartsFlow(bool truefalse);
			// IXANY
		void	ignoreBreak(bool truefalse);
			// IGNBRK
		void	sendSignalOnBreak(bool truefalse);
			// BRKINT
		void	mapNewLineToCarriageReturnOnInput(bool truefalse);
			// INLCR
		#ifdef ONOEOT
		void	discardEndOfTransmission(bool truefalse);
			// ONOEOT
		#endif
		void	ignoreCarriageReturn(bool truefalse);
			// IGNCR
		void	mapCarriageReturnToNewLineOnInput(bool truefalse);
			// ICRNL
		#ifdef IUCLC
		void	lowerCase(bool truefalse);
			// IUCLC
		#endif
		void	bellIfLineTooLong(bool truefalse);
			// IMAXBEL

		// getters...
		bool	inputParityCheck();
		bool	markParityErrors();
		bool	stripParityBits();
		bool	softwareFlowControlOnOutput();
		bool	softwareFlowControlOnInput();
		bool	anyCharacterStartsFlow();
		bool	ignoreBreak();
		bool	sendSignalOnBreak();
		bool	mapNewLineToCarriageReturnOnInput();
		#ifdef ONOEOT
		bool	discardEndOfTransmission();
		#endif
		bool	ignoreCarriageReturn();
		bool	mapCarriageReturnToNewLineOnInput();
		#ifdef IUCLC
		bool	lowerCase();
		#endif
		bool	bellIfLineTooLong();




		// output options
		void	postProcessOutput(bool truefalse);
			// OPOST
		#ifdef OLCUC
		void	outputUpperCase(bool truefalse);
			// OLCUC
		#endif
		void	mapNewLineToCarriageReturnNewLineOnOutput(
							bool truefalse);
			// ONLCR
		void	mapCarriageReturnToNewLineOnOutput(bool truefalse);
			// OCRNL
		void	outputCarriageReturnAtColumnZero(bool truefalse);
			// true=~ONOCR
		void	mapNewLineToCarriageReturnOnOutput(bool truefalse);
			// ONLRET
		#ifdef OFILL
		void	useFillCharactersForDelay(bool truefalse);
			// OFILL
		#endif
		#ifdef OFDEL
		void	useDelForFill(bool truefalse);
			// OFDEL
		#endif
		void	expandTabToSpaces(bool truefalse);
			// TAB3/XTABS

		#ifdef NLDLY
		void	delayAfterNewLine(newlinedelay_t nldelay);
			// NLDLY
		#endif
		#ifdef CRDLY
		void	delayAfterCarriageReturn(carriagereturndelay_t crdelay);
			// CRDLY
		#endif
		#ifdef TABDLY
		void	delayAfterTab(tabdelay_t tabdelay);
			// TABDLY
		#endif
		#ifdef BSDLY
		void	delayAfterBackSpace(backspacedelay_t bsdelay);
			// BSDLY
		#endif
		#ifdef VTDLY
		void	delayAfterVerticalTab(verticaltabdelay_t vtdelay);
			// VTDLY
		#endif
		#ifdef FFDLY
		void	delayAfterFormFeed(formfeeddelay_t ffdelay);
			// FFDLY
		#endif


		// getters...
		bool	postProcessOutput();
		#ifdef OLCUC
		bool	outputUpperCase();
		#endif
		bool	mapNewLineToCarriageReturnNewLineOnOutput();
		bool	mapCarriageReturnToNewLineOnOutput();
		bool	outputCarriageReturnAtColumnZero();
		bool	mapNewLineToCarriageReturnOnOutput();
		#ifdef OFILL
		bool	useFillCharactersForDelay();
		#endif
		#ifdef OFDEL
		bool	useDelForFill();
		#endif
		bool	expandTabToSpaces();
		#ifdef NLDLY
		newlinedelay_t		delayAfterNewLine();
		#endif
		#ifdef CRDLY
		carriagereturndelay_t	delayAfterCarriageReturn();
		#endif
		#ifdef TABDLY
		tabdelay_t		delayAfterTab();
		#endif
		#ifdef BSDLY
		backspacedelay_t	delayAfterBackSpace();
		#endif
		#ifdef VTDLY
		verticaltabdelay_t	delayAfterVerticalTab();
		#endif
		#ifdef FFDLY
		formfeeddelay_t		delayAfterFormFeed();
		#endif



		// control characters
		void	interruptCharacter(cc_t character);
			// VINTR
		void	quitCharacter(cc_t character);
			// VQUIT
		void	eraseCharacter(cc_t character);
			// VERASE
		void	killCharacter(cc_t character);
			// VKILL
		void	endOfFileCharacter(cc_t character);
			// VEOF
		void	endOfLineCharacter(cc_t character);
			// VEOL
		void	secondEndOfLineCharacter(cc_t character);
			// VEOL2
		void	switchCharacer(cc_t character);
			// VSWITCH
		void	startCharacter(cc_t character);
			// VSTART
		void	stopCharacter(cc_t character);
			// VSTOP
		void	suspendCharacter(cc_t character);
			// VSUSP
		#ifdef VDSUSP
		void	delayedSuspendCharacter(cc_t character);
			// VDSUSP
		#endif
		void	literalNextCharcter(cc_t character);
			// VLNEXT
		void	wordEraseCharcter(cc_t character);
			// VWERASE

		void	reprintCharacter(cc_t character);
			// VREPRINT
		void	discardPendingOutputCharacter(cc_t character);
			// VDISCARD, IEXTEN must be on

		#ifdef VSTATUS
		void	statusRequestCharacter(cc_t character);
			// VSTATUS
		#endif

		void	readThreshold(cc_t count);
		void	readTimeout(cc_t deciseconds);


		// getters...
		cc_t	interruptCharacter();
		cc_t	quitCharacter();
		cc_t	eraseCharacter();
		cc_t	killCharacter();
		cc_t	endOfFileCharacter();
		cc_t	endOfLineCharacter();
		cc_t	secondEndOfLineCharacter();
		cc_t	switchCharacer();
		cc_t	startCharacter();
		cc_t	stopCharacter();
		cc_t	suspendCharacter();
		cc_t	delayedSuspendCharacter();
		cc_t	literalNextCharcter();
		cc_t	wordEraseCharcter();

		cc_t	reprintCharacter();
		cc_t	discardPendingOutputCharacter();

		cc_t	statusRequestCharacter();

		cc_t	readThreshold();
		cc_t	readTimeout();




		termios	*getTermios();

	#include <rudiments/private/serialportprofile.h>
};

#ifdef HAVE_RUDIMENTS_INLINES
	#include <rudiments/private/serialportprofileinlines.h>
#endif

#endif
