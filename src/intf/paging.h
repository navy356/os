#pragma once
#include <stdint.h>
typedef struct
{
  uint64_t present                   :1;
  uint64_t writeable                 :1;
  uint64_t user_access               :1;
  uint64_t write_through             :1;
  uint64_t cache_disabled            :1;
  uint64_t accessed                  :1;
  uint64_t ignored_3                 :1;
  uint64_t size                      :1; // must be 0
  uint64_t ignored_2                 :4;
  uint64_t page_ppn                  :28;
  uint64_t reserved_1                :12; // must be 0
  uint64_t ignored_1                 :11;
  uint64_t execution_disabled        :1;
} __attribute__((__packed__)) PageMapLevel4Entry;