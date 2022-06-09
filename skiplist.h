#pragma once

#include "skiplist_node.h"
#include "range_spec.h"

class Skiplist {
    public:
        static const double SkipListP;
        static const int MaxSkipListLevel;
        Skiplist();
        int getRandomLevel();
        SkiplistNode* search(const string& ele, double score);
        SkiplistNode* insert(const string& ele, double score);
        bool del(const string& ele, double score);
        void deleteNode(SkiplistNode* x, vector<SkiplistNode*>& update);
        SkiplistNode* updateScore(const string& ele, double curscore, double newscore);
        bool isInRange(RangeSpec *spec);
    private:
        SkiplistNode *header;
        SkiplistNode *tail;
        long length;
        int level;
};
