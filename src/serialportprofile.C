// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/serialportprofile.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/character.h>
#include <rudiments/charstring.h>

#include <stdio.h>

serialportprofile::serialportprofile() {
	defaultOptions();
}

serialportprofile::~serialportprofile() {
}

void serialportprofile::setControlOptions(tcflag_t flags) {
	tio.c_cflag=flags;
}

void serialportprofile::setLocalOptions(tcflag_t flags) {
	tio.c_lflag=flags;
}

void serialportprofile::setInputOptions(tcflag_t flags) {
	tio.c_iflag=flags;
}

void serialportprofile::setOutputOptions(tcflag_t flags) {
	tio.c_oflag=flags;
}

void serialportprofile::setControlCharacters(cc_t *c_cc) {
	rawbuffer::copy((void *)&tio.c_cc,(void *)c_cc,sizeof(cc_t)*NCCS);
}

void serialportprofile::setOptions(termios *newtio) {
	rawbuffer::copy((void *)&tio,(void *)newtio,sizeof(tio));
}

void serialportprofile::defaultControlOptions() {
	tio.c_cflag=0;
}

void serialportprofile::defaultLocalOptions() {
	tio.c_lflag=0;
}

void serialportprofile::defaultInputOptions() {
	tio.c_iflag=0;
}

void serialportprofile::defaultOutputOptions() {
	tio.c_oflag=0;
}

void serialportprofile::defaultControlCharacters() {
	rawbuffer::set((void *)&tio.c_cc,0,sizeof(tio.c_cc));
}

void serialportprofile::defaultOptions() {
	rawbuffer::set((void *)&tio,0,sizeof(tio));
}

bool serialportprofile::inputBaud(serialportprofile::baudrate_t baudrate) {
	return !cfsetispeed(&tio,(tcflag_t)baudrate);
}

bool serialportprofile::outputBaud(serialportprofile::baudrate_t baudrate) {
	return !cfsetospeed(&tio,(tcflag_t)baudrate);
}

void serialportprofile::baud(serialportprofile::baudrate_t baudrate) {

	#ifdef CBAUD
		#ifdef CBAUDEX
		tio.c_cflag&=~(CBAUD|CBAUDEX);
		#else
		tio.c_cflag&=~CBAUD;
		#endif
	#else
		tio.c_cflag&=~(B0|B50|B75|B110|B134|B150|B200|B300|
				B600|B1200|B1800|B2400|B4800|B9600
				#if defined(B19200)
				|B19200
				#elif defined(EXTA)
				|EXTA
				#endif
				#if defined(B38400)
				|B38400
				#elif defined(EXTB)
				|EXTB
				#endif
				#ifdef B57600
				|B57600
				#endif
				#ifdef B76800
				|B76800
				#endif
				#ifdef B115200
				|B115200
				#endif
				#ifdef B230400
				|B230400
				#endif
				#ifdef B460800
				|B460800
				#endif
				#ifdef B500000
				|B500000
				#endif
				#ifdef B576000
				|B576000
				#endif
				#ifdef B921600
				|B921600
				#endif
				#ifdef B1000000
				|B1000000
				#endif
				#ifdef B1142000
				|B1152000
				#endif
				#ifdef B1500000
				|B1500000
				#endif
				#ifdef B2000000
				|B2000000
				#endif
				#ifdef B2500000
				|B2500000
				#endif
				#ifdef B3000000
				|B3000000
				#endif
				#ifdef B3500000
				|B3500000
				#endif
				#ifdef B4000000
				|B4000000
				#endif
				);
	#endif
	tio.c_cflag|=(tcflag_t)baudrate;
}

void serialportprofile::characterSize(serialportprofile::charsize_t size) {
	tio.c_cflag&=~CSIZE;
	tio.c_cflag|=(tcflag_t)size;
}

#define SET_FLAG(truefalse,flag,value) \
	if (truefalse) { tio.flag|=value; } else { tio.flag&=~value; }

