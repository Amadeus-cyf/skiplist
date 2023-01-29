# skiplist
Implement redis' skiplist in C++. Refer to leveldb's skiplist and [the Essay of William Pugh](https://15721.courses.cs.cmu.edu/spring2018/papers/08-oltpindexes1/pugh-skiplists-cacm1990.pdf).

## Building
For macos,
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
/* get a key by rank (0-index based) */
const size_t rank = skiplist.getRankofElement("key");
```

Get keys by range
```C++
/* get 4 keys starting at the key of index 0 */
const std::vector<string>& keys = skiplist.getElementsByRange(0, 4);
/* get 2 keys starting at the second last key */
const std::vector<string>& last_keys = skiplist.getElements(-2, 2);
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
2023-01-29T14:47:42+08:00
Running ./skiplist_benchmark
Run on (10 X 24.1214 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 1.35, 1.86, 1.87
```

### Running Benchmark
```sh
cd build && ./skiplist_benchmark
```

### Performance
```
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
Insert                   5868 ns         5842 ns       122100
Search                   4099 ns         4099 ns       154396
Update                   6621 ns         6621 ns        91885
Delete                   4718 ns         4718 ns       137806
GetElementByRank          579 ns          579 ns      1149350
GetRankOfElement         3999 ns         3999 ns       173237
GetElementsByRange    3521490 ns      3521495 ns          184
```

## Formatting Code
```sh
clang-format -i --style=file src/skiplist.h && clang-format -i --style=file src/skiplist_test.cc && clang-format -i --style=file benchmarks/skiplist_benchmark.cc
```
