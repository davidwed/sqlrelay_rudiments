// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

RUDIMENTS_INLINE
void serialportprofile::setControlOptions(tcflag_t flags) {
	tio.c_cflag=flags;
}

RUDIMENTS_INLINE
void serialportprofile::setLocalOptions(tcflag_t flags) {
	tio.c_lflag=flags;
}

RUDIMENTS_INLINE
void serialportprofile::setInputOptions(tcflag_t flags) {
	tio.c_iflag=flags;
}

RUDIMENTS_INLINE
void serialportprofile::setOutputOptions(tcflag_t flags) {
	tio.c_oflag=flags;
}

RUDIMENTS_INLINE
void serialportprofile::baud(serialportprofile::baudrate_t baud) {
	#ifdef CBAUDEX
	tio.c_cflag&=(CBAUD|CBAUDEX);
	#else
	tio.c_cflag&=CBAUD;
	#endif
	tio.c_cflag|=(tcflag_t)baud;
}

RUDIMENTS_INLINE
void serialportprofile::characterSize(serialportprofile::charsize_t size) {
	tio.c_cflag&=CSIZE;
	tio.c_cflag|=(tcflag_t)size;
}

#define SET_FLAG(truefalse,flag,value) \
	if (truefalse) { tio.flag|=value; } else { tio.flag&=~value; }

#define SET_CHAR(character,value) \
	tio.c_cc[character]=value;

#define GET_FLAG(flag,value) (tio.flag&value)

#define GET_CHAR(character) (tio.c_cc[character])

RUDIMENTS_INLINE
void serialportprofile::stopBits(serialportprofile::stopbits_t stopbits) {
	SET_FLAG((stopbits==onestopbit),c_cflag,CSTOPB)
}

RUDIMENTS_INLINE
void serialportprofile::receiverOn(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,CREAD)
}

RUDIMENTS_INLINE
void serialportprofile::parityCheckEnabled(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,PARENB)
}

RUDIMENTS_INLINE
void serialportprofile::oddParity(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,PARODD)
}

RUDIMENTS_INLINE
void serialportprofile::hangupOnClose(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,HUPCL)
}

RUDIMENTS_INLINE
void serialportprofile::ignoreModemControlLines(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,CLOCAL)
}

#ifdef LOBLK
RUDIMENTS_INLINE
void serialportprofile::blockJobControlOutput(bool truefalse) {
	SET_FLAG(truefalse,c_cflag,LOBLK)
}
#endif

RUDIMENTS_INLINE
void serialportprofile::hardwareFlowControl(bool truefalse) {
	#ifdef NEW_CRTSCTS
		SET_FLAG(truefalse,c_cflag,NEW_CRTSCTS)
	#else
		SET_FLAG(truefalse,c_cflag,CRTSCTS)
	#endif
}

RUDIMENTS_INLINE
serialportprofile::baudrate_t serialportprofile::baud() {
	return (baudrate_t)GET_FLAG(c_cflag,CBAUD);
}

RUDIMENTS_INLINE
serialportprofile::charsize_t serialportprofile::characterSize() {
	return (charsize_t)GET_FLAG(c_cflag,CSIZE);
}

RUDIMENTS_INLINE
serialportprofile::stopbits_t serialportprofile::stopBits() {
	bool	two=GET_FLAG(c_cflag,CSTOPB);
	return (two)?twostopbits:onestopbit;
}

RUDIMENTS_INLINE
bool serialportprofile::receiverOn() {
	return GET_FLAG(c_cflag,CREAD);
}

RUDIMENTS_INLINE
bool serialportprofile::parityCheckEnabled() {
	return GET_FLAG(c_cflag,PARENB);
}

RUDIMENTS_INLINE
bool serialportprofile::oddParity() {
	return GET_FLAG(c_cflag,PARODD);
}

RUDIMENTS_INLINE
bool serialportprofile::hangupOnClose() {
	return GET_FLAG(c_cflag,HUPCL);
}