#define SET_CHAR(character,value) \
	tio.c_cc[character]=value;

#define GET_FLAG(flag,value) (tio.flag&value)

#define GET_CHAR(character) (tio.c_cc[character])

void serialportprofile::twoStopBits(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,CSTOPB)
}

void serialportprofile::receiverOn(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,CREAD)
}

void serialportprofile::parityCheck(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,PARENB)
}

void serialportprofile::oddParity(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,PARODD)
}

void serialportprofile::hangupOnClose(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,HUPCL)
}

void serialportprofile::ignoreModemControlLines(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,CLOCAL)
}

#ifdef LOBLK
void serialportprofile::blockJobControlOutput(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,LOBLK)
}
#endif

void serialportprofile::hardwareFlowControl(bool truefalse) {
	#if defined(CRTSCTS)
		SET_FLAG(truefalse,c_cflag,CRTSCTS)
	#elif defined(NEW_CRTSCTS)
		SET_FLAG(truefalse,c_cflag,NEW_CRTSCTS)
	#elif defined(CRTS_IFLOW)
		SET_FLAG(truefalse,c_cflag,CRTS_IFLOW)
	#elif defined(CCTS_OFLOW)
		SET_FLAG(truefalse,c_cflag,CRTS_IFLOW)
	#endif
}

// FIXME:
//	CDTRCTS - DTR/CTS full-duplex flow control
//	MDMBUF - DTR/DCD hardware flow control
//	CHWFLOW - (MDMBUF|CRTSCTS|CDTRCTS)

serialportprofile::baudrate_t serialportprofile::baud() {
	#ifdef CBAUD
		return (baudrate_t)GET_FLAG(c_cflag,CBAUD);
	#else
		tcflag_t	cbaud=
				(B0|B50|B75|B110|B134|B150|B200|B300|
				B600|B1200|B1800|B2400|B4800|B9600
				#if defined(B19200)
				|B19200
				#elif defined(EXTA)
				|EXTA
				#endif
				#if defined(B38400)
				|B38400
				#elif defined(EXTB)
				|EXTB
				#endif
				#ifdef B57600
				|B57600
				#endif
				#ifdef B76800
				|B76800
				#endif
				#ifdef B115200
				|B115200
				#endif
				#ifdef B230400
				|B230400
				#endif
				#ifdef B460800
				|B460800
				#endif
				#ifdef B500000
				|B500000
				#endif
				#ifdef B576000
				|B576000
				#endif
				#ifdef B921600
				|B921600
				#endif
				#ifdef B1000000
				|B1000000
				#endif
				#ifdef B1142000
				|B1152000
				#endif
				#ifdef B1500000
				|B1500000
				#endif
				#ifdef B2000000
				|B2000000
				#endif
				#ifdef B2500000
				|B2500000
				#endif
				#ifdef B3000000
				|B3000000
				#endif
				#ifdef B3500000
				|B3500000
				#endif
				#ifdef B4000000
				|B4000000
				#endif
				);
		return (baudrate_t)GET_FLAG(c_cflag,cbaud);
	#endif
}

serialportprofile::baudrate_t serialportprofile::inputBaud() {
	return (serialportprofile::baudrate_t)cfgetispeed(&tio);
}

serialportprofile::baudrate_t serialportprofile::outputBaud() {
	return (serialportprofile::baudrate_t)cfgetospeed(&tio);
}

serialportprofile::charsize_t serialportprofile::characterSize() {
	return (charsize_t)GET_FLAG(c_cflag,CSIZE);
}

bool serialportprofile::twoStopBits() {
	return (charsize_t)GET_FLAG(c_cflag,CSTOPB);
}

bool serialportprofile::receiverOn() {
	return GET_FLAG(c_cflag,CREAD);
}

bool serialportprofile::parityCheck() {
	return GET_FLAG(c_cflag,PARENB);
}

bool serialportprofile::oddParity() {
	return GET_FLAG(c_cflag,PARODD);
}

