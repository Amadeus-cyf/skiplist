#include <benchmark/benchmark.h>

#include "skiplist.h"

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
    bool exist = rand() % 2 == 1;

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
    bool exist = rand() % 2 == 1;

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

static void GetElementByRank(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.getElementByRank(rand() % skiplist.size());
  }
}

static void GetRankOfElement(benchmark::State& state) {
  for (auto _ : state) {
    bool exist = rand() % 2 == 1;

    if (exist) {
      skiplist.getRankofElement(keys[rand() % keys.size()]);
    } else {
      skiplist.getRankofElement(randString(10));
    }
  }
}

static void GetElementsByRange(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.getElementsByRange(rand() % keys.size(), rand() % (keys.size() + 1));
  }
}

static void GetElementsByRevRange(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.getElementsByRevRange(rand() % keys.size(), rand() % (keys.size() + 1));
  }
}

static void GetElementsGt(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.getElementsGt(randString(10));
  }
}

static void GetElementsLt(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.getElementsLt(randString(10));
  }
}

static void GetElementsInRange(benchmark::State& state) {
  for (auto _ : state) {
    const std::string &s1 = randString(10), &s2 = randString(10);
    skiplist.getElementsInRange(s1 < s2 ? s1 : s2, s1 < s2 ? s2 : s1);
  }
}

BENCHMARK(Insert);
BENCHMARK(Search);
BENCHMARK(Update);
BENCHMARK(Delete);
BENCHMARK(GetElementByRank);
BENCHMARK(GetRankOfElement);
BENCHMARK(GetElementsByRange);
BENCHMARK(GetElementsByRevRange);
BENCHMARK(GetElementsGt);
BENCHMARK(GetElementsLt);
BENCHMARK(GetElementsInRange);

}  // namespace skiplist

BENCHMARK_MAIN();
