#pragma once

#include <string>
#include <vector>
#include "skiplist_level.h"

using std::string;
using std::vector;

class SkiplistNode {
    public:
        inline SkiplistNode(string& ele, double score) : ele(ele), score(score) {};
        inline SkiplistNode(string& ele, double score, int level): ele(ele), score(score) {
            this->level.resize(level);
        }
        
        string ele;
        double score;
        vector<SkiplistLevel*> level;
        SkiplistNode *backward;
};
