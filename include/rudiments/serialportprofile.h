// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORTPROFILE_H
#define RUDIMENTS_SERIALPORTPROFILE_H

#include <rudiments/private/serialportprofileincludes.h>

class serialportprofileprivate;

// cfmakeraw, cfsetspeed???
// wraps struct termios

/** A serialportprofile is a set of parameters for a serial port such as baud
 *  rate, parity, bits-per character and lots of other settings.  On some
 *  systems, these are referred to as "termios".
 *
 *  In antiquity, teletypes and dumb terminals were the most common
 *  serial-attached devices and they required these to be set to very specific
 *  values.  These days serial ports are more often used for talking to
 *  peripherals on embedded systems but many of the same concepts still
 *  apply. */
class RUDIMENTS_DLLSPEC serialportprofile {
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

		/** Creates an instance of the serialportprofile class. */
		serialportprofile();

		/** Deletes this instance of the serialportprofile class. */
		~serialportprofile();

		/** Sets the control options to system defaults. */
		void	defaultControlOptions();

		/** Sets the local options to system defaults. */
		void	defaultLocalOptions();

		/** Sets the input options to system defaults. */
		void	defaultInputOptions();

		/** Sets the output options to system defaults. */
		void	defaultOutputOptions();

		/** Sets the control characters to system defaults. */
		void	defaultControlCharacters();

		/** Sets all options and control
		 *  characters to system defaults. */
		void	defaultOptions();

		/** Set control characters to the values pointed to by
		 *  "c_cc".  Note that c_cc must be an array of length
		 *  NCCS. */
		void	setControlCharacters(const unsigned char *c_cc);

		/** Serial port configuration requirements are often
		 *  given as a 3 character string representing the
		 *  number of bits-per-char, the parity and the number
		 *  of stop bits such as "8n1", "7e2", etc.
		 *  This methods parses such a string and sets the
		 *  profile to match those values. */
		void	evalOptionsString(const char *string);

		/** Set input mode to "inputmode". */
		void		inputMode(inputmode_t inputmode);

		/** Get the current input mode setting. */
		inputmode_t	inputMode();

		/** Set flow control to "flowcontrol". */
		void		flowControl(flowcontrol_t flowcontrol);

		/** Get the current flow control setting. */
		flowcontrol_t	flowControl();

		/** Sets the (input and output) baud rate.  Default is 0. */
		void	baud(const char *baudrate);

		/** Sets the (input and output) baud rate.  Default is 0. */
		void	baud(baudrate_t baudrate);

		/** Sets the input baud rate.  Default is 0. */
		bool	inputBaud(const char *baudrate);

		/** Sets the input baud rate.  Default is 0. */
		bool	inputBaud(baudrate_t baudrate);

		/** Sets the output baud rate.  Default is 0. */
		bool	outputBaud(const char *baudrate);

		/** Sets the output baud rate.  Default is 0. */
		bool	outputBaud(baudrate_t baudrate);

		/** Set the character size.  Default is 5 bits.
		 *  Corresponding termios flag: CSIZE. */
		void	characterSize(charsize_t size);

		/** Use two stop bits if "truefalse" is true
		 *  or one stop bit if "truefalse" is false.
		 *  Default is one stop bit.
		 *  Corresponding termios flag: CSTOPB. */
		void	twoStopBits(bool truefalse);

		/** Turns the receiver on or off.
		 *  (Enables serial port to read incoming data or not.)
		 *  Default is off.
		 *  (You probably always want to set this on.)
		 *  Corresponding termios flag: CREAD. */
		void	receiverOn(bool truefalse);

		/** Turns parity checking on or off.
		 *  Default is off.
		 *  Corresponding termios flag: CPARENB. */
		void	parityCheck(bool truefalse);

		/** Use odd parity if "truefalse" is true
		 *  or even parity if "truefalse" is false.
		 *  Default is even parity.
		 *  Corresponding termios flag: PARODD. */
		void	oddParity(bool truefalse);

