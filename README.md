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
2023-01-22T14:33:13+08:00
Running ./skiplist_benchmark
Run on (10 X 24.0606 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 2.93, 2.43, 2.20
```

### Running Benchmark
```sh
cd build && ./skiplist_benchmark
```

### Performance
```
-----------------------------------------------------
Benchmark           Time             CPU   Iterations
-----------------------------------------------------
Insert           5592 ns         5591 ns       111527
Search           4335 ns         4335 ns       161564
Update           6804 ns         6804 ns        89739
Delete           5091 ns         5091 ns       124217
GetByRank         673 ns          673 ns       879176
```

## Formatting Code
```sh
clang-format -i --style=file src/skiplist.h && clang-format -i --style=file src/skiplist_test.cc && clang-format -i --style=file benchmarks/skiplist_benchmark.cc
```