bool serialportprofile::hangupOnClose() {
	return GET_FLAG(c_cflag,HUPCL);
}

bool serialportprofile::ignoreModemControlLines() {
	return GET_FLAG(c_cflag,CLOCAL);
}

#ifdef LOBLK
bool serialportprofile::blockJobControlOutput() {
	return GET_FLAG(c_cflag,LOBLK);
}
#endif

bool serialportprofile::hardwareFlowControl() {
	#if defined(CRTSCTS)
		return GET_FLAG(c_cflag,CRTSCTS);
	#elif defined(NEW_CRTSCTS)
		return GET_FLAG(c_cflag,NEW_CRTSCTS);
	#elif defined(CRTS_IFLOW)
		return GET_FLAG(c_cflag,CRTS_IFLOW);
	#elif defined(CCTS_OFLOW)
		return GET_FLAG(c_cflag,CRTS_OFLOW);
	#endif
}

void serialportprofile::generateSignals(bool truefalse){
	SET_FLAG(truefalse,c_lflag,ISIG)
}

void serialportprofile::canonicalInput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ICANON)
}

#ifdef XCASE
void serialportprofile::escapedUpperCase(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,XCASE)
}
#endif

void serialportprofile::echoInput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHO)
}

void serialportprofile::eraseCharactersOn(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOE)
}

void serialportprofile::killCharacterOn(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOK)
}

void serialportprofile::echoNewLine(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHONL)
}

void serialportprofile::extendedFunctions(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,IEXTEN)
}

void serialportprofile::echoControlCharacters(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOCTL)
}

#ifdef ECHOPRT
void serialportprofile::echoErasedCharacter(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOPRT)
}
#endif

void serialportprofile::emulateKill(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOKE)
}

void serialportprofile::noFlushAfterInterruptOrQuit(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,NOFLSH)
}

#ifdef PENDIN
void serialportprofile::retypePendingCharacters(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,PENDIN)
}
#endif

void serialportprofile::sendSignalForBackgroundOutput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,TOSTOP)
}

bool serialportprofile::generateSignals() {
	return GET_FLAG(c_lflag,ISIG);
}

bool serialportprofile::canonicalInput() {
	return GET_FLAG(c_lflag,ICANON);
}

#ifdef XCASE
bool serialportprofile::escapedUpperCase() {
	return GET_FLAG(c_lflag,XCASE);
}
#endif

bool serialportprofile::echoInput() {
	return GET_FLAG(c_lflag,ECHO);
}

bool serialportprofile::eraseCharactersOn() {
	return GET_FLAG(c_lflag,ECHOE);
}

bool serialportprofile::killCharacterOn() {
	return GET_FLAG(c_lflag,ECHOK);
}

bool serialportprofile::echoNewLine() {
	return GET_FLAG(c_lflag,ECHONL);
}

bool serialportprofile::extendedFunctions() {
	return GET_FLAG(c_lflag,IEXTEN);
}

bool serialportprofile::echoControlCharacters() {
	return GET_FLAG(c_lflag,ECHOCTL);
}

#ifdef ECHOPRT
bool serialportprofile::echoErasedCharacter() {
	return GET_FLAG(c_lflag,ECHOPRT);
}
#endif

bool serialportprofile::emulateKill() {
	return GET_FLAG(c_lflag,ECHOKE);
}

bool serialportprofile::noFlushAfterInterruptOrQuit() {
	return GET_FLAG(c_lflag,NOFLSH);
}

#ifdef PENDIN
bool serialportprofile::retypePendingCharacters() {
	return GET_FLAG(c_lflag,PENDIN);
}
#endif

bool serialportprofile::sendSignalForBackgroundOutput() {
	return GET_FLAG(c_lflag,TOSTOP);
}

void serialportprofile::inputParityCheck(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,INPCK)
}

void serialportprofile::ignoreParityErrors(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IGNPAR)
}

void serialportprofile::markParityErrors(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,PARMRK)
}

