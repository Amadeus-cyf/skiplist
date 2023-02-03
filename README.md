# skiplist
Implement redis' skiplist in C++. Refer to leveldb's skiplist and [the Essay of William Pugh](https://15721.courses.cs.cmu.edu/spring2018/papers/08-oltpindexes1/pugh-skiplists-cacm1990.pdf).

## Building
```sh
mkdir build && cd build
cmake .. && cmake --build .
```

## Usage
Init with default comparator. The type must support operator `<`, `>` and `==`.
```C++
#include "skiplist.h"

Skiplist<string> skiplist(4); /* initial depths */
```

Init with custom comparator.
```C++
#include "skiplist.h"

const auto compare =
    [](const string& k1, const string& k2) { return k1 < k2 ? -1 : (k1 == k2 ? 0 : 1); };
Skiplist<string, decltype(compare)> skiplist(4, compare);
```

Insert a key.
```C++
/* return true if success */
skiplist.insert("key0");
```

Check whether a key exists.
```C++
if(skiplist.contains("key1")) {
  /* do something */
}
```

Get a key by rank
```C++
/* get first element */
const string& first_key = skiplist.getElementByRank(0);
/* get last element */
const string& last_key = skiplist.getElementByRank(-1);
```

Get rank of a key
```C++
/* get the rank(0-index based) of a key */
const size_t rank = skiplist.getRankofElement("key");
```

Get keys by range
```C++
/* get 4 keys starting at the key of index 0 */
const std::vector<string>& keys = skiplist.getElementsByRange(0, 4);
/* get 2 keys starting at the second last key */
const std::vector<string>& last_keys = skiplist.getElementsByRange(-2, 2);
```

Get keys by reverse range
```C++
/* get 4 keys reversely starting at the key of index 0 */
const std::vector<string>& keys = skiplist.getElementsByRevRange(0, 4);
/* get 2 keys reversely starting at the second last key */
const std::vector<string>& last_keys = skiplist.getElementsByRevRange(-2, 2);
```

Delete a key.
```C++
/* return true if success */
skiplist.del("key1");
```

Update a key.
```C++
/* return true if success */
skiplist.update("key2", "key5");
```

Iterate over the skiplist.
```C++
for (auto it = skiplist.begin(); it != skiplist.end(); ++it) {
  /* do something */
}
```

## Running Unit Tests
```sh
cd build && ./skiplist_test
```

## Benchmarks
### Setup
Use a skiplist with initial depth = 16. All keys in the skiplist are random generated string with length = 10.
```
2023-02-03T21:36:57+08:00
Running ./skiplist_benchmark
Run on (10 X 24.1212 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 1.90, 1.85, 1.82
```

### Running Benchmark
```sh
cd build && ./skiplist_benchmark
```

### Performance
```
----------------------------------------------------------------
Benchmark                      Time             CPU   Iterations
----------------------------------------------------------------
Insert                      5497 ns         5497 ns       100000
Search                      4601 ns         4601 ns       159738
Update                      6843 ns         6843 ns        95910
Delete                      5234 ns         5233 ns       137233
GetElementByRank             525 ns          525 ns      1153441
GetRankOfElement            3959 ns         3959 ns       174487
GetElementsByRange       2365208 ns      2365212 ns          231
GetElementsByRevRange    2367075 ns      2367060 ns          334
```

## Formatting Code
```sh
clang-format -i --style=file src/skiplist.h && clang-format -i --style=file src/skiplist_test.cc && clang-format -i --style=file benchmarks/skiplist_benchmark.cc
```