		/** Lower modem control lines after
		 *  last process closes the device (hang up).
		 *  Default is not to hang up on close.
		 *  Corresponding termios flag: HUPCL. */
		void	hangupOnClose(bool truefalse);

		/** Ignore (possibly sporadic) job control and hangup
		 *  signals that could terminate or otherwise control
		 *  the program.
		 *  Default is not to ignore.
		 *  (This should be set to true when communicating
		 *  with a device connected directly to the computer as
		 *  opposed to over a modem.)
		 *  Corresponding termios flag: CLOCAL. */
		void	ignoreModemControlLines(bool truefalse);

		/** Block output from a nonconcurrent shell layer.
		 *  Default is not to block.
		 *  Corresponding termios flag: LOBLK. */
		void	blockJobControlOutput(bool truefalse);

		/** Use RTS/CTS flow control.
		 *  Default is not to use RTS/CTS flow control.
		 *  Corresponding termios flags:
		 *  CRTSCTS/NEW_CRTSCTS/CRTS_IFLOW/CRTS_OFLOW. */
		void	hardwareFlowControl(bool truefalse);

		/** Returns current (input and output) baud. */
		baudrate_t	baud();

		/** Returns current input baud. */
		baudrate_t	inputBaud();

		/** Returns current output baud. */
		baudrate_t	outputBaud();

		/** Returns current character size. */
		charsize_t	characterSize();

		/** Returns true if two stop bits are being used. */
		bool		twoStopBits();

		/** Returns true if the receiver is on. */
		bool		receiverOn();

		/** Returns true if parity check is enabled. */
		bool		parityCheck();

		/** Returns true if odd parity is being used. */
		bool		oddParity();

		/** Returns true if the modem lines will be
		 *  lowered when the last device closes the
		 *  device. */
		bool		hangupOnClose();

		/** Returns true if (possibly sporadic) job
		 *  control and hangup signals are being ignored. */
		bool		ignoreModemControlLines();

		/** Returns true if output from a nonconcurrent
		 *  shell layer is being blocked. */
		bool		blockJobControlOutput();

		/** Returns true if RTS/CTS hardware flow
		 *  control is being used. */
		bool		hardwareFlowControl();

		/** Cause corresponding signals to be raised when any of
		 *  the INTR, QUIT, SUSP or DSUSP characters are received
		 *  default is not to raise signals
		 *  Corresponding termios flag: ISIG. */
		void	generateSignals(bool truefalse);

		/** Enable special characters EOF, EOL, EOL2, ERASE,
		 *  KILL, LNEXT, REPRINT, STATUS and WERASE, buffer by
		 *  lines.
		 *  Default is not to enable the special characters and
		 *  not buffer by lines.
		 *  Corresponding termios flag: ICANON. */
		void	canonicalInput(bool truefalse);

		/** If cannonicalInput() is also set true, converts
		 *  characters from input to lower case unless they are
		 *  preceeded by a \.  On output, preceeds each upper
		 *  case character with a \ and converts all lower case
		 *  characters to uppercase.
		 *  Default is not to use escaped upper case.
		 *  (This is used when communicating with terminals that
		 *  can display upper or lower case, but only transmit and
		 *  receive upper case.)
		 *  Corresponding termios flag: XCASE. */
		void	escapedUpperCase(bool truefalse);

		/** If cannonicalInput() is also set true then echo input.
		 *  Corresponding termios flag: ECHO. */
		void	echoInput(bool truefalse);

		/** If cannonicalInput() is also set true, character
		 *  set using eraseCharacter() erases the preceeding
		 *  character and character set using
		 *  wordEraseCharacter() erases the preceeding word.
		 *  Corresponding termios flag: ECHOE. */
		void	eraseCharactersOn(bool truefalse);

		/** If cannonicalInput() is also set true, character
		 *  set using killCharacter() erases the current line.
		 *  Corresponding termios flag: ECHOK. */
		void	killCharacterOn(bool truefalse);

		/** If cannonicalInput() is also set true, new line
		 *  characters are echoed even if echoInput() is set false.
		 *  Corresponding termios flag: ECHONL. */
		void	echoNewLine(bool truefalse);

