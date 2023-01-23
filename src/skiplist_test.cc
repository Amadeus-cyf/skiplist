#include "skiplist.h"

#include <gtest/gtest.h>

#include <string>

using std::string;

namespace skiplist {

void scanSkiplist(const Skiplist<string>& skiplist);

Skiplist<string> skiplist(4);

TEST(SkiplistTest, Insertion) {
  ASSERT_TRUE(skiplist.insert("key1"));
  ASSERT_TRUE(skiplist.insert("key2"));
  ASSERT_TRUE(skiplist.insert("key0"));
  ASSERT_FALSE(skiplist.insert("key1"));
  ASSERT_EQ(skiplist.size(), 3);

  ASSERT_TRUE(skiplist.contains("key1"));
  ASSERT_TRUE(skiplist.contains("key2"));
  ASSERT_TRUE(skiplist.contains("key0"));
  ASSERT_TRUE(!skiplist.contains("key_not_exist"));
}

TEST(SkiplistTest, Deletion) {
  ASSERT_TRUE(skiplist.del("key1"));
  ASSERT_FALSE(skiplist.contains("key1"));
  ASSERT_EQ(skiplist.size(), 2);
  ASSERT_FALSE(skiplist.del("key1"));
  ASSERT_EQ(skiplist.size(), 2);

  ASSERT_TRUE(skiplist.del("key2"));
  ASSERT_FALSE(skiplist.contains("key2"));
  ASSERT_EQ(skiplist.size(), 1);
  ASSERT_FALSE(skiplist.del("key2"));
  ASSERT_EQ(skiplist.size(), 1);

  ASSERT_FALSE(skiplist.del("key_not_exist"));
}

TEST(SkiplistTest, Update) {
  ASSERT_TRUE(skiplist.insert("key1"));
  ASSERT_TRUE(skiplist.insert("key2"));
  ASSERT_TRUE(skiplist.insert("key3"));
  ASSERT_EQ(skiplist.size(), 4);

  ASSERT_TRUE(skiplist.update("key3", "key5"));
  ASSERT_EQ(skiplist.size(), 4);
  ASSERT_FALSE(skiplist.contains("key3"));
  ASSERT_TRUE(skiplist.contains("key5"));

  ASSERT_TRUE(skiplist.update("key1", "key4"));

  ASSERT_EQ(skiplist.size(), 4);
  ASSERT_FALSE(skiplist.contains("key1"));
  ASSERT_TRUE(skiplist.contains("key4"));

  ASSERT_TRUE(!skiplist.update("key_not_exist", "key6"));
  ASSERT_EQ(skiplist.size(), 4);
}

TEST(SkiplistTest, GetElementByRank) {
  const string& s0 = skiplist.getElementByRank(0);
  ASSERT_EQ(s0, "key0");

  const string& s1 = skiplist.getElementByRank(1);
  ASSERT_EQ(s1, "key2");

  const string& s2 = skiplist.getElementByRank(-1);
  ASSERT_EQ(s2, "key5");

  const string& s3 = skiplist.getElementByRank(-2);
  ASSERT_EQ(s3, "key4");

  const string& s4 = skiplist.getElementByRank(-4);
  ASSERT_EQ(s4, "key0");

  ASSERT_THROW(skiplist.getElementByRank(skiplist.size()), std::out_of_range);
  ASSERT_THROW(skiplist.getElementByRank(-skiplist.size() - 1), std::out_of_range);
  ASSERT_THROW(skiplist.getElementByRank(INT_MAX), std::out_of_range);
  ASSERT_THROW(skiplist.getElementByRank(INT_MIN), std::out_of_range);
}

TEST(SkiplistTest, Iteration) {
  typename Skiplist<string>::Iterator it(&skiplist);
  it.seekToLast();
  ASSERT_EQ(*it, "key5");

  --it;
  ASSERT_EQ(*it, "key4");

  ++it;
  ASSERT_EQ(*it, "key5");

  it.seekToFirst();
  ASSERT_EQ(*it, "key0");

  ++it;
  ASSERT_EQ(*it, "key2");

  scanSkiplist(skiplist);
}

void scanSkiplist(const Skiplist<string>& skiplist) {
  printf("----start scanning skiplist----\n");
  for (auto it = skiplist.begin(); it != skiplist.end(); ++it) {
    printf("%s\n", (*it).c_str());
  }
  printf("----end----\n");

  skiplist.print();
}

}  // namespace skiplist
