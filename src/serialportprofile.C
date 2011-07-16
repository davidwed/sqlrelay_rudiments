// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/serialportprofile.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/character.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#include <unistd.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class serialportprofileprivate {
	friend class serialportprofile;
	private:
		termios	_tio;
};

serialportprofile::serialportprofile() {
	pvt=new serialportprofileprivate;
	defaultOptions();
}

serialportprofile::~serialportprofile() {
	delete pvt;
}

void serialportprofile::setControlOptions(tcflag_t flags) {
	pvt->_tio.c_cflag=flags;
}

void serialportprofile::setLocalOptions(tcflag_t flags) {
	pvt->_tio.c_lflag=flags;
}

void serialportprofile::setInputOptions(tcflag_t flags) {
	pvt->_tio.c_iflag=flags;
}

void serialportprofile::setOutputOptions(tcflag_t flags) {
	pvt->_tio.c_oflag=flags;
}

void serialportprofile::setControlCharacters(const cc_t *c_cc) {
	rawbuffer::copy(&pvt->_tio.c_cc,c_cc,sizeof(cc_t)*NCCS);
}

void serialportprofile::setOptions(const termios *newtio) {
	rawbuffer::copy(&pvt->_tio,newtio,sizeof(pvt->_tio));
}

void serialportprofile::defaultControlOptions() {
	pvt->_tio.c_cflag=0;
}

void serialportprofile::defaultLocalOptions() {
	pvt->_tio.c_lflag=0;
}

void serialportprofile::defaultInputOptions() {
	pvt->_tio.c_iflag=0;
}

void serialportprofile::defaultOutputOptions() {
	pvt->_tio.c_oflag=0;
}

void serialportprofile::defaultControlCharacters() {
	rawbuffer::zero(&pvt->_tio.c_cc,sizeof(pvt->_tio.c_cc));
}

void serialportprofile::defaultOptions() {
	rawbuffer::zero(&pvt->_tio,sizeof(pvt->_tio));
}

static tcflag_t	br[]={
	B0,
	B50,
	B75,
	B110,
	B134,
	B150,
	B200,
	B300,
	B600,
	B1200,
	B1800,
	B2400,
	B4800,
	B9600,
	#if defined(B19200)
	B19200,
	#elif defined(EXTA)
	EXTA,
	#endif
	#if defined(B38400)
	B38400,
	#elif defined(EXTB)
	EXTB,
	#endif
	#ifdef B57600
	B57600,
	#endif
	#ifdef B76800
	B76800,
	#endif
	#ifdef B115200
	B115200,
	#endif
	#ifdef B230400
	B230400,
	#endif
	#ifdef B460800
	B460800,
	#endif
	#ifdef B500000
	B500000,
	#endif
	#ifdef B576000
	B576000,
	#endif
	#ifdef B921600
	B921600,
	#endif
	#ifdef B1000000
	B1000000,
	#endif
	#ifdef B1142000
	B1152000,
	#endif
	#ifdef B1500000
	B1500000,
	#endif
	#ifdef B2000000
	B2000000,
	#endif
	#ifdef B2500000
	B2500000,
	#endif
	#ifdef B3000000
	B3000000,
	#endif
	#ifdef B3500000
	B3500000,
	#endif
	#ifdef B4000000
	B4000000,
	#endif
};

bool serialportprofile::inputBaud(serialportprofile::baudrate_t baudrate) {
	return !cfsetispeed(&pvt->_tio,br[baudrate]);
}

bool serialportprofile::outputBaud(serialportprofile::baudrate_t baudrate) {
	return !cfsetospeed(&pvt->_tio,br[baudrate]);
}

