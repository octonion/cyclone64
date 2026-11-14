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

void *abi_pointer_at(unsigned long address) { return (void *)address; }

struct abi_mixed { int tag; long wide; double fraction; };
struct abi_small { int x; int y; };
struct abi_mixed abi_mixed_roundtrip(struct abi_mixed x) {
  x.tag--; x.wide++; x.fraction*=2; return x;
}
struct abi_small abi_small_roundtrip(struct abi_small x) { x.x--; x.y++; return x; }
int abi_apply_int(int (*f)(int),int x) { return f(x); }
long abi_apply_long(long (*f)(long),long x) { return f(x); }
int abi_apply_reader(int (*f)(int *),int *p) { return f(p); }
struct abi_small abi_apply_small(struct abi_small (*f)(struct abi_small),struct abi_small x) { return f(x); }
