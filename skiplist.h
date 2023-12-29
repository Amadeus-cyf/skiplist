#pragma once

#include <iostream>

namespace skiplist {

template <typename Key>
const auto default_compare =
    [](const Key& k1, const Key& k2) { return k1 < k2 ? -1 : (k1 == k2 ? 0 : 1); };

template <typename Key, typename Comparator = decltype(default_compare<Key>)>
class Skiplist {
 private:
  struct SkiplistLevel;
  struct SkiplistNode;

 public:
  class Iterator;
  Skiplist();
  explicit Skiplist(const size_t level);
  explicit Skiplist(const size_t level, const Comparator& compare_);
  Iterator Begin() const;
  Iterator End() const;
  bool Insert(const Key& key);
  bool Contains(const Key& key);
  bool Delete(const Key& key);
  bool Update(const Key& key, const Key& new_key);
  const Key& GetElementByRank(int rank);
  ssize_t GetRankofElement(const Key& key);
  std::vector<Key> GetElementsByRange(int start, int end);
  std::vector<Key> GetElementsByRevRange(int start, int end);
  std::vector<Key> GetElementsGt(const Key& start);
  std::vector<Key> GetElementsGte(const Key& start);
  std::vector<Key> GetElementsLt(const Key& end);
  std::vector<Key> GetElementsLte(const Key& end);
  std::vector<Key> GetElementsInRange(const Key& start, const Key& end);
  const Key& operator[](size_t i);
  size_t Size() { return size_; }
  void Clear();
  void Print() const;
  ~Skiplist();

 private:
  static constexpr const int InitSkiplistLevel = 2;
  static constexpr const int MaxSkiplistLevel = 16;
  static constexpr const double SkiplistP = 0.5;
  size_t RandomLevel();
  bool Lt(const Key& k1, const Key& k2);
  bool Lte(const Key& k1, const Key& k2);
  bool Gt(const Key& k1, const Key& k2);
  bool Gte(const Key& k1, const Key& k2);
  bool Eq(const Key& k1, const Key& k2);
  void DeleteNode(const Key& key, SkiplistNode* prev[MaxSkiplistLevel]);
  const SkiplistNode* GetElement(size_t rank);
  std::vector<Key> GetElements(size_t start, size_t end);
  std::vector<Key> GetElementsRev(size_t start, size_t end);
  std::vector<Key> GetElementsGt(const Key& start, bool Eq);
  std::vector<Key> GetElementsLt(const Key& end, bool Eq);
  const SkiplistNode* GetFirstElementGt(const Key& key, bool Eq);
  const SkiplistNode* GetLastElementLt(const Key& key, bool Eq);
  void Reset();
  const SkiplistNode* FindLast() const;
  SkiplistNode* head_;
  const Comparator compare_;
  size_t level_;
  size_t size_;
};

/* SkiplistLevel */
template <typename Key, typename Comparator>
struct Skiplist<Key, Comparator>::SkiplistLevel {
  explicit SkiplistLevel(SkiplistNode* next, size_t span) : next_(next), span_(span){};
  void Reset();
  ~SkiplistLevel() { Reset(); }
  SkiplistNode* next_;
  size_t span_;
};

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::SkiplistLevel::Reset() {
  next_ = nullptr;
  span_ = 0;
}

/* SkiplistNode */
template <typename Key, typename Comparator>
struct Skiplist<Key, Comparator>::SkiplistNode {
 public:
  static SkiplistNode* CreateSkiplistNode(const Key& key, size_t level);
  static SkiplistNode* CreateSkiplistNode(size_t level);
  const SkiplistNode* GetNext(size_t level) const { return levels_[level]->next_; };
  SkiplistNode* GetNext(size_t level) { return levels_[level]->next_; };
  void SetNext(size_t level, const SkiplistNode* next) {
    levels_[level]->next_ = const_cast<SkiplistNode*>(next);
  };
  size_t GetSpan(size_t level) const { return levels_[level]->span_; };
  size_t GetSpan(size_t level) { return levels_[level]->span_; };
  void SetSpan(size_t level, size_t span_) { levels_[level]->span_ = span_; };
  const SkiplistNode* GetPrev() const { return prev_; };
  SkiplistNode* GetPrev() { return prev_; };
  void SetPrev(const SkiplistNode* prev_node) { prev_ = const_cast<SkiplistNode*>(prev_node); };
  void InitLevel(size_t level) { levels_[level] = new SkiplistLevel(nullptr, 0); };
  void Reset();
  ~SkiplistNode();
  Key key_;

