#include "skiplist.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

const double Skiplist::SkipListP = 1.0 / 2;
const int Skiplist::MaxSkipListLevel = 16;

Skiplist::Skiplist(int level) : level(level), length(0) {
    string s = "";
    this->header = SkiplistNode::createSkiplistNode("", 0, level);
}

int Skiplist::getRandomLevel() {
    int level = 1;
    
    while(((double)rand() / RAND_MAX) < SkipListP && level < MaxSkipListLevel) {
        ++level;
    }

    return level < MaxSkipListLevel ? level : MaxSkipListLevel;
}

SkiplistNode* Skiplist::search(const string& ele, double score) {
    printf("search %s %f\n", ele.c_str(), score);
    SkiplistNode *x = this->header;

    for(int i = this->level-1; i >= 0; --i) {    
        while(x->level[i]->forward && (x->level[i]->forward->score < score 
        || (x->level[i]->forward->score == score && x->level[i]->forward->ele.compare(ele) < 0))) {
            x = x->level[i]->forward;
        }

        SkiplistNode *f = x->level[i]->forward;

        if(f && f->score == score && f->ele.compare(ele) == 0) {
            return f;
        }
    }

    return nullptr;
}

SkiplistNode* Skiplist::insert(const string& ele, double score) {
    printf("%s %f\n", ele.c_str(), score);

    SkiplistNode *x = this->header;

    vector<SkiplistNode*> update(this->level);
    vector<int> rank(this->level);

    printf("calculate rank\n");

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

    printf("get level %d\n", level);

    if(level > this->level) {
        for(int i = this->level; i < level; ++i) {
            SkiplistLevel *l = new SkiplistLevel(nullptr, this->length);
            this->header->level.push_back(l);
        }

        this->level = level;
    }

    x = SkiplistNode::createSkiplistNode(ele, score, this->level);

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
    return x;
}

bool Skiplist::del(const string& ele, double score) {
    vector<SkiplistNode*> update;
    SkiplistNode *x = this->header;

    for(int i = this->level-1; i >= 0; --i) {
        while(x->level[i]->forward && (x->level[i]->forward->score < x->score 
        || (x->level[i]->forward->score == x->score && x->level[i]->forward->ele.compare(ele) < 0))) {
            x = x->level[i]->forward;
        }
        
        update[i] = x;
    }

    x = x->level[0]->forward;

    if(x->score != score || x->ele.compare(ele) != 0) {
        return false;
    }

    this->deleteNode(x, update);
    SkiplistNode::freeNode(x);

    return true;
}

void Skiplist::deleteNode(SkiplistNode* x, vector<SkiplistNode*>& update) {
    for(int i = this->level-1; i >= 0; --i) {
        if(update[i]->level[i]->forward == x) {
            update[i]->level[i]->forward = x->level[i]->forward;
            update[i]->level[i]->span += (x->level[i]->span-1);
        } else {
            --update[i]->level[i]->span;
        }
    }

    if(x->level[0]->forward) {
        x->level[0]->forward->backward = x->backward;
    } else {
        this->tail = x->backward;
    }
    
    for(int i = this->level-1; i >= 1 && !this->header->level[i]->forward; --i) {
        --this->level;
        this->header->level.pop_back();
    }

    --this->length;
}

SkiplistNode* Skiplist::updateScore(const string& ele, double curscore, double newscore) {
    vector<SkiplistNode*> update(this->level);
    SkiplistNode *x = this->header;

    for(int i = this->level-1; i >= 0; --i) {
        while(x->level[i]->forward && (x->level[i]->forward->score < curscore 
        || (x->level[i]->forward->score == curscore && x->level[i]->forward->ele.compare(ele) < 0))) {
            x = x->level[i]->forward;
        }

        update[i] = x;
    }

    x = update[0]->level[0]->forward;

    if(x->ele.compare(ele) != 0 || x->score != curscore) {
        return nullptr;
    }

    if((!x->backward || x->backward->score <= curscore) 
        && (!x->level[0]->forward || x->level[0]->forward->score >= curscore)) {
        /* if new score is still at the same position, update score directly */
        x->score = newscore;
        return x;
    }

    this->deleteNode(x, update);
    SkiplistNode::freeNode(x);

    return this->insert(ele, newscore);
}

bool Skiplist::isInRange(RangeSpec *spec) {
    SkiplistNode *x = this->tail;

    double min = spec->getMin(), max = spec->getMax();
    bool minex = spec->getMinex(), maxex = spec->getMaxex();

    if(min > max || (min == max && (!minex || !maxex))) {
        return false;
    }

    if(!this->tail || !spec->gteThanMin(this->tail->score)) {
        return false;
    }

    if(!this->header || !spec->lteThanMax(this->header->score)) {
        return false;
    }

    return true;
}

Skiplist::~Skiplist() {
    SkiplistNode* x = this->header;

    while(x) {
        printf("delete %s %f\n", x->ele.c_str(), x->score);
        SkiplistNode* next = x->level[0]->forward;
        x->level[0]->forward = nullptr;
        x->backward = nullptr;

        delete x;
        x = next;
    }
}
