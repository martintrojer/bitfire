#ifndef __common_h
#define __common_h

extern unsigned int gimagesize;
//
int Str2Int(const char *inputstr,unsigned int *intnum);
int GetIntegerInput(unsigned int * num);

//Command/Menu Handlers
void ProgramExtFlash(int argc, const char **argv);
void SerialDownload(int argc, const char **argv);
void LoadLinux(int argc, const char **argv);

#endif