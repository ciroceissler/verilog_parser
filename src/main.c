#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data2.h"

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

  free(descr);

  return 0;
}

