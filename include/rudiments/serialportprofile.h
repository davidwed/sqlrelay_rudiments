// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERIALPORTPROFILE_H
#define RUDIMENTS_SERIALPORTPROFILE_H

#include <rudiments/private/serialportprofileincludes.h>

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

	enum	stopbits_t {
		onestopbit=0,
		twostopbits
	};

	enum	newlinedelay_t {
		nl_none=NL0,
		nl_100=NL1
	};

	enum	carriagereturndelay_t {
		cr_none=CR0,
		cr_depends=CR1,
		cr_100=CR2,
		cr_150=CR3
	};

	enum	tabdelay_t {
		td_none=TAB0,
		td_depends=TAB1,
		td_100=TAB2
	};

	enum	backspacedelay_t {
		bs_none=BS0,
		bs_50=BS1
	};

	enum	verticaltabdelay_t {
		vt_none=VT0,
		vt_2=VT1
	};

	enum	formfeeddelay_t {
		ff_none=FF0,
		ff_2=FF1
	};

	public:

			serialportprofile();

		void	setControlOptions(tcflag_t flags);
		void	setLocalOptions(tcflag_t flags);
		void	setInputOptions(tcflag_t flags);
		void	setOutputOptions(tcflag_t flags);

		void	evalOptionsString(const char *string);
			// 8n1, 7e2, etc...

		void	defaultControlOptions();
		void	defaultLocalOptions();
		void	defaultInputOptions();
		void	defaultOutputOptions();
		void	defaultControlCharacters();
		void	defaultOptions();

		void		inputMode(inputmode_t inputmode);
		inputmode_t	inputMode();

		void		flowControl(flowcontrol_t flowcontrol);
		flowcontrol_t	flowControl();

		// control options

		// setters...
		void	baud(baudrate_t baud);
			// CBAUD, CBAUDEX
		bool	inputBaud(baudrate_t baud);
		bool	outputBaud(baudrate_t baud);
		void	characterSize(charsize_t size);
			// CSIZE
		void	stopBits(stopbits_t stopbits);
			// one=~CSTOPB two=CSTOPB
		void	receiverOn(bool truefalse);
			// true=CREAD
		void	parityCheck(bool truefalse);
			// true=PARENB
		void	oddParity(bool truefalse);
			// true=PARODD
		void	hangupOnClose(bool truefalse);
			// true=HUPCL
		void	ignoreModemControlLines(bool truefalse);
			// true=CLOCAL
		#ifdef LOBLK
		void	blockJobControlOutput(bool truefalse);
			// true=LOBLK
		#endif
		void	hardwareFlowControl(bool truefalse);
			// true=CRTSCTS

		// getters...
		baudrate_t	baud();
		baudrate_t	inputBaud();
		baudrate_t	outputBaud();
		charsize_t	characterSize();
		stopbits_t	stopBits();
		bool		receiverOn();
		bool		parityCheck();
		bool		oddParity();
		bool		hangupOnClose();
		bool		ignoreModemControlLines();
		bool		blockJobControlOutput();
		bool		hardwareFlowControl();



		// local options
		// setters...
		void	generateSignals(bool truefalse);
			// true=ISIG
		void	canonicalInput(bool truefalse);
			// ICANON
		#ifdef XCASE
		void	upperCaseUnescapedCharacters(bool truefalse);
			// XCASE
		#endif
		void	echoInput(bool truefalse);
			// ECHO
		void	echoErase(bool truefalse);
			// ECHOE
		void	echoNewLineAfterKill(bool truefalse);
			// ECHOK
		void	echoNewLine(bool truefalse);
			// ECHONL
		void	flushAfterInterruptOrQuit(bool truefalse);
			// true=~NOFLSH
		void	extendedFunctions(bool truefalse);
			// IEXTEN
		void	echoControlCharacters(bool truefalse);
			// ECHOCTL
		void	echoErasedCharacter(bool truefalse);
			// ECHOPRT
		void	lineKill(bool truefalse);
			// ECHOKE
		void	retypePendingCharacters(bool truefalse);
			// PENDIN
		void	sendSignalForBackgroundOutput(bool truefalse);
			// TOSTOP

		// getters...
		bool	generateSignals();
		bool	canonicalInput();
		#ifdef XCASE
		bool	upperCaseUnescapedCharacters();
		#endif
		bool	echoInput();
		bool	echoErase();
		bool	echoNewLineAfterKill();
		bool	echoNewLine();
		bool	flushAfterInterruptOrQuit();
		bool	extendedFunctions();
		bool	echoControlCharacters();
		bool	echoErasedCharacter();
		bool	lineKill();
		bool	retypePendingCharacters();
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
		void	ignoreCarriageReturn(bool truefalse);
			// IGNCR
		void	mapCarriageReturnToNewLineOnInput(bool truefalse);
			// ICRNL
		void	lowerCase(bool truefalse);
			// IUCLC
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
		bool	ignoreCarriageReturn();
		bool	mapCarriageReturnToNewLineOnInput();
		bool	lowerCase();
		bool	bellIfLineTooLong();




		// output options
		void	postProcessOutput(bool truefalse);
			// OPOST
		void	outputUpperCase(bool truefalse);
			// OLCUC
		void	mapNewLineToCarriageReturnNewLineOnOutput(
							bool truefalse);
			// ONLCR
		void	mapCarriageReturnToNewLineOnOutput(bool truefalse);
			// OCRNL
		void	outputCarriageReturnAtColumnZero(bool truefalse);
			// true=~ONOCR
		void	mapNewLineToCarriageReturnOnOutput(bool truefalse);
			// ONLRET
		void	useFillCharactersForDelay(bool truefalse);
			// OFILL
		void	useDelForFill(bool truefalse);
			// OFDEL
		void	expandTabToSpaces(bool truefalse);
			// TAB3/XTABS

		void	delayAfterNewLine(newlinedelay_t nldelay);
			// NLDLY
		void	delayAfterCarriageReturn(carriagereturndelay_t crdelay);
			// CRDLY
		void	delayAfterTab(tabdelay_t tabdelay);
			// TABDLY
		void	delayAfterBackSpace(backspacedelay_t bsdelay);
			// BSDLY
		void	delayAfterVerticalTab(verticaltabdelay_t vtdelay);
			// VTDLY
		void	delayAfterFormFeed(formfeeddelay_t ffdelay);
			// FFDLY


		// getters...
		bool	postProcessOutput();
		bool	outputUpperCase();
		bool	mapNewLineToCarriageReturnNewLineOnOutput();
		bool	mapCarriageReturnToNewLineOnOutput();
		bool	outputCarriageReturnAtColumnZero();
		bool	mapNewLineToCarriageReturnOnOutput();
		bool	useFillCharactersForDelay();
		bool	useDelForFill();
		bool	expandTabToSpaces();
		newlinedelay_t		delayAfterNewLine();
		carriagereturndelay_t	delayAfterCarriageReturn();
		tabdelay_t		delayAfterTab();
		backspacedelay_t	delayAfterBackSpace();
		verticaltabdelay_t	delayAfterVerticalTab();
		formfeeddelay_t		delayAfterFormFeed();



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
