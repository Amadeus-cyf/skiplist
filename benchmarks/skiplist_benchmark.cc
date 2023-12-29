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
    skiplist.Insert(key);
  }
}

static void Search(benchmark::State& state) {
  for (auto _ : state) {
    int exist = rand() % 2;

    if (exist) {
      /* search for an existing key */
      const std::string& key = keys[rand() % keys.size()];
      skiplist.Contains(key);
    } else {
      /* search for a non-existing key */
      skiplist.Contains(randString(10));
    }
  }
}

static void Update(benchmark::State& state) {
  for (auto _ : state) {
    bool exist = rand() % 2 == 1;

    if (exist) {
      /* update an existing key */
      const std::string& key = keys[rand() % keys.size()];
      skiplist.Update(key, randString(10));
    } else {
      /* update a non-existing key */
      skiplist.Update(randString(10), randString(10));
    }
  }
}

static void Delete(benchmark::State& state) {
  for (auto _ : state) {
    bool exist = rand() % 2 == 1;

    if (exist) {
      /* delete an existing key */
      const std::string& key = keys[rand() % keys.size()];
      skiplist.Delete(key);
    } else {
      /* delete a non-existing key */
      skiplist.Delete(randString(10));
    }
  }
}

static void GetElementByRank(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.GetElementByRank(rand() % skiplist.Size());
  }
}

static void GetRankOfElement(benchmark::State& state) {
  for (auto _ : state) {
    bool exist = rand() % 2 == 1;

    if (exist) {
      skiplist.GetRankofElement(keys[rand() % keys.size()]);
    } else {
      skiplist.GetRankofElement(randString(10));
    }
  }
}

static void GetElementsByRange(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.GetElementsByRange(rand() % keys.size(), rand() % (keys.size() + 1));
  }
}

static void GetElementsByRevRange(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.GetElementsByRevRange(rand() % keys.size(), rand() % (keys.size() + 1));
  }
}

static void GetElementsGt(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.GetElementsGt(randString(10));
  }
}

static void GetElementsLt(benchmark::State& state) {
  for (auto _ : state) {
    skiplist.GetElementsLt(randString(10));
  }
}

static void GetElementsInRange(benchmark::State& state) {
  for (auto _ : state) {
    const std::string &s1 = randString(10), &s2 = randString(10);
    skiplist.GetElementsInRange(s1 < s2 ? s1 : s2, s1 < s2 ? s2 : s1);
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
