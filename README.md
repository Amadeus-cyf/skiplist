# skiplist
Implement redis' skiplist in C++. Refer to leveldb's skiplist and also [the essay](https://15721.courses.cs.cmu.edu/spring2018/papers/08-oltpindexes1/pugh-skiplists-cacm1990.pdf).

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
Skiplist<string, decltype(compare)> skiplist(4);
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

## Format Code
```sh
clang-format -i --style=file src/skiplist.h && clang-format -i --style=file src/skiplist_test.cc
```
