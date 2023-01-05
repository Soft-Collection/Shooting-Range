#ifndef __CFUNCTIONS_H__
#define __CFUNCTIONS_H__

class CFunctions
{
public:
	static uint32_t LFSR_Galois_Next_32(uint32_t previous)
	{
		uint32_t lfsr = previous;
		uint32_t lsb = lfsr & 1;     //Get LSB (i.e., the output bit).
		lfsr >>= 1;                  //Shift register.
		//0xA3000000 = 0b10100011000000000000000000000000
		lfsr ^= (-lsb) & 0xA3000000; //If the output bit is 1, apply toggle mask. The value has 1 at bits corresponding to taps, 0 elsewhere.
		return (lfsr);
	}
};

#endif // __CFUNCTIONS_H__