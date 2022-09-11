#include "seq_turn_signal.h"

void TURN_SIGNAL::run() {
    PATTERN_PARAM_T pp;

    startTimer();
    setPatternParam(&pp, DAY_TURN_ON, true, true);
    (this->*pattern_table[pp.pattern].pat)(&pp);

    while(true) {
#if SIGNAL_IN_DEBUG
        //printf("ENABLE CHECK!! SWR:%s SWL:%s\n",
        //    param.right_en?"True":"False",
        //    param.left_en?"True":"False");
  #endif
        patternChekc(&pp);
        showPatternParam(&pp);
        (this->*pattern_table[pp.pattern].pat)(&pp);
    }
}

bool TURN_SIGNAL::swInputTimerCallback(struct repeating_timer *t){
    static bool led = false;
    static bool r_in[INPUT_REG_SIZE] = {0};
    static bool l_in[INPUT_REG_SIZE] = {0};
    swInputTimer_t *p;

    p = (swInputTimer_t*)(t->user_data);

    gpio_put(DEBUG_LED_PIN, led);
    led = !led;

    bool r_tmp[INPUT_REG_SIZE];
    bool l_tmp[INPUT_REG_SIZE];

    memcpy(r_tmp, r_in, INPUT_REG_SIZE);
    memcpy(l_tmp, l_in, INPUT_REG_SIZE);
    regShift(r_tmp, gpio_get(R_SIG_INPUT_PIN));
    regShift(l_tmp, gpio_get(L_SIG_INPUT_PIN));
    memcpy(r_in, r_tmp, INPUT_REG_SIZE);
    memcpy(l_in, l_tmp, INPUT_REG_SIZE);

    p->right_en = regEnCheck(r_in);
    p->left_en = regEnCheck(l_in);
#if SIGNAL_IN_DEBUG
        printf("Timer!! %lld a->%d SWR:%s SWL:%s\n", time_us_64(), p->a,
            (p->right_en)?"True":"False",
            (p->left_en)?"True":"False");
#endif
    return true;
}
