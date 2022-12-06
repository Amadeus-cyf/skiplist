#pragma once

#include <vector>

namespace skiplist {

template <typename Key, typename Comparator>
class Skiplist {
 public:
  static constexpr const int MaxSkiplistLevel = 16;
  static constexpr const double SkiplistP = 0.5;
  Skiplist(int level, Comparator compare);
  void insert(const Key& key);
  bool contain(const Key& key);
  bool del(const Key& key);
  bool update(const Key& key, const Key& update_key);
  int size() { return len; }

 private:
  struct SkiplistLevel;
  struct SkiplistNode;
  int getRandomLevel();
  bool lt(const Key& k1, const Key& k2);
  SkiplistNode* head;
  Comparator compare;
  int level;
  int len;
};

template <typename Key, typename Comparator>
struct Skiplist<Key, Comparator>::SkiplistLevel {
  SkiplistLevel(SkiplistNode* next, int span) : next(next), span(0){};
  SkiplistNode* next;
  int span;
};

template <typename Key, typename Comparator>
struct Skiplist<Key, Comparator>::SkiplistNode {
 public:
  static SkiplistNode* createSkiplistNode(const Key& key, int level) {
    SkiplistNode* n = new SkiplistNode(key);
    for (int i = 0; i < level; ++i) {
      n->levels[i] = new SkiplistLevel(nullptr, 0);
    }
    return n;
  }

  static SkiplistNode* createSkiplistNode(int level) {
    SkiplistNode* n = new SkiplistNode();
    for (int i = 0; i < level; ++i) {
      n->levels[i] = new SkiplistLevel(nullptr, 0);
    }
    return n;
  }

  Key key;
  SkiplistLevel* levels[MaxSkiplistLevel];
  SkiplistNode* prev;

 private:
  SkiplistNode() : prev(nullptr){};
  SkiplistNode(const Key& key) : key(key), prev(nullptr) {
    memset(levels, 0, sizeof levels);
  };
};

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::Skiplist(int level, Comparator compare)
    : level(level),
      head(SkiplistNode::createSkiplistNode(level)),
      compare(compare),
      len(0){};

template <typename Key, typename Comparator>
int Skiplist<Key, Comparator>::getRandomLevel() {
  int level = 1;
  while (((double)rand() / RAND_MAX) < SkiplistP && level < MaxSkiplistLevel) {
    ++level;
  }
  return level < MaxSkiplistLevel ? level : MaxSkiplistLevel;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::lt(const Key& k1, const Key& k2) {
  return compare(k1, k2) < 0;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::insert(const Key& key) {
  int insert_level = getRandomLevel();

  const SkiplistNode* insert_after[insert_level];
  int rank[insert_level];

  const SkiplistNode* n = head;
  for (int i = level - 1; i >= 0; --i) {
    rank[i] = i == level - 1 ? 0 : rank[i + 1];
    const SkiplistNode* next = n->levels[i]->next;
    while (next && lt(key, next->key)) {
      rank[i] += n->levels[i]->span;
      n = next;
    }
    insert_after[i] = n;
  }

  for (int i = level; i < insert_level; ++i) {
    head->levels[i] = new SkiplistLevel(nullptr, 0);
    insert_after[i] = head;
  }

  if (level < insert_level) level = insert_level;

  SkiplistNode* node = SkiplistNode::createSkiplistNode(key, level);

  for (int i = 0; i < insert_level; ++i) {
    int span = insert_after[i]->levels[i]->span;
    node->levels[i]->next = insert_after[i]->levels[i]->next;
    node->levels[i]->span = span - rank[0] + rank[1];
    insert_after[i]->levels[i]->next = node;
    insert_after[i]->levels[i]->span = rank[0] - rank[i] + 1;
  }

  node->prev = const_cast<SkiplistNode*>(insert_after[0]);

  if (node->levels[0]->next) {
    node->levels[0]->next->prev = node;
  }
  ++len;
}

}  // namespace skiplist
