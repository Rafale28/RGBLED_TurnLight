#include "seq_turn_signal.h"

uint8_t TURN_SIGNAL::gamma(uint8_t c, float g) {
    return (uint8_t)(pow((float)c / 255.0, 1.0 / g) * 255.0);
}
float TURN_SIGNAL::sinCalc(int duty) {
    return sin( duty * M_PI / 180 );
}

void TURN_SIGNAL::setPatternParam(PATTERN_PARAM_T *pp_p, int ptn, bool right, bool left) {
    pp_p->pattern = ptn;
    pp_p->right = right;
    pp_p->left = left;
}


void TURN_SIGNAL::paramInit() {
    param.a = 114514;
    param.right_en  = false;
    param.left_en   = false;
}

const void TURN_SIGNAL::regShift(bool *r_in, bool s_in) {
    for (int i=INPUT_REG_SIZE - 1; i >0 ; i--) {
        r_in[i] = r_in[i - 1];
    }
    r_in[0] = s_in;
}

bool TURN_SIGNAL::regEnCheck(bool *r) {
    bool ret = false;
    for (int i=INPUT_REG_SIZE - 1; i >0 ; i--) {
        ret |= r[i];
    }
    return ret;
}

void TURN_SIGNAL::gpioInit(){
    gpio_init(DEBUG_LED_PIN);
    gpio_init(R_SIG_INPUT_PIN);
    gpio_init(L_SIG_INPUT_PIN);

    gpio_set_dir(DEBUG_LED_PIN, GPIO_OUT);
    gpio_set_dir(R_SIG_INPUT_PIN, GPIO_IN);
    gpio_set_dir(L_SIG_INPUT_PIN, GPIO_IN);
}

int TURN_SIGNAL::patternChekc(PATTERN_PARAM_T *pp_p) {
    if (param.right_en || param.left_en) {
        setPatternParam(pp_p, GAMMA_P1, param.right_en, param.left_en);
    } else {
        if (pp_p->pattern != DAY && pp_p->pattern != DAY_TURN_ON)
            setPatternParam(pp_p, DAY_TURN_ON, pp_p->right, pp_p->left);
        else 
            setPatternParam(pp_p, DAY, true, true);
    }
}

void TURN_SIGNAL::startTimer() {
    paramInit();
    add_repeating_timer_ms(50, swInputTimerCallback, (void *)&param, &timer);
}

void TURN_SIGNAL::showPatternParam(PATTERN_PARAM_T *pp_p) {
    printf("Param, pattern:%s, R:%s, L:%s\n",
        getName(pp_p->pattern),
        pp_p->right?"True":"False",
        pp_p->left?"True":"False");
}

int TURN_SIGNAL::getPcount() {
    return count_of(pattern_table);
}

const char *TURN_SIGNAL::getName(int ptn) {
    return this->pattern_table[ptn].name;
}

uint16_t TURN_SIGNAL::getPixels() {
    return led_info.num_pixels;
}
