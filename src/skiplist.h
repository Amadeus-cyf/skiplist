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
  bool contains(const Key& key);
  bool del(const Key& key);
  bool update(const Key& key, const Key& new_key);
  void reset();
  int size() { return len; }
  ~Skiplist();

 private:
  struct SkiplistLevel;
  struct SkiplistNode;
  int getRandomLevel();
  bool lt(const Key& k1, const Key& k2);
  bool lte(const Key& k1, const Key& k2);
  bool gt(const Key& k1, const Key& k2);
  bool gte(const Key& k1, const Key& k2);
  bool eq(const Key& k1, const Key& k2);
  bool deleteNode(const Key& key, SkiplistNode* prev[MaxSkiplistLevel]);
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
  const SkiplistNode* getNext(int level) const { return levels[level]->next; };
  SkiplistNode* getNext(int level) { return levels[level]->next; };
  void setNext(int level, const SkiplistNode* next) {
    levels[level]->next = const_cast<SkiplistNode*>(next);
  };
  int getSpan(int level) const { return levels[level]->span; };
  int getSpan(int level) { return levels[level]->span; };
  void setSpan(int level, int span) { levels[level]->span = span; };
  const SkiplistNode* getPrev() const { return prev; };
  SkiplistNode* getPrev() { return prev; };
  void setPrev(const SkiplistNode* prev_node) {
    prev = const_cast<SkiplistNode*>(prev_node);
  };
  void initLevel(int level) { levels[level] = new SkiplistLevel(nullptr, 0); };
  void reset();
  ~SkiplistNode();
  Key key;

 private:
  SkiplistNode() : prev(nullptr){};
  explicit SkiplistNode(const Key& key) : key(key), prev(nullptr) {
    memset(levels, 0, sizeof levels);
  };
  SkiplistLevel* levels[MaxSkiplistLevel];
  SkiplistNode* prev;
};

template <typename Key, typename Comparator>
typename Skiplist<Key, Comparator>::SkiplistNode*
Skiplist<Key, Comparator>::SkiplistNode::createSkiplistNode(const Key& key,
                                                            int level) {
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
bool Skiplist<Key, Comparator>::lte(const Key& k1, const Key& k2) {
  return lt(k1, k2) || eq(k1, k2);
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::gt(const Key& k1, const Key& k2) {
  return compare(k1, k2) > 0;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::gte(const Key& k1, const Key& k2) {
  return gt(k1, k2) || eq(k1, k2);
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
    while (n->getNext(i) && lt(n->getNext(i)->key, key)) {
      rank[i] += n->getSpan(i);
      n = n->getNext(i);
    }
    update[i] = n;
  }

  /*
   * If random level is larger than level
   * init empty skiplist level for extra levels and update level to the random
   * level.
   */
  for (int i = level; i < insert_level; ++i) {
    head->initLevel(i);
    update[i] = head;
  }

  if (level < insert_level) level = insert_level;

  SkiplistNode* node = SkiplistNode::createSkiplistNode(key, level);

  /*
   * insert the key into the skiplist in each level
   */
  for (int i = 0; i < insert_level; ++i) {
    int span = update[i]->getSpan(i);
    node->setNext(i, update[i]->getNext(i));
    node->setSpan(i, span - rank[0] + rank[i]);
    SkiplistNode* n = const_cast<SkiplistNode*>(update[i]);
    n->setNext(i, node);
    n->setSpan(i, rank[0] - rank[i] + 1);
  }

  node->setPrev(update[0]);

  /* for level 0, update backward pointer since it's a double linked list */
  if (node->getNext(0)) {
    node->getNext(0)->setPrev(node);
  }
  ++len;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::contains(const Key& key) {
  const SkiplistNode* n = head;

  for (int i = level - 1; i >= 0; --i) {
    while (n->getNext(i) && lt(n->getNext(i)->key, key)) {
      n = n->getNext(i);
    }

    const SkiplistNode* next = n->getNext(i);
    if (next && eq(next->key, key)) {
      return true;
    }
  }

  return false;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::del(const Key& key) {
  SkiplistNode* n = head;
  SkiplistNode* update[MaxSkiplistLevel];
  memset(update, 0, sizeof update);

  for (int i = level - 1; i >= 0; --i) {
    while (n->getNext(i) && lt(n->getNext(i)->key, key)) {
      n = n->getNext(i);
    }
    update[i] = n;
  }

  bool r = deleteNode(key, update);

  return r;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::update(const Key& key, const Key& new_key) {
  SkiplistNode* update[MaxSkiplistLevel];
  memset(update, 0, sizeof(update));

  SkiplistNode* node = head;
  for (int i = level - 1; i >= 0; --i) {
    while (node->getNext(i) && lt(node->getNext(i)->key, key)) {
      node = node->getNext(i);
    }
    update[i] = node;
  }

  if (!update[0]->getNext(0) || !eq(update[0]->getNext(0)->key, key)) {
    /* key not found */
    return false;
  }

  /* getNext(0) must return a non null value since it's the node containing the
   * original key */
  const SkiplistNode* next = update[0]->getNext(0)->getNext(0);
  if ((update[0] == head || gte(new_key, update[0]->key)) &&
      (!next || lte(new_key, next->key))) {
    /* if in the key's position is not changed, update the key directly */
    SkiplistNode* next = update[0]->getNext(0);
    next->key = new_key;
  } else {
    /* otherwise, delete the original node and insert a new one */
    deleteNode(key, update);
    insert(new_key);
  }

  return true;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::deleteNode(
    const Key& key, SkiplistNode* update[MaxSkiplistLevel]) {
  bool deleted = false;
  SkiplistNode* node_to_delete = update[0]->getNext(0);

  for (int i = level - 1; i >= 0; --i) {
    if (update[i] && update[i]->getNext(i) &&
        eq(update[i]->getNext(i)->key, key)) {
      SkiplistNode* next = update[i]->getNext(i);
      update[i]->setNext(i, next ? next->getNext(i) : nullptr);
      update[i]->setSpan(
          i, update[i]->getSpan(i) + (next ? next->getSpan(i) - 1 : 0));
      deleted = true;
    }
  }

  /* for level 0, update backward pointer since it's a double linked list */
  if (deleted && update[0]->getNext(0)) {
    update[0]->getNext(0)->setPrev(update[0]);
  }

  if (deleted) {
    delete node_to_delete;
    --len;
  }

  return deleted;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::reset() {
  SkiplistNode* node = head->getNext(0);
  while (node) {
    SkiplistNode* next = node->getNext(0);
    node->setPrev(nullptr);
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
