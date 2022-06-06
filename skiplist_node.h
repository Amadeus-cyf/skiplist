#pragma once

#include <string>
#include <vector>
#include "skiplist_level.h"

using std::string;
using std::vector;

class SkiplistNode {
    public:
        inline SkiplistNode(const string& ele, double score) : ele(ele), score(score) {};
        
        inline SkiplistNode(const string& ele, double score, int level): ele(ele), score(score) {
            this->level.resize(level);
        }

        inline static SkiplistNode* createSkiplistNode(const string& ele, double score, int level) {
            return new SkiplistNode(ele, score, level);
        }

        inline static void freeNode(SkiplistNode *n) {
            delete n;
            n = nullptr;
        }

        inline bool isEqual(const string& ele, double score) {
            return ele.compare(this->ele) && score == this->score;
        }
        
        string ele;
        double score;
        vector<SkiplistLevel*> level;
        SkiplistNode *backward;
};