RUDIMENTS_INLINE
bool serialportprofile::ignoreModemControlLines() {
	return GET_FLAG(c_cflag,CLOCAL);
}

#ifdef LOBLK
RUDIMENTS_INLINE
bool serialportprofile::blockJobControlOutput() {
	return GET_FLAG(c_cflag,LOBLK);
}
#endif

RUDIMENTS_INLINE
bool serialportprofile::hardwareFlowControl() {
	#ifdef NEW_CRTSCTS
		return GET_FLAG(c_cflag,NEW_CRTSCTS);
	#else
		return GET_FLAG(c_cflag,CRTSCTS);
	#endif
}

RUDIMENTS_INLINE
void serialportprofile::generateSignals(bool truefalse){
	SET_FLAG(truefalse,c_lflag,ISIG)
}

RUDIMENTS_INLINE
void serialportprofile::canonicalInput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ICANON)
}

#ifdef XCASE
RUDIMENTS_INLINE
void serialportprofile::upperCaseUnescapedCharacters(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,XCASE)
}
#endif

RUDIMENTS_INLINE
void serialportprofile::echoInput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHO)
}

RUDIMENTS_INLINE
void serialportprofile::echoErase(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOE)
}

RUDIMENTS_INLINE
void serialportprofile::echoNewLineAfterKill(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOK)
}

RUDIMENTS_INLINE
void serialportprofile::echoNewLine(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHONL)
}

RUDIMENTS_INLINE
void serialportprofile::flushAfterInterruptOrQuit(bool truefalse) {
	SET_FLAG((!truefalse),c_lflag,NOFLSH)
}

RUDIMENTS_INLINE
void serialportprofile::extendedFunctions(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,IEXTEN)
}

RUDIMENTS_INLINE
void serialportprofile::echoControlCharacters(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOCTL)
}

RUDIMENTS_INLINE
void serialportprofile::echoErasedCharacter(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOPRT)
}

RUDIMENTS_INLINE
void serialportprofile::lineKill(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,ECHOKE)
}

RUDIMENTS_INLINE
void serialportprofile::retypePendingCharacters(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,PENDIN)
}

RUDIMENTS_INLINE
void serialportprofile::sendSignalForBackgroundOutput(bool truefalse) {
	SET_FLAG(truefalse,c_lflag,TOSTOP)
}

RUDIMENTS_INLINE
bool serialportprofile::generateSignals() {
	return GET_FLAG(c_lflag,ISIG);
}

RUDIMENTS_INLINE
bool serialportprofile::canonicalInput() {
	return GET_FLAG(c_lflag,ICANON);
}

#ifdef HAVE_XCASE
RUDIMENTS_INLINE
bool serialportprofile::upperCaseUnescapedCharacters() {
	return GET_FLAG(c_lflag,XCASE);
}
#endif

RUDIMENTS_INLINE
bool serialportprofile::echoInput() {
	return GET_FLAG(c_lflag,ECHO);
}

RUDIMENTS_INLINE
bool serialportprofile::echoErase() {
	return GET_FLAG(c_lflag,ECHOE);
}

RUDIMENTS_INLINE
bool serialportprofile::echoNewLineAfterKill() {
	return GET_FLAG(c_lflag,ECHOK);
}

RUDIMENTS_INLINE
bool serialportprofile::echoNewLine() {
	return GET_FLAG(c_lflag,ECHONL);
}

RUDIMENTS_INLINE
bool serialportprofile::flushAfterInterruptOrQuit() {
	bool	retval=!(GET_FLAG(c_lflag,NOFLSH));
	return retval;
}

RUDIMENTS_INLINE
bool serialportprofile::extendedFunctions() {
	return GET_FLAG(c_lflag,IEXTEN);
}

RUDIMENTS_INLINE
bool serialportprofile::echoControlCharacters() {
	return GET_FLAG(c_lflag,ECHOCTL);
}