		/** If cannonicalInput() is also set true, enables
		 *  characters set by secondEndOfLineCharacter(),
		 *  reprintCharacter() and wordEraseCharacter().  Also
		 *  enables lowerCase().
		 *  Corresponding termios flag: IEXTEN. */
		void	extendedFunctions(bool truefalse);

		/** If echoInput() is also set true, control characters
		 *  are echoed as ^X where X is the ascii code for the
		 *  character plus 0x40.
		 *  Corresponding termios flag: ECHOCTL. */
		void	echoControlCharacters(bool truefalse);

		/** If cannonicalInput() and echoInput() are also set
		 *  true, characters are printed as they are erased.
		 *  Corresponding termios flag: ECHOPRT. */
		void	echoErasedCharacter(bool truefalse);

		/** If cannonicalInput() is also set, the character set
		 *  using killCharacter() causes the line to be erased
		 *  by erasing each character on the line.
		 *  (Useful when a terminal doesn't support the KILL
		 *  character but does support the ERASE character.)
		 *  Corresponding termios flag: ECHOKE. */
		void	emulateKill(bool truefalse);

		/** Disables flushing of the input/output queues when
		 *  generating SIGINT, SIGQUIT or SIGSUSP signals.
		 *  Corresponding termios flag: NOFLSH. */
		void	noFlushAfterInterruptOrQuit(bool truefalse);

		/** All characters in the input queue are reprinted when
		 *  the next character is read.
		 *  Corresponding termios flag: PENDIN. */
		void	retypePendingCharacters(bool truefalse);

		/** Send the SIGTTOU signal to the process group of a
		 *  background process which tries to write to its
		 *  controlling terminal.
		 *  Corresponding termios flag: TOSTOP. */
		void	sendSignalForBackgroundOutput(bool truefalse);

		/** Returns true if corresponding signals will be raised
		 *  when any of the INTR, QUIT, SUSP or DSUSP characters
		 *  are received default is not to raise signals. */
		bool	generateSignals();

		/** Returns true if special characters EOF, EOL, EOL2,
		 *  ERASE, KILL, LNEXT, REPRINT, STATUS and WERASE are
		 *  enabled and buffering is being done by lines. */
		bool	canonicalInput();

		/** Returns true if on input, characters are being
		 *  converted from upper to lower case unless they are
		 *  preceeded by a \ and on output lower case characters
		 *  are being converted to upper case and upper case
		 *  characters are being preceeded with a \. */
		bool	escapedUpperCase();

		/** Returns true if input characters are being echoed. */
		bool	echoInput();

		/** Returns true if the character set using
		 *  eraseCharacter() erases the preceeding
		 *  character and the character set using
		 *  wordEraseCharacter() erases the preceeding word. */
		bool	eraseCharactersOn();

		/** Returns true if the character set using
		 *  killCharacter() erases the current line. */
		bool	killCharacterOn();

		/** Returns true if new line characters are being echoed
		 *  even if echoInput() is set false. */
		bool	echoNewLine();

		/** Returns true if characters set by
		 *  secondEndOfLineCharacter(), reprintCharacter() and
		 *  wordEraseCharacter() are enabled and lowerCase() is
		 *  enabled. */
		bool	extendedFunctions();

		/** Returns true if control characters are being echoed
		 *  as ^X where X is the ascii code for the character
		 *  plus 0x40. */
		bool	echoControlCharacters();

		/** Returns true if characters are being printed as they
		 *  are erased. */
		bool	echoErasedCharacter();

		/** Returns true if the character set using
		 *  killCharacter() causes the line to be erased
		 *  by erasing each character on the line. */
		bool	emulateKill();

		/** Returns true if flushing of the input/output queues
		 *  when generating SIGINT, SIGQUIT or SIGSUSP signals
		 *  is enabled. */
		bool	noFlushAfterInterruptOrQuit();

		/** Returns true if all characters in the input queue
		 *  are reprinted when the next character is read. */
		bool	retypePendingCharacters();

