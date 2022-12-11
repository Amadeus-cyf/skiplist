#pragma once

#include <vector>

namespace skiplist {

template <typename Key, typename Comparator>
class Skiplist {
 public:
  static constexpr const int MaxSkiplistLevel = 16;
  static constexpr const double SkiplistP = 0.5;
  explicit Skiplist(int level, Comparator compare);
  void insert(const Key& key);
  bool contain(const Key& key);
  bool del(const Key& key);
  bool update(const Key& key, const Key& update_key);
  void reset();
  int size() { return len; }
  ~Skiplist();

 private:
  struct SkiplistLevel;
  struct SkiplistNode;
  int getRandomLevel();
  bool lt(const Key& k1, const Key& k2);
  bool eq(const Key& k1, const Key& k2);
  bool deleteNode(const Key& key, const SkiplistNode* prev[MaxSkiplistLevel]);
  SkiplistNode* head;
  Comparator compare;
  int level;
  int len;
};

/* SkiplistLevel */
template <typename Key, typename Comparator>
struct Skiplist<Key, Comparator>::SkiplistLevel {
  explicit SkiplistLevel(SkiplistNode* next, int span) : next(next), span(0){};
  ~SkiplistLevel() { next = nullptr; }
  SkiplistNode* next;
  int span;
};

/* SkiplistNode */
template <typename Key, typename Comparator>
struct Skiplist<Key, Comparator>::SkiplistNode {
 public:
  static SkiplistNode* createSkiplistNode(const Key& key, int level);
  static SkiplistNode* createSkiplistNode(int level);
  void reset();
  ~SkiplistNode();

  Key key;
  SkiplistLevel* levels[MaxSkiplistLevel];
  SkiplistNode* prev;

 private:
  SkiplistNode() : prev(nullptr){};
  explicit SkiplistNode(const Key& key) : key(key), prev(nullptr) {
    memset(levels, 0, sizeof levels);
  };
};

template <typename Key, typename Comparator>
typename Skiplist<Key, Comparator>::SkiplistNode*
Skiplist<Key, Comparator>::SkiplistNode::createSkiplistNode(const Key& key, int level) {
  SkiplistNode* n = new SkiplistNode(key);
  for (int i = 0; i < level; ++i) {
    n->levels[i] = new SkiplistLevel(nullptr, 0);
  }
  return n;
}

template <typename Key, typename Comparator>
typename Skiplist<Key, Comparator>::SkiplistNode*
Skiplist<Key, Comparator>::SkiplistNode::createSkiplistNode(int level) {
  SkiplistNode* n = new SkiplistNode();
  for (int i = 0; i < level; ++i) {
    n->levels[i] = new SkiplistLevel(nullptr, 0);
  }
  return n;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::SkiplistNode::reset() {
  for (int i = 0; i < MaxSkiplistLevel; ++i) {
    delete levels[i];
    levels[i] = nullptr;
  }
  prev = nullptr;
}

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::SkiplistNode::~SkiplistNode() {
  reset();
}

/* Skiplist */
template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::Skiplist(int level, Comparator compare)
    : level(level), head(SkiplistNode::createSkiplistNode(level)), compare(compare), len(0){};

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
bool Skiplist<Key, Comparator>::eq(const Key& k1, const Key& k2) {
  return compare(k1, k2) == 0;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::insert(const Key& key) {
  int insert_level = getRandomLevel();

  const SkiplistNode* update[insert_level];
  int rank[insert_level];

  /*
   * get the last key less than current key in each level
   */
  const SkiplistNode* n = head;
  for (int i = level - 1; i >= 0; --i) {
    rank[i] = i == level - 1 ? 0 : rank[i + 1];
    while (n->levels[i]->next && lt(n->levels[i]->next->key, key)) {
      rank[i] += n->levels[i]->span;
      n = n->levels[i]->next;
    }
    update[i] = n;
  }

  /*
   * If random level is larger than level
   * init empty skiplist level for extra levels and update level to the random level.
   */
  for (int i = level; i < insert_level; ++i) {
    head->levels[i] = new SkiplistLevel(nullptr, 0);
    update[i] = head;
  }

  if (level < insert_level) level = insert_level;

  SkiplistNode* node = SkiplistNode::createSkiplistNode(key, level);

  /*
   * insert the key into the skiplist in each level
   */
  for (int i = 0; i < insert_level; ++i) {
    int span = update[i]->levels[i]->span;
    node->levels[i]->next = update[i]->levels[i]->next;
    node->levels[i]->span = span - rank[0] + rank[i];
    update[i]->levels[i]->next = node;
    update[i]->levels[i]->span = rank[0] - rank[i] + 1;
  }

  node->prev = const_cast<SkiplistNode*>(update[0]);

  /* for level 0, update backward pointer since it's a double linked list */
  if (node->levels[0]->next) {
    node->levels[0]->next->prev = node;
  }
  ++len;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::contain(const Key& key) {
  const SkiplistNode* n = head;

  for (int i = level - 1; i >= 0; --i) {
    while (n->levels[i]->next && lt(n->levels[i]->next->key, key)) {
      n = n->levels[i]->next;
    }

    const SkiplistNode* next = n->levels[i]->next;
    if (next && eq(next->key, key)) {
      return true;
    }
  }

  return false;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::del(const Key& key) {
  const SkiplistNode* n = head;
  const SkiplistNode* update[MaxSkiplistLevel];
  memset(update, 0, sizeof update);

  for (int i = level - 1; i >= 0; --i) {
    while (n->levels[i]->next && lt(n->levels[i]->next->key, key)) {
      n = n->levels[i]->next;
    }
    update[i] = n;
  }

  bool r = deleteNode(key, update);
  if (r) --len;

  return r;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::deleteNode(const Key& key,
                                           const SkiplistNode* update[MaxSkiplistLevel]) {
  bool deleted = false;
  for (int i = level - 1; i >= 0; --i) {
    if (update[i] && update[i]->levels[i]->next && eq(update[i]->levels[i]->next->key, key)) {
      const SkiplistNode* node = update[i]->levels[i]->next;
      update[i]->levels[i]->next = node ? node->levels[i]->next : nullptr;
      update[i]->levels[i]->span += node ? (node->levels[i]->span - 1) : 0;
      deleted = true;
    }
  }

  /* for level 0, update backward pointer since it's a double linked list */
  if (deleted && update[0]->levels[0]->next) {
    update[0]->levels[0]->next->prev = const_cast<SkiplistNode*>(update[0]);
  }

  return deleted;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::reset() {
  SkiplistNode* node = head->levels[0]->next;
  while (node) {
    SkiplistNode* next = node->levels[0]->next;
    node->prev = nullptr;
    delete node;
    node = next;
  }

  head->reset();
  len = 0;
}

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::~Skiplist() {
  reset();
}

}  // namespace skiplist
