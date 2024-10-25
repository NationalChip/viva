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

#include "fst.h"

int main(int argc, char* argv[]) {
  using gxdecoder::Fst;
  const char *usage = "Showing details text information of fsm format file\n"
                      "Usage: fst-info fst-file]\n"
                      "eg: fst-info in.fst\n";

  if (argc != 2) {
    printf("%s", usage);
    return -1;
  }

  Fst fst(argv[1]);
  fst.Info();
  return 0;
}

