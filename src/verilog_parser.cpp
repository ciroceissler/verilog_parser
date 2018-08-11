#include "verilog_parser.h"

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

DESCRIPTOR* parser_verilog(const char* filename, int* size) {
  std::vector<DESCRIPTOR*> v_descr;

  std::string cmd = "python3 parser.py ";

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
    descr->to_name = static_cast<char*>(calloc(length, sizeof(char)));
    std::memcpy(descr->to_name, args[4].c_str(), length);

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
    descr->name = static_cast<char*>(calloc(length, sizeof(char)));
    std::memcpy(descr->name, args[8].c_str(), length);

    if      (args[9] == "FFD")   descr->attr = FFD;
    else if (args[9] == "FFDR")  descr->attr = FFDR;
    else if (args[9] == "FFDP")  descr->attr = FFDP;
    else if (args[9] == "FFDRP") descr->attr = FFDRP;

    descr->gate_id = atoi(args[10].c_str());

    v_descr.push_back(descr);
  }

  // maintain compatibility with C code

  *size = v_descr.size();

  DESCRIPTOR *result = static_cast<DESCRIPTOR*>(calloc(v_descr.size(),sizeof(DESCRIPTOR)));

  for (int i = 0; i < v_descr.size(); i++) {
    result[i] = *v_descr[i];
  }

  return result;
}

