#include <benchmark/benchmark.h>

#include "src/skiplist.h"

namespace skiplist {

Skiplist<std::string> skiplist(16);
std::vector<std::string> keys;

std::string randString(const int len) {
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  std::string str;
  str.reserve(len);

  for (int i = 0; i < len; ++i) {
    str += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return str;
}

static void Insert(benchmark::State& state) {
  for (auto _ : state) {
    const std::string& key = randString(10);
    keys.push_back(key);
    skiplist.insert(key);
  }
}

static void Search(benchmark::State& state) {
  for (auto _ : state) {
    int exist = rand() % 2;

    if (exist) {
      /* search for an existing key */
      const std::string& key = keys[rand() % keys.size()];
      skiplist.contains(key);
    } else {
      /* search for a non-existing key */
      skiplist.contains(randString(10));
    }
  }
}

static void Update(benchmark::State& state) {
  for (auto _ : state) {
    int exist = rand() % 2;

    if (exist) {
      /* update an existing key */
      const std::string& key = keys[rand() % keys.size()];
      skiplist.update(key, randString(10));
    } else {
      /* update a non-existing key */
      skiplist.update(randString(10), randString(10));
    }
  }
}

static void Delete(benchmark::State& state) {
  for (auto _ : state) {
    int exist = rand() % 2;

    if (exist) {
      /* delete an existing key */
      const std::string& key = keys[rand() % keys.size()];
      skiplist.del(key);
    } else {
      /* delete a non-existing key */
      skiplist.del(randString(10));
    }
  }
}

BENCHMARK(Insert);
BENCHMARK(Search);
BENCHMARK(Update);
BENCHMARK(Delete);

}  // namespace skiplist

BENCHMARK_MAIN();
