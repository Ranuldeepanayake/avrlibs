# 1 ".././dht11.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 ".././dht11.c"







# 1 ".././dht11.h" 1
# 12 ".././dht11.h"
# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 1 3
# 99 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 3
# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\sfr_defs.h" 1 3
# 126 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\sfr_defs.h" 3
# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\inttypes.h" 1 3
# 37 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\inttypes.h" 3
# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\lib\\gcc\\avr\\4.9.2\\include\\stdint.h" 1 3 4
# 9 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\lib\\gcc\\avr\\4.9.2\\include\\stdint.h" 3 4
# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\stdint.h" 1 3 4
# 125 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\stdint.h" 3 4
typedef signed int int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
typedef signed int int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int uint16_t __attribute__ ((__mode__ (__HI__)));
typedef signed int int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int uint32_t __attribute__ ((__mode__ (__SI__)));

typedef signed int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int uint64_t __attribute__((__mode__(__DI__)));
# 146 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\stdint.h" 3 4
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 163 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\stdint.h" 3 4
typedef int8_t int_least8_t;




typedef uint8_t uint_least8_t;




typedef int16_t int_least16_t;




typedef uint16_t uint_least16_t;




typedef int32_t int_least32_t;




typedef uint32_t uint_least32_t;







typedef int64_t int_least64_t;






typedef uint64_t uint_least64_t;
# 217 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\stdint.h" 3 4
typedef int8_t int_fast8_t;




typedef uint8_t uint_fast8_t;




typedef int16_t int_fast16_t;




typedef uint16_t uint_fast16_t;




typedef int32_t int_fast32_t;




typedef uint32_t uint_fast32_t;







typedef int64_t int_fast64_t;






typedef uint64_t uint_fast64_t;
# 277 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\stdint.h" 3 4
typedef int64_t intmax_t;




typedef uint64_t uintmax_t;
# 10 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\lib\\gcc\\avr\\4.9.2\\include\\stdint.h" 2 3 4
# 38 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\inttypes.h" 2 3
# 77 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\inttypes.h" 3
typedef int32_t int_farptr_t;



typedef uint32_t uint_farptr_t;
# 127 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\sfr_defs.h" 2 3
# 100 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 2 3
# 272 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 3
# 1 "C:\\Program Files (x86)\\Atmel\\Studio\\7.0\\Packs\\atmel\\ATmega_DFP\\1.2.209\\include/avr/iom328p.h" 1 3
# 273 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 2 3
# 627 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 3
# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\portpins.h" 1 3
# 628 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 2 3

# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\common.h" 1 3
# 630 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 2 3

# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\version.h" 1 3
# 632 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 2 3






# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\fuse.h" 1 3
# 239 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\fuse.h" 3
typedef struct
{
    unsigned char low;
    unsigned char high;
    unsigned char extended;
} __fuse_t;
# 639 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 2 3


# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\lock.h" 1 3
# 642 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\avr\\io.h" 2 3
# 13 ".././dht11.h" 2
# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 1 3
# 45 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 3
# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay_basic.h" 1 3
# 40 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay_basic.h" 3
static __inline__ void _delay_loop_1(uint8_t __count) __attribute__((__always_inline__));
static __inline__ void _delay_loop_2(uint16_t __count) __attribute__((__always_inline__));
# 80 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay_basic.h" 3
void
_delay_loop_1(uint8_t __count)
{
 __asm__ volatile (
  "1: dec %0" "\n\t"
  "brne 1b"
  : "=r" (__count)
  : "0" (__count)
 );
}
# 102 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay_basic.h" 3
void
_delay_loop_2(uint16_t __count)
{
 __asm__ volatile (
  "1: sbiw %0,1" "\n\t"
  "brne 1b"
  : "=w" (__count)
  : "0" (__count)
 );
}
# 46 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 2 3
# 1 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 1 3
# 127 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern double cos(double __x) __attribute__((__const__));





extern double sin(double __x) __attribute__((__const__));





extern double tan(double __x) __attribute__((__const__));






extern double fabs(double __x) __attribute__((__const__));






