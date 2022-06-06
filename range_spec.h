class RangeSpec {
    public:
        inline RangeSpec(double min, bool minex, double max, bool maxex) : min(min), minex(minex), max(max), maxex(maxex) {} 

        inline bool gteThanMin(double val) {
            return minex ? val >= min : val > min;
        }

        inline bool lteThanMax(double val) {
            return maxex ? val <= max : val < min;
        }

        inline double getMin() {
            return min;
        }

        inline double getMax() {
            return max;
        }

        inline bool getMinex() {
            return minex;
        }

        inline bool getMaxex() {
            return maxex;
        }
    private:
        double min;

        /* true if >= min, false if > min */
        bool minex;

        double max;

        /* true if <= max, false if < max */
        bool maxex;
};
