/**************************************************************************
 *
 *   File Name:  e_homing.h
 *
 *   Purpose:   Contains the prototypes for all the functions of
 *              encoder homing.
 *
 **************************************************************************/

#define EHF_MASK 0x0008 /* Encoder Homing Frame pattern */

/* Function Prototypes */

Word16 w_encoder_homing_frame_w_test (Word16 input_frame[]);

void w_encoder_reset (void);

void w_reset_enc (void);