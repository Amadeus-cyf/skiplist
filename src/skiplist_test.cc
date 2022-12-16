#include "skiplist.h"

#include <string>

using skiplist::Skiplist;
using std::string;

int main() {
  Skiplist<string> skiplist(4);
  skiplist.insert("key1");
  skiplist.insert("key2");
  skiplist.insert("key0");
  assert(skiplist.size() == 3);

  assert(skiplist.contains("key1"));
  assert(skiplist.contains("key2"));
  assert(skiplist.contains("key0"));
  assert(!skiplist.contains("key_not_exist"));

  assert(skiplist.del("key1"));
  assert(!skiplist.contains("key1"));
  assert(skiplist.size() == 2);
  assert(!skiplist.del("key1"));
  assert(skiplist.size() == 2);

  assert(skiplist.del("key2"));
  assert(!skiplist.contains("key2"));
  assert(skiplist.size() == 1);
  assert(!skiplist.del("key2"));
  assert(skiplist.size() == 1);

  assert(!skiplist.del("key_not_exist"));

  skiplist.insert("key1");
  skiplist.insert("key2");
  skiplist.insert("key3");
  assert(skiplist.size() == 4);

  assert(skiplist.update("key3", "key5"));
  assert(skiplist.size() == 4);
  assert(!skiplist.contains("key3"));
  assert(skiplist.contains("key5"));

  assert(skiplist.update("key1", "key4"));
  assert(skiplist.size() == 4);
  assert(!skiplist.contains("key1"));
  assert(skiplist.contains("key4"));

  assert(!skiplist.update("key_not_exist", "key6"));
  assert(skiplist.size() == 4);
}
