#pragma once
#include <iostream>
#include <memory>
namespace MoePus {
	using BlockType = size_t;

	constexpr BlockType inline GenKey() {
		const unsigned long long FNV_64_PRIME = 0x100000001b3ULL;
		unsigned long long hash = 14695981039346656037u;
		for (int i = 0; i < sizeof(__TIME__); i++)
		{
			hash ^= __TIME__[i];
			hash *= FNV_64_PRIME;
		}
		return static_cast<BlockType>(hash);
	}

	template<typename T, int N, BlockType KEY = MoePus::GenKey()>
	class ObData {
	public:
		constexpr static int block_count = N / sizeof(BlockType) + (N % sizeof(BlockType) ? 1 : 0);
		constexpr static BlockType key = KEY;
		constexpr static BlockType counts = N;
		constexpr ObData() {
		}
		template <class U> constexpr ObData(const U* _data) {
			for (int i = 0; i < N / sizeof(U); i++)
			{
				data[i * sizeof(U) / sizeof(BlockType)] |=
					(BlockType(_data[i]) << ((i % (sizeof(BlockType) / sizeof(T))) * sizeof(U) * 8));
			}
			enc();
		}
		constexpr void enc()
		{
			for (int i = 0; i < block_count; i++)
			{
				data[i] ^= key;
			}
		}
		const T* dec()
		{
			BlockType k = key + (((BlockType)data * ((BlockType)data + 1)) & 1);
			int i;
			for (i = 0; i < block_count - (N % sizeof(BlockType) ? 1 : 0); i++)
			{
				data[i] ^= k;
			}
			if (N % sizeof(BlockType))
			{
				BlockType m = data[i] ^ k;
				memcpy(&data[i], &m, N % sizeof(BlockType));
			}
			return (T*)&data[0];
		}
		operator const T* () const
		{
			return (T*)get_data();
		};
		inline constexpr const BlockType* get_data() const { return data; }
	private:
		BlockType data[block_count]{};
	};

	template<typename T, size_t N>
	constexpr bool IsStringLiteral(T const(&)[N]) { return true; }

	template<typename T, size_t N>
	constexpr bool IsStringLiteral(T(&)[N]) { return false; }

	template<typename T>
	constexpr bool IsStringLiteral(T) { return false; }
}

#define MP_OB(x) \
[](){ \
static_assert(MoePus::IsStringLiteral(x), "MP_OB supports string literal only.");\
using data_type = std::remove_const_t<std::remove_pointer_t<std::decay_t<decltype(x)>>>; \
constexpr static auto sec = MoePus::ObData<const data_type, sizeof(x) - sizeof(data_type)>(x); \
constexpr int length = sizeof(x) / sizeof(data_type) - 1; \
std::basic_string<data_type, std::char_traits<data_type>, std::allocator<data_type>> p((data_type*)&sec, length); \
((std::remove_const_t<decltype(sec)>*)p.data())->dec(); \
return p; \
}()

#define MP_OBD(x) \
[](){ \
using data_type = std::remove_const_t<std::decay_t<decltype(x[0])>>; \
constexpr static auto sec = MoePus::ObData<const data_type, sizeof(x)>(x); \
auto d = std::make_unique<std::remove_const_t<decltype(sec)>>(sec); \
d->dec(); \
return d; \
}();

//--

#define CONV_STR2DEC_1(str, i)  (str[i]>'0'?str[i]-'0':0)
#define CONV_STR2DEC_2(str, i)  (CONV_STR2DEC_1(str, i)*10 + str[i+1]-'0')
#define CONV_STR2DEC_3(str, i)  (CONV_STR2DEC_2(str, i)*10 + str[i+2]-'0')
#define CONV_STR2DEC_4(str, i)  (CONV_STR2DEC_3(str, i)*10 + str[i+3]-'0')

// Some definitions for calculation
#define SEC_PER_MIN             60UL
#define SEC_PER_HOUR            3600UL
#define SEC_PER_DAY             86400UL
#define SEC_PER_YEAR            (SEC_PER_DAY*365)
#define UNIX_START_YEAR         1970UL

// Custom "glue logic" to convert the month name to a usable number
#define GET_MONTH(str, i)      (str[i]=='J' && str[i+1]=='a' && str[i+2]=='n' ? 1 :     \
                                str[i]=='F' && str[i+1]=='e' && str[i+2]=='b' ? 2 :     \
                                str[i]=='M' && str[i+1]=='a' && str[i+2]=='r' ? 3 :     \
                                str[i]=='A' && str[i+1]=='p' && str[i+2]=='r' ? 4 :     \
                                str[i]=='M' && str[i+1]=='a' && str[i+2]=='y' ? 5 :     \
                                str[i]=='J' && str[i+1]=='u' && str[i+2]=='n' ? 6 :     \
                                str[i]=='J' && str[i+1]=='u' && str[i+2]=='l' ? 7 :     \
                                str[i]=='A' && str[i+1]=='u' && str[i+2]=='g' ? 8 :     \
                                str[i]=='S' && str[i+1]=='e' && str[i+2]=='p' ? 9 :     \
                                str[i]=='O' && str[i+1]=='c' && str[i+2]=='t' ? 10 :    \
                                str[i]=='N' && str[i+1]=='o' && str[i+2]=='v' ? 11 :    \
                                str[i]=='D' && str[i+1]=='e' && str[i+2]=='c' ? 12 : 0)

