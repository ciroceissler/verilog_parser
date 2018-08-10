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

void split(std::string str, char token, std::vector<std::string> &result) {
  std::istringstream f(str);
  std::string s;

  while (std::getline(f, s, token)) {
    result.push_back(s);
  }
}

std::string exec(const std::string cmd) {
  std::string result;
  std::array<char, 128> buffer;
  std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

  if (!pipe)
    throw std::runtime_error("popen() failed!");

  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }

  return result;
}

extern "C" DESCRIPTOR* parser_verilog(const char* filename, int &size) {
  std::vector<DESCRIPTOR*> v_descr;

  std::string cmd = "python3 ../src/parser.py ";

  cmd += filename;

  std::string output = exec(cmd);

  std::vector<std::string> element;

  split(output, '\n', element);

  for (auto it = element.begin(); it != element.end(); ++it) {
    int length;

    std::vector<std::string> args;
    std::vector<std::string> v;

    split(*it, ';', args);

    DESCRIPTOR *descr = new DESCRIPTOR;

    if      (args[0] == "INTERNAL") descr->attr = INTERNAL;
    else if (args[0] == "PI")       descr->attr = PI;
    else if (args[0] == "PO")       descr->attr = PO;

    if      (args[1] == "AND")      descr->type = AND;
    else if (args[1] == "NAND")     descr->type = NAND;
    else if (args[1] == "OR")       descr->type = OR;
    else if (args[1] == "NOR")      descr->type = NOR;
    else if (args[1] == "BUF")      descr->type = BUF;
    else if (args[1] == "INV")      descr->type = NOT;
    else if (args[1] == "XOR")      descr->type = EXOR;
    else if (args[1] == "XNOR")     descr->type = EXNOR;
    else if (args[1] == "FF")       descr->type = FF;
    else if (args[1] == "LOGIC0")   descr->type = LOGIC0;   // FIXME: ?
    else if (args[1] == "LOGIC1")   descr->type = LOGIC1;   // FIXME: ?
    else if (args[1] == "CALLBACK") descr->type = CALLBACK; // FIXME: ?

    descr->fanin  = atoi(args[2].c_str());
    descr->fanout = atoi(args[3].c_str());

    // FIXME: args[4] / to_name only one output net?

    length = args[4].size() + 1;
    descr->to_name = new char(length);
    strncpy(descr->to_name, args[4].c_str(), length);

    descr->level = atoi(args[5].c_str());

    v.clear();
    split(args[6], ',', v);

    descr->to = new int(v.size());

    for (unsigned int i = 0; i < v.size(); i++) {
      descr->to[i] = atoi(v[i].c_str());
    }

    v.clear();
    split(args[7], ',', v);

    descr->from = new int(v.size());

    for (unsigned int i = 0; i < v.size(); i++) {
      descr->from[i] = atoi(v[i].c_str());
    }

    length = args[8].size() + 1;
    descr->name = new char(length);
    strncpy(descr->name, args[8].c_str(), length);

    if      (args[9] == "FFD")   descr->attr = FFD;
    else if (args[9] == "FFDR")  descr->attr = FFDR;
    else if (args[9] == "FFDP")  descr->attr = FFDP;
    else if (args[9] == "FFDRP") descr->attr = FFDRP;

    descr->gate_id = atoi(args[10].c_str());

    v_descr.push_back(descr);
  }

  // maintain compatibility with C code

  size = v_descr.size();

  DESCRIPTOR *result = static_cast<DESCRIPTOR*>(calloc(v_descr.size(),sizeof(DESCRIPTOR)));

  for (int i = 0; i < v_descr.size(); i++) {
    result[i] = *v_descr[i];
  }

  return result;
}

int main(int argc, char* argv[]) {
  int size;

  DESCRIPTOR *descr;

  std::string filename = (argc > 1) ? argv[1] : "top.v";

  descr = parser_verilog(filename.c_str(), size);

  for (int i = 0; i < size; i++) {
    std::cout << "attr    : " << (int) descr[i].attr << std::endl;
    std::cout << "type    : " << (int) descr[i].type << std::endl;
    std::cout << "fanin   : " << descr[i].fanin << std::endl;
    std::cout << "fanout  : " << descr[i].fanout << std::endl;
    std::cout << "to_name : " << descr[i].to_name << std::endl;
    std::cout << "level   : " << descr[i].level << std::endl;
    std::cout << "to[0]   : " << descr[i].to[0] << std::endl;
    std::cout << "from[0] : " << descr[i].from[0] << std::endl;
    std::cout << "name    : " << descr[i].name << std::endl;
    std::cout << "ff_type : " << (int) descr[i].ff_type<< std::endl;
    std::cout << "gate_id : " << (int) descr[i].gate_id << std::endl;
    std::cout << "==========================" << std::endl;
  }

  return 0;
}

