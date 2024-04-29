#ifndef __linux__
#error "Currently this program supports x86_64 Linux and ARM64 Linux only."
#endif
#ifdef __x86_64__
#include "syscall_x86_64.c"
#else
#ifdef __aarch64__
#include "syscall_arm64.c"
#else
#error "Currently this program supports x86_64 Linux and ARM64 Linux only."
#endif
#endif
#pragma pack(1)
