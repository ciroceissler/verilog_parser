#ifndef VERILOG_PARSER_H
#define VERILOG_PARSER_H

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <cstring>

/* Extend FF type */
#define FFD   0
#define FFDR  1
#define FFDP  2
#define FFDRP 3

/* gate ATTRIBUTES */
#define INTERNAL 0
#define PI       1
#define PO       2

/* GATE TYPES. INTERNAL CODES */
/* TYPES with LSB==1 are NEGATED (ask corno@polito.it) */
/* TYPES must be CONSECUTIVE (ask squillero@polito.it) */
enum {
  AND = 0,  /* 0 */
  NAND,     /* 1 */
  OR,       /* 2 */
  NOR,      /* 3 */
  BUF,      /* 4 */
  NOT,      /* 5 */
  EXOR,     /* 6 */
  EXNOR,    /* 7 */
  LOGIC0,   /* 8 */
  LOGIC1,   /* 9 */
  FF = 10,  /* 10 */
  CALLBACK  /* 11 */
};

typedef struct {
  char  attr;        /* PI, PO, INTERNAL */
  signed char type;  /* AND NAND .... FF */
  int fanin, fanout; /* number of gate inputs/outputs */
  char  *to_name;    /* name of the output net */
  int level;         /* distance from PIs or PPIs */
  int *to, *from;    /* pointers to arrays of gate indexes */
  char *name;        /* gate instance name */

  char  ff_type;     /* FFD   = FF
                        FFDR  = FF with reset
                        FFDP  = FF with preset
                        FFDRP = FF with reset and preset */
  int gate_id;      /* index to a structure describing library */
} DESCRIPTOR;

extern "C" DESCRIPTOR* create_verilog(const char* filename, int* size);

#endif  //  VERILOG_PARSER_H

