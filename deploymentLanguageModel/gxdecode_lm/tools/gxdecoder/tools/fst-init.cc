// Copyright (c) 2016 Personal (Binbin Zhang)
// Created on 2016-11-08
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>

#include "parse-option.h"
#include "fst.h"

int main(int argc, char *argv[]) {
  using gxdecoder::Fst;
  using gxdecoder::SymbolTable;
  using gxdecoder::ParseOptions;
  const char *usage = "Init fst from topo file, like the way openfst compile"
                      "Usage: aslp-fst-init topo_file out_file\n"
                      "eg: aslp-fst-init topo_file out.fst\n";

  ParseOptions po(usage);
  std::string isymbols = "";
  po.Register("isymbols", &isymbols, "input symbol file");
  std::string osymbols = "";
  po.Register("osymbols", &osymbols, "output symbol file");

  po.Read(argc, argv);

  if (po.NumArgs() != 2) {
    po.PrintUsage();
    exit(1);
  }

  std::string topo_file = po.GetArg(1),
              out_file = po.GetArg(2);

  Fst fst;
  if (isymbols == "" || osymbols == "") {
    fst.ReadTopo(topo_file);
  } else {
    SymbolTable isymbol_table(isymbols), osymbol_table(osymbols);
    fst.ReadTopo(topo_file, isymbol_table, osymbol_table);
  }
  fst.Write(out_file);
  return 0;
}
