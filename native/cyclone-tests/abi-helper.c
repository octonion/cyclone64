unsigned long abi_echo(unsigned long value) { return value; }

unsigned long abi_high_value(void) { return 0x123456789abcdef0UL; }
long abi_signed_long(void) { return -1L; }
unsigned int abi_unsigned_int(void) { return 1U; }
unsigned long abi_large_index(void) { return 0x100000001UL; }

#include <limits.h>
int abi_literal_values(unsigned long a, unsigned long b, long c, long d,
                       unsigned long long e, long long f) {
  return a == ULONG_MAX && b == ULONG_MAX && c == LONG_MAX && d == LONG_MIN &&
         e == ULLONG_MAX && f == LLONG_MIN;
}
