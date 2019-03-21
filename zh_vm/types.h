#pragma once

#define OK 1
#define ERROR 0

#define AX 0
#define AH AX
#define AL AX + 1

#define BX 2
#define BH BX
#define BL BX + 1

#define CX 4
#define CH CX
#define CL CX + 1

#define DX 6
#define DH DX
#define DL DX + 1

typedef int Status;