RUDIMENTS_INLINE
bool serialportprofile::echoErasedCharacter() {
	return GET_FLAG(c_lflag,ECHOPRT);
}

RUDIMENTS_INLINE
bool serialportprofile::lineKill() {
	return GET_FLAG(c_lflag,ECHOKE);
}

RUDIMENTS_INLINE
bool serialportprofile::retypePendingCharacters() {
	return GET_FLAG(c_lflag,PENDIN);
}

RUDIMENTS_INLINE
bool serialportprofile::sendSignalForBackgroundOutput() {
	return GET_FLAG(c_lflag,TOSTOP);
}

RUDIMENTS_INLINE
void serialportprofile::enableParityCheck(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,INPCK)
}

RUDIMENTS_INLINE
void serialportprofile::markParityErrors(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,PARMRK)
}

RUDIMENTS_INLINE
void serialportprofile::stripParityBits(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,ISTRIP)
}

RUDIMENTS_INLINE
void serialportprofile::enableSoftwareFlowControlOnOutput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IXON)
}

RUDIMENTS_INLINE
void serialportprofile::enableSoftwareFlowControlOnInput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IXOFF)
}

RUDIMENTS_INLINE
void serialportprofile::enableAnyCharacterStartsFlow(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IXANY)
}

RUDIMENTS_INLINE
void serialportprofile::ignoreBreak(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IGNBRK)
}

RUDIMENTS_INLINE
void serialportprofile::sendSignalOnBreak(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,BRKINT)
}

RUDIMENTS_INLINE
void serialportprofile::mapNewLineToCarriageReturnOnInput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,INLCR)
}

RUDIMENTS_INLINE
void serialportprofile::ignoreCarriageReturn(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IGNCR)
}

RUDIMENTS_INLINE
void serialportprofile::mapCarriageReturnToNewLineOnInput(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,ICRNL)
}

RUDIMENTS_INLINE
void serialportprofile::lowerCase(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IUCLC)
}

RUDIMENTS_INLINE
void serialportprofile::bellIfLineTooLong(bool truefalse) {
	SET_FLAG(truefalse,c_iflag,IMAXBEL)
}

RUDIMENTS_INLINE
bool serialportprofile::enableParityCheck() {
	return GET_FLAG(c_iflag,INPCK);
}

RUDIMENTS_INLINE
bool serialportprofile::markParityErrors() {
	return GET_FLAG(c_iflag,PARMRK);
}

RUDIMENTS_INLINE
bool serialportprofile::stripParityBits() {
	return GET_FLAG(c_iflag,ISTRIP);
}

RUDIMENTS_INLINE
bool serialportprofile::enableSoftwareFlowControlOnOutput() {
	return GET_FLAG(c_iflag,IXON);
}

RUDIMENTS_INLINE
bool serialportprofile::enableSoftwareFlowControlOnInput() {
	return GET_FLAG(c_iflag,IXOFF);
}

RUDIMENTS_INLINE
bool serialportprofile::enableAnyCharacterStartsFlow() {
	return GET_FLAG(c_iflag,IXANY);
}

RUDIMENTS_INLINE
bool serialportprofile::ignoreBreak() {
	return GET_FLAG(c_iflag,IGNBRK);
}

RUDIMENTS_INLINE
bool serialportprofile::sendSignalOnBreak() {
	return GET_FLAG(c_iflag,BRKINT);
}

RUDIMENTS_INLINE
bool serialportprofile::mapNewLineToCarriageReturnOnInput() {
	return GET_FLAG(c_iflag,INLCR);
}

RUDIMENTS_INLINE
bool serialportprofile::ignoreCarriageReturn() {
	return GET_FLAG(c_iflag,IGNCR);
}

RUDIMENTS_INLINE
bool serialportprofile::mapCarriageReturnToNewLineOnInput() {
	return GET_FLAG(c_iflag,ICRNL);
}

RUDIMENTS_INLINE
bool serialportprofile::lowerCase() {
	return GET_FLAG(c_iflag,IUCLC);
}