		/** Returns true if the SIGTTOU signal is being sent
		 *  to the process group of a background process which
		 *  tries to write to its controlling terminal. */
		bool	sendSignalForBackgroundOutput();

		/** Enable parity checking on input
		 *  Corresponding termios flag: INPCK. */
		void	inputParityCheck(bool truefalse);

		/** Ignore parity errors (ie. if a character has a
		 *  parity error, just return what we got, rather than
		 *  marking or converting it).
		 *  Corresponding termios flag: IGNPAR. */
		void	ignoreParityErrors(bool truefalse);

		/** Unless ignoreParityErrors() is set true, prefix a
		 *  character with a parity error with \337 \0.  The
		 *  default is to convert it to \0 unless
		 *  ignoreParityErrors() is set true.
		 *  Corresponding termios flag: PARMRK. */
		void	markParityErrors(bool truefalse);

		/** Set the 8th bit of each character (the parity bit) to 0.
		 *  Corresponding termios flag: ISTRIP. */
		void	stripParityBits(bool truefalse);

		/** Enable XON/XOFF flow control on output.
		 *  Corresponding termios flag: IXON. */
		void	softwareFlowControlOnOutput(bool truefalse);

		/** Enable XON/XOFF flow control on input.
		 *  Corresponding termios flag: IXOFF. */
		void	softwareFlowControlOnInput(bool truefalse);

		/** Enable any character to restart output.
		 *  Corresponding termios flag: IXANY. */
		void	anyCharacterStartsFlow(bool truefalse);

		/** Ignore BREAK character. 
		 *  Corresponding termios flag: IGNBRK. */
		void	ignoreBreak(bool truefalse);

		/** If ignoreBreak() isn't set true and a BREAK character
		 *  is received, flush input and output queues and send a
		 *  SIGINT if the serial port is the controlling terminal
		 *  of the process group.  If ignoreBreak() is not set,
		 *  the default is to return a \0 character or \377 \0
		 *  if markParityErrors() is set true.
		 *  Corresponding termios flag: BRKINT. */
		void	sendSignalOnBreak(bool truefalse);

		/** Translate new line to carriage return on input
		 *  termios flag: INLCR. */
		void	mapNewLineToCarriageReturnOnInput(bool truefalse);

		/** ??? 
		 *  Corresponding termios flag: ONOEOT. */
		void	discardEndOfTransmission(bool truefalse);

		/** Ignore carriage return on input.
		 *  Corresponding termios flag: IGNCR. */
		void	ignoreCarriageReturn(bool truefalse);

		/** Translate carriage return to new line on input.
		 *  Corresponding termios flag: ICRNL. */
		void	mapCarriageReturnToNewLineOnInput(bool truefalse);

		/** Map uppercase characters to lowercase on input.
		 *  Corresponding termios flag: IUCLC. */
		void	lowerCase(bool truefalse);

		/** Ring bell when input queue is full.
		 *  Corresponding termios flag: IMAXBEL. */
		void	bellIfLineTooLong(bool truefalse);

		/** Returns true if input parity checking is enabled. */
		bool	inputParityCheck();

		/** Returns true if parity errors are being ignored. */
		bool	ignoreParityErrors();

		/** Returns true if characters with parity errors are
		 *  prefixed with \377 \0. */
		bool	markParityErrors();

		/** Returns true if the 8th bit (the parity bit) of
		 *  each character is being set to 0. */
		bool	stripParityBits();

		/** Returns true if XON/XOFF flow control is enabled on
		 *  output. */
		bool	softwareFlowControlOnOutput();

		/** Returns true if XON/XOFF flow control is enabled on
		 *  input. */
		bool	softwareFlowControlOnInput();

		/** Returns true if any character will restart output. */
		bool	anyCharacterStartsFlow();

		/** Returns true if the BREAK character is being ignored. */
		bool	ignoreBreak();

		/** Returns true if input and output queues are flushed
		 *  and a SIGINT is sent to the process group if the
		 *  serial port is the controlling terminal when a BREAK
		 *  character is received. */
		bool	sendSignalOnBreak();

