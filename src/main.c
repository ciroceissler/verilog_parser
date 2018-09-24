#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data2.h"
#include "fault2.h"

FAULT *faultlist;

int main(int argc, char *argv[]) {

  create_verilog(argv[1]);

  for (int i = 0; i < n_descr; i++) {
    printf("attr    : %d\n", (int) descr[i].attr);
    printf("type    : %d\n", (int) descr[i].type);
    printf("fanin   : %d\n", descr[i].fanin);
    printf("fanout  : %d\n", descr[i].fanout);
    printf("to_name : %s\n", descr[i].to_name);
    printf("level   : %d\n", descr[i].level);
    printf("to[0]   : %d\n", descr[i].to[0]);
    printf("from[0] : %d\n", descr[i].from[0]);
    printf("name    : %s\n", descr[i].name);
    printf("ff_type : %d\n", (int) descr[i].ff_type);
    printf("gate_id : %d\n", (int) descr[i].gate_id);

    printf("==========================\n");
  }

  create_fau(NULL);

  for (int i = 0; i < 2*n_descr; i++) {
    printf("descr      : %d\n", faultlist[i].descr);
    printf("from       : %d\n", faultlist[i].from);
    printf("pin        : %d\n", faultlist[i].pin);
    printf("val        : %d\n", faultlist[i].val);
    printf("size       : %d\n", faultlist[i].size);
    printf("status     : %d\n", faultlist[i].status);
    printf("type       : %d\n", faultlist[i].type);
    printf("activation : %d\n", faultlist[i].activation);

    printf("==========================\n");
  }

  free(descr);

  return 0;
}

