#pragma once

#include "skiplist_node.h"

class Skiplist {
    public:
        static const int MaxSkipListLevel = 1024;
        Skiplist();
        int getRandomLevel();
        SkiplistNode* createSkiplistNode(string& ele, double score);
        void insert(string& ele, double score);
    private:
        SkiplistNode *header;
        SkiplistNode *tail;
        long length;
        long level;
};