void serialportprofile::stripParityBits(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,ISTRIP)
}

void serialportprofile::softwareFlowControlOnOutput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IXON)
}

void serialportprofile::softwareFlowControlOnInput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IXOFF)
}

void serialportprofile::anyCharacterStartsFlow(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IXANY)
}

void serialportprofile::ignoreBreak(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IGNBRK)
}

void serialportprofile::sendSignalOnBreak(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,BRKINT)
}

void serialportprofile::mapNewLineToCarriageReturnOnInput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,INLCR)
}

void serialportprofile::ignoreCarriageReturn(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IGNCR)
}

void serialportprofile::mapCarriageReturnToNewLineOnInput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,ICRNL)
}

#ifdef IUCLC
void serialportprofile::lowerCase(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IUCLC)
}
#endif

void serialportprofile::bellIfLineTooLong(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IMAXBEL)
}

bool serialportprofile::inputParityCheck() {
	return GET_FLAG(c_iflag,INPCK);
}

bool serialportprofile::ignoreParityErrors() {
	return GET_FLAG(c_iflag,IGNPAR);
}

bool serialportprofile::markParityErrors() {
	return GET_FLAG(c_iflag,PARMRK);
}

bool serialportprofile::stripParityBits() {
	return GET_FLAG(c_iflag,ISTRIP);
}

bool serialportprofile::softwareFlowControlOnOutput() {
	return GET_FLAG(c_iflag,IXON);
}

bool serialportprofile::softwareFlowControlOnInput() {
	return GET_FLAG(c_iflag,IXOFF);
}

bool serialportprofile::anyCharacterStartsFlow() {
	return GET_FLAG(c_iflag,IXANY);
}

bool serialportprofile::ignoreBreak() {
	return GET_FLAG(c_iflag,IGNBRK);
}

bool serialportprofile::sendSignalOnBreak() {
	return GET_FLAG(c_iflag,BRKINT);
}

bool serialportprofile::mapNewLineToCarriageReturnOnInput() {
	return GET_FLAG(c_iflag,INLCR);
}

bool serialportprofile::ignoreCarriageReturn() {
	return GET_FLAG(c_iflag,IGNCR);
}

bool serialportprofile::mapCarriageReturnToNewLineOnInput() {
	return GET_FLAG(c_iflag,ICRNL);
}

#ifdef IUCLC
bool serialportprofile::lowerCase() {
	return GET_FLAG(c_iflag,IUCLC);
}
#endif

bool serialportprofile::bellIfLineTooLong() {
	return GET_FLAG(c_iflag,IMAXBEL);
}

void serialportprofile::postProcessOutput(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OPOST)
}

#ifdef OLCUC
void serialportprofile::outputUpperCase(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OLCUC)
}
#endif

void serialportprofile::mapNewLineToCarriageReturnNewLineOnOutput(
							bool truefalse) {
	SET_FLAG(truefalse,c_oflag,ONLCR)
}

#ifdef ONOEOT
void serialportprofile::discardEndOfTransmission(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,ONOEOT)
}
#endif

void serialportprofile::mapCarriageReturnToNewLineOnOutput(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OCRNL)
}

void serialportprofile::dontOutputCarriageReturnAtColumnZero(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,ONOCR)
}

void serialportprofile::mapNewLineToCarriageReturnOnOutput(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,ONLRET)
}

#ifdef OFILL
void serialportprofile::useFillCharactersForDelay(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OFILL)
}
#endif

#ifdef OFDEL
void serialportprofile::useDelForFill(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OFDEL)
}
#endif

void serialportprofile::expandTabToSpaces(bool truefalse) {
	#if defined(XTABS)
		SET_FLAG(truefalse,c_oflag,XTABS)
	#elif defined(OXTABS)
		SET_FLAG(truefalse,c_oflag,OXTABS)
	#else
		SET_FLAG(truefalse,c_oflag,TAB3)
	#endif
}