void serialportprofile::baud(serialportprofile::baudrate_t baudrate) {

	#ifdef CBAUD
		#ifdef CBAUDEX
		pvt->_tio.c_cflag&=~(CBAUD|CBAUDEX);
		#else
		pvt->_tio.c_cflag&=~CBAUD;
		#endif
	#else
		pvt->_tio.c_cflag&=~(B0|B50|B75|B110|B134|B150|B200|B300|
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
	pvt->_tio.c_cflag|=br[baudrate];
}

void serialportprofile::characterSize(serialportprofile::charsize_t size) {
	pvt->_tio.c_cflag&=~CSIZE;
	pvt->_tio.c_cflag|=(tcflag_t)size;
}

#define SET_FLAG(truefalse,flag,value) \
	if (truefalse) { pvt->_tio.flag|=value; } else { pvt->_tio.flag&=~value; }

#define SET_CHAR(character,value) \
	pvt->_tio.c_cc[character]=value;

#define GET_FLAG(flag,value) (pvt->_tio.flag&value)

#define GET_CHAR(character) (pvt->_tio.c_cc[character])

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

void serialportprofile::blockJobControlOutput(bool truefalse) {
	#ifdef LOBLK
		SET_FLAG(truefalse,c_cflag,LOBLK)
	#endif
}

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
	return (serialportprofile::baudrate_t)cfgetispeed(&pvt->_tio);
}

serialportprofile::baudrate_t serialportprofile::outputBaud() {
	return (serialportprofile::baudrate_t)cfgetospeed(&pvt->_tio);
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

bool serialportprofile::blockJobControlOutput() {
	#ifdef LOBLK
		return GET_FLAG(c_cflag,LOBLK);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::hardwareFlowControl() {
	#if defined(CRTSCTS)
		return GET_FLAG(c_cflag,CRTSCTS);
	#elif defined(NEW_CRTSCTS)
		return GET_FLAG(c_cflag,NEW_CRTSCTS);
	#elif defined(CRTS_IFLOW)
		return GET_FLAG(c_cflag,CRTS_IFLOW);
	#elif defined(CCTS_OFLOW)
		return GET_FLAG(c_cflag,CRTS_OFLOW);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

void serialportprofile::generateSignals(bool truefalse){
	SET_FLAG(truefalse,c_lflag,ISIG)
}

void serialportprofile::canonicalInput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ICANON)
}

void serialportprofile::escapedUpperCase(bool truefalse) {
	#ifdef XCASE
		SET_FLAG(truefalse,c_lflag,XCASE)
	#endif
}

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
	#ifdef ECHOCTL
		SET_FLAG(truefalse,c_lflag,ECHOCTL)
	#endif
}

void serialportprofile::echoErasedCharacter(bool truefalse) {
	#ifdef ECHOPRT
		SET_FLAG(truefalse,c_lflag,ECHOPRT)
	#endif
}

void serialportprofile::emulateKill(bool truefalse) {
	#ifdef ECHOKE
		SET_FLAG(truefalse,c_lflag,ECHOKE)
	#endif
}

void serialportprofile::noFlushAfterInterruptOrQuit(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,NOFLSH)
}

void serialportprofile::retypePendingCharacters(bool truefalse) {
	#ifdef PENDIN
		SET_FLAG(truefalse,c_lflag,PENDIN)
	#endif
}

void serialportprofile::sendSignalForBackgroundOutput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,TOSTOP)
}

bool serialportprofile::generateSignals() {
	return GET_FLAG(c_lflag,ISIG);
}

bool serialportprofile::canonicalInput() {
	return GET_FLAG(c_lflag,ICANON);
}

