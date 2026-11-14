/* Stable anonymous C type tags across independently translated modules.
 * Hashing lives only in the compiler; emitted applications need no crypto API. */
#include <openssl/sha.h>
#include <stddef.h>
void _cyc_type_digest(const char *text, size_t length, char output[65]) {
  unsigned char digest[SHA256_DIGEST_LENGTH];
  static const char hex[]="0123456789abcdef";
  SHA256((const unsigned char *)text,length,digest);
  for (size_t i=0;i<sizeof digest;++i) {
    output[2*i]=hex[digest[i]>>4]; output[2*i+1]=hex[digest[i]&15];
  }
  output[64]=0;
}