#ifdef NLDLY
void serialportprofile::delayAfterNewLine(
		serialportprofile::newlinedelay_t nldelay) {
	tio.c_oflag&=~NLDLY;
	tio.c_oflag|=(tcflag_t)nldelay;
}
#endif

#ifdef CRDLY
void serialportprofile::delayAfterCarriageReturn(
		serialportprofile::carriagereturndelay_t crdelay) {
	tio.c_oflag&=~CRDLY;
	tio.c_oflag|=(tcflag_t)crdelay;
}
#endif

#ifdef TABDLY
void serialportprofile::delayAfterTab(
		serialportprofile::tabdelay_t tabdelay) {
	tio.c_oflag&=~TABDLY;
	tio.c_oflag|=(tcflag_t)tabdelay;
}
#endif

#ifdef BSDLY
void serialportprofile::delayAfterBackSpace(
		serialportprofile::backspacedelay_t bsdelay) {
	tio.c_oflag&=~BSDLY;
	tio.c_oflag|=(tcflag_t)bsdelay;
}
#endif

#ifdef VTDLY
void serialportprofile::delayAfterVerticalTab(
		serialportprofile::verticaltabdelay_t vtdelay) {
	tio.c_oflag&=~VTDLY;
	tio.c_oflag|=(tcflag_t)vtdelay;
}
#endif

#ifdef FFDLY
void serialportprofile::delayAfterFormFeed(
		serialportprofile::formfeeddelay_t ffdelay) {
	tio.c_oflag&=~FFDLY;
	tio.c_oflag|=(tcflag_t)ffdelay;
}
#endif

bool serialportprofile::postProcessOutput() {
	return GET_FLAG(c_oflag,OPOST);
}

#ifdef OLCUC
bool serialportprofile::outputUpperCase() {
	return GET_FLAG(c_oflag,OLCUC);
}
#endif

bool serialportprofile::mapNewLineToCarriageReturnNewLineOnOutput() {
	return GET_FLAG(c_oflag,ONLCR);
}

#ifdef ONOEOT
bool serialportprofile::discardEndOfTransmission() {
	return GET_FLAG(c_oflag,ONOEOT);
}
#endif

bool serialportprofile::mapCarriageReturnToNewLineOnOutput() {
	return GET_FLAG(c_oflag,OCRNL);
}

bool serialportprofile::dontOutputCarriageReturnAtColumnZero() {
	return GET_FLAG(c_oflag,ONOCR);
}

bool serialportprofile::mapNewLineToCarriageReturnOnOutput() {
	return GET_FLAG(c_oflag,ONLRET);
}

#ifdef OFILL
bool serialportprofile::useFillCharactersForDelay() {
	return GET_FLAG(c_oflag,OFILL);
}
#endif

#ifdef OFDEL
bool serialportprofile::useDelForFill() {
	return GET_FLAG(c_oflag,OFDEL);
}
#endif

bool serialportprofile::expandTabToSpaces() {
	#if defined(XTABS)
		return GET_FLAG(c_oflag,XTABS);
	#elif defined(OXTABS)
		return GET_FLAG(c_oflag,OXTABS);
	#else
		return GET_FLAG(c_oflag,TAB3);
	#endif
}

#ifdef NLDLY
serialportprofile::newlinedelay_t
		serialportprofile::delayAfterNewLine() {
	return (newlinedelay_t)GET_FLAG(c_cflag,NLDLY);
}
#endif

#ifdef CRDLY
serialportprofile::carriagereturndelay_t
		serialportprofile::delayAfterCarriageReturn() {
	return (carriagereturndelay_t)GET_FLAG(c_cflag,CRDLY);
}
#endif

#ifdef TABDLY
serialportprofile::tabdelay_t
		serialportprofile::delayAfterTab() {
	return (tabdelay_t)GET_FLAG(c_cflag,TABDLY);
}
#endif

#ifdef BSDLY
serialportprofile::backspacedelay_t
		serialportprofile::delayAfterBackSpace() {
	return (backspacedelay_t)GET_FLAG(c_cflag,BSDLY);
}
#endif

