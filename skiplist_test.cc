#include "skiplist.h"

#include <gtest/gtest.h>

#include <string>

namespace skiplist {
class SkiplistTest : public testing::Test {
 protected:
  static void SetUpTestSuite() { skiplist = new Skiplist<std::string>(4); }
  static void TearDownTestSuite() {
    delete skiplist;
    skiplist = nullptr;
  }
  static Skiplist<std::string>* skiplist;
};

void ScanSkiplist(const Skiplist<std::string>* skiplist);

Skiplist<std::string>* SkiplistTest::skiplist;

TEST_F(SkiplistTest, Insertion) {
  ASSERT_TRUE(skiplist->Insert("key1"));
  ASSERT_TRUE(skiplist->Insert("key2"));
  ASSERT_TRUE(skiplist->Insert("key0"));
  ASSERT_FALSE(skiplist->Insert("key1"));
  ASSERT_EQ(skiplist->Size(), 3);

  ASSERT_TRUE(skiplist->Contains("key1"));
  ASSERT_TRUE(skiplist->Contains("key2"));
  ASSERT_TRUE(skiplist->Contains("key0"));
  ASSERT_TRUE(!skiplist->Contains("key_not_exist"));
}

TEST_F(SkiplistTest, Deletion) {
  ASSERT_TRUE(skiplist->Delete("key1"));
  ASSERT_FALSE(skiplist->Contains("key1"));
  ASSERT_EQ(skiplist->Size(), 2);
  ASSERT_FALSE(skiplist->Delete("key1"));
  ASSERT_EQ(skiplist->Size(), 2);

  ASSERT_TRUE(skiplist->Delete("key2"));
  ASSERT_FALSE(skiplist->Contains("key2"));
  ASSERT_EQ(skiplist->Size(), 1);
  ASSERT_FALSE(skiplist->Delete("key2"));
  ASSERT_EQ(skiplist->Size(), 1);

  ASSERT_FALSE(skiplist->Delete("key_not_exist"));
}

TEST_F(SkiplistTest, Update) {
  ASSERT_TRUE(skiplist->Insert("key1"));
  ASSERT_TRUE(skiplist->Insert("key2"));
  ASSERT_TRUE(skiplist->Insert("key3"));
  ASSERT_EQ(skiplist->Size(), 4);

  ASSERT_TRUE(skiplist->Update("key3", "key5"));
  ASSERT_EQ(skiplist->Size(), 4);
  ASSERT_FALSE(skiplist->Contains("key3"));
  ASSERT_TRUE(skiplist->Contains("key5"));

  ASSERT_TRUE(skiplist->Update("key1", "key4"));

  ASSERT_EQ(skiplist->Size(), 4);
  ASSERT_FALSE(skiplist->Contains("key1"));
  ASSERT_TRUE(skiplist->Contains("key4"));

  ASSERT_TRUE(!skiplist->Update("key_not_exist", "key6"));
  ASSERT_EQ(skiplist->Size(), 4);
}

TEST_F(SkiplistTest, GetElementByRank) {
  const std::string& s0 = skiplist->GetElementByRank(0);
  ASSERT_EQ(s0, "key0");

  const std::string& s1 = skiplist->GetElementByRank(1);
  ASSERT_EQ(s1, "key2");

  const std::string& s2 = skiplist->GetElementByRank(-1);
  ASSERT_EQ(s2, "key5");

  const std::string& s3 = skiplist->GetElementByRank(-2);
  ASSERT_EQ(s3, "key4");

  const std::string& s4 = skiplist->GetElementByRank(-4);
  ASSERT_EQ(s4, "key0");

  ASSERT_THROW(skiplist->GetElementByRank(skiplist->Size()), std::out_of_range);
  ASSERT_THROW(skiplist->GetElementByRank(-skiplist->Size() - 1), std::out_of_range);
  ASSERT_THROW(skiplist->GetElementByRank(INT_MAX), std::out_of_range);
  ASSERT_THROW(skiplist->GetElementByRank(INT_MIN), std::out_of_range);
}

TEST_F(SkiplistTest, GetRankofElement) {
  ssize_t r0 = skiplist->GetRankofElement("key0");
  ASSERT_EQ(r0, 0);

  ssize_t r1 = skiplist->GetRankofElement("key2");
  ASSERT_EQ(r1, 1);

  ssize_t r2 = skiplist->GetRankofElement("key5");
  ASSERT_EQ(r2, 3);

  ssize_t r3 = skiplist->GetRankofElement("key_not_exist");
  ASSERT_EQ(r3, -1);
}

TEST_F(SkiplistTest, GetElementsByRange) {
  const std::vector<std::string>& k1 = skiplist->GetElementsByRange(0, 3);
  ASSERT_EQ(k1.size(), 4);
  ASSERT_EQ(k1[0], "key0");
  ASSERT_EQ(k1[3], "key5");

  const std::vector<std::string>& k2 = skiplist->GetElementsByRange(1, 1);
  ASSERT_EQ(k2.size(), 1);
  ASSERT_EQ(k2[0], "key2");

  const std::vector<std::string>& k3 = skiplist->GetElementsByRange(3, 0);
  ASSERT_EQ(k3.size(), 0);

  const std::vector<std::string>& k4 = skiplist->GetElementsByRange(3, INT_MAX);
  ASSERT_EQ(k4.size(), 1);
  ASSERT_EQ(k4[0], "key5");

  const std::vector<std::string>& k5 = skiplist->GetElementsByRange(-1, 1);
  ASSERT_EQ(k5.size(), 0);

  const std::vector<std::string>& k6 = skiplist->GetElementsByRange(-2, 3);
  ASSERT_EQ(k6.size(), 2);
  ASSERT_EQ(k6[0], "key4");
  ASSERT_EQ(k6[1], "key5");

  const std::vector<std::string>& k7 = skiplist->GetElementsByRange(-2, INT_MAX);
  ASSERT_EQ(k7.size(), 2);
  ASSERT_EQ(k7[0], "key4");
  ASSERT_EQ(k7[1], "key5");

  const std::vector<std::string>& k8 = skiplist->GetElementsByRange(-3, -2);
  ASSERT_EQ(k8.size(), 2);
  ASSERT_EQ(k8[0], "key2");
  ASSERT_EQ(k8[1], "key4");

  const std::vector<std::string>& k9 = skiplist->GetElementsByRange(INT_MAX, 1);
  ASSERT_EQ(k9.size(), 0);

  const std::vector<std::string>& k10 = skiplist->GetElementsByRange(INT_MIN, 1);
  ASSERT_EQ(k10.size(), 0);

  const std::vector<std::string>& k11 = skiplist->GetElementsByRange(-2, -3);
  ASSERT_EQ(k11.size(), 0);
}

TEST_F(SkiplistTest, GetElementsByRevRange) {
  const std::vector<std::string>& k1 = skiplist->GetElementsByRevRange(0, 3);
  ASSERT_EQ(k1.size(), 4);
  ASSERT_EQ(k1[0], "key5");
  ASSERT_EQ(k1[1], "key4");
  ASSERT_EQ(k1[2], "key2");
  ASSERT_EQ(k1[3], "key0");

  const std::vector<std::string>& k2 = skiplist->GetElementsByRevRange(1, 1);
  ASSERT_EQ(k2.size(), 1);
  ASSERT_EQ(k2[0], "key4");

  const std::vector<std::string>& k3 = skiplist->GetElementsByRevRange(0, INT_MAX);
  ASSERT_EQ(k3.size(), 4);
  ASSERT_EQ(k3[0], "key5");
  ASSERT_EQ(k3[1], "key4");
  ASSERT_EQ(k3[2], "key2");
  ASSERT_EQ(k3[3], "key0");

  const std::vector<std::string>& k4 = skiplist->GetElementsByRevRange(-3, -1);
  ASSERT_EQ(k4.size(), 3);
  ASSERT_EQ(k4[0], "key4");
  ASSERT_EQ(k4[1], "key2");
  ASSERT_EQ(k4[2], "key0");

  const std::vector<std::string>& k5 = skiplist->GetElementsByRevRange(-3, -4);
  ASSERT_EQ(k5.size(), 0);

  const std::vector<std::string>& k6 = skiplist->GetElementsByRevRange(-4, INT_MAX);
  ASSERT_EQ(k6.size(), 4);
  ASSERT_EQ(k6[0], "key5");
  ASSERT_EQ(k6[1], "key4");
  ASSERT_EQ(k6[2], "key2");
  ASSERT_EQ(k6[3], "key0");

  const std::vector<std::string>& k7 = skiplist->GetElementsByRevRange(0, 1);
  ASSERT_EQ(k7.size(), 2);
  ASSERT_EQ(k7[0], "key5");
  ASSERT_EQ(k7[1], "key4");

  const std::vector<std::string>& k8 = skiplist->GetElementsByRevRange(INT_MAX, 1);
  ASSERT_EQ(k8.size(), 0);

  const std::vector<std::string>& k9 = skiplist->GetElementsByRevRange(INT_MIN, 1);
  ASSERT_EQ(k9.size(), 0);
}

TEST_F(SkiplistTest, GetElementsGt) {
  const std::vector<std::string>& k0 = skiplist->GetElementsGt("key0");
  ASSERT_EQ(k0.size(), 3);
  ASSERT_EQ(k0[0], "key2");

  const std::vector<std::string>& k1 = skiplist->GetElementsGt("key3");
  ASSERT_EQ(k1.size(), 2);
  ASSERT_EQ(k1[0], "key4");

  const std::vector<std::string>& k2 = skiplist->GetElementsGt("key6");
  ASSERT_EQ(k2.size(), 0);

  const std::vector<std::string>& k3 = skiplist->GetElementsGt("abc");
  ASSERT_EQ(k3.size(), 4);
  ASSERT_EQ(k3[0], "key0");
}

TEST_F(SkiplistTest, GetElementsGte) {
  const std::vector<std::string>& k0 = skiplist->GetElementsGte("key0");
  ASSERT_EQ(k0.size(), 4);
  ASSERT_EQ(k0[0], "key0");

  const std::vector<std::string>& k1 = skiplist->GetElementsGte("key3");
  ASSERT_EQ(k1.size(), 2);
  ASSERT_EQ(k1[0], "key4");

  const std::vector<std::string>& k2 = skiplist->GetElementsGte("key6");
  ASSERT_EQ(k2.size(), 0);

  const std::vector<std::string>& k3 = skiplist->GetElementsGte("key5");
  ASSERT_EQ(k3.size(), 1);
  ASSERT_EQ(k3[0], "key5");
}

TEST_F(SkiplistTest, GetElementsLt) {
  const std::vector<std::string>& k0 = skiplist->GetElementsLt("key5");
  ASSERT_EQ(k0.size(), 3);
  ASSERT_EQ(k0[0], "key0");
  ASSERT_EQ(k0[2], "key4");

  const std::vector<std::string>& k1 = skiplist->GetElementsLt("key3");
  ASSERT_EQ(k1.size(), 2);
  ASSERT_EQ(k1[0], "key0");
  ASSERT_EQ(k1[1], "key2");

  const std::vector<std::string>& k2 = skiplist->GetElementsLt("key2");
  ASSERT_EQ(k2.size(), 1);
  ASSERT_EQ(k2[0], "key0");

  const std::vector<std::string>& k3 = skiplist->GetElementsLt("key0");
  ASSERT_EQ(k3.size(), 0);

  const std::vector<std::string>& k4 = skiplist->GetElementsLt("key6");
  ASSERT_EQ(k4.size(), 4);
  ASSERT_EQ(k4[0], "key0");
  ASSERT_EQ(k4[3], "key5");
}

TEST_F(SkiplistTest, GetElementsLte) {
  const std::vector<std::string>& k0 = skiplist->GetElementsLte("key5");
  ASSERT_EQ(k0.size(), 4);
  ASSERT_EQ(k0[0], "key0");
  ASSERT_EQ(k0[3], "key5");

  const std::vector<std::string>& k1 = skiplist->GetElementsLte("key3");
  ASSERT_EQ(k1.size(), 2);
  ASSERT_EQ(k1[0], "key0");
  ASSERT_EQ(k1[1], "key2");

  const std::vector<std::string>& k2 = skiplist->GetElementsLte("key2");
  ASSERT_EQ(k2.size(), 2);
  ASSERT_EQ(k2[0], "key0");
  ASSERT_EQ(k2[1], "key2");

  const std::vector<std::string>& k3 = skiplist->GetElementsLte("key0");
  ASSERT_EQ(k3.size(), 1);
  ASSERT_EQ(k3[0], "key0");

  const std::vector<std::string>& k4 = skiplist->GetElementsLte("abc");
  ASSERT_EQ(k4.size(), 0);
}

TEST_F(SkiplistTest, GetElementsInRange) {
  const std::vector<std::string>& k0 = skiplist->GetElementsInRange("key0", "key6");
  ASSERT_EQ(k0.size(), 4);
  ASSERT_EQ(k0[0], "key0");
  ASSERT_EQ(k0[3], "key5");

  const std::vector<std::string>& k1 = skiplist->GetElementsInRange("key0", "key5");
  ASSERT_EQ(k1.size(), 3);
  ASSERT_EQ(k1[0], "key0");
  ASSERT_EQ(k1[2], "key4");

  const std::vector<std::string>& k2 = skiplist->GetElementsInRange("key0", "key3");
  ASSERT_EQ(k2.size(), 2);
  ASSERT_EQ(k2[0], "key0");
  ASSERT_EQ(k2[1], "key2");

  const std::vector<std::string>& k3 = skiplist->GetElementsInRange("key0", "key2");
  ASSERT_EQ(k3.size(), 1);
  ASSERT_EQ(k3[0], "key0");

  const std::vector<std::string>& k4 = skiplist->GetElementsInRange("key0", "key1");
  ASSERT_EQ(k4.size(), 1);
  ASSERT_EQ(k4[0], "key0");

  const std::vector<std::string>& k5 = skiplist->GetElementsInRange("key0", "key0");
  ASSERT_EQ(k5.size(), 0);

  const std::vector<std::string>& k6 = skiplist->GetElementsInRange("abc", "xyz");
  ASSERT_EQ(k6.size(), 4);
  ASSERT_EQ(k6[0], "key0");
  ASSERT_EQ(k6[3], "key5");

  const std::vector<std::string>& k7 = skiplist->GetElementsInRange("abc", "aed");
  ASSERT_EQ(k7.size(), 0);

  const std::vector<std::string>& k8 = skiplist->GetElementsInRange("wxy", "xyz");
  ASSERT_EQ(k8.size(), 0);

  const std::vector<std::string>& k9 = skiplist->GetElementsInRange("key5", "key0");
  ASSERT_EQ(k9.size(), 0);
}

TEST_F(SkiplistTest, ArrayAccess) {
  ASSERT_EQ((*skiplist)[0], "key0");
  ASSERT_EQ((*skiplist)[1], "key2");
  ASSERT_EQ((*skiplist)[2], "key4");
  ASSERT_EQ((*skiplist)[3], "key5");
  ASSERT_EQ((*skiplist)[skiplist->Size() - 1], "key5");

  ASSERT_THROW((*skiplist)[skiplist->Size()], std::out_of_range);
  ASSERT_THROW((*skiplist)[-1], std::out_of_range);
  ASSERT_THROW((*skiplist)[INT_MAX], std::out_of_range);
  ASSERT_THROW((*skiplist)[INT_MIN], std::out_of_range);
}

TEST_F(SkiplistTest, Iteration) {
  typename Skiplist<std::string>::Iterator it(skiplist);
  it.SeekToLast();
  ASSERT_EQ(*it, "key5");

  --it;
  ASSERT_EQ(*it, "key4");

  ++it;
  ASSERT_EQ(*it, "key5");

  it.SeekToFirst();
  ASSERT_EQ(*it, "key0");

  ++it;
  ASSERT_EQ(*it, "key2");

  ScanSkiplist(skiplist);
}

void ScanSkiplist(const Skiplist<std::string>* skiplist) {
  printf("----start scanning skiplist----\n");
  for (auto it = skiplist->Begin(); it != skiplist->End(); ++it) {
    printf("%s\n", (*it).c_str());
  }
  printf("----end----\n");

  skiplist->Print();
}

struct Comparator {
  int operator()(const std::string& k1, const std::string& k2) const {
    return k1 < k2 ? 1 : (k1 == k2 ? 0 : -1);
  }
};

class CustomSkiplistTest : public testing::Test {
 protected:
  static void SetUpTestSuite() {
    Comparator cmp;
    skiplist = new Skiplist<std::string, Comparator>(4, cmp);
  }
  static void TearDownTestSuite() {
    delete skiplist;
    skiplist = nullptr;
  }
  static Skiplist<std::string, Comparator>* skiplist;
};

Skiplist<std::string, Comparator>* CustomSkiplistTest::skiplist;

TEST_F(CustomSkiplistTest, Insertion) {
  ASSERT_TRUE(skiplist->Insert("key1"));
  ASSERT_TRUE(skiplist->Insert("key2"));
  ASSERT_TRUE(skiplist->Insert("key0"));
  ASSERT_FALSE(skiplist->Insert("key1"));
  ASSERT_EQ(skiplist->Size(), 3);

  ASSERT_TRUE(skiplist->Contains("key1"));
  ASSERT_TRUE(skiplist->Contains("key2"));
  ASSERT_TRUE(skiplist->Contains("key0"));
  ASSERT_TRUE(!skiplist->Contains("key_not_exist"));
}

TEST_F(CustomSkiplistTest, ArrayAccess) {
  ASSERT_EQ((*skiplist)[0], "key2");
  ASSERT_EQ((*skiplist)[1], "key1");
  ASSERT_EQ((*skiplist)[2], "key0");
  ASSERT_EQ((*skiplist)[skiplist->Size() - 1], "key0");

  ASSERT_THROW((*skiplist)[skiplist->Size()], std::out_of_range);
  ASSERT_THROW((*skiplist)[-1], std::out_of_range);
  ASSERT_THROW((*skiplist)[INT_MAX], std::out_of_range);
  ASSERT_THROW((*skiplist)[INT_MIN], std::out_of_range);
}

TEST_F(CustomSkiplistTest, GetElementsInRange) {
  const std::vector<std::string>& ks = skiplist->GetElementsInRange("key2", "key0");
  ASSERT_EQ(ks.size(), 2);
  ASSERT_EQ(ks[0], "key2");
  ASSERT_EQ(ks[1], "key1");
}
}  // namespace skiplist