		/** Returns true if new lines are mapped to carriage
		 *  returns on input. */
		bool	mapNewLineToCarriageReturnOnInput();

		/** ??? */
		bool	discardEndOfTransmission();

		/** Returns true if carriage returns are ignored on input. */
		bool	ignoreCarriageReturn();

		/** Returns true if carriage returns are mapped to new
		 *  lines on input. */
		bool	mapCarriageReturnToNewLineOnInput();

		/** Returns true if uppercase characters are mapped to
		 *  lowercase on input. */
		bool	lowerCase();

		/** Returns true if the bell will be rung when the input
		 *  queue is full. */
		bool	bellIfLineTooLong();


		/** Enables implementation-defined output processing.
		 *  Corresponding termios flag: OPOST. */
		void	postProcessOutput(bool truefalse);

		/** Map lowercase characters to uppercase on output.
		 *  Corresponding termios flag: OLCUC. */
		void	outputUpperCase(bool truefalse);

		/** Map new line to carriage return/new line on output.
		 *  Corresponding termios flag: ONLCR. */
		void	mapNewLineToCarriageReturnNewLineOnOutput(
							bool truefalse);

		/** Map carriage return to new line on output.
		 *  Corresponding termios flag: OCRNL. */
		void	mapCarriageReturnToNewLineOnOutput(bool truefalse);

		/** Don't output carriage return at column 0.
		 *  Corresponding termios flag: ONOCR. */
		void	dontOutputCarriageReturnAtColumnZero(bool truefalse);

		/** Map new line to carriage return on output.
		 *  Corresponding termios flag: ONLRET. */
		void	mapNewLineToCarriageReturnOnOutput(bool truefalse);

		/** Send fill characters for delay instead of using a
		 *  timed delay.
		 *  Corresponding termios flag: OFILL. */
		void	useFillCharactersForDelay(bool truefalse);

		/** Use the DEL character instead of NULL for the fill
		 *  character.
		 *  Corresponding termios flag: OFDEL. */
		void	useDelForFill(bool truefalse);

		/** Map tabs to spaces. 
		 *  Corresponding termios flag: XTAGS/OXTABS/TAB3. */
		void	expandTabToSpaces(bool truefalse);

		/** Send a delay after each new line character.
		 *  Corresponding termios flag: NLDLY. */
		void	delayAfterNewLine(newlinedelay_t nldelay);

		/** Send a delay after each carriage return character.
		 *  Corresponding termios flag: CRDLY. */
		void	delayAfterCarriageReturn(carriagereturndelay_t crdelay);

		/** Send a delay after each tab character
		 *  Corresponding termios flag: TABDLY. */
		void	delayAfterTab(tabdelay_t tabdelay);

		/** Send a delay after each backspace character
		 *  Corresponding termios flag: BSDLY. */
		void	delayAfterBackSpace(backspacedelay_t bsdelay);

		/** Send a delay after each vertical tab character.
		 *  Corresponding termios flag: VTDLY. */
		void	delayAfterVerticalTab(verticaltabdelay_t vtdelay);

		/** Send a delay after each form feed character
		 *  Corresponding termios flag: FFDLY. */
		void	delayAfterFormFeed(formfeeddelay_t ffdelay);

		/** Returns true if implementation-defined output
		 *  processing is enabled. */
		bool	postProcessOutput();

		/** Returns true if lowercase characters are mapped to
		 *  uppercase on output. */
		bool	outputUpperCase();

		/** Returns true if new lines are mapped to carriage
		 *  return/new line on output. */
		bool	mapNewLineToCarriageReturnNewLineOnOutput();

		/** Returns true if carriage returns are mapped to new
		 *  lines on output. */
		bool	mapCarriageReturnToNewLineOnOutput();

		/** Returns true if carriage returns aren't sent at
		 *  column 0. */
		bool	dontOutputCarriageReturnAtColumnZero();

		/** Returns true if new lines are mapped to carriage
		 *  returns on output. */
		bool	mapNewLineToCarriageReturnOnOutput();

