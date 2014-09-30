/* HR (GSM 06.20) codec wrapper */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gsmhr.h"

#include "typedefs.h"
#include "homing.h"
#include "sp_dec.h"
#include "sp_enc.h"


#define EXPORT __attribute__((visibility("default")))


struct gsmhr {
	int dec_reset_flg;
};

const short gsmhr_mo[12]={0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800}; //or-bitmask
const char gsmhr_ma[18]={5, 11,9,8, 1, 2, 8,9,5, 4,9,5, 4,9,5, 4,9,5}; //bits for each short
static short swTAFCnt=1;

static short LastFrameType=0;


    /*___________________________________________________________________________  

 |                                                                           |  
 |     This subroutine calculates the 'SID flag'                             |  
 |                                                                           |  

 |     Input:     pswParameters[18]                                          |  
 |                           input parameters of the speech decoder          |  

 |                                                                           |  
 |                pswErrorFlag[3]                                            |  
 |                           error flags, generated by channel decoder       |  

 |                                                                           |  
 |     Return Value:                                                         |  

 |                0:         speech frame detected                           |  
 |                1:         most likely SID frame received                  |  

 |                2:         SID frame detected                              |  
 |                                                                           |  
 |___________________________________________________________________________|  

 |                                                                           |  
 |     History:                                                              |  

 |                                                                           |  
 |     12-Oct-1994: Bug removed: error corrected in case of a mode (unvoiced/|  
 |                  voiced) mismatch, if a SID frame was received as an      |  

 |                  unvoiced frame                                           |  
 |___________________________________________________________________________|  

*/   
   
