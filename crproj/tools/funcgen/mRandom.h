#ifndef _M_RANDOM_
#define _M_RANDOM_

#define NN 312
#define MM 156
#define MATRIX_A 0xB5026F5AA96619E9ULL
#define UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
#define LM 0x7FFFFFFFULL /* Least significant 31 bits */

class mRandom
{
	DWORD64			mt[NN];
	DWORD64			mag01[2];
	INT				mti;
	DWORD64			seed;

public:
	mRandom();
	void init_genrand64(unsigned long long seed);
	void init_by_array64(unsigned long long init_key[], unsigned long long key_length);
	DWORD64 get();
	DWORD64 set();
	DWORD64 get_less(DWORD64 From,DWORD64 To);
	DWORD64 get_equal(DWORD64 From,DWORD64 To);
	DWORD64 get_seed();
	void set_seed(DWORD64 IN genSeed);
};

#endif