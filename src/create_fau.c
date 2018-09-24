#include <stdlib.h>

#include "data2.h"
#include "fault2.h"

extern FAULT *faultlist;
extern DESCRIPTOR *descr;

extern int n_descr;

int create_fau(char *name) {

  faultlist = (FAULT*) malloc (2*n_descr*sizeof(FAULT));

  for (int i = 0; i < n_descr; i++) {
    for (int j = 0; j < 2; j++) {
      faultlist[2*i + j].descr      = descr[i].gate_id;
      faultlist[2*i + j].from       = descr[i].from[0];
      faultlist[2*i + j].pin        = descr[i].from[0];
      faultlist[2*i + j].val        = (j == 0) ? STUCK_AT_0 : STUCK_AT_1;
      faultlist[2*i + j].size       = descr[i].fanin + descr[i].fanout;

      faultlist[2*i + j].status     = 0;
      faultlist[2*i + j].type       = 0;
      faultlist[2*i + j].activation = 0;
    }
  }

  return 0;
}