bool serialportprofile::escapedUpperCase() {
	#ifdef XCASE
		return GET_FLAG(c_lflag,XCASE);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

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
	#ifdef ECHOCTL
		return GET_FLAG(c_lflag,ECHOCTL);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::echoErasedCharacter() {
	#ifdef ECHOPRT
		return GET_FLAG(c_lflag,ECHOPRT);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::emulateKill() {
	#ifdef ECHOKE
		return GET_FLAG(c_lflag,ECHOKE);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::noFlushAfterInterruptOrQuit() {
	return GET_FLAG(c_lflag,NOFLSH);
}

bool serialportprofile::retypePendingCharacters() {
	#ifdef PENDIN
		return GET_FLAG(c_lflag,PENDIN);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

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

void serialportprofile::lowerCase(bool truefalse) {
	#ifdef IUCLC
		SET_FLAG(truefalse,c_iflag,IUCLC)
	#endif
}

void serialportprofile::bellIfLineTooLong(bool truefalse) {
	#ifdef IMAXBEL
		SET_FLAG(truefalse,c_iflag,IMAXBEL)
	#endif
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

bool serialportprofile::lowerCase() {
	#ifdef IUCLC
		return GET_FLAG(c_iflag,IUCLC);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::bellIfLineTooLong() {
	#ifdef IMAXBEL
		return GET_FLAG(c_iflag,IMAXBEL);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

void serialportprofile::postProcessOutput(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OPOST)
}

void serialportprofile::outputUpperCase(bool truefalse) {
	#ifdef OLCUC
		SET_FLAG(truefalse,c_oflag,OLCUC)
	#endif
}

void serialportprofile::mapNewLineToCarriageReturnNewLineOnOutput(
							bool truefalse) {
	SET_FLAG(truefalse,c_oflag,ONLCR)
}

void serialportprofile::discardEndOfTransmission(bool truefalse) {
	#ifdef ONOEOT
		SET_FLAG(truefalse,c_oflag,ONOEOT)
	#endif
}

void serialportprofile::mapCarriageReturnToNewLineOnOutput(bool truefalse) {
	#ifdef OCRNL
		SET_FLAG(truefalse,c_oflag,OCRNL)
	#endif
}

void serialportprofile::dontOutputCarriageReturnAtColumnZero(bool truefalse) {
	#ifdef ONOCR
		SET_FLAG(truefalse,c_oflag,ONOCR)
	#endif
}

void serialportprofile::mapNewLineToCarriageReturnOnOutput(bool truefalse) {
	#ifdef ONLRET
		SET_FLAG(truefalse,c_oflag,ONLRET)
	#endif
}

void serialportprofile::useFillCharactersForDelay(bool truefalse) {
	#ifdef OFILL
		SET_FLAG(truefalse,c_oflag,OFILL)
	#endif
}

void serialportprofile::useDelForFill(bool truefalse) {
	#ifdef OFDEL
		SET_FLAG(truefalse,c_oflag,OFDEL)
	#endif
}

void serialportprofile::expandTabToSpaces(bool truefalse) {
	#if defined(XTABS)
		SET_FLAG(truefalse,c_oflag,XTABS)
	#elif defined(OXTABS)
		SET_FLAG(truefalse,c_oflag,OXTABS)
	#else
		SET_FLAG(truefalse,c_oflag,TAB3)
	#endif
}

void serialportprofile::delayAfterNewLine(
		serialportprofile::newlinedelay_t nldelay) {
	#ifdef NLDLY
		static tcflag_t	nld[]={NL0,NL1};
		pvt->_tio.c_oflag&=~NLDLY;
		pvt->_tio.c_oflag|=nld[nldelay];
	#endif
}

void serialportprofile::delayAfterCarriageReturn(
		serialportprofile::carriagereturndelay_t crdelay) {
	#ifdef CRDLY
		static tcflag_t	crd[]={CR0,CR1,CR2,CR3};
		pvt->_tio.c_oflag&=~CRDLY;
		pvt->_tio.c_oflag|=crd[crdelay];
	#endif
}

void serialportprofile::delayAfterTab(
		serialportprofile::tabdelay_t tabdelay) {
	#ifdef TABDLY
		static tcflag_t td[]={
			#ifdef TAB0
			TAB0,
			#endif
			#ifdef TAB1
			TAB1,
			#endif
			#ifdef TAB2
			TAB2,
			#endif
			#ifdef TAB3
			TAB3
			#endif
			};
		pvt->_tio.c_oflag&=~TABDLY;
		pvt->_tio.c_oflag|=td[tabdelay];
	#endif
}

void serialportprofile::delayAfterBackSpace(
		serialportprofile::backspacedelay_t bsdelay) {
	#ifdef BSDLY
		static tcflag_t	bsd[]={BS0,BS1};
		pvt->_tio.c_oflag&=~BSDLY;
		pvt->_tio.c_oflag|=bsd[bsdelay];
	#endif
}

void serialportprofile::delayAfterVerticalTab(
		serialportprofile::verticaltabdelay_t vtdelay) {
	#ifdef VTDLY
		static tcflag_t	vtd[]={VT0,VT1};
		pvt->_tio.c_oflag&=~VTDLY;
		pvt->_tio.c_oflag|=vtd[vtdelay];
	#endif
}

void serialportprofile::delayAfterFormFeed(
		serialportprofile::formfeeddelay_t ffdelay) {
	#ifdef FFDLY
		static tcflag_t	ffd[]={FF0,FF1};
		pvt->_tio.c_oflag&=~FFDLY;
		pvt->_tio.c_oflag|=ffd[ffdelay];
	#endif
}

bool serialportprofile::postProcessOutput() {
	return GET_FLAG(c_oflag,OPOST);
}

bool serialportprofile::outputUpperCase() {
	#ifdef OLCUC
		return GET_FLAG(c_oflag,OLCUC);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::mapNewLineToCarriageReturnNewLineOnOutput() {
	return GET_FLAG(c_oflag,ONLCR);
}

bool serialportprofile::discardEndOfTransmission() {
	#ifdef ONOEOT
		return GET_FLAG(c_oflag,ONOEOT);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::mapCarriageReturnToNewLineOnOutput() {
	#ifdef OCRNL
		return GET_FLAG(c_oflag,OCRNL);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::dontOutputCarriageReturnAtColumnZero() {
	#ifdef ONOCR
		return GET_FLAG(c_oflag,ONOCR);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::mapNewLineToCarriageReturnOnOutput() {
	#ifdef ONLRET
		return GET_FLAG(c_oflag,ONLRET);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::useFillCharactersForDelay() {
	#ifdef OFILL
		return GET_FLAG(c_oflag,OFILL);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::useDelForFill() {
	#ifdef OFDEL
		return GET_FLAG(c_oflag,OFDEL);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serialportprofile::expandTabToSpaces() {
	#if defined(XTABS)
		return GET_FLAG(c_oflag,XTABS);
	#elif defined(OXTABS)
		return GET_FLAG(c_oflag,OXTABS);
	#else
		return GET_FLAG(c_oflag,TAB3);
	#endif
}

serialportprofile::newlinedelay_t
		serialportprofile::delayAfterNewLine() {
	#ifdef NLDLY
		switch (GET_FLAG(c_cflag,NLDLY)) {
			case NL1:
				return nl_100;
			default:
				return nl_none;
		}
	#else
		return nl_none;
	#endif
}

serialportprofile::carriagereturndelay_t
		serialportprofile::delayAfterCarriageReturn() {
	#ifdef CRDLY
		switch (GET_FLAG(c_cflag,CRDLY)) {
			case CR1:
				return cr_depends;
			case CR2:
				return cr_100;
			case CR3:
				return cr_150;
			default:
				return cr_none;
		}
	#else
		return cr_none;
	#endif
}

serialportprofile::tabdelay_t
		serialportprofile::delayAfterTab() {
	#ifdef TABDLY
		switch (GET_FLAG(c_cflag,TABDLY)) {
			#ifdef TAB1
			case TAB1:
				return td_1;
			#endif
			#ifdef TAB2
			case TAB2:
				return td_2;
			#endif
			#ifdef TAB3
			case TAB3:
				return td_xtabs;
			#endif
			default:
				return td_0;
		}
	#else
		return td_0;
	#endif
}

serialportprofile::backspacedelay_t
		serialportprofile::delayAfterBackSpace() {
	#ifdef BSDLY
		switch (GET_FLAG(c_cflag,BSDLY)) {
			case BS1:
				return bs_50;
			default:
				return bs_none;
		}
	#else
		return bs_none;
	#endif
}

serialportprofile::verticaltabdelay_t
		serialportprofile::delayAfterVerticalTab() {
	#ifdef VTDLY
		switch (GET_FLAG(c_cflag,VTDLY)) {
			case VT1:
				return vt_2;
			default:
				return vt_none;
		}
	#else
		return vt_none;
	#endif
}

serialportprofile::formfeeddelay_t
		serialportprofile::delayAfterFormFeed() {
	#ifdef FFDLY
		switch (GET_FLAG(c_cflag,FFDLY)) {
			case FF1:
				return ff_2;
			default:
				return ff_none;
		}
	#else
		return ff_none;
	#endif
}

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
	#ifdef VEOL2
		SET_CHAR(VEOL2,character)
	#endif
}

void serialportprofile::switchCharacer(cc_t character) {
	#if defined(VSWTCH)
		SET_CHAR(VSWTCH,character)
	#elif defined(VSWTC)
		SET_CHAR(VSWTC,character)
	#endif
}

void serialportprofile::startCharacter(cc_t character) {
	SET_CHAR(VSTART,character)
}

void serialportprofile::stopCharacter(cc_t character) {
	SET_CHAR(VSTOP,character)
}

void serialportprofile::suspendCharacter(cc_t character) {
	SET_CHAR(VSUSP,character)
}

void serialportprofile::delayedSuspendCharacter(cc_t character) {
	#ifdef VDSUSP
		SET_CHAR(VDSUSP,character)
	#endif
}

void serialportprofile::literalNextCharcter(cc_t character) {
	#ifdef VLNEXT
		SET_CHAR(VLNEXT,character)
	#endif
}

void serialportprofile::wordEraseCharcter(cc_t character) {
	#ifdef VWERASE
		SET_CHAR(VWERASE,character)
	#endif
}


void serialportprofile::reprintCharacter(cc_t character) {
	#ifdef VREPRINT
		SET_CHAR(VREPRINT,character)
	#endif
}

void serialportprofile::discardPendingOutputCharacter(cc_t character) {
	#ifdef VDISCARD
		SET_CHAR(VDISCARD,character)
	#endif
}

void serialportprofile::statusRequestCharacter(cc_t character) {
	#ifdef VSTATUS
		SET_CHAR(VSTATUS,character)
	#endif
}

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
	#ifdef VEOL2
		return GET_CHAR(VEOL2);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

cc_t serialportprofile::switchCharacer() {
	#if defined(VSWTCH)
		return GET_CHAR(VSWTCH);
	#elif defined(VSWTC)
		return GET_CHAR(VSWTC);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

cc_t serialportprofile::startCharacter() {
	return GET_CHAR(VSTART);
}

cc_t serialportprofile::stopCharacter() {
	return GET_CHAR(VSTOP);
}

cc_t serialportprofile::suspendCharacter() {
	return GET_CHAR(VSUSP);
}

cc_t serialportprofile::delayedSuspendCharacter() {
	#ifdef VDSUSP
		return GET_CHAR(VDSUSP);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

cc_t serialportprofile::literalNextCharcter() {
	#ifdef VLNEXT
		return GET_CHAR(VLNEXT);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

cc_t serialportprofile::wordEraseCharcter() {
	#ifdef VWERASE
		return GET_CHAR(VWERASE);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

cc_t serialportprofile::reprintCharacter() {
	#ifdef VREPRINT
		return GET_CHAR(VREPRINT);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

cc_t serialportprofile::discardPendingOutputCharacter() {
	#ifdef VDISCARD
		return GET_CHAR(VDISCARD);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

cc_t serialportprofile::statusRequestCharacter() {
	#ifdef VSTATUS
		return GET_CHAR(VSTATUS);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

cc_t serialportprofile::readThreshold() {
	return GET_CHAR(VMIN);
}

cc_t serialportprofile::readTimeout() {
	return GET_CHAR(VTIME);
}

termios *serialportprofile::getTermios() {
	return &pvt->_tio;
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
	#if defined(B19200)
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

#ifdef RUDIMENTS_NAMESPACE
}
#endif