#ifdef VTDLY
serialportprofile::verticaltabdelay_t
		serialportprofile::delayAfterVerticalTab() {
	return (verticaltabdelay_t)GET_FLAG(c_cflag,VTDLY);
}
#endif

#ifdef FFDLY
serialportprofile::formfeeddelay_t
		serialportprofile::delayAfterFormFeed() {
	return (formfeeddelay_t)GET_FLAG(c_cflag,FFDLY);
}
#endif

void serialportprofile::interruptCharacter(cc_t character) {
	SET_CHAR(VINTR,character)
}

void serialportprofile::quitCharacter(cc_t character) {
	SET_CHAR(VQUIT,character)
}

void serialportprofile::eraseCharacter(cc_t character) {
	SET_CHAR(VERASE,character)
}

void serialportprofile::killCharacter(cc_t character) {
	SET_CHAR(VKILL,character)
}

void serialportprofile::endOfFileCharacter(cc_t character) {
	SET_CHAR(VEOF,character)
}

void serialportprofile::endOfLineCharacter(cc_t character) {
	SET_CHAR(VEOL,character)
}

void serialportprofile::secondEndOfLineCharacter(cc_t character) {
	SET_CHAR(VEOL2,character)
}

#if defined(VSWTCH) || defined(VSWTC)
void serialportprofile::switchCharacer(cc_t character) {
	#ifdef VSWTCH
	SET_CHAR(VSWTCH,character)
	#else
	SET_CHAR(VSWTC,character)
	#endif
}
#endif

void serialportprofile::startCharacter(cc_t character) {
	SET_CHAR(VSTART,character)
}

void serialportprofile::stopCharacter(cc_t character) {
	SET_CHAR(VSTOP,character)
}

void serialportprofile::suspendCharacter(cc_t character) {
	SET_CHAR(VSUSP,character)
}

#ifdef VDSUSP
void serialportprofile::delayedSuspendCharacter(cc_t character) {
	SET_CHAR(VDSUSP,character)
}
#endif

void serialportprofile::literalNextCharcter(cc_t character) {
	SET_CHAR(VLNEXT,character)
}

void serialportprofile::wordEraseCharcter(cc_t character) {
	SET_CHAR(VWERASE,character)
}


void serialportprofile::reprintCharacter(cc_t character) {
	SET_CHAR(VREPRINT,character)
}

void serialportprofile::discardPendingOutputCharacter(cc_t character) {
	SET_CHAR(VDISCARD,character)
}

#ifdef VSTATUS
void serialportprofile::statusRequestCharacter(cc_t character) {
	SET_CHAR(VSTATUS,character)
}
#endif

void serialportprofile::readThreshold(cc_t count) {
	SET_CHAR(VMIN,count)
}

void serialportprofile::readTimeout(cc_t deciseconds) {
	SET_CHAR(VTIME,deciseconds)
}

cc_t serialportprofile::interruptCharacter() {
	return GET_CHAR(VINTR);
}

cc_t serialportprofile::quitCharacter() {
	return GET_CHAR(VQUIT);
}

cc_t serialportprofile::eraseCharacter() {
	return GET_CHAR(VERASE);
}

cc_t serialportprofile::killCharacter() {
	return GET_CHAR(VKILL);
}

cc_t serialportprofile::endOfFileCharacter() {
	return GET_CHAR(VEOF);
}

cc_t serialportprofile::endOfLineCharacter() {
	return GET_CHAR(VEOL);
}

cc_t serialportprofile::secondEndOfLineCharacter() {
	return GET_CHAR(VEOL2);
}

#if defined(VSWTCH) || defined(VSWTC)
cc_t serialportprofile::switchCharacer() {
	#ifdef VSWTCH
	return GET_CHAR(VSWTCH);
	#else
	return GET_CHAR(VSWTC);
	#endif
}
#endif

