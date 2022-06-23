#pragma once

class SkiplistNode;

class SkiplistLevel {
    public:
        inline SkiplistLevel(SkiplistNode *forward, long span) : forward(forward), span(span){};
        SkiplistNode *forward;
        long span; 

        inline ~SkiplistLevel() {
            if(this->forward) {
                this->forward = nullptr;
            }
        }
};
