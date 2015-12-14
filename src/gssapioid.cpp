/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* lib/gssapi/generic/oid_ops.c */
/*
 * Copyright 1995 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
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
/*
 * Copyright 1993 by OpenVision Technologies, Inc.
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

/* GSS-API V2 interfaces to manipulate OIDs */

#include <rudiments/character.h>

#include <gssapi/gssapi_generic.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_HAVE_LIMITS_H
	#include <limits.h>
#endif

/* Return the length of a DER OID subidentifier encoding. */
static size_t
arc_encoded_length(unsigned long arc)
{
    size_t len = 1;

    for (arc >>= 7; arc; arc >>= 7)
        len++;
    return len;
}

/* Encode a subidentifier into *bufp and advance it to the encoding's end. */
static void
arc_encode(unsigned long arc, unsigned char **bufp)
{
    unsigned char *p;

    /* Advance to the end and encode backwards. */
    p = *bufp = *bufp + arc_encoded_length(arc);
    *--p = arc & 0x7f;
    for (arc >>= 7; arc; arc >>= 7)
        *--p = (arc & 0x7f) | 0x80;
}

/* Fetch an arc value from *bufp and advance past it and any following spaces
 * or periods.  Return 1 on success, 0 if *bufp is not at a valid arc value. */
static int
get_arc(const unsigned char **bufp, const unsigned char *end,
        unsigned long *arc_out)
{
    const unsigned char *p = *bufp;
    unsigned long arc = 0, newval;

    if (p == end || !character::isDigit(*p))
        return 0;
    for (; p < end && character::isDigit(*p); p++) {
        newval = arc * 10 + (*p - '0');
        if (newval < arc)
            return 0;
        arc = newval;
    }
    while (p < end && (character::isWhitespace(*p) || *p == '.'))
        p++;
    *bufp = p;
    *arc_out = arc;
    return 1;
}

/*
 * Convert a sequence of two or more decimal arc values into a DER-encoded OID.
 * The values may be separated by any combination of whitespace and period
 * characters, and may be optionally surrounded with braces.  Leading
 * whitespace and trailing garbage is allowed.  The first arc value must be 0,
 * 1, or 2, and the second value must be less than 40 if the first value is not
 * 2.
 */
static OM_uint32
gss_str_to_oid1(OM_uint32 *minor_status,
                       gss_buffer_t oid_str,
                       gss_OID *oid_out)
{
    const unsigned char *p, *end, *arc3_start;
    unsigned char *out;
    unsigned long arc, arc1, arc2;
    size_t nbytes;
    int brace = 0;
    gss_OID oid;

    if (minor_status != NULL)
        *minor_status = 0;

    if (oid_out != NULL)
        *oid_out = GSS_C_NO_OID;

    if (GSS_EMPTY_BUFFER(oid_str))
        return (GSS_S_CALL_INACCESSIBLE_READ);

    if (oid_out == NULL)
        return (GSS_S_CALL_INACCESSIBLE_WRITE);

    /* Skip past initial spaces and, optionally, an open brace. */
    brace = 0;
    p = (unsigned char *)oid_str->value;
    end = p + oid_str->length;
    while (p < end && character::isWhitespace(*p))
        p++;
    if (p < end && *p == '{') {
        brace = 1;
        p++;
    }
    while (p < end && character::isWhitespace(*p))
        p++;

    /* Get the first two arc values, to be encoded as one subidentifier. */
    if (!get_arc(&p, end, &arc1) || !get_arc(&p, end, &arc2))
        return (GSS_S_FAILURE);
    if (arc1 > 2 || (arc1 < 2 && arc2 > 39) || arc2 > ULONG_MAX - 80)
        return (GSS_S_FAILURE);
    arc3_start = p;

    /* Compute the total length of the encoding while checking syntax. */
    nbytes = arc_encoded_length(arc1 * 40 + arc2);
    while (get_arc(&p, end, &arc))
        nbytes += arc_encoded_length(arc);
    if (brace && (p == end || *p != '}'))
        return (GSS_S_FAILURE);

    /* Allocate an oid structure. */
    oid = (gss_OID)malloc(sizeof(*oid));
    if (oid == NULL)
        return (GSS_S_FAILURE);
    oid->elements = malloc(nbytes);
    if (oid->elements == NULL) {
        free(oid);
        return (GSS_S_FAILURE);
    }
    oid->length = nbytes;

    out = (unsigned char *)oid->elements;
    arc_encode(arc1 * 40 + arc2, &out);
    p = arc3_start;
    while (get_arc(&p, end, &arc))
        arc_encode(arc, &out);
    //assert(out - nbytes == oid->elements);
    *oid_out = oid;
    return(GSS_S_COMPLETE);
}
