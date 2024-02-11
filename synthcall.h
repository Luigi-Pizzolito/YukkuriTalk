#ifndef _SYNTHCALL_H
#define _SYNTHCALL_H

unsigned char* synth(const char *str, int *size);
void free_synth(unsigned char *wav);

#endif