static Shortword swSidDetection(Shortword pswParameters[],   
                                Shortword pswErrorFlag[])   
{   
  static Shortword ppswIBit[2][18] = {   
        5, 11,9,8, 1, 2, 7,7,5, 7,7,5, 7,7,5, 7,7,5,  /* unvoiced */   
        5, 11,9,8, 1, 2, 8,9,5, 4,9,5, 4,9,5, 4,9,5}; /* voiced */   
   
  static Shortword ppswCL1pCL2[2][18] = {   
                              0x0001, /* R0      */  /* unvoiced */   
                              0x00ef, /* LPC1    */   

                              0x003e, /* LPC2    */   
                              0x007f, /* LPC3    */   
                              0x0001, /* INT LPC */   
                              0x0003, /* Mode    */   
                              0x001f, /* Code1_1 */   
                              0x0072, /* Code2_1 */   
                              0x0012, /* GSP0_1  */   
                              0x003f, /* Code1_2 */   
                              0x007f, /* Code2_2 */   
                              0x0008, /* GSP0_2  */   
                              0x007f, /* Code1_3 */   
                              0x007f, /* Code2_3 */   

                              0x0008, /* GSP0_3  */   
                              0x007f, /* Code1_4 */   
                              0x007f, /* Code2_4 */   
                              0x000c, /* GSP0_4  */   
                                 
                              0x0000, /* R0      */  /* voiced */   
                              0x0000, /* LPC1    */   
                              0x0000, /* LPC2    */   
                              0x0000, /* LPC3    */   
                              0x0001, /* INT LPC */   
                              0x0003, /* Mode    */   
                              0x00ff, /* Lag_1   */   
                              0x01ff, /* Code_1  */   
                              0x001f, /* GSP0_1  */   
                              0x000f, /* Lag_2   */   
                              0x01ff, /* Code_2  */   
                              0x001f, /* GSP0_2  */   
                              0x000f, /* Lag_3   */   
                              0x01ff, /* Code_3  */   
                              0x001f, /* GSP0_3  */   
                              0x000f, /* Lag_4   */   
                              0x01ff, /* Code_4  */   
                              0x001f}; /* GSP0_4 */   
   
  static Shortword ppswCL2[2][18] = {   
                              0x0000, /* R0      */ /* unvoiced */   
                              0x0000, /* LPC1    */   
                              0x0000, /* LPC2    */   
                              0x0000, /* LPC3    */   
                              0x0000, /* INT LPC */   
                              0x0000, /* Mode    */    
                              0x0000, /* Code1_1 */   
                              0x0000, /* Code2_1 */   
                              0x0000, /* GSP0_1  */   
                              0x0000, /* Code1_2 */   
                              0x0000, /* Code2_2 */   
                              0x0000, /* GSP0_2  */   
                              0x0000, /* Code1_3 */   
                              0x0007, /* Code2_3 */  /* 3 bits */   
                              0x0000, /* GSP0_3  */   
                              0x007f, /* Code1_4 */  /* 7 bits */   
                              0x007f, /* Code2_4 */  /* 7 bits */   
                              0x0000, /* GSP0_4  */   
   
                              0x0000, /* R0      */  /* voiced */   
                              0x0000, /* LPC1    */   
                              0x0000, /* LPC2    */   
                              0x0000, /* LPC3    */   
                              0x0000, /* INT LPC */   
                              0x0000, /* Mode    */   
                              0x0000, /* Lag_1   */   
                              0x0000, /* Code_1  */   
                              0x0000, /* GSP0_1  */   
                              0x0000, /* Lag_2   */   
                              0x0000, /* Code_2  */   
                              0x0000, /* GSP0_2  */   
                              0x0000, /* Lag_3   */   
                              0x00ff, /* Code_3  */  /* 8 bits */   
                              0x0000, /* GSP0_3  */   
                              0x0000, /* Lag_4   */   
                              0x01ff, /* Code_4  */  /* 9 bits */   
                              0x0000}; /* GSP0_4 */     
   
  Shortword swMode, swBitMask;   
  Shortword swSidN1, swSidN2, swSidN1pN2;   
  Shortword swSid ;   
   
  short siI, siII;   
   
    
   
  /* count transmission errors within the SID codeword      */   
  /* count number of bits equal '0' within the SID codeword */   
  /* ------------------------------------------------------ */   
   
  if (pswParameters[5] == 0)   
    swMode = 0;   
  else    
    swMode = 1;   
     
   
  swSidN1pN2 = 0;         /* N1 + N2 */   
  swSidN2    = 0;   

  swSidN1    = 0;   
     
  for (siI = 0; siI < 18; siI++) {   
      swBitMask = 0x0001;      
      for (siII = 0; siII < ppswIBit[swMode][siI]; siII++) {   
        if ( (pswParameters[siI] & swBitMask) == 0 ) {   
          if ( (ppswCL1pCL2[swMode][siI] & swBitMask) != 0 ) swSidN1pN2++;   
          if ( (ppswCL2[swMode][siI] & swBitMask)     != 0 ) swSidN2++;   
        }      
        swBitMask = swBitMask << 1;      
      }   
  }   
   
  swSidN1 = swSidN1pN2 - swSidN2;   
   
   
  /* frame classification */   


  /* -------------------- */   
   
  if (pswErrorFlag[2]) {   
   
    if (swSidN1 < 3)   
       swSid = 2;   
    else if (swSidN1pN2 < 16)   
       swSid = 1;   
    else   
       swSid = 0;   
        
    if ( (swSidN1pN2 >= 16) && (swSidN1pN2 <= 25) ) {   
      pswErrorFlag[0] = 1;        
    }   
   
  }   
  else {   
   
    if (swSidN1 < 3)   
       swSid = 2;   
    else if (swSidN1pN2 < 11)   
       swSid = 1;   
    else   
       swSid = 0;   
   
  }   
   
   
  /* in case of a mode mismatch */   
  /*----------------------------*/   
     
  if ( (swSid == 2) && (swMode == 0) ) swSid = 1;   
   
  return(swSid);   
   
}   


EXPORT struct gsmhr *
gsmhr_init(short isDTX) 
{
	struct gsmhr *state;

	state = calloc(1, sizeof(struct gsmhr));
	if (!state)
		return NULL;

	state->dec_reset_flg = 1;
	giDTXon=isDTX;	
	resetEnc();
	return state;
}

EXPORT void
gsmhr_exit(struct gsmhr *state)
{
	free(state);
}

