#include "ws2812.h"

class TURN_SIGNAL : public WS2812 {
public:

    void pattern_snakes(uint len, uint t);
    void pattern_random(uint len, uint t);
    void pattern_sparkle(uint len, uint t);
    void pattern_greys(uint len, uint t);

    typedef void (TURN_SIGNAL::*pattern)(uint, uint);
    typedef struct pattern_t{
        pattern pat;
        const char *name;
    } PATTERN_T;
    PATTERN_T pattern_table[4] = {
            {&TURN_SIGNAL::pattern_snakes,  "Snakes!"},
            {&TURN_SIGNAL::pattern_random,  "Random data"},
            {&TURN_SIGNAL::pattern_sparkle, "Sparkles"},
            {&TURN_SIGNAL::pattern_greys,   "Greys"}
    };

    void run(int ptn, int t) {
        (this->*pattern_table[ptn].pat)(NUM_PIXELS, t);
    }
    int getPcount() {
        return count_of(pattern_table);
    }
    const char *getName(int ptn) {
        return this->pattern_table[ptn].name;
    }
};
