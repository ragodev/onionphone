/* vim: set tabstop=4:softtabstop=4:shiftwidth=4:noexpandtab */

/***********************************************************************
Copyright (C) 2013 Xiph.Org Foundation and contributors.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
- Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
- Neither the name of Internet Society, IETF or IETF Trust, nor the
names of specific contributors, may be used to endorse or promote
products derived from this software without specific prior written
permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
***********************************************************************/

#ifndef SILK_MACROS_ARMv4_H
#define SILK_MACROS_ARMv4_H

/* (a32 * (int32_t)((int16_t)(b32))) >> 16 output have to be 32bit int */
#undef silk_SMULWB
static inline int32_t silk_SMULWB_armv4(int32_t a, int16_t b)
{
	unsigned rd_lo;
	int rd_hi;
 __asm__("#silk_SMULWB\n\t" "smull %0, %1, %2, %3\n\t":"=&r"(rd_lo),
		"=&r"
		(rd_hi)
 :		"%r"(a), "r"(b << 16)
	    );
	return rd_hi;
}

#define silk_SMULWB(a, b) (silk_SMULWB_armv4(a, b))

/* a32 + (b32 * (int32_t)((int16_t)(c32))) >> 16 output have to be 32bit int */
#undef silk_SMLAWB
#define silk_SMLAWB(a, b, c) ((a) + silk_SMULWB(b, c))

/* (a32 * (b32 >> 16)) >> 16 */
#undef silk_SMULWT
static inline int32_t silk_SMULWT_armv4(int32_t a, int32_t b)
{
	unsigned rd_lo;
	int rd_hi;
 __asm__("#silk_SMULWT\n\t" "smull %0, %1, %2, %3\n\t":"=&r"(rd_lo),
		"=&r"
		(rd_hi)
 :		"%r"(a), "r"(b & ~0xFFFF)
	    );
	return rd_hi;
}

#define silk_SMULWT(a, b) (silk_SMULWT_armv4(a, b))

/* a32 + (b32 * (c32 >> 16)) >> 16 */
#undef silk_SMLAWT
#define silk_SMLAWT(a, b, c) ((a) + silk_SMULWT(b, c))

/* (a32 * b32) >> 16 */
#undef silk_SMULWW
static inline int32_t silk_SMULWW_armv4(int32_t a, int32_t b)
{
	unsigned rd_lo;
	int rd_hi;
 __asm__("#silk_SMULWW\n\t" "smull %0, %1, %2, %3\n\t":"=&r"(rd_lo),
		"=&r"
		(rd_hi)
 :		"%r"(a), "r"(b)
	    );
	return (rd_hi << 16) + (rd_lo >> 16);
}

#define silk_SMULWW(a, b) (silk_SMULWW_armv4(a, b))

#undef silk_SMLAWW
static inline int32_t silk_SMLAWW_armv4(int32_t a, int32_t b,
					   int32_t c)
{
	unsigned rd_lo;
	int rd_hi;
 __asm__("#silk_SMLAWW\n\t" "smull %0, %1, %2, %3\n\t":"=&r"(rd_lo),
		"=&r"
		(rd_hi)
 :		"%r"(b), "r"(c)
	    );
	return a + (rd_hi << 16) + (rd_lo >> 16);
}

#define silk_SMLAWW(a, b, c) (silk_SMLAWW_armv4(a, b, c))

#endif				/* SILK_MACROS_ARMv4_H */