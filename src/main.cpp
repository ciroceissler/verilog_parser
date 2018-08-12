#include "verilog_parser.h"

int main(int argc, char* argv[]) {
  int size;

  DESCRIPTOR *descr;

  std::string filename = (argc > 1) ? argv[1] : "top.v";

  descr = create_verilog(filename.c_str(), &size);

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

