/*
 * Copyright 1994 by OpenVision Technologies, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appears in all copies and
 * that both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of OpenVision not be used
 * in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission. OpenVision makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * OPENVISION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL OPENVISION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
/*
 * Copyright (C) 2003, 2004 by the Massachusetts Institute of Technology.
 * All rights reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  Furthermore if you modify this software you must label
 * your software as modified software and not distribute it in such a
 * fashion that it might be confused with the original M.I.T. software.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 */

#include <rudiments/stdio.h>
#include <rudiments/gss.h>

#include "gssmisc.h"

void displayAvailableMechanisms() {
	gss	g;
	const char * const	*mechs=g.getAvailableMechanisms();
	stdoutput.printf("  Available Mechanisms {\n");
	for (const char * const *m=mechs; *m; m++) {
		stdoutput.printf("    %s\n",*m);
	}
	stdoutput.printf("  }\n");
}

void displayCredentials(gsscredentials *gcred) {
	stdoutput.write("  Credentials {\n");
	stdoutput.printf("    name: %s\n",gcred->getName());
	stdoutput.printf("    desired lifetime: %d\n",
				gcred->getDesiredLifetime());
	stdoutput.printf("    actual lifetime: %d\n",
				gcred->getActualLifetime());
	stdoutput.write("    desired mechanisms:\n");
	for (uint64_t i=0; i<gcred->getDesiredMechanismCount(); i++) {
		gssmechanism	*gmech=gcred->getDesiredMechanism(i);
		if (gmech) {
			stdoutput.printf("      %s\n",gmech->getString());
		}
	}
	stdoutput.write("    actual mechanisms:\n");
	for (uint64_t i=0; i<gcred->getActualMechanismCount(); i++) {
		gssmechanism	*gmech=gcred->getActualMechanism(i);
		if (gmech) {
			stdoutput.printf("      %s\n",gmech->getString());
		}
	}
	stdoutput.write("  }\n");
}

static void displayFlags(uint32_t flags) {
	if (flags&GSS_C_DELEG_FLAG) {
		stdoutput.write("GSS_C_DELEG_FLAG ");
	}
	if (flags&GSS_C_MUTUAL_FLAG) {
		stdoutput.write("GSS_C_MUTUAL_FLAG ");
	}
	if (flags&GSS_C_REPLAY_FLAG) {
		stdoutput.write("GSS_C_REPLAY_FLAG ");
	}
	if (flags&GSS_C_SEQUENCE_FLAG) {
		stdoutput.write("GSS_C_SEQUENCE_FLAG ");
	}
	if (flags&GSS_C_CONF_FLAG) {
		stdoutput.write("GSS_C_CONF_FLAG ");
	}
	if (flags&GSS_C_INTEG_FLAG) {
		stdoutput.write("GSS_C_INTEG_FLAG ");
	}
	if (flags&GSS_C_ANON_FLAG) {
		stdoutput.write("GSS_C_ANON_FLAG ");
	}
	if (flags&GSS_C_PROT_READY_FLAG) {
		stdoutput.write("GSS_C_PROT_READY_FLAG ");
	}
	if (flags&GSS_C_TRANS_FLAG) {
		stdoutput.write("GSS_C_TRANS_FLAG ");
	}
	#ifdef GSS_C_DELEG_POLICY_FLAG
	if (flags&GSS_C_DELEG_POLICY_FLAG) {
		stdoutput.write("GSS_C_DELEG_POLICY_FLAG ");
	}
	#endif
}

void displayContext(gsscontext *gctx) {
	stdoutput.write("  Context {\n");
	stdoutput.printf("    initiator: %s\n",
				gctx->getInitiator());
	stdoutput.printf("    initiator type: %s\n",
				gctx->getInitiatorType());
	stdoutput.printf("    acceptor: %s\n",
				gctx->getAcceptor());
	stdoutput.printf("    acceptor type: %s\n",
				gctx->getAcceptorType());
	stdoutput.printf("    desired lifetime: %d\n",
				gctx->getDesiredLifetime());
	stdoutput.printf("    actual lifetime: %d\n",
				gctx->getActualLifetime());
	stdoutput.printf("    remaining lifetime: %d\n",
				gctx->getRemainingLifetime());
	stdoutput.printf("    desired mechanism: %s\n",
			(gctx->getDesiredMechanism())?
				gctx->getDesiredMechanism()->getString():
				"(none)");
	stdoutput.printf("    actual mechanism: %s\n",
			(gctx->getActualMechanism())?
				gctx->getActualMechanism()->getString():
				"(none)");
	stdoutput.write("    desired flags: ");
	displayFlags(gctx->getDesiredFlags());
	stdoutput.write('\n');
	stdoutput.write("    actual flags: ");
	displayFlags(gctx->getActualFlags());
	stdoutput.write('\n');
	stdoutput.printf("    initiator: %s\n",
				(gctx->getIsInitiator())?"self":"peer");
	stdoutput.printf("    open: %s\n",
				(gctx->getIsOpen())?"yes":"no");
	stdoutput.printf("    file descriptor: %d\n",
				(gctx->getFileDescriptor())?
					gctx->getFileDescriptor()->
						getFileDescriptor():-1);
	stdoutput.write("  }\n");
}

void displayData(const char *msg, const unsigned char *data, size_t datasize) {
	stdoutput.printf("\n  %s (size=%d):\n  ",msg,datasize);
	stdoutput.safePrint(data,datasize);
	stdoutput.write('\n');
}
