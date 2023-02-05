# skiplist
Implement redis' skiplist in C++. Refer to leveldb's skiplist and [the Essay of William Pugh](https://15721.courses.cs.cmu.edu/spring2018/papers/08-oltpindexes1/pugh-skiplists-cacm1990.pdf).

## Building
```sh
mkdir build && cd build
cmake .. && cmake --build .
```

## Sample
```
h3---4---end
h2---1---key0---2---key4---1---end
h1---1---key0---1---key2---1---key4---1---end
h0---1---key0---1---key2---1---key4---1---key5---0---end
```

## Usage
Init with default comparator. The type must support operators `<`, `>` and `==`.
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

/* access via index, 0-index based */
const string& first_key = skiplist[0];
const string& last_key = skiplist[skiplist.size()-1];
```

Get rank of a key
```C++
/* get the rank(0-index based) of a key */
const size_t rank = skiplist.getRankofElement("key");
```

Get keys by range
```C++
/* get 4 keys starting at index 0 */
const std::vector<string>& keys = skiplist.getElementsByRange(0, 4);
/* get 2 keys starting at index size-1 */
const std::vector<string>& last_keys = skiplist.getElementsByRange(-2, 2);
```

Get keys by reverse range
```C++
/* get 4 keys reversely starting at the key of index 0 */
const std::vector<string>& keys = skiplist.getElementsByRevRange(0, 4);
/* get 2 keys reversely starting at the second last key */
const std::vector<string>& last_keys = skiplist.getElementsByRevRange(-2, 2);
```

Get keys greater than a value
```C++
/* return all keys greater than "key_to_compare" */
const std::vector<string>& keys = skiplist.getElementsGt("key_to_compare");
```

Get keys greater than or equal to a value
```C++
/* return all keys greater than or equal to "key_to_compare" */
const std::vector<string>& keys = skiplist.getElementsGte("key_to_compare");
```

Get keys less than a value
```C++
/* return all keys less than "key_to_compare" */
const std::vector<string>& keys = skiplist.getElementsLt("key_to_compare");
```

Get keys less than or equal to a value
```C++
/* return all keys less than or equal to "key_to_compare" */
const std::vector<string>& keys = skiplist.getElementsLte("key_to_compare");
```

Get keys within a range
```C++
/* return all keys within the range [key_start, key_end) */
const std::vector<string>& keys = skiplist.getElementsInRange("key_start", "key_end");
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

Print the skiplist.
```C++
skiplist.print();
```

## Running Unit Tests
```sh
cd build && ./skiplist_test
```

## Benchmarks
### Setup
Use a skiplist with initial depth = 16. All keys in the skiplist are random generated strings with length = 10.
```
2023-02-05T15:38:21+08:00
Running ./skiplist_benchmark
Run on (10 X 24.0411 MHz CPU s)
CPU Caches:
  L1 Data 64 KiB
  L1 Instruction 128 KiB
  L2 Unified 4096 KiB (x10)
Load Average: 2.43, 2.12, 1.88
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
Insert                      5653 ns         5652 ns       115831
Search                      4776 ns         4776 ns       147758
Update                      7002 ns         7002 ns        93333
Delete                      4718 ns         4718 ns       134987
GetElementByRank             583 ns          583 ns      1087176
GetRankOfElement            4025 ns         4025 ns       172011
GetElementsByRange       3439821 ns      3439819 ns          221
GetElementsByRevRange    3278682 ns      3278642 ns          316
GetElementsGt            6601971 ns      6601740 ns          127
GetElementsLt            6820850 ns      6820857 ns           77
GetElementsInRange       5668682 ns      5668310 ns          100
```
