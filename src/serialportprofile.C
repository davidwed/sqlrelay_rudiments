// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/serialportprofile.h>

#ifndef HAVE_RUDIMENTS_INLINES
	#include <rudiments/private/serialportprofileinlines.h>
#endif

#include <rudiments/character.h>
#include <rudiments/charstring.h>

serialportprofile::serialportprofile() {
	defaultOptions();
}

serialportprofile::~serialportprofile() {
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
