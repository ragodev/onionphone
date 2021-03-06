/* vim: set tabstop=4:softtabstop=4:shiftwidth=4:noexpandtab */

/*___________________________________________________________________________
 |                                                                           |
 | Basic arithmetic operators.                                               |
 |___________________________________________________________________________|
*/

/*___________________________________________________________________________
 |                                                                           |
 |   Include-Files                                                           |
 |___________________________________________________________________________|
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "basic_op.h"

#if (WMOPS)

extern BASIC_OP w_counter;

#endif

/*___________________________________________________________________________
 |                                                                           |
 |   Local Functions                                                         |
 |___________________________________________________________________________|
*/
int16_t w_saturate(int32_t L_var1);

/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 |___________________________________________________________________________|
*/
int w_Overflow = 0;
int w_Carry = 0;

/*___________________________________________________________________________
 |                                                                           |
 |   Functions                                                               |
 |___________________________________________________________________________|
*/

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_saturate                                                |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Limit the 32 bit input to the range of a 16 bit word.                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_saturate(int32_t L_var1)
{
	int16_t var_out;

	if (L_var1 > 0X00007fffL) {
		w_Overflow = 1;
		var_out = MAX_16;
	} else if (L_var1 < (int32_t) 0xffff8000L) {
		w_Overflow = 1;
		var_out = MIN_16;
	} else {
		w_Overflow = 0;
		var_out = w_extract_l(L_var1);
#if (WMOPS)
		w_counter.w_extract_l--;
#endif
	}

	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_add                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the w_addition (var1+var2) with overflow control and saturation;|
 |    the 16 bit result is set at +32767 when overflow occurs or at -32768   |
 |    when underflow occurs.                                                 |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_add(int16_t var1, int16_t var2)
{
	int16_t var_out;
	int32_t L_sum;

	L_sum = (int32_t) var1 + var2;
	var_out = w_saturate(L_sum);
#if (WMOPS)
	w_counter.w_add++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_sub                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the w_subtraction (var1+var2) with overflow control and satu-   |
 |    ration; the 16 bit result is set at +32767 when overflow occurs or at  |
 |    -32768 when underflow occurs.                                          |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_sub(int16_t var1, int16_t var2)
{
	int16_t var_out;
	int32_t L_diff;

	L_diff = (int32_t) var1 - var2;
	var_out = w_saturate(L_diff);
#if (WMOPS)
	w_counter.w_sub++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_abs_s                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Absolute value of var1; w_abs_s(-32768) = 32767.                         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_abs_s(int16_t var1)
{
	int16_t var_out;

	if (var1 == (int16_t) 0X8000) {
		var_out = MAX_16;
	} else {
		if (var1 < 0) {
			var_out = -var1;
		} else {
			var_out = var1;
		}
	}
#if (WMOPS)
	w_counter.w_abs_s++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_shl                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 left var2 positions.Zero fill|
 |   the var2 LSB of the result. If var2 is negative, arithmetically shift   |
 |   var1 right by -var2 with sign extension. Saturate the result in case of |
 |   underflows or overflows.                                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_shl(int16_t var1, int16_t var2)
{
	int16_t var_out;
	int32_t result;

	if (var2 < 0) {
		var_out = w_shr(var1, -var2);
#if (WMOPS)
		w_counter.w_shr--;
#endif
	} else {
		result = (int32_t) var1 *((int32_t) 1 << var2);

		if ((var2 > 15 && var1 != 0)
		    || (result != (int32_t) ((int16_t) result))) {
			w_Overflow = 1;
			var_out = (var1 > 0) ? MAX_16 : MIN_16;
		} else {
			var_out = w_extract_l(result);
#if (WMOPS)
			w_counter.w_extract_l--;
#endif
		}
	}
#if (WMOPS)
	w_counter.w_shl++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_shr                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 right var2 positions with    |
 |   sign extension. If var2 is negative, arithmetically shift var1 left by  |
 |   -var2 with sign extension. Saturate the result in case of underflows or |
 |   overflows.                                                              |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_shr(int16_t var1, int16_t var2)
{
	int16_t var_out;

	if (var2 < 0) {
		var_out = w_shl(var1, -var2);
#if (WMOPS)
		w_counter.w_shl--;
#endif
	} else {
		if (var2 >= 15) {
			var_out = (var1 < 0) ? -1 : 0;
		} else {
			if (var1 < 0) {
				var_out = ~((~var1) >> var2);
			} else {
				var_out = var1 >> var2;
			}
		}
	}

#if (WMOPS)
	w_counter.w_shr++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_mult                                                    |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the w_multiplication of var1 by var2 and gives a 16 bit result  |
 |    which is scaled i.e.:                                                  |
 |             w_mult(var1,var2) = w_extract_l(w_L_w_shr((var1 times var2),15)) and  |
 |             w_mult(-32768,-32768) = 32767.                                  |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_mult(int16_t var1, int16_t var2)
{
	int16_t var_out;
	int32_t L_product;

	L_product = (int32_t) var1 *(int32_t) var2;

	L_product = (L_product & (int32_t) 0xffff8000L) >> 15;

	if (L_product & (int32_t) 0x00010000L)
		L_product = L_product | (int32_t) 0xffff0000L;

	var_out = w_saturate(L_product);
#if (WMOPS)
	w_counter.w_mult++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_w_mult                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   w_L_w_mult is the 32 bit result of the w_multiplication of var1 times var2    |
 |   with one shift left i.e.:                                               |
 |        w_L_w_mult(var1,var2) = w_L_w_shl((var1 times var2),1) and                   |
 |        w_L_w_mult(-32768,-32768) = 2147483647.                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/

int32_t w_L_w_mult(int16_t var1, int16_t var2)
{
	int32_t L_var_out;

	L_var_out = (int32_t) var1 *(int32_t) var2;

	if (L_var_out != (int32_t) 0x40000000L) {
		L_var_out *= 2;
	} else {
		w_Overflow = 1;
		L_var_out = MAX_32;
	}

#if (WMOPS)
	w_counter.w_L_w_mult++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_negate                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Negate var1 with saturation, w_saturate in the case where input is -32768:|
 |                w_negate(var1) = w_sub(0,var1).                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_negate(int16_t var1)
{
	int16_t var_out;

	var_out = (var1 == MIN_16) ? MAX_16 : -var1;
#if (WMOPS)
	w_counter.w_negate++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_extract_h                                               |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Return the 16 MSB of L_var1.                                            |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (int32_t ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_extract_h(int32_t L_var1)
{
	int16_t var_out;

	var_out = (int16_t) (L_var1 >> 16);
#if (WMOPS)
	w_counter.w_extract_h++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_extract_l                                               |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Return the 16 LSB of L_var1.                                            |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (int32_t ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_extract_l(int32_t L_var1)
{
	int16_t var_out;

	var_out = (int16_t) L_var1;
#if (WMOPS)
	w_counter.w_extract_l++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_round                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Round the lower 16 bits of the 32 bit input number into the MS 16 bits  |
 |   with saturation. Shift the resulting bits right by 16 and return the 16 |
 |   bit number:                                                             |
 |               w_round(L_var1) = w_extract_h(L_w_add(L_var1,32768))              |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (int32_t ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_round(int32_t L_var1)
{
	int16_t var_out;
	int32_t L_w_rounded;

	L_w_rounded = L_w_add(L_var1, (int32_t) 0x00008000L);
#if (WMOPS)
	w_counter.L_w_add--;
#endif
	var_out = w_extract_h(L_w_rounded);
#if (WMOPS)
	w_counter.w_extract_h--;
	w_counter.w_round++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_mac                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Add the 32 bit    |
 |   result to L_var3 with saturation, return a 32 bit result:               |
 |        w_L_mac(L_var3,var1,var2) = L_w_add(L_var3,w_L_w_mult(var1,var2)).         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/

int32_t w_L_mac(int32_t L_var3, int16_t var1, int16_t var2)
{
	int32_t L_var_out;
	int32_t L_product;

	L_product = w_L_w_mult(var1, var2);
#if (WMOPS)
	w_counter.w_L_w_mult--;
#endif
	L_var_out = L_w_add(L_var3, L_product);
#if (WMOPS)
	w_counter.L_w_add--;
	w_counter.w_L_mac++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_msu                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Subtract the 32   |
 |   bit result to L_var3 with saturation, return a 32 bit result:           |
 |        w_L_msu(L_var3,var1,var2) = w_L_w_sub(L_var3,w_L_w_mult(var1,var2)).         |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/

int32_t w_L_msu(int32_t L_var3, int16_t var1, int16_t var2)
{
	int32_t L_var_out;
	int32_t L_product;

	L_product = w_L_w_mult(var1, var2);
#if (WMOPS)
	w_counter.w_L_w_mult--;
#endif
	L_var_out = w_L_w_sub(L_var3, L_product);
#if (WMOPS)
	w_counter.w_L_w_sub--;
	w_counter.w_L_msu++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_w_L_macNs                                                 |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Multiply var1 by var2 and shift the result left by 1. Add the 32 bit    |
 |   result to L_var3 without saturation, return a 32 bit result. Generate   |
 |   carry and overflow values :                                             |
 |        w_w_L_macNs(L_var3,var1,var2) = L_w_add_c(L_var3,w_L_w_mult(var1,var2)).     |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var3   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |                                                                           |
 |   Caution :                                                               |
 |                                                                           |
 |    In some cases the w_Carry flag has to be cleared or set before using     |
 |    operators which take into account its value.                           |
 |___________________________________________________________________________|
*/

int32_t w_w_L_macNs(int32_t L_var3, int16_t var1, int16_t var2)
{
	int32_t L_var_out;

	L_var_out = w_L_w_mult(var1, var2);
#if (WMOPS)
	w_counter.w_L_w_mult--;
#endif
	L_var_out = L_w_add_c(L_var3, L_var_out);
#if (WMOPS)
	w_counter.L_w_add_c--;
	w_counter.w_w_L_macNs++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_w_add                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   32 bits w_addition of the two 32 bits variables (L_var1+L_var2) with      |
 |   overflow control and saturation; the result is set at +2147483647 when  |
 |   overflow occurs or at -2147483648 when underflow occurs.                |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/

int32_t L_w_add(int32_t L_var1, int32_t L_var2)
{
	int32_t L_var_out;

	L_var_out = L_var1 + L_var2;

	if (((L_var1 ^ L_var2) & MIN_32) == 0) {
		if ((L_var_out ^ L_var1) & MIN_32) {
			L_var_out = (L_var1 < 0) ? MIN_32 : MAX_32;
			w_Overflow = 1;
		}
	}
#if (WMOPS)
	w_counter.L_w_add++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_w_sub                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   32 bits w_subtraction of the two 32 bits variables (L_var1-L_var2) with   |
 |   overflow control and saturation; the result is set at +2147483647 when  |
 |   overflow occurs or at -2147483648 when underflow occurs.                |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/

int32_t w_L_w_sub(int32_t L_var1, int32_t L_var2)
{
	int32_t L_var_out;

	L_var_out = L_var1 - L_var2;

	if (((L_var1 ^ L_var2) & MIN_32) != 0) {
		if ((L_var_out ^ L_var1) & MIN_32) {
			L_var_out = (L_var1 < 0L) ? MIN_32 : MAX_32;
			w_Overflow = 1;
		}
	}
#if (WMOPS)
	w_counter.w_L_w_sub++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_w_add_c                                                 |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Performs 32 bits w_addition of the two 32 bits variables (L_var1+L_var2+C)|
 |   with carry. No saturation. Generate carry and w_Overflow values. The car- |
 |   ry and overflow values are binary variables which can be w_tested and as- |
 |   signed values.                                                          |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |                                                                           |
 |   Caution :                                                               |
 |                                                                           |
 |    In some cases the w_Carry flag has to be cleared or set before using     |
 |    operators which take into account its value.                           |
 |___________________________________________________________________________|
*/
int32_t L_w_add_c(int32_t L_var1, int32_t L_var2)
{
	int32_t L_var_out;
	int32_t L_w_test;
	int carry_int = 0;

	L_var_out = L_var1 + L_var2 + w_Carry;

	L_w_test = L_var1 + L_var2;

	if ((L_var1 > 0) && (L_var2 > 0) && (L_w_test < 0)) {
		w_Overflow = 1;
		carry_int = 0;
	} else {
		if ((L_var1 < 0) && (L_var2 < 0)) {
			if (L_w_test >= 0) {
				w_Overflow = 1;
				carry_int = 1;
			} else {
				w_Overflow = 0;
				carry_int = 1;
			}
		} else {
			if (((L_var1 ^ L_var2) < 0) && (L_w_test >= 0)) {
				w_Overflow = 0;
				carry_int = 1;
			} else {
				w_Overflow = 0;
				carry_int = 0;
			}
		}
	}

	if (w_Carry) {
		if (L_w_test == MAX_32) {
			w_Overflow = 1;
			w_Carry = carry_int;
		} else {
			if (L_w_test == (int32_t) 0xFFFFFFFFL) {
				w_Carry = 1;
			} else {
				w_Carry = carry_int;
			}
		}
	} else {
		w_Carry = carry_int;
	}

#if (WMOPS)
	w_counter.L_w_add_c++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_w_negate                                                |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Negate the 32 bit variable L_var1 with saturation; w_saturate in the case |
 |   where input is -2147483648 (0x8000 0000).                               |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/

int32_t w_L_w_negate(int32_t L_var1)
{
	int32_t L_var_out;

	L_var_out = (L_var1 == MIN_32) ? MAX_32 : -L_var1;
#if (WMOPS)
	w_counter.w_L_w_negate++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_w_mult_r                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as w_mult with w_rounding, i.e.:                                       |
 |     w_w_mult_r(var1,var2) = w_extract_l(w_L_w_shr(((var1 * var2) + 16384),15)) and  |
 |     w_w_mult_r(-32768,-32768) = 32767.                                        |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int16_t w_w_mult_r(int16_t var1, int16_t var2)
{
	int16_t var_out;
	int32_t L_product_arr;

	L_product_arr = (int32_t) var1 *(int32_t) var2;	/* product */
	L_product_arr += (int32_t) 0x00004000L;	/* w_round */
	L_product_arr &= (int32_t) 0xffff8000L;
	L_product_arr >>= 15;	/* shift */

	if (L_product_arr & (int32_t) 0x00010000L) {	/* sign extend when necessary */
		L_product_arr |= (int32_t) 0xffff0000L;
	}
	var_out = w_saturate(L_product_arr);
#if (WMOPS)
	w_counter.w_w_mult_r++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_w_shl                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 32 bit input L_var1 left var2 positions. Zero  |
 |   fill the var2 LSB of the result. If var2 is negative, arithmetically    |
 |   shift L_var1 right by -var2 with sign extension. Saturate the result in |
 |   case of underflows or overflows.                                        |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/

int32_t w_L_w_shl(int32_t L_var1, int16_t var2)
{
	int32_t L_var_out;

	if (var2 <= 0) {
		L_var_out = w_L_w_shr(L_var1, -var2);
#if (WMOPS)
		w_counter.w_L_w_shr--;
#endif
	} else {
		for (; var2 > 0; var2--) {
			if (L_var1 > (int32_t) 0X3fffffffL) {
				w_Overflow = 1;
				L_var_out = MAX_32;
				break;
			} else {
				if (L_var1 < (int32_t) 0xc0000000L) {
					w_Overflow = 1;
					L_var_out = MIN_32;
					break;
				}
			}
			L_var1 *= 2;
			L_var_out = L_var1;
		}
	}
#if (WMOPS)
	w_counter.w_L_w_shl++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_w_shr                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 32 bit input L_var1 right var2 positions with  |
 |   sign extension. If var2 is negative, arithmetically shift L_var1 left   |
 |   by -var2 and w_zero fill the -var2 LSB of the result. Saturate the result |
 |   in case of underflows or overflows.                                     |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
*/

int32_t w_L_w_shr(int32_t L_var1, int16_t var2)
{
	int32_t L_var_out;

	if (var2 < 0) {
		L_var_out = w_L_w_shl(L_var1, -var2);
#if (WMOPS)
		w_counter.w_L_w_shl--;
#endif
	} else {
		if (var2 >= 31) {
			L_var_out = (L_var1 < 0L) ? -1 : 0;
		} else {
			if (L_var1 < 0) {
				L_var_out = ~((~L_var1) >> var2);
			} else {
				L_var_out = L_var1 >> var2;
			}
		}
	}
#if (WMOPS)
	w_counter.w_L_w_shr++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_deposit_h                                             |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Deposit the 16 bit var1 into the 16 MS bits of the 32 bit output. The   |
 |   16 LS bits of the output are w_zeroed.                                    |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff 0000.                |
 |___________________________________________________________________________|
*/

int32_t w_L_deposit_h(int16_t var1)
{
	int32_t L_var_out;

	L_var_out = (int32_t) var1 << 16;
#if (WMOPS)
	w_counter.w_L_deposit_h++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_deposit_l                                             |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Deposit the 16 bit var1 into the 16 LS bits of the 32 bit output. The   |
 |   16 MS bits of the output are sign extended.                             |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0xFFFF 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/

int32_t w_L_deposit_l(int16_t var1)
{
	int32_t L_var_out;

	L_var_out = (int32_t) var1;
#if (WMOPS)
	w_counter.w_L_deposit_l++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_w_L_w_w_shr_r                                                 |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Same as w_L_w_shr(L_var1,var2) but with w_rounding. Saturate the result in    |
 |   case of underflows or overflows :                                       |
 |    - If var2 is greater than w_zero :                                       |
 |          if (w_L_w_sub(w_L_w_shl(w_L_w_shr(L_var1,var2),1),w_L_w_shr(L_var1,w_sub(var2,1))))|
 |          is equal to w_zero                                                 |
 |                     then                                                  |
 |                     w_w_L_w_w_shr_r(L_var1,var2) = w_L_w_shr(L_var1,var2)             |
 |                     else                                                  |
 |                     w_w_L_w_w_shr_r(L_var1,var2) = L_w_add(w_L_w_shr(L_var1,var2),1)    |
 |    - If var2 is less than or equal to w_zero :                              |
 |                     w_w_L_w_w_shr_r(L_var1,var2) = w_L_w_shr(L_var1,var2).            |
 |                                                                           |
 |   Complexity weight : 3                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= var_out <= 0x7fff ffff.                |
 |___________________________________________________________________________|
*/

int32_t w_w_L_w_w_shr_r(int32_t L_var1, int16_t var2)
{
	int32_t L_var_out;

	if (var2 > 31) {
		L_var_out = 0;
	} else {
		L_var_out = w_L_w_shr(L_var1, var2);
#if (WMOPS)
		w_counter.w_L_w_shr--;
#endif
		if (var2 > 0) {
			if ((L_var1 & ((int32_t) 1 << (var2 - 1))) != 0) {
				L_var_out++;
			}
		}
	}
#if (WMOPS)
	w_counter.w_w_L_w_w_shr_r++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_L_abs                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Absolute value of L_var1; Saturate in case where the input is          |
 |                                                               -214783648  |
 |                                                                           |
 |   Complexity weight : 3                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x0000 0000 <= var_out <= 0x7fff ffff.                |
 |___________________________________________________________________________|
*/

int32_t w_L_abs(int32_t L_var1)
{
	int32_t L_var_out;

	if (L_var1 == MIN_32) {
		L_var_out = MAX_32;
	} else {
		if (L_var1 < 0) {
			L_var_out = -L_var1;
		} else {
			L_var_out = L_var1;
		}
	}

#if (WMOPS)
	w_counter.w_L_abs++;
#endif
	return (L_var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_norm_s                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces the number of left shift needed to normalize the 16 bit varia- |
 |   ble var1 for positive values on the interval with minimum of 16384 and  |
 |   maximum of 32767, and for negative values on the interval with minimum  |
 |   of -32768 and maximum of -16384; in order to normalize the result, the  |
 |   following operation must be done :                                      |
 |                    norm_var1 = w_shl(var1,w_norm_s(var1)).                    |
 |                                                                           |
 |   Complexity weight : 15                                                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 000f.                |
 |___________________________________________________________________________|
*/

int16_t w_norm_s(int16_t var1)
{
	int16_t var_out;

	if (var1 == 0) {
		var_out = 0;
	} else {
		if (var1 == (int16_t) 0xffff) {
			var_out = 15;
		} else {
			if (var1 < 0) {
				var1 = ~var1;
			}
			for (var_out = 0; var1 < 0x4000; var_out++) {
				var1 <<= 1;
			}
		}
	}

#if (WMOPS)
	w_counter.w_norm_s++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_div_s                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces a result which is the fractional integer division of var1  by  |
 |   var2; var1 and var2 must be positive and var2 must be greater or equal  |
 |   to var1; the result is positive (leading bit equal to 0) and truncated  |
 |   to 16 bits.                                                             |
 |   If var1 = var2 then div(var1,var2) = 32767.                             |
 |                                                                           |
 |   Complexity weight : 18                                                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0x0000 0000 <= var1 <= var2 and var2 != 0.            |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : var1 <= var2 <= 0x0000 7fff and var2 != 0.            |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 7fff.                |
 |             It's a Q15 value (point between b15 and b14).                 |
 |___________________________________________________________________________|
*/

int16_t w_div_s(int16_t var1, int16_t var2)
{
	int16_t var_out = 0;
	int16_t iteration;
	int32_t L_num;
	int32_t L_denom;

	if ((var1 > var2) || (var1 < 0) || (var2 < 0)) {
		printf("Division Error var1=%d  var2=%d\n", var1, var2);
		exit(0);
	}
	if (var2 == 0) {
		printf("Division by 0, Fatal w_error \n");
		exit(0);
	}
	if (var1 == 0) {
		var_out = 0;
	} else {
		if (var1 == var2) {
			var_out = MAX_16;
		} else {
			L_num = w_L_deposit_l(var1);
#if (WMOPS)
			w_counter.w_L_deposit_l--;
#endif
			L_denom = w_L_deposit_l(var2);
#if (WMOPS)
			w_counter.w_L_deposit_l--;
#endif

			for (iteration = 0; iteration < 15; iteration++) {
				var_out <<= 1;
				L_num <<= 1;

				if (L_num >= L_denom) {
					L_num = w_L_w_sub(L_num, L_denom);
#if (WMOPS)
					w_counter.w_L_w_sub--;
#endif
					var_out = w_add(var_out, 1);
#if (WMOPS)
					w_counter.w_add--;
#endif
				}
			}
		}
	}

#if (WMOPS)
	w_counter.w_div_s++;
#endif
	return (var_out);
}

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : w_norm_l                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Produces the number of left shifts needed to normalize the 32 bit varia-|
 |   ble L_var1 for positive values on the interval with minimum of          |
 |   1073741824 and maximum of 2147483647, and for negative values on the in-|
 |   terval with minimum of -2147483648 and maximum of -1073741824; in order |
 |   to normalize the result, the following operation must be done :         |
 |                   norm_L_var1 = w_L_w_shl(L_var1,w_norm_l(L_var1)).             |
 |                                                                           |
 |   Complexity weight : 30                                                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (int32_t) whose value falls in the  |
 |             range : 0x8000 0000 <= var1 <= 0x7fff ffff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (int16_t) whose value falls in the |
 |             range : 0x0000 0000 <= var_out <= 0x0000 001f.                |
 |___________________________________________________________________________|
*/

int16_t w_norm_l(int32_t L_var1)
{
	int16_t var_out;

	if (L_var1 == 0) {
		var_out = 0;
	} else {
		if (L_var1 == (int32_t) 0xffffffffL) {
			var_out = 31;
		} else {
			if (L_var1 < 0) {
				L_var1 = ~L_var1;
			}
			for (var_out = 0; L_var1 < (int32_t) 0x40000000L;
			     var_out++) {
				L_var1 <<= 1;
			}
		}
	}

#if (WMOPS)
	w_counter.w_norm_l++;
#endif
	return (var_out);
}