extern double fmod(double __x, double __y) __attribute__((__const__));
# 168 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern double modf(double __x, double *__iptr);


extern float modff (float __x, float *__iptr);




extern double sqrt(double __x) __attribute__((__const__));


extern float sqrtf (float) __attribute__((__const__));




extern double cbrt(double __x) __attribute__((__const__));
# 195 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern double hypot (double __x, double __y) __attribute__((__const__));







extern double square(double __x) __attribute__((__const__));






extern double floor(double __x) __attribute__((__const__));






extern double ceil(double __x) __attribute__((__const__));
# 235 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern double frexp(double __x, int *__pexp);







extern double ldexp(double __x, int __exp) __attribute__((__const__));





extern double exp(double __x) __attribute__((__const__));





extern double cosh(double __x) __attribute__((__const__));





extern double sinh(double __x) __attribute__((__const__));





extern double tanh(double __x) __attribute__((__const__));







extern double acos(double __x) __attribute__((__const__));







extern double asin(double __x) __attribute__((__const__));






extern double atan(double __x) __attribute__((__const__));
# 299 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern double atan2(double __y, double __x) __attribute__((__const__));





extern double log(double __x) __attribute__((__const__));





extern double log10(double __x) __attribute__((__const__));





extern double pow(double __x, double __y) __attribute__((__const__));






extern int isnan(double __x) __attribute__((__const__));
# 334 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern int isinf(double __x) __attribute__((__const__));






__attribute__((__const__)) static inline int isfinite (double __x)
{
    unsigned char __exp;
    __asm__ (
 "mov	%0, %C1		\n\t"
 "lsl	%0		\n\t"
 "mov	%0, %D1		\n\t"
 "rol	%0		"
 : "=r" (__exp)
 : "r" (__x) );
    return __exp != 0xff;
}






__attribute__((__const__)) static inline double copysign (double __x, double __y)
{
    __asm__ (
 "bst	%D2, 7	\n\t"
 "bld	%D0, 7	"
 : "=r" (__x)
 : "0" (__x), "r" (__y) );
    return __x;
}
# 377 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern int signbit (double __x) __attribute__((__const__));






extern double fdim (double __x, double __y) __attribute__((__const__));
# 393 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern double fma (double __x, double __y, double __z) __attribute__((__const__));







extern double fmax (double __x, double __y) __attribute__((__const__));







extern double fmin (double __x, double __y) __attribute__((__const__));






extern double trunc (double __x) __attribute__((__const__));
# 427 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern double round (double __x) __attribute__((__const__));
# 440 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern long lround (double __x) __attribute__((__const__));
# 454 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\math.h" 3
extern long lrint (double __x) __attribute__((__const__));
# 47 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 2 3
# 86 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 3
static __inline__ void _delay_us(double __us) __attribute__((__always_inline__));
static __inline__ void _delay_ms(double __ms) __attribute__((__always_inline__));
# 165 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 3
void
_delay_ms(double __ms)
{
 double __tmp ;



 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(unsigned long);
 __tmp = ((16000000) / 1e3) * __ms;
# 184 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 210 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 3
}
# 254 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 3
void
_delay_us(double __us)
{
 double __tmp ;



 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(unsigned long);
 __tmp = ((16000000) / 1e6) * __us;
# 273 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 299 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h" 3
}
# 14 ".././dht11.h" 2





void dht11_get_raw_data();
# 9 ".././dht11.c" 2

void dht11_set(){

}

void dht11_get_raw_data(){
 (*(volatile uint8_t *)((0x0A) + 0x20))|= 0x80;

 (*(volatile uint8_t *)((0x0B) + 0x20))&= ~0x80;
 _delay_ms(18);
 (*(volatile uint8_t *)((0x0B) + 0x20))|= 0x80;
 _delay_us(40);
}

float dht11_get_temperature(uint8_t unit){
 return 0;
}

float dht11_get_humidity(uint8_t unit){
 return 0;
}

float dht11_get_heat_index(uint8_t unit){
 return 0;
}
