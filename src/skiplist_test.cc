#include "skiplist.h"

#include <string>

using skiplist::Skiplist;
using std::string;

int main() {
  auto compare = [](const string& k1, const string& k2) {
    return k1 < k2 ? -1 : (k1 == k2 ? 0 : 1);
  };

  Skiplist<string, decltype(compare)> skiplist(4, compare);
  skiplist.insert("key1");
  skiplist.insert("key2");
  skiplist.insert("key0");
  assert(skiplist.size() == 3);

  assert(skiplist.contain("key1"));
  assert(skiplist.contain("key2"));
  assert(skiplist.contain("key0"));
  assert(!skiplist.contain("key_not_exist"));
}
