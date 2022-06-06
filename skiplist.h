#pragma once

#include "skiplist_node.h"
#include "range_spec.h"

class Skiplist {
    public:
        static const int MaxSkipListLevel = 1024;
        Skiplist();
        int getRandomLevel();
        SkiplistNode* insert(const string& ele, double score);
        void del(SkiplistNode *x);
        void deleteNode(SkiplistNode* x, vector<SkiplistNode*>& update);
        SkiplistNode* updateScore(const string& ele, double curscore, double newscore);
        bool isInRange(RangeSpec *spec);
    private:
        SkiplistNode *header;
        SkiplistNode *tail;
        long length;
        int level;
};