RUDIMENTS_INLINE
bool serialportprofile::bellIfLineTooLong() {
	return GET_FLAG(c_iflag,IMAXBEL);
}

RUDIMENTS_INLINE
void serialportprofile::postProcessOutput(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OPOST)
}

RUDIMENTS_INLINE
void serialportprofile::outputUpperCase(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OLCUC)
}

RUDIMENTS_INLINE
void serialportprofile::mapNewLineToCarriageReturnNewLineOnOutput(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,ONLCR)
}

RUDIMENTS_INLINE
void serialportprofile::mapCarriageReturnToNewLineOnOutput(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OCRNL)
}

RUDIMENTS_INLINE
void serialportprofile::outputCarriageReturnAtColumnZero(bool truefalse) {
	SET_FLAG((!truefalse),c_oflag,ONOCR)
}

RUDIMENTS_INLINE
void serialportprofile::mapNewLineToCarriageReturnOnOutput(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,ONLRET)
}

RUDIMENTS_INLINE
void serialportprofile::useFillCharactersForDelay(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OFILL)
}

RUDIMENTS_INLINE
void serialportprofile::useDelForFill(bool truefalse) {
	SET_FLAG(truefalse,c_oflag,OFDEL)
}

RUDIMENTS_INLINE
void serialportprofile::expandTabToSpaces(bool truefalse) {
	#ifdef XTABS
		SET_FLAG(truefalse,c_oflag,XTABS)
	#else
		SET_FLAG(truefalse,c_oflag,TAB3)
	#endif
}


RUDIMENTS_INLINE
void serialportprofile::delayAfterNewLine(
		serialportprofile::newlinedelay_t nldelay) {
	tio.c_oflag&=NLDLY;
	tio.c_oflag|=(tcflag_t)nldelay;
}

RUDIMENTS_INLINE
void serialportprofile::delayAfterCarriageReturn(
		serialportprofile::carriagereturndelay_t crdelay) {
	tio.c_oflag&=CRDLY;
	tio.c_oflag|=(tcflag_t)crdelay;
}

RUDIMENTS_INLINE
void serialportprofile::delayAfterTab(
		serialportprofile::tabdelay_t tabdelay) {
	tio.c_oflag&=TABDLY;
	tio.c_oflag|=(tcflag_t)tabdelay;
}

RUDIMENTS_INLINE
void serialportprofile::delayAfterBackSpace(
		serialportprofile::backspacedelay_t bsdelay) {
	tio.c_oflag&=BSDLY;
	tio.c_oflag|=(tcflag_t)bsdelay;
}

RUDIMENTS_INLINE
void serialportprofile::delayAfterVerticalTab(
		serialportprofile::verticaltabdelay_t vtdelay) {
	tio.c_oflag&=VTDLY;
	tio.c_oflag|=(tcflag_t)vtdelay;
}

RUDIMENTS_INLINE
void serialportprofile::delayAfterFormFeed(
		serialportprofile::formfeeddelay_t ffdelay) {
	tio.c_oflag&=FFDLY;
	tio.c_oflag|=(tcflag_t)ffdelay;
}

RUDIMENTS_INLINE
bool serialportprofile::postProcessOutput() {
	return GET_FLAG(c_oflag,OPOST);
}

RUDIMENTS_INLINE
bool serialportprofile::outputUpperCase() {
	return GET_FLAG(c_oflag,OLCUC);
}

RUDIMENTS_INLINE
bool serialportprofile::mapNewLineToCarriageReturnNewLineOnOutput() {
	return GET_FLAG(c_oflag,ONLCR);
}

RUDIMENTS_INLINE
bool serialportprofile::mapCarriageReturnToNewLineOnOutput() {
	return GET_FLAG(c_oflag,OCRNL);
}

RUDIMENTS_INLINE
bool serialportprofile::outputCarriageReturnAtColumnZero() {
	return GET_FLAG(c_oflag,ONOCR);
}