EXPORT int
gsmhr_encode(struct gsmhr *state, unsigned char *rb, const short *pcm)
{
	int enc_reset_flg;
	Shortword pcm_b[F_LEN];
        Shortword hr_params[20];
	int i;
        
	char a=0; //param's bytes counter
        char b=0; //param's bits counter
        char c; //output's bits counter

	memcpy(pcm_b, pcm, F_LEN*sizeof(int16_t));

	enc_reset_flg = encoderHomingFrameTest(pcm_b);
//160 short samples -> 112 bits (14 bytes) of speech parameters + SP_flag + VAD_flag
	speechEncoder(pcm_b, hr_params);  

	if (enc_reset_flg)
		resetEnc();


	//pack 18 short to 112 bits (14 bytes)
	memset(rb,0,14);
	for(c=0;c<112;c++)
	{
 	 if(hr_params[a]&gsmhr_mo[b]) rb[c>>3]|=gsmhr_mo[c&0x07];
	 b++;
	 if(b>=gsmhr_ma[a])
	 {
	  b=0;
	  a++;
	 }
	}
	//returned value: 0=iddle (SID), 1=SP-tail, 2-first SID, 3=speech 
	//params[18]-VAD, params19-SP
	i=2*hr_params[18]+hr_params[19];
        if((i==0)&&(LastFrameType!=0)) 
	{
         i=2;
	 LastFrameType=0;
	}
	else LastFrameType=i;
        
	return i;
 
 
}

EXPORT int
gsmhr_decode(struct gsmhr *state, short *pcm, const unsigned char *rb)
{
#define WHOLE_FRAME		18
#define TO_FIRST_SUBFRAME	 9

	int dec_reset_flg;
	char a=0; //param's bytes counter
        char b=0; //param's bits counter
        char c; //output's bits counter
	Shortword hr_params[22];
	Shortword hr_params_b[22];
	Shortword *hr_eflags=hr_params+18;  //BFI, UFI, BCI

	//check for iddle frame	
	if(rb) 
        {
         unsigned int *p=(unsigned int*)rb;
         if(!p[0] && !p[1] && !p[2]) rb=0;
        }

	//unpuck 112 bits (14 bytes) to 18 short
        memset(hr_params, 0, 44);
        if(rb) //process frame
	{
	 for(c=0;c<112;c++)
	 {
	  if(rb[c>>3]&gsmhr_mo[c&0x07]) hr_params[a]|=gsmhr_mo[b]; 
	  b++;
	  if(b>=gsmhr_ma[a])
	  {
	   b=0;
	   a++;
	  }
	 }
        }
		
	//memcpy(hr_params, rb, 40);

	//evaluate error flags (normally are sets by transport layer)
        hr_params[18]=0; //BFI
	hr_params[19]=0; //UFI
	hr_params[20]=0; //BCI
	
	//detects sid frames
	if(giDTXon)
	{
	 if(rb) hr_params[20]=swSidDetection(hr_params, hr_eflags); //0-speech, 2-valid SID 
   	 else hr_params[20]=1; //evaluate 1-invalid SID frame
	}
	else hr_params[20]=0; //while no DTX all frames are speech
	
	//evaluate TAF flag for every 12th speech frame
	if((swTAFCnt==0)&&(hr_params[20]==0)) hr_params[21]=1; else hr_params[21]=0; //TAF
	swTAFCnt = (swTAFCnt + 1) % 12;

	memcpy(hr_params_b, hr_params, 22*sizeof(int16_t));

	if (state->dec_reset_flg)
		dec_reset_flg = decoderHomingFrameTest(hr_params_b, TO_FIRST_SUBFRAME);
	else
		dec_reset_flg = 0;

	if (dec_reset_flg && state->dec_reset_flg) {
		int i;
		for (i=0; i<F_LEN; i++)
			pcm[i] = EHF_MASK;
	} else {
		speechDecoder(hr_params_b, pcm);
	}

	if (!state->dec_reset_flg)
		dec_reset_flg = decoderHomingFrameTest(hr_params_b, WHOLE_FRAME);

	if (dec_reset_flg)
		resetDec();

	state->dec_reset_flg = dec_reset_flg;

	return 0;
}