cc_t serialportprofile::startCharacter() {
	return GET_CHAR(VSTART);
}

cc_t serialportprofile::stopCharacter() {
	return GET_CHAR(VSTOP);
}

cc_t serialportprofile::suspendCharacter() {
	return GET_CHAR(VSUSP);
}

#ifdef VDSUSP
cc_t serialportprofile::delayedSuspendCharacter() {
	return GET_CHAR(VDSUSP);
}
#endif

cc_t serialportprofile::literalNextCharcter() {
	return GET_CHAR(VLNEXT);
}

cc_t serialportprofile::wordEraseCharcter() {
	return GET_CHAR(VWERASE);
}

cc_t serialportprofile::reprintCharacter() {
	return GET_CHAR(VREPRINT);
}

cc_t serialportprofile::discardPendingOutputCharacter() {
	return GET_CHAR(VDISCARD);
}

#ifdef VSTATUS
cc_t serialportprofile::statusRequestCharacter() {
	return GET_CHAR(VSTATUS);
}
#endif

cc_t serialportprofile::readThreshold() {
	return GET_CHAR(VMIN);
}

cc_t serialportprofile::readTimeout() {
	return GET_CHAR(VTIME);
}

termios *serialportprofile::getTermios() {
	return &tio;
}

serialportprofile::baudrate_t serialportprofile::translateBaudString(
							const char *baudrate) {

	if (!charstring::compare(baudrate,"0")) {
		return baud_0;
	} else if (!charstring::compare(baudrate,"50")) {
		return baud_50;
	} else if (!charstring::compare(baudrate,"75")) {
		return baud_75;
	} else if (!charstring::compare(baudrate,"110")) {
		return baud_110;
	} else if (!charstring::compare(baudrate,"134")) {
		return baud_134;
	} else if (!charstring::compare(baudrate,"150")) {
		return baud_150;
	} else if (!charstring::compare(baudrate,"200")) {
		return baud_200;
	} else if (!charstring::compare(baudrate,"300")) {
		return baud_300;
	} else if (!charstring::compare(baudrate,"600")) {
		return baud_600;
	} else if (!charstring::compare(baudrate,"1200")) {
		return baud_1200;
	} else if (!charstring::compare(baudrate,"1800")) {
		return baud_1800;
	} else if (!charstring::compare(baudrate,"2400")) {
		return baud_2400;
	} else if (!charstring::compare(baudrate,"4800")) {
		return baud_4800;
	} else if (!charstring::compare(baudrate,"9600")) {
		return baud_9600;
	#if defined(B57600)
	} else if (!charstring::compare(baudrate,"19200")) {
		return baud_19200;
	#elif defined(EXTA)
	} else if (!charstring::compare(baudrate,"EXTA")) {
		return baud_19200;
	#endif
	#if defined(B38400)
	} else if (!charstring::compare(baudrate,"38400")) {
		return baud_38400;
	#elif defined(EXTB)
	} else if (!charstring::compare(baudrate,"EXTB")) {
		return baud_38400;
	#endif
	#ifdef B57600
	} else if (!charstring::compare(baudrate,"57600")) {
		return baud_57600;
	#endif
	#ifdef B76800
	} else if (!charstring::compare(baudrate,"76800")) {
		return baud_76800;
	#endif
	#ifdef B115200
	} else if (!charstring::compare(baudrate,"115200")) {
		return baud_115200;
	#endif
	#ifdef B230400
	} else if (!charstring::compare(baudrate,"230400")) {
		return baud_230400;
	#endif
	#ifdef B460800
	} else if (!charstring::compare(baudrate,"460800")) {
		return baud_460800;
	#endif
	#ifdef B500000
	} else if (!charstring::compare(baudrate,"500000")) {
		return baud_500000;
	#endif
	#ifdef B576000
	} else if (!charstring::compare(baudrate,"576000")) {
		return baud_576000;
	#endif
	#ifdef B921600
	} else if (!charstring::compare(baudrate,"921600")) {
		return baud_921600;
	#endif
	#ifdef B1000000
	} else if (!charstring::compare(baudrate,"1000000")) {
		return baud_1000000;
	#endif
	#ifdef B1142000
	} else if (!charstring::compare(baudrate,"1152000")) {
		return baud_1152000;
	#endif
	#ifdef B1500000
	} else if (!charstring::compare(baudrate,"1500000")) {
		return baud_1500000;
	#endif
	#ifdef B2000000
	} else if (!charstring::compare(baudrate,"2000000")) {
		return baud_2000000;
	#endif
	#ifdef B2500000
	} else if (!charstring::compare(baudrate,"2500000")) {
		return baud_2500000;
	#endif
	#ifdef B3000000
	} else if (!charstring::compare(baudrate,"3000000")) {
		return baud_3000000;
	#endif
	#ifdef B3500000
	} else if (!charstring::compare(baudrate,"3500000")) {
		return baud_3500000;
	#endif
	#ifdef B4000000
	} else if (!charstring::compare(baudrate,"4000000")) {
		return baud_4000000;
	#endif
	}
	return baud_0;
}

