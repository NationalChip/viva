// Copyright (c) 2016 Personal (Binbin Zhang)
// Created on 2018-07-11
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


#ifndef OBJECT_POOL_H_
#define OBJECT_POOL_H_

#include <stdlib.h>

#include <string>
#include <sstream>

#include "utils.h"

namespace gxdecoder {

const size_t kMaxBlockSize = 10240;

template <class Type>
class IObjectPool {
 public:
  explicit IObjectPool(int init_size = 32) {}
  virtual ~IObjectPool() {}
  virtual Type* New() = 0;
  virtual void Delete(Type*) = 0;
  virtual std::string Report() = 0;
};

template <class Type>
class NaiveObjectPool : public IObjectPool<Type> {
 public:
  explicit NaiveObjectPool(int init_size = 32): allocated_(0), deleted_(0) {}
  ~NaiveObjectPool() {}

  virtual inline Type* New() {
    allocated_++;
    return new Type();
  }

  virtual inline void Delete(Type* object) {
    deleted_++;
    delete object;
  }

  virtual std::string Report() {
    std::stringstream ss;
    ss << "allocated " << allocated_ << " deleted " << deleted_;
    return ss.str();
  }

 private:
  int allocated_;
  int deleted_;
};

template <class Type>
class CacheObjectPool : public IObjectPool<Type> {
 public:
  explicit CacheObjectPool(int init_size = 32): allocated_(0), free_(0) {
    // Object size must greater than 4
    CHECK(sizeof(Type) >= sizeof(void *));
    current_cursor_ = 0;
    first_node_ = new Node(init_size);
    last_node_ = first_node_;
    latest_deleted_ = NULL;
    allocated_ = init_size;
  }

  ~CacheObjectPool() {
    Node* node = first_node_;
    while (node) {
      Node *next_node = node->next_node;
      delete node;
      node = next_node;
    }
  }

 private:
  struct Node {
    Type* memory;
    size_t capacity;
    Node* next_node;

    explicit Node(size_t size): memory(NULL), capacity(size), next_node(NULL) {
      CHECK(capacity > 0);
      memory = new Type[capacity]();
    }

    ~Node() {
      delete [] memory;
    }

    inline Type* operator ()(int i) {
      CHECK(i < capacity);
      return memory + i;
    }
  };

 public:
  virtual inline Type* New() {
    Type *object = NULL;
    if (latest_deleted_ != NULL) {
      object = latest_deleted_;
      latest_deleted_ = *(reinterpret_cast<Type **>(latest_deleted_));
      free_--;
    } else {
      // if current block is used up, reallocate bigger memory node
      if (current_cursor_ >= last_node_->capacity) {
        size_t size = last_node_->capacity * 2;
        if (size > kMaxBlockSize) size = kMaxBlockSize;
        Node* new_node = new Node(size);
        last_node_->next_node = new_node;
        last_node_ = new_node;
        current_cursor_ = 0;
        allocated_ += size;
      }
      object = (*last_node_)(current_cursor_);
      current_cursor_++;
    }
    return object;
  }

  virtual inline void Delete(Type* object) {
    object->~Type();
    *(reinterpret_cast<Type **>(object)) = latest_deleted_;
    latest_deleted_ = object;
    free_++;
  }

  virtual std::string Report() {
    std::stringstream ss;
    ss << "allocated " << allocated_ << " free " << free_
       << " cursor " << current_cursor_;
    return ss.str();
  }

 private:
  int32_t allocated_;
  int32_t free_;
  int32_t current_cursor_;
  Type* latest_deleted_;  // latest deleted object, an implicit stack in it
  Node* first_node_, *last_node_;  // link list head and tail
};

}  // namespace gxdecoder

#endif  // OBJECT_POOL_H_