		/** Returns true if fill characters are sent for delay
		 *  instead of using a timed delay. */
		bool	useFillCharactersForDelay();

		/** Returns true if the DEL character is used instead of
		 *  NULL for the fill character. */
		bool	useDelForFill();

		/** Returns true if the tabs are mapped to spaces. */
		bool	expandTabToSpaces();

		/** Returns the delay that is sent after new line
		 *  characters. */
		newlinedelay_t		delayAfterNewLine();

		/** Returns the delay that is sent after carriage return
		 *  characters. */
		carriagereturndelay_t	delayAfterCarriageReturn();

		/** Returns the delay that is sent after tab characters. */
		tabdelay_t		delayAfterTab();

		/** Returns the delay that is sent after backspace
		 *  characters. */
		backspacedelay_t	delayAfterBackSpace();

		/** Returns the delay that is sent after vertical tab
		 *  characters. */
		verticaltabdelay_t	delayAfterVerticalTab();

		/** Returns the delay that is sent after form feed
		 *  characters. */
		formfeeddelay_t		delayAfterFormFeed();

		/** Sets the character that will cause a SIGINT to be
		 *  sent to the process when generateSignals() is set
		 *  true.
		 *  Corresponding termios flag: VINTR. */
		void	interruptCharacter(unsigned char character);

		/** Sets the character that will cause a SIGQUIT to be
		 *  sent to the process when generateSignals() is set
		 *  true.
		 *  Corresponding termios flag: VQUIT. */
		void	quitCharacter(unsigned char character);

		/** Sets the character that will cause a character erase
		 *  when canonicalInput() is set to true
		 *  Corresponding termios flag: VERASE. */
		void	eraseCharacter(unsigned char character);

		/** Sets the character that will cause a line erase
		 *  when canonicalInput() is set to true
		 *  Corresponding termios flag: VKILL. */
		void	killCharacter(unsigned char character);

		/** Sets the character that will cause the pending tty
		 *  buffer to be sent to the program without waiting for
		 *  end-of-line and read()'s to return 0 when
		 *  canonicalInput() is set to true
		 *  Corresponding termios flag: VEOF. */
		void	endOfFileCharacter(unsigned char character);

		/** Sets the end-of-line character, recognized when 
		 *  canonicalInput() is set to true
		 *  Corresponding termios flag: VEOL. */
		void	endOfLineCharacter(unsigned char character);

		/** Sets the "other" end-of-line character, recognized
		 *  when canonicalInput() is set to true
		 *  Corresponding termios flag: VEOL2. */
		void	secondEndOfLineCharacter(unsigned char character);

		/** Sets the switch character.
		 *  Corresponding termios flag: VSWTCH/VSWTC. */
		void	switchCharacer(unsigned char character);

		/** Sets the start character for XON/XOFF flow control
		 *  Corresponding termios flag: VSTART. */
		void	startCharacter(unsigned char character);

		/** Sets the stop character for XON/XOFF flow control
		 *  Corresponding termios flag: VSTOP. */
		void	stopCharacter(unsigned char character);

		/** Sets the character that will cause a SIGSUSP to be
		 *  sent to the process when generateSignals() is set
		 *  true.
		 *  Corresponding termios flag: VSUSP. */
		void	suspendCharacter(unsigned char character);

		/** Sets the character that will cause a SIGTSTP to be
		 *  sent to the process when generateSignals() and
		 *  extendedFunctions() are set true.
		 *  Corresponding termios flag: VDSUSP. */
		void	delayedSuspendCharacter(unsigned char character);

		/** Sets the character that "quotes" the next character,
		 *  depriving it of special meaning, recognized when
		 *  extendedFunctions() is set true.
		 *  Corresponding termios flag: VLNEXT. */
		void	literalNextCharcter(unsigned char character);

		/** Sets the word erase character, recognized when
		 *  canonicalInput() and extendedFunctions() are set true.
		 *  Corresponding termios flag: VWERASE. */
		void	wordEraseCharcter(unsigned char character);

