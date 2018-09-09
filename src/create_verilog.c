#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data2.h"

DESCRIPTOR* create_verilog(const char* filename, int* size) {
  DESCRIPTOR *descr = (DESCRIPTOR*) malloc (sizeof(DESCRIPTOR));
  FILE *file = fopen(filename, "r");

  char line[256];

  *size = 0;

  while (fgets(line, sizeof(line), file)) {
    int i;
    int j;
    char *pch;
    char *to;
    char *from;

    i = *size;

    *size = *size + 1;

    descr = realloc(descr, *size*sizeof(DESCRIPTOR));

    // write: descr->attr
    pch = strtok(line, ";");

    if      (strcmp(pch, "INTERNAL") == 0) descr[i].attr = INTERNAL;
    else if (strcmp(pch, "PI") == 0)       descr[i].attr = PI;
    else if (strcmp(pch, "PO") == 0)       descr[i].attr = PO;

    // write: descr->type
    pch = strtok(NULL, ";");

    if      (strcmp(pch, "AND") == 0)      descr[i].type = AND;
    else if (strcmp(pch, "NAND") == 0)     descr[i].type = NAND;
    else if (strcmp(pch, "OR") == 0)       descr[i].type = OR;
    else if (strcmp(pch, "NOR") == 0)      descr[i].type = NOR;
    else if (strcmp(pch, "BUF") == 0)      descr[i].type = BUF;
    else if (strcmp(pch, "INV") == 0)      descr[i].type = NOT;
    else if (strcmp(pch, "XOR") == 0)      descr[i].type = EXOR;
    else if (strcmp(pch, "XNOR") == 0)     descr[i].type = EXNOR;
    else if (strcmp(pch, "FF") == 0)       descr[i].type = FF;
    else if (strcmp(pch, "LOGIC0") == 0)   descr[i].type = LOGIC0;
    else if (strcmp(pch, "LOGIC1") == 0)   descr[i].type = LOGIC1;
    else if (strcmp(pch, "CALLBACK") == 0) descr[i].type = CALLBACK;

    // write: descr->fanin
    pch = strtok(NULL, ";");

    descr[i].fanin  = atoi(pch);

    // write: descr->fanout
    pch = strtok(NULL, ";");

    descr[i].fanout = atoi(pch);

    // write: descr->to_name
    pch = strtok(NULL, ";");

    descr[i].to_name = (char*)(calloc(strlen(pch), sizeof(char)));

    strcpy(descr[i].to_name, pch);

    // write: descr->level
    pch = strtok(NULL, ";");

    descr[i].level = atoi(pch);

    // tmp: descr->to
    pch = strtok(NULL, ";");

    to = (char*)(calloc(strlen(pch), sizeof(char)));

    strcpy(to, pch);

    // write: descr->from
    pch = strtok(NULL, ";");

    from = (char*)(calloc(strlen(pch), sizeof(char)));

    strcpy(from, pch);

    // write: descr->name
    pch = strtok(NULL, ";");

    descr[i].name = (char*)(calloc(strlen(pch), sizeof(char)));

    strcpy(descr[i].name, pch);

    // write: descr->ff_type
    pch = strtok(NULL, ";");

    if      (strcmp(pch, "FFD") == 0)   descr[i].ff_type = FFD;
    else if (strcmp(pch, "FFDR") == 0)  descr[i].ff_type = FFDR;
    else if (strcmp(pch, "FFDP") == 0)  descr[i].ff_type = FFDP;
    else if (strcmp(pch, "FFDRP") == 0) descr[i].ff_type = FFDRP;

    // write: descr->gate_id
    pch = strtok(NULL, ";");

    descr[i].gate_id = atoi(pch);

    // write: descr->to
    pch = strtok(to, ",");

    j = 0;
    while(pch) {
      if (0 == j) {
        descr[i].to = (int*)malloc(sizeof(int));
      } else {
        descr[i].to = (int*)realloc(descr[i].to, (j + 1)*sizeof(int));
      }

      descr[i].to[j] = atoi(pch);

      pch = strtok(NULL, ",");

      j = j + 1;
    }

    // write: descr->from
    pch = strtok(from, ",");

    j = 0;
    while(pch) {
      if (0 == j) {
        descr[i].from = (int*)malloc(sizeof(int));
      } else {
        descr[i].from = (int*)realloc(descr[i].from, (j + 1)*sizeof(int));
      }

      descr[i].from[j] = atoi(pch);

      pch = strtok(NULL, ",");

      j = j + 1;
    }
  }

  fclose(file);

  return descr;
}