RUDIMENTS_INLINE
bool serialportprofile::mapNewLineToCarriageReturnOnOutput() {
	return GET_FLAG(c_oflag,ONLRET);
}

RUDIMENTS_INLINE
bool serialportprofile::useFillCharactersForDelay() {
	return GET_FLAG(c_oflag,OFILL);
}

RUDIMENTS_INLINE
bool serialportprofile::useDelForFill() {
	return GET_FLAG(c_oflag,OFDEL);
}

RUDIMENTS_INLINE
bool serialportprofile::expandTabToSpaces() {
	#ifdef XTABS
		return GET_FLAG(c_oflag,XTABS);
	#else
		return GET_FLAG(c_oflag,TAB3);
	#endif
}

RUDIMENTS_INLINE
serialportprofile::newlinedelay_t
		serialportprofile::delayAfterNewLine() {
	return (newlinedelay_t)GET_FLAG(c_cflag,NLDLY);
}

RUDIMENTS_INLINE
serialportprofile::carriagereturndelay_t
		serialportprofile::delayAfterCarriageReturn() {
	return (carriagereturndelay_t)GET_FLAG(c_cflag,CRDLY);
}

RUDIMENTS_INLINE
serialportprofile::tabdelay_t
		serialportprofile::delayAfterTab() {
	return (tabdelay_t)GET_FLAG(c_cflag,TABDLY);
}

RUDIMENTS_INLINE
serialportprofile::backspacedelay_t
		serialportprofile::delayAfterBackSpace() {
	return (backspacedelay_t)GET_FLAG(c_cflag,BSDLY);
}

RUDIMENTS_INLINE
serialportprofile::verticaltabdelay_t
		serialportprofile::delayAfterVerticalTab() {
	return (verticaltabdelay_t)GET_FLAG(c_cflag,VTDLY);
}

RUDIMENTS_INLINE
serialportprofile::formfeeddelay_t
		serialportprofile::delayAfterFormFeed() {
	return (formfeeddelay_t)GET_FLAG(c_cflag,FFDLY);
}

RUDIMENTS_INLINE
void serialportprofile::interruptCharacter(cc_t character) {
	SET_CHAR(VINTR,character)
}

RUDIMENTS_INLINE
void serialportprofile::quitCharacter(cc_t character) {
	SET_CHAR(VQUIT,character)
}

RUDIMENTS_INLINE
void serialportprofile::eraseCharacter(cc_t character) {
	SET_CHAR(VERASE,character)
}

RUDIMENTS_INLINE
void serialportprofile::killCharacter(cc_t character) {
	SET_CHAR(VKILL,character)
}

RUDIMENTS_INLINE
void serialportprofile::endOfFileCharacter(cc_t character) {
	SET_CHAR(VEOF,character)
}

RUDIMENTS_INLINE
void serialportprofile::endOfLineCharacter(cc_t character) {
	SET_CHAR(VEOL,character)
}

RUDIMENTS_INLINE
void serialportprofile::secondEndOfLineCharacter(cc_t character) {
	SET_CHAR(VEOL2,character)
}

RUDIMENTS_INLINE
void serialportprofile::switchCharacer(cc_t character) {
	#ifdef VSWTCH
	SET_CHAR(VSWTCH,character)
	#else
	SET_CHAR(VSWTC,character)
	#endif
}

RUDIMENTS_INLINE
void serialportprofile::startCharacter(cc_t character) {
	SET_CHAR(VSTART,character)
}

RUDIMENTS_INLINE
void serialportprofile::stopCharacter(cc_t character) {
	SET_CHAR(VSTOP,character)
}

RUDIMENTS_INLINE
void serialportprofile::suspendCharacter(cc_t character) {
	SET_CHAR(VSUSP,character)
}

#ifdef VDSUSP
RUDIMENTS_INLINE
void serialportprofile::delayedSuspendCharacter(cc_t character) {
	SET_CHAR(VDSUSP,character)
}
#endif