		/** Sets the character that causes unread characters to
		 *  be reprinted, recognized when canonicalInput() and
		 *  extendedFunctions() are set true.
		 *  Corresponding termios flag: VREPRINT. */
		void	reprintCharacter(unsigned char character);

		/** Sets the character that toggles discarding pending
		 *  output, recognized when extendedFunctions() is set
		 *  true.
		 *  Corresponding termios flag: VDISCARD. */
		void	discardPendingOutputCharacter(unsigned char character);

		/** Sets the status request character.
		 *  Corresponding termios flag: VSTATUS. */
		void	statusRequestCharacter(unsigned char character);

		/** Sets the number of characters that must be read
		 *  before a read() will begin waiting for readTimeout()
		 *  deciseconds before falling through.
		 *  Corresponding termios flag: VMIN. */
		void	readThreshold(unsigned char count);

		/** Sets the number of deciseconds that a read() will
		 *  wait after reading readThreshold() characters before
		 *  falling through.
		 *  Corresponding termios flag: VTIME. */
		void	readTimeout(unsigned char deciseconds);


		/** Returns the character that will cause a SIGINT to be
		 *  sent to the process when generateSignals() is set
		 *  true. */
		unsigned char	interruptCharacter();

		/** Returns the character that will cause a SIGQUIT to be
		 *  sent to the process when generateSignals() is set
		 *  true. */
		unsigned char	quitCharacter();

		/** Returns the character that will cause a character
		 *  erase when canonicalInput() is set to true. */
		unsigned char	eraseCharacter();

		/** Returns the character that will cause a line erase
		 *  when canonicalInput() is set to true. */
		unsigned char	killCharacter();

		/** Returns the character that will cause the pending tty
		 *  buffer to be sent to the program without waiting for
		 *  end-of-line and read()'s to return 0 when
		 *  canonicalInput() is set to true. */
		unsigned char	endOfFileCharacter();

		/** Returns the end-of-line character, recognized when 
		 *  canonicalInput() is set to true. */
		unsigned char	endOfLineCharacter();

		/** Returns the "other" end-of-line character, recognized
		 *  when canonicalInput() is set to true. */
		unsigned char	secondEndOfLineCharacter();

		/** Returns the switch character. */
		unsigned char	switchCharacer();

		/** Returns the start character for XON/XOFF flow control. */
		unsigned char	startCharacter();

		/** Returns the stop character for XON/XOFF flow control. */
		unsigned char	stopCharacter();

		/** Returns the character that will cause a SIGSUSP to be
		 *  sent to the process when generateSignals() is set
		 *  true. */
		unsigned char	suspendCharacter();

		/** Returns the character that will cause a SIGTSTP to be
		 *  sent to the process when generateSignals() and
		 *  extendedFunctions() are set true. */
		unsigned char	delayedSuspendCharacter();

		/** Returns the character that "quotes" the next
		 *  character, depriving it of special meaning,
		 *  recognized when extendedFunctions() is set true. */
		unsigned char	literalNextCharcter();

		/** Returns the word erase character, recognized when
		 *  canonicalInput() and extendedFunctions() are set true. */
		unsigned char	wordEraseCharcter();


		/** Returns the character that causes unread characters
		 *  to be reprinted, recognized when canonicalInput() and
		 *  extendedFunctions() are set true. */
		unsigned char	reprintCharacter();

		/** Returns the character that toggles discarding pending
		 *  output, recognized when extendedFunctions() is set
		 *  true. */
		unsigned char	discardPendingOutputCharacter();

		/** Returns the status request character. */
		unsigned char	statusRequestCharacter();

		/** Returns the number of characters that must be read
		 *  before a read() will begin waiting for readTimeout()
		 *  deciseconds before falling through. */
		unsigned char	readThreshold();

		/** Returns the number of deciseconds that a read() will
		 *  wait after reading readThreshold() characters before
		 *  falling through. */
		unsigned char	readTimeout();

	#include <rudiments/private/serialportprofile.h>
};

#endif
