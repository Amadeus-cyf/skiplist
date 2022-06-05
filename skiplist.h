#pragma once

#include "skiplist_node.h"

class Skiplist {
    public:
        static const int MaxSkipListLevel = 1024;
        Skiplist();
        int getRandomLevel();
        void insert(string& ele, double score);
        void del(SkiplistNode *x);
        void deleteNode(SkiplistNode* x, vector<SkiplistNode*>& update);
    private:
        SkiplistNode *header;
        SkiplistNode *tail;
        long length;
        int level;
};
