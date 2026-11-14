#include <stdio.h>
#include <string.h>
#include <unistd.h>
extern void _cyc_type_digest(const char *, size_t, char *);
int main(void) {
  char output[67]; memset(output,0x5a,sizeof output);
  _cyc_type_digest("abc",3,output+1);
  if (output[0]!=0x5a || output[66]!=0x5a || strcmp(output+1,"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad")) _exit(1);
  _cyc_type_digest("",0,output+1);
  if(strcmp(output+1,"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855")) _exit(1);
  puts("anonymous type digest: SHA-256 vectors and output bounds: PASS");
}