RUDIMENTS_INLINE
void serialportprofile::literalNextCharcter(cc_t character) {
	SET_CHAR(VLNEXT,character)
}

RUDIMENTS_INLINE
void serialportprofile::wordEraseCharcter(cc_t character) {
	SET_CHAR(VWERASE,character)
}


RUDIMENTS_INLINE
void serialportprofile::reprintCharacter(cc_t character) {
	SET_CHAR(VREPRINT,character)
}

RUDIMENTS_INLINE
void serialportprofile::discardPendingOutputCharacter(cc_t character) {
	SET_CHAR(VDISCARD,character)
}

#ifdef VSTATUS
RUDIMENTS_INLINE
void serialportprofile::statusRequestCharacter(cc_t character) {
	SET_CHAR(VSTATUS,character)
}
#endif

RUDIMENTS_INLINE
void serialportprofile::readThreshold(cc_t count) {
	SET_CHAR(VMIN,count)
}

RUDIMENTS_INLINE
void serialportprofile::readTimeout(cc_t deciseconds) {
	SET_CHAR(VTIME,deciseconds)
}

RUDIMENTS_INLINE
cc_t serialportprofile::interruptCharacter() {
	return GET_CHAR(VINTR);
}

RUDIMENTS_INLINE
cc_t serialportprofile::quitCharacter() {
	return GET_CHAR(VQUIT);
}

RUDIMENTS_INLINE
cc_t serialportprofile::eraseCharacter() {
	return GET_CHAR(VERASE);
}

RUDIMENTS_INLINE
cc_t serialportprofile::killCharacter() {
	return GET_CHAR(VKILL);
}

RUDIMENTS_INLINE
cc_t serialportprofile::endOfFileCharacter() {
	return GET_CHAR(VEOF);
}

RUDIMENTS_INLINE
cc_t serialportprofile::endOfLineCharacter() {
	return GET_CHAR(VEOL);
}

RUDIMENTS_INLINE
cc_t serialportprofile::secondEndOfLineCharacter() {
	return GET_CHAR(VEOL2);
}

RUDIMENTS_INLINE
cc_t serialportprofile::switchCharacer() {
	#ifdef VSWTCH
	return GET_CHAR(VSWTCH);
	#else
	return GET_CHAR(VSWTC);
	#endif
}

RUDIMENTS_INLINE
cc_t serialportprofile::startCharacter() {
	return GET_CHAR(VSTART);
}

RUDIMENTS_INLINE
cc_t serialportprofile::stopCharacter() {
	return GET_CHAR(VSTOP);
}

RUDIMENTS_INLINE
cc_t serialportprofile::suspendCharacter() {
	return GET_CHAR(VSUSP);
}

#ifdef VDSUSP
RUDIMENTS_INLINE
cc_t serialportprofile::delayedSuspendCharacter() {
	return GET_CHAR(VDSUSP);
}
#endif

RUDIMENTS_INLINE
cc_t serialportprofile::literalNextCharcter() {
	return GET_CHAR(VLNEXT);
}

RUDIMENTS_INLINE
cc_t serialportprofile::wordEraseCharcter() {
	return GET_CHAR(VWERASE);
}

RUDIMENTS_INLINE
cc_t serialportprofile::reprintCharacter() {
	return GET_CHAR(VREPRINT);
}

RUDIMENTS_INLINE
cc_t serialportprofile::discardPendingOutputCharacter() {
	return GET_CHAR(VDISCARD);
}

#ifdef VSTATUS
RUDIMENTS_INLINE
cc_t serialportprofile::statusRequestCharacter() {
	return GET_CHAR(VSTATUS);
}
#endif

RUDIMENTS_INLINE
cc_t serialportprofile::readThreshold() {
	return GET_CHAR(VMIN);
}

RUDIMENTS_INLINE
cc_t serialportprofile::readTimeout() {
	return GET_CHAR(VTIME);
}

RUDIMENTS_INLINE
termios *serialportprofile::getTermios() {
	return &tio;
}