void serialportprofile::baud(const char *baudrate) {
	baud(translateBaudString(baudrate));
}

bool serialportprofile::inputBaud(const char *baudrate) {
	return inputBaud(translateBaudString(baudrate));
}

bool serialportprofile::outputBaud(const char *baudrate) {
	return outputBaud(translateBaudString(baudrate));
}

void serialportprofile::inputMode(serialportprofile::inputmode_t inputmode) {
	if (inputmode==cannonical) {
		canonicalInput(true);
		echoInput(true);
		eraseCharactersOn(true);
	} else {
		canonicalInput(false);
		echoInput(false);
		eraseCharactersOn(false);
	}
}

serialportprofile::inputmode_t serialportprofile::inputMode() {
	return (canonicalInput() && echoInput() && eraseCharactersOn())?
							cannonical:raw;
}

void serialportprofile::evalOptionsString(const char *string) {

	char	bitsperchar=string[0];
	char	parity=character::toUpperCase(string[1]);
	char	stopbits=string[2];

	charsize_t	charsize=(charsize_t)cs_8;
	switch (bitsperchar) {
		case '5':
			charsize=cs_5;
			break;
		case '6':
			charsize=cs_6;
			break;
		case '7':
			charsize=cs_7;
			break;
	}
	characterSize(charsize);

	switch (parity) {
		case 'E':
			inputParityCheck(true);
			stripParityBits(true);
			parityCheck(true);
			oddParity(false);
			break;
		case 'O':
			inputParityCheck(true);
			stripParityBits(true);
			parityCheck(true);
			oddParity(true);
			break;
		default:
			inputParityCheck(false);
			stripParityBits(false);
			parityCheck(false);
			break;
	}

	
	twoStopBits((stopbits=='2'));
}

void serialportprofile::flowControl(
		serialportprofile::flowcontrol_t flowcontrol) {

	if (flowcontrol==fc_none) {
		softwareFlowControlOnOutput(false);
		softwareFlowControlOnInput(false);
		anyCharacterStartsFlow(false);
		hardwareFlowControl(false);
	} else if (flowcontrol==fc_software) {
		softwareFlowControlOnOutput(true);
		softwareFlowControlOnInput(true);
		anyCharacterStartsFlow(true);
		hardwareFlowControl(false);
	} else if (flowcontrol==fc_hardware) {
		softwareFlowControlOnOutput(false);
		softwareFlowControlOnInput(false);
		anyCharacterStartsFlow(false);
		hardwareFlowControl(true);
	}
}

serialportprofile::flowcontrol_t serialportprofile::flowControl() {
	if (hardwareFlowControl()) {
		return fc_hardware;
	} else if (softwareFlowControlOnOutput() ||
			softwareFlowControlOnInput()) {
		return fc_software;
	} else {
		return fc_none;
	}
}
