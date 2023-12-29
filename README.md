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

skiplist::Skiplist<std::string> skiplist(4); /* initial depths */
```

Init with custom comparator.
```C++
#include "skiplist.h"

const auto compare =
    [](const std::string& k1, const std::string& k2) { return k1 < k2 ? -1 : (k1 == k2 ? 0 : 1); };
skiplist::Skiplist<std::string, decltype(compare)> skiplist(4, compare);
```

Insert a key.
```C++
/* return true if success */
skiplist.Insert("key0");
```

Check whether a key exists.
```C++
if(skiplist.Contains("key1")) {
  /* do something */
}
```

Get a key by rank
```C++
/* get first element */
const std::string& first_key = skiplist.GetElementByRank(0);
/* get last element */
const std::string& last_key = skiplist.GetElementByRank(-1);

/* access via index, 0-index based */
const std::string& first_key = skiplist[0];
const std::string& last_key = skiplist[skiplist.size()-1];
```

Get rank of a key
```C++
/* get the rank(0-index based) of a key */
const size_t rank = skiplist.GetRankofElement("key");
```

Get keys by range
```C++
/* get keys between [0, 4] */
const std::vector<std::string>& keys = skiplist.GetElementsByRange(0, 4);
/* get the second last and the last key */
const std::vector<std::string>& last_keys = skiplist.GetElementsByRange(-2, -1);
```

Get keys by reverse range
```C++
/* get keys reversely between [0, 4]  */
const std::vector<std::string>& keys = skiplist.GetElementsByRevRange(0, 4);
/* get reversely the second last and the last key */
const std::vector<std::string>& last_keys = skiplist.GetElementsByRevRange(-2, -1);
```

Get keys greater than a value
```C++
/* return all keys greater than "key_to_compare" */
const std::vector<std::string>& keys = skiplist.GetElementsGt("key_to_compare");
```

Get keys greater than or equal to a value
```C++
/* return all keys greater than or equal to "key_to_compare" */
const std::vector<std::string>& keys = skiplist.GetElementsGte("key_to_compare");
```

Get keys less than a value
```C++
/* return all keys less than "key_to_compare" */
const std::vector<std::string>& keys = skiplist.GetElementsLt("key_to_compare");
```

Get keys less than or equal to a value
```C++
/* return all keys less than or equal to "key_to_compare" */
const std::vector<std::string>& keys = skiplist.GetElementsLte("key_to_compare");
```

Get keys within a range
```C++
/* return all keys within the range [key_start, key_end) */
const std::vector<std::string>& keys = skiplist.GetElementsInRange("key_start", "key_end");
```

Delete a key.
```C++
/* return true if success */
skiplist.Delete("key1");
```

Update a key.
```C++
/* return true if success */
skiplist.Update("key2", "key5");
```

Iterate over the skiplist.
```C++
for (auto it = skiplist.Begin(); it != skiplist.End(); ++it) {
  /* do something */
}
```

Print the skiplist.
```C++
skiplist.Print();
```

## Running Unit Tests
```sh
cd build && ./skiplist_tests
```

## Benchmarks
### Running Benchmark
```sh
cd build && ./skiplist_benchmark
```
