#include "skiplist.h"

void insert(Skiplist*);
void del(Skiplist*);

int main() {
    Skiplist* list = new Skiplist(4);

    del(list);

    delete list;
}

void insert(Skiplist* list) {
    list->insert("key1", 2);
    list->insert("key2", 3);

    SkiplistNode* n1 = list->search("key1", 2);
    SkiplistNode* n2 = list->search("key2", 3);
    SkiplistNode* n3 = list->search("key3", 3);

    assert(n1);
    assert(n2);

    printf("%s %f\n", n1->ele.c_str(), n1->score);
    printf("%s %f\n", n2->ele.c_str(), n2->score);

    assert(n3 == nullptr);
}

void del(Skiplist* list) {
    insert(list);

    SkiplistNode *n1 = list->search("key1", 2);

    assert(n1);

    assert(!list->del("key1", 3));

    assert(list->del("key1", 2));

    assert(!list->del("key1", 2));

    n1 = list->search("key1", 2);

    assert(n1 == nullptr);
}
