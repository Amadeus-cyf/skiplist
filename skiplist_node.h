#pragma once

#include <string>
#include <vector>
#include "skiplist_level.h"

using std::string;
using std::vector;

class SkiplistNode {
    public:
        inline static SkiplistNode* createSkiplistNode(const string& ele, double score, int level) {
            SkiplistNode *n = new SkiplistNode(ele, score, level);

            for(int i = 0; i < level; i++) {
                n->level[i] = new SkiplistLevel(nullptr, 0);
            }

            return n;
        }

        inline static void freeNode(SkiplistNode *n) {
            delete n;
            n = nullptr;
        }

        inline bool isEqual(const string& ele, double score) {
            return ele.compare(this->ele) && score == this->score;
        }
        
        inline ~SkiplistNode() {
            for(int i = 0; i < this->level.size(); ++i) {
                if(this->level[i]) {
                    delete this->level[i];
                    this->level[i] = nullptr;
                }
            }
        }

        string ele;
        double score;
        vector<SkiplistLevel*> level;
        SkiplistNode *backward;
    private:
        inline SkiplistNode(const string& ele, double score) : ele(ele), score(score), backward(nullptr) {};
        
        inline SkiplistNode(const string& ele, double score, int level): ele(ele), score(score), backward(nullptr) {
            this->level.resize(level);
        }
};
