// Copyright (c) 2018 Personal (Binbin Zhang)
// Created on 2018-04-09
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

#include "utils.h"
#include "varint.h"

namespace gxdecoder {

template<class T>
void ReadBasic(std::istream& is, T* t) {
  is.read(reinterpret_cast<char *>(t), sizeof(T));
}

template<class T>
void WriteBasic(std::ostream& os, T t) {
  os.write(reinterpret_cast<char *>(&t), sizeof(T));
}

#ifdef USE_VARINT
template<>
void ReadBasic(std::istream& is, int32_t* t) {
  *t = Varint::ReadInt32(is);
}

template<>
void WriteBasic(std::ostream& os, int32_t t) {
  Varint::WriteInt32(os, t);
}
#endif  // USE_VARINT

template void ReadBasic<int32_t>(std::istream& is, int32_t*);
template void WriteBasic<int32_t>(std::ostream& os, int32_t);
template void ReadBasic<float>(std::istream& is, float*);
template void WriteBasic<float>(std::ostream& os, float);

}  // namespace gxdecoder
