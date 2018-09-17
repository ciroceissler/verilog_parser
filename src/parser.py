from __future__ import absolute_import
from __future__ import print_function
import sys
import os
import re
import pprint
from optparse import OptionParser

# the next line can be removed after installation
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import pyverilog.utils.version
from pyverilog.vparser.parser import parse

class Descriptor:
    attr     = ''
    type     = ''
    fanin     = 0
    fanout   = 0
    to_name  = []
    level    = 0
    to       = []
    fr0m     = []
    name     = ''
    ff_type  = ''
    gate_id  = 0

    def debug(self):
        print('attr     : ' + self.attr)
        print('type     : ' + self.type)
        print('fain     : ' + str(self.fanin))
        print('fanout   : ' + str(self.fanout))
        print('to_name  : ' + str(self.to_name))
        print('level    : ' + str(self.level))
        print('to       : ' + str(self.to))
        print('from     : ' + str(self.fr0m))
        print('name     : ' + self.name)
        print('ff_type  : ' + self.ff_type)
        print('gate_id  : ' + str(self.gate_id))
        print('============')

    def show(self):
        if len(self.to_name) == 0:
            self.to_name.append("")

        print(self.attr                                           + ';'  \
            + self.type                                           + ';'  \
            + str(self.fanin)                                     + ';'  \
            + str(self.fanout)                                    + ';'  \
            + str(self.to_name[0])                                + ';'  \
            + str(self.level)                                     + ';'  \
            + str(self.to).replace('[', '').replace(']', '')      + ' ;' \
            + str(self.fr0m).replace('[', '').replace(']', '')    + ' ;' \
            + self.name                                           + ';'  \
            + self.ff_type                                        + ';'  \
            + str(self.gate_id))

def gate_id():
    if 'cnt' not in gate_id.__dict__:
        gate_id.cnt = -1
    gate_id.cnt += 1

    return gate_id.cnt

def calc_primaries(tuple_verilog, pi, po):
    for var in tuple_verilog.children():
        if (type(var).__name__ == 'Input'):
            pi.append(var.name)

        if (type(var).__name__ == 'Output'):
            po.append(var.name)

        calc_primaries(var, pi, po)

def calc_fan_recursive(tuple_verilog, conn, instance_str):
    for var in tuple_verilog.children():
        input_ports = ['I', 'D', 'CK', 'RESET']

        if (type(var).__name__ == 'Instance'):
            instance_str = str(var.name)

        if (type(var).__name__ == 'PortArg'):
            try:
                conn[var.argname][0] += 1

                if (any(map(var.portname.startswith, input_ports))):
                    conn[var.argname][1].append(instance_str)
                else:
                    conn[var.argname][2].append(instance_str)
            except:
                if (any(map(var.portname.startswith, input_ports))):
                    conn[var.argname] = [1, [instance_str], []]
                else:
                    conn[var.argname] = [1, [], [instance_str]]

        calc_fan_recursive(var, conn, instance_str)

def recursive(tuple_verilog, conn, pi, po, descriptors, mod):
    for var in tuple_verilog.children():

        if (type(var).__name__ == 'Instance'):
            attr        = 'INTERNAL'
            type_str    = ''
            fanin       = 0
            fanout      = 0
            to_name     = []
            to          = []
            fr0m        = []
            ff_type_str = ''

            type_str = var.module.replace('_GATE', '')
            type_str = re.sub('\d', '', type_str)

            if ('FLIP_FLOP' in type_str):
                type_str = 'FF'

            if ('FLIP_FLOP_D' == var.module):
                ff_type_str = 'FFD'
            elif ('FLIP_FLOP_D_RESET' == var.module):
                ff_type_str = 'FFDR'
            elif ('FLIP_FLOP_D_PRESET' == var.module):
                ff_type_str = 'FFDP'
            elif ('FLIP_FLOP_D_RESET_PRESET' == var.module):
                ff_type_str = 'FFDRP'
            else:
                ff_type_str = 'FFD'

            for port in var.portlist:
                input_ports = ['I', 'D', 'CK', 'RESET']
                output_ports = ['O', 'Q']

                if (str(port.argname) in pi):
                    attr = 'PI'

                if (str(port.argname) in po):
                    attr = 'PO'

                if (any(map(port.portname.startswith, input_ports))):
                    fanin += 1

                    fr0m.extend(conn[port.argname][2])

                if (any(map(port.portname.startswith, output_ports))):
                    fanout = conn[port.argname][0] - 1

                    to.extend(conn[port.argname][1])

                    if (type(port.argname).__name__ == 'Pointer'):
                        to_name.append(str(port.argname.var) + '[' + str(port.argname.ptr) + ']')
                    else:
                        to_name.append(str(port.argname))


            desc = Descriptor()

            desc.attr     = attr
            desc.type     = type_str
            desc.fanin    = fanin
            desc.fanout   = fanout
            desc.to_name  = to_name
            desc.level    = 0
            desc.to       = to
            desc.fr0m     = fr0m
            desc.name     = var.name
            desc.ff_type  = ff_type_str
            desc.gate_id  = gate_id()

            mod[desc.name] = desc.gate_id

            descriptors.append(desc)

        recursive(var, conn, pi, po, descriptors, mod)

def update_index(descriptors, mod):
    i = 0
    while i < len(descriptors):
        j = 0
        while j < len(descriptors[i].to):
            descriptors[i].to[j] = mod[descriptors[i].to[j]]
            j = j + 1

        j = 0
        while j < len(descriptors[i].fr0m):
            descriptors[i].fr0m[j] = mod[descriptors[i].fr0m[j]]
            j = j + 1

        i = i + 1

def walk(descriptors, index, level, list):

    if len(list) > 0:
        if index in list:
            return 1

    list.append(index)

    for var in descriptors[index].to:
        if level > descriptors[var].level:
            descriptors[var].level = level

        if descriptors[var].attr == 'INTERNAL':
            walk(descriptors, var, level + 1, list)

    return 0

def update_level(descriptors):
    i = 0;

    while i < len(descriptors):

        list = []

        if descriptors[i].attr == 'PI':
            descriptors[i].level = 0
            walk(descriptors, i, 1, list)

        i = i + 1

def main():
    INFO    = "Verilog code parser"
    VERSION = pyverilog.utils.version.VERSION
    USAGE   = "Usage: python example_parser.py file ..."

    sys.setrecursionlimit(50000)

    def showVersion():
        print(INFO)
        print(VERSION)
        print(USAGE)
        sys.exit()

    optparser = OptionParser()
    optparser.add_option("-v","--version",action="store_true",dest="showversion",
                         default=False,help="Show the version")
    optparser.add_option("-I","--include",dest="include",action="append",
                         default=[],help="Include path")
    optparser.add_option("-D",dest="define",action="append",
                         default=[],help="Macro Definition")
    (options, args) = optparser.parse_args()

    filelist = args
    if options.showversion:
        showVersion()

    for f in filelist:
        if not os.path.exists(f): raise IOError("file not found: " + f)

    if len(filelist) == 0:
        showVersion()

    ast, directives = parse(filelist,
                            preprocess_include=options.include,
                            preprocess_define=options.define)
    pi   = []
    po   = []
    conn = {}
    mod  = {}

    calc_primaries(ast, pi, po)
    calc_fan_recursive(ast, conn, '')

    descriptors = []

    recursive(ast, conn, pi, po, descriptors, mod)

    update_index(descriptors, mod)

    update_level(descriptors)

    for var in descriptors:
        var.show()

if __name__ == '__main__':
    main()