 private:
  SkiplistNode() : prev_(nullptr) { memset(levels_, 0, sizeof levels_); };
  explicit SkiplistNode(const Key& key) : key_(key), prev_(nullptr) {
    memset(levels_, 0, sizeof levels_);
  };
  SkiplistLevel* levels_[MaxSkiplistLevel];
  SkiplistNode* prev_;
};

template <typename Key, typename Comparator>
typename Skiplist<Key, Comparator>::SkiplistNode*
Skiplist<Key, Comparator>::SkiplistNode::CreateSkiplistNode(const Key& key, size_t level) {
  SkiplistNode* n = new SkiplistNode(key);
  for (int i = 0; i < level; ++i) {
    n->InitLevel(i);
  }
  return n;
}

template <typename Key, typename Comparator>
typename Skiplist<Key, Comparator>::SkiplistNode*
Skiplist<Key, Comparator>::SkiplistNode::CreateSkiplistNode(size_t level) {
  SkiplistNode* n = new SkiplistNode();
  for (int i = 0; i < level; ++i) {
    n->InitLevel(i);
  }
  return n;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::SkiplistNode::Reset() {
  for (int i = 0; i < MaxSkiplistLevel; ++i) {
    delete levels_[i];
    levels_[i] = nullptr;
  }
  prev_ = nullptr;
}

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::SkiplistNode::~SkiplistNode() {
  Reset();
}

/* Iterator */
template <typename Key, typename Comparator>
class Skiplist<Key, Comparator>::Iterator {
 public:
  explicit Iterator(const Skiplist* skiplist);
  explicit Iterator(const Skiplist* skiplist, const SkiplistNode* node);
  Iterator(const Iterator& it);
  void SeekToFirst();
  void SeekToLast();
  Iterator& operator=(const Iterator& it);
  void operator--();
  void operator++();
  bool operator==(const Iterator& it);
  bool operator!=(const Iterator& it);
  const Key& operator*();

 private:
  const SkiplistNode* node_;
  const Skiplist* skiplist_;
};

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::Iterator::Iterator(const Skiplist* skiplist)
    : skiplist_(skiplist), node_(nullptr) {}

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::Iterator::Iterator(const Skiplist* skiplist, const SkiplistNode* node)
    : skiplist_(skiplist), node_(node) {}

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::Iterator::Iterator(const Iterator& it)
    : skiplist_(it.skiplist_), node_(it.node_) {}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::Iterator::SeekToFirst() {
  node_ = skiplist_->head_->GetNext(0);
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::Iterator::SeekToLast() {
  node_ = skiplist_->FindLast();
  if (node_ == skiplist_->head_) node_ = nullptr;
}

template <typename Key, typename Comparator>
typename Skiplist<Key, Comparator>::Iterator& Skiplist<Key, Comparator>::Iterator::operator=(
    const Iterator& it) {
  skiplist_ = it.skiplist_;
  node_ = it.node__;
  return *this;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::Iterator::operator++() {
  node_ = node_->GetNext(0);
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::Iterator::operator--() {
  node_ = node_->GetPrev();
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Iterator::operator==(const Iterator& it) {
  return skiplist_ == it.skiplist_ && node_ == it.node_;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Iterator::operator!=(const Iterator& it) {
  return !((*this) == it);
}

template <typename Key, typename Comparator>
const Key& Skiplist<Key, Comparator>::Iterator::operator*() {
  return node_->key_;
}

/* Skiplist */
template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::Skiplist()
    : level_(InitSkiplistLevel),
      head_(SkiplistNode::CreateSkiplistNode(InitSkiplistLevel)),
      compare_(default_compare<Key>),
      size_(0){};

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::Skiplist(const size_t level)
    : level_(level),
      head_(SkiplistNode::CreateSkiplistNode(level)),
      compare_(default_compare<Key>),
      size_(0){};

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::Skiplist(const size_t level, const Comparator& compare_)
    : level_(level), head_(SkiplistNode::CreateSkiplistNode(level)), compare_(compare_), size_(0){};

template <typename Key, typename Comparator>
typename Skiplist<Key, Comparator>::Iterator Skiplist<Key, Comparator>::Begin() const {
  return Iterator(this, head_->GetNext(0));
}

template <typename Key, typename Comparator>
typename Skiplist<Key, Comparator>::Iterator Skiplist<Key, Comparator>::End() const {
  return Iterator(this, FindLast()->GetNext(0));
}

template <typename Key, typename Comparator>
size_t Skiplist<Key, Comparator>::RandomLevel() {
  size_t level = 1;
  while (level < MaxSkiplistLevel && ((double)rand() / RAND_MAX) < SkiplistP) {
    ++level;
  }
  return level;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Lt(const Key& k1, const Key& k2) {
  return compare_(k1, k2) < 0;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Lte(const Key& k1, const Key& k2) {
  return Lt(k1, k2) || Eq(k1, k2);
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Gt(const Key& k1, const Key& k2) {
  return compare_(k1, k2) > 0;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Gte(const Key& k1, const Key& k2) {
  return Gt(k1, k2) || Eq(k1, k2);
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Eq(const Key& k1, const Key& k2) {
  return compare_(k1, k2) == 0;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Insert(const Key& key) {
  int insert_level = RandomLevel();

  /*
   * If random level is larger than level
   * init empty skiplist level for extra levels and update level to the random
   * level.
   */
  for (int i = level_; i < insert_level; ++i) {
    head_->InitLevel(i);
    head_->SetSpan(i, size_);
  }

  if (insert_level > level_) level_ = insert_level;

  const SkiplistNode* update[level_];
  size_t rank[level_];

  /* Get the last key less than current key in each level */
  const SkiplistNode* n = head_;
  for (int i = level_ - 1; i >= 0; --i) {
    rank[i] = (i == level_ - 1) ? 0 : rank[i + 1];
    while (n->GetNext(i) && Lte(n->GetNext(i)->key_, key)) {
      rank[i] += n->GetSpan(i);
      n = n->GetNext(i);
    }
    if (Eq(n->key_, key)) return false;
    update[i] = n;
  }

  SkiplistNode* node = SkiplistNode::CreateSkiplistNode(key, level_);

  /*
   * insert the key and update span_
   */
  for (int i = 0; i < level_; ++i) {
    if (i < insert_level) {
      /* need to insert the key */
      size_t span = update[i]->GetSpan(i);
      node->SetNext(i, update[i]->GetNext(i));
      node->SetSpan(i, span - rank[0] + rank[i]);
      SkiplistNode* n = const_cast<SkiplistNode*>(update[i]);
      n->SetNext(i, node);
      n->SetSpan(i, rank[0] - rank[i] + 1);
    } else {
      /* only increase span_ by 1 */
      SkiplistNode* n = const_cast<SkiplistNode*>(update[i]);
      n->SetSpan(i, update[i]->GetSpan(i) + 1);
    }
  }

  /* for level 0, update backward pointer since it's a double linked list */
  node->SetPrev(update[0]);
  if (node->GetNext(0)) {
    node->GetNext(0)->SetPrev(node);
  }
  ++size_;
  return true;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Contains(const Key& key) {
  const SkiplistNode* n = head_;

  for (int i = level_ - 1; i >= 0; --i) {
    while (n->GetNext(i) && Lt(n->GetNext(i)->key_, key)) {
      n = n->GetNext(i);
    }

    const SkiplistNode* next = n->GetNext(i);
    if (next && Eq(next->key_, key)) {
      return true;
    }
  }

  return false;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Delete(const Key& key) {
  SkiplistNode* n = head_;
  SkiplistNode* update[MaxSkiplistLevel];
  memset(update, 0, sizeof update);

  bool exist = false;
  for (int i = level_ - 1; i >= 0; --i) {
    while (n->GetNext(i) && Lt(n->GetNext(i)->key_, key)) {
      n = n->GetNext(i);
    }
    if (n->GetNext(i) && Eq(n->GetNext(i)->key_, key)) {
      exist = true;
    }
    update[i] = n;
  }

  if (!exist) return false;

  DeleteNode(key, update);
  return true;
}

template <typename Key, typename Comparator>
bool Skiplist<Key, Comparator>::Update(const Key& key, const Key& new_key) {
  SkiplistNode* update[MaxSkiplistLevel];
  memset(update, 0, sizeof(update));

  SkiplistNode* node = head_;
  for (int i = level_ - 1; i >= 0; --i) {
    while (node->GetNext(i) && Lt(node->GetNext(i)->key_, key)) {
      node = node->GetNext(i);
    }
    update[i] = node;
  }

  if (!update[0]->GetNext(0) || !Eq(update[0]->GetNext(0)->key_, key)) {
    /* key not found */
    return false;
  }

  /* the first GetNext(0) must return a non null value since it's the node containing the
   * original key */
  const SkiplistNode* next_next = update[0]->GetNext(0)->GetNext(0);
  if ((update[0] == head_ || Gte(new_key, update[0]->key_)) &&
      (!next_next || Lte(new_key, next_next->key_))) {
    /* if in the key's position is not changed, update the key directly */
    SkiplistNode* next = update[0]->GetNext(0);
    next->key_ = new_key;
    return true;
  } else {
    /* otherwise, delete the original node and insert a new one */
    DeleteNode(key, update);
    return Insert(new_key);
  }
}

template <typename Key, typename Comparator>
const Key& Skiplist<Key, Comparator>::GetElementByRank(int rank) {
  if (rank < 0) {
    rank += size_;
  }

  if (rank < 0) {
    throw std::out_of_range("skiplist index out of bound");
  }

  const SkiplistNode* node = GetElement(rank);

  if (!node) throw std::out_of_range("skiplist index out of bound");

  return node->key_;
}

template <typename Key, typename Comparator>
ssize_t Skiplist<Key, Comparator>::GetRankofElement(const Key& key) {
  size_t rank = 0;
  const SkiplistNode* node = head_;

  for (int i = level_ - 1; i >= 0; --i) {
    while (node->GetNext(i) && Lt(node->GetNext(i)->key_, key)) {
      rank += node->GetSpan(i);
      node = node->GetNext(i);
    }
    if (node->GetNext(i) && Eq(node->GetNext(i)->key_, key)) {
      return rank + node->GetSpan(i) - 1;
    }
  }

  return -1;
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsByRange(int start, int end) {
  if (start < 0) {
    start += size_;
  }
  if (end < 0) {
    end += size_;
  }
  if (start < 0 || end < 0) return {};
  return GetElements(start, end);
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsByRevRange(int start, int end) {
  if (start < 0) {
    start += size_;
  }
  if (end < 0) {
    end += size_;
  }
  if (start < 0 || end < 0) return {};
  return GetElementsRev(start, end);
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsGt(const Key& start) {
  return GetElementsGt(start, false);
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsGte(const Key& start) {
  return GetElementsGt(start, true);
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsLt(const Key& end) {
  return GetElementsLt(end, false);
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsLte(const Key& end) {
  return GetElementsLt(end, true);
}

/*
 * return all keys within the range [start, end)
 */
template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsInRange(const Key& start, const Key& end) {
  if (Gte(start, end)) return {};

  const SkiplistNode* ns = GetFirstElementGt(start, true);

  std::vector<Key> keys;
  while (ns && Lt(ns->key_, end)) {
    keys.push_back(ns->key_);
    ns = ns->GetNext(0);
  }

  return keys;
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsGt(const Key& start, bool Eq) {
  const SkiplistNode* ns = GetFirstElementGt(start, Eq);

  std::vector<Key> keys;
  while (ns) {
    keys.push_back(ns->key_);
    ns = ns->GetNext(0);
  }

  return keys;
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsLt(const Key& start, bool Eq) {
  const SkiplistNode* ns = GetLastElementLt(start, Eq);
  if (ns == head_) return {};

  std::vector<Key> keys;
  const SkiplistNode* n = head_->GetNext(0);
  while (n != ns->GetNext(0)) {
    keys.push_back(n->key_);
    n = n->GetNext(0);
  }

  return keys;
}

template <typename Key, typename Comparator>
const typename Skiplist<Key, Comparator>::SkiplistNode*
Skiplist<Key, Comparator>::GetFirstElementGt(const Key& key, bool Eq) {
  const SkiplistNode* node = head_;
  for (int i = level_ - 1; i >= 0; --i) {
    while (node->GetNext(i) &&
           (Eq ? Lt(node->GetNext(i)->key_, key) : Lte(node->GetNext(i)->key_, key))) {
      node = node->GetNext(i);
    }
  }
  return node->GetNext(0);
}

template <typename Key, typename Comparator>
const typename Skiplist<Key, Comparator>::SkiplistNode* Skiplist<Key, Comparator>::GetLastElementLt(
    const Key& key, bool Eq) {
  const SkiplistNode* node = head_;
  for (int i = level_ - 1; i >= 0; --i) {
    while (node->GetNext(i) &&
           (Eq ? Lte(node->GetNext(i)->key_, key) : Lt(node->GetNext(i)->key_, key))) {
      node = node->GetNext(i);
    }
  }
  return node;
}

template <typename Key, typename Comparator>
const Key& Skiplist<Key, Comparator>::operator[](size_t i) {
  const SkiplistNode* node = GetElement(i);

  if (node == nullptr) throw std::out_of_range("skiplist index out of bound");

  return node->key_;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::Clear() {
  Reset();
  level_ = InitSkiplistLevel;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::Print() const {
  const SkiplistNode* node = head_;
  for (int i = level_ - 1; i >= 0; --i) {
    printf("h%d", i);
    while (node) {
      std::cout << node->key_;
      printf("---%zu---", node->GetSpan(i));
      node = node->GetNext(i);
    }
    printf("end\n");
    node = head_;
  }
}

/*
 * the function assumes that the node exists in the skiplist.
 * should make sure the node contained in the skiplist before calling this function.
 */
template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::DeleteNode(const Key& key, SkiplistNode* update[MaxSkiplistLevel]) {
  SkiplistNode* node_to_delete = update[0]->GetNext(0);

  for (int i = level_ - 1; i >= 0; --i) {
    const SkiplistNode* next = update[i]->GetNext(i);
    if (update[i] && next && Eq(next->key_, key)) {
      update[i]->SetNext(i, next ? next->GetNext(i) : nullptr);
      update[i]->SetSpan(i, update[i]->GetSpan(i) + (next ? next->GetSpan(i) : 0) - 1);
    } else if (update[i]) {
      update[i]->SetSpan(i, update[i]->GetSpan(i) - 1);
    }
  }

  /* for level 0, update backward pointer since it's a double linked list */
  if (update[0]->GetNext(0)) {
    update[0]->GetNext(0)->SetPrev(update[0]);
  }

  delete node_to_delete;
  --size_;
}

template <typename Key, typename Comparator>
const typename Skiplist<Key, Comparator>::SkiplistNode* Skiplist<Key, Comparator>::GetElement(
    size_t rank) {
  if (rank >= size_) return nullptr;

  size_t span_ = 0;
  const SkiplistNode* node = head_;

  for (int i = level_ - 1; i >= 0; --i) {
    while (node->GetNext(i) && (span_ + node->GetSpan(i) < rank + 1)) {
      span_ += node->GetSpan(i);
      node = node->GetNext(i);
    }

    if (node->GetNext(i) && span_ + node->GetSpan(i) == rank + 1) {
      return node->GetNext(i);
    }
  }

  return nullptr;
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElements(size_t start, size_t end) {
  if (start > end) return {};

  const SkiplistNode* node = GetElement(start);
  if (node == nullptr) return {};

  std::vector<Key> keys;
  while (node && start <= end) {
    keys.push_back(node->key_);
    node = node->GetNext(0);
    ++start;
  }
  return keys;
}

template <typename Key, typename Comparator>
std::vector<Key> Skiplist<Key, Comparator>::GetElementsRev(size_t start, size_t end) {
  if (start > end) return {};

  const SkiplistNode* node = GetElement(size_ - 1 - start);
  if (node == nullptr) return {};

  std::vector<Key> keys;
  while (node != head_ && start <= end) {
    keys.push_back(node->key_);
    node = node->GetPrev();
    ++start;
  }
  return keys;
}

template <typename Key, typename Comparator>
const typename Skiplist<Key, Comparator>::SkiplistNode* Skiplist<Key, Comparator>::FindLast()
    const {
  const SkiplistNode* node = head_;

  int l = level_;
  while (--l >= 0) {
    while (node->GetNext(l)) {
      node = node->GetNext(l);
    }
  }

  return node;
}

template <typename Key, typename Comparator>
void Skiplist<Key, Comparator>::Reset() {
  SkiplistNode* node = head_->GetNext(0);
  while (node) {
    SkiplistNode* next = node->GetNext(0);
    delete node;
    node = next;
  }
  head_->Reset();
  size_ = 0;
}

template <typename Key, typename Comparator>
Skiplist<Key, Comparator>::~Skiplist() {
  Reset();
}

}  // namespace skiplist
