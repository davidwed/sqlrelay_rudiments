// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/serialportprofile.h>

#ifndef HAVE_RUDIMENTS_INLINES
	#include <rudiments/private/serialportprofileinlines.h>
#endif

#include <rudiments/character.h>

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
