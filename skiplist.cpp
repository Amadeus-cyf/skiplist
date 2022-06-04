#include "skiplist.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

Skiplist::Skiplist() : level(1), length(0) {
    string s = "";
    this->header = new SkiplistNode(s, 0, this->level);
}

int Skiplist::getRandomLevel() {
    static const int threshold = MaxSkipListLevel * rand();

    int level = 1;
    
    while(rand() < threshold) {
        ++level;
    }

    return level < MaxSkipListLevel ? level : MaxSkipListLevel;
}

SkiplistNode* Skiplist::createSkiplistNode(string& ele, double score) {
    return new SkiplistNode(ele, score, this->level);
}

void Skiplist::insert(string& ele, double score) {
    SkiplistNode *x = this->header;

    vector<SkiplistNode*> update(this->level);
    vector<int> rank(this->level);

    for(int i = this->level-1; i >= 0; --i) {
        rank[i] == i == this->level-1 ? 0 : rank[i+1];

        while(x->level[i]->forward && (x->level[i]->forward->score < score 
            || (x->level[i]->forward->score == score && x->level[i]->forward->ele.compare(ele) < 0))) {
               rank[i] += x->level[i]->span;
               x = x->level[i]->forward;
        }

        update[i] = x;
    }

    int level = getRandomLevel();

    if(level > this->level) {
        for(int i = this->level; i < level; ++i) {
            SkiplistLevel *l = new SkiplistLevel(nullptr, this->length);
            this->header->level.push_back(l);
        }

        this->level = level;
    }

    x = this->createSkiplistNode(ele, score);

    for(int i = level-1; i >= 0; --i) {
        x->level[i]->forward = update[i]->level[i]->forward;
        update[i]->level[i]->forward = x;
        
        x->level[i]->span = update[i]->level[i]->span - (rank[0] - rank[i]);
        update[i]->level[i]->span = rank[0] - rank[i] + 1;
    }

    for(int i = level; i < this->level; ++i) {
        ++update[i]->level[i]->span;
    }

    x->backward = update[0] == this->header ? nullptr : update[0];

    if(x->level[0]->forward) {
        x->level[0]->forward->backward = x;
    } else {
        this->tail = x;
    }

    ++this->length;
}