#define GET_MONTH2DAYS(month)  ((month == 1 ? 0 : 31 +                      \
                                (month == 2 ? 0 : 28 +                      \
                                (month == 3 ? 0 : 31 +                      \
                                (month == 4 ? 0 : 30 +                      \
                                (month == 5 ? 0 : 31 +                      \
                                (month == 6 ? 0 : 30 +                      \
                                (month == 7 ? 0 : 31 +                      \
                                (month == 8 ? 0 : 31 +                      \
                                (month == 9 ? 0 : 30 +                      \
                                (month == 10 ? 0 : 31 +                     \
                                (month == 11 ? 0 : 30))))))))))))           \


#define GET_LEAP_DAYS           ((__TIME_YEARS__-1968)/4 - (__TIME_MONTH__ <=2 ? 1 : 0))



#define __TIME_SECONDS__        CONV_STR2DEC_2(__TIME__, 6)
#define __TIME_MINUTES__        CONV_STR2DEC_2(__TIME__, 3)
#define __TIME_HOURS__          CONV_STR2DEC_2(__TIME__, 0)
#define __TIME_DAYS__           CONV_STR2DEC_2(__DATE__, 4)
#define __TIME_MONTH__          GET_MONTH(__DATE__, 0)
#define __TIME_YEARS__          CONV_STR2DEC_4(__DATE__, 7)

#define __TIME_UNIX__         ((__TIME_YEARS__-UNIX_START_YEAR)*SEC_PER_YEAR+       \
                                GET_LEAP_DAYS*SEC_PER_DAY+                          \
                                GET_MONTH2DAYS(__TIME_MONTH__)*SEC_PER_DAY+         \
                                __TIME_DAYS__*SEC_PER_DAY-SEC_PER_DAY+              \
                                __TIME_HOURS__*SEC_PER_HOUR+                        \
                                __TIME_MINUTES__*SEC_PER_MIN+                       \
                                __TIME_SECONDS__)
								
/* --------------------------- ABOUT ---------------------------------
Original Author: Adam Yaxley
Website: https://github.com/adamyaxley
License: See end of file
Obfuscate
Guaranteed compile-time string literal obfuscation library for C++14
Usage:
Pass string literals into the AY_OBFUSCATE macro to obfuscate them at
compile time. AY_OBFUSCATE returns a temporary ay::obfuscated_data
object that is implicitly convertable to a char*.
Example:
auto obfuscated_string = AY_OBFUSCATE("Hello World");
std::cout << obfuscated_string << std::endl;
------------------------------------------------------------------- */

#include <cstddef>

namespace ay
{
	// Obfuscates a string at compile time
	template <std::size_t N, char KEY>
	class obfuscator
	{
	public:
		// Obfuscates the string 'data' on construction
		constexpr obfuscator(const char* data)
		{
			static_assert(KEY != '\0', "KEY must not be the null character.");

			// On construction each of the characters in the string is obfuscated with an XOR cipher based on KEY
			for (std::size_t i = 0; i < N; i++)
			{
				m_data[i] = data[i] ^ KEY; 
			}
		}

		constexpr const char* getData() const
		{
			return &m_data[0]; 
		}

		constexpr std::size_t getSize() const
		{
			return N; 
		}

		constexpr char getKey() const
		{
			return KEY; 
		}

	private:

		char m_data[N]{}; 
	};

	// Handles decryption and re-encryption of an encrypted string at runtime
	template <std::size_t N, char KEY>
	class obfuscated_data
	{
	public:
		obfuscated_data(const obfuscator<N, KEY>& obfuscator)
		{
			for (int i = 0; i < N; i++)
			{
				m_data[i] = obfuscator.getData()[i]; 
			}
		}

		~obfuscated_data()
		{
			// Zero m_data to remove it from memory
			for (int i = 0; i < N; i++)
			{
				m_data[i] = 0; 
			}
		}

		// Returns a pointer to the plain text string, decrypting it if necessary
		operator char*()
		{
			decrypt(); 
			return m_data; 
		}

		// Manually decrypt the string
		void decrypt()
		{
			if (is_encrypted())
			{
				
				for (std::size_t i = 0; i < N; i++)
				{
					m_data[i] ^= KEY; 
				}
			}
		}

		// Manually re-encrypt the string
		void encrypt()
		{
			//
			if (!is_encrypted())
			{
				//
				for (std::size_t i = 0; i < N; i++)
				{
					m_data[i] ^= KEY; 
				}
			}
		}

		// Returns true if this string is currently encrypted, false otherwise.
		bool is_encrypted() const
		{
			return m_data[N - 1] != '\0'; 
		}

	private:

		// Local storage for the string. Call is_encrypted() to check whether or not the string is currently obfuscated.
		char m_data[N];
	};

	// This function exists purely to extract the number of elements 'N' in the array 'data'
	template <std::size_t N, char KEY = '.'>
	constexpr auto makeObfuscator(const char(&data)[N])
	{
		return obfuscator<N, KEY>(data); //
	}
}

// Obfuscates the string 'data' at compile-time and returns an ay::obfuscated_data object that has
// functions for decrypting the string and is also implicitly convertable to a char*

#define XorStr(data) AY_OBFUSCATE_KEY(data, __TIME_UNIX__)

// Obfuscates the string 'data' with 'key' at compile-time and returns an ay::obfuscated_data object that has
// functions for decrypting the string and is also implicitly convertable to a char*
#define AY_OBFUSCATE_KEY(data, key) \
	[](){ \
		constexpr auto n = sizeof(data)/sizeof(data[0]); \
		static_assert(data[n - 1] == '\0', "String must be null terminated"); \
		constexpr auto obfuscator = ay::makeObfuscator<n, key>(data); \
		return ay::obfuscated_data<n, key>(obfuscator); \
	}()
	
