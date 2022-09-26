#include "seq_turn_signal.h"

void TURN_SIGNAL::run() {
    PATTERN_PARAM_T pp;
    SW_INPUT_STATE_T sis;
    int mode = MODE_DEFAULT;

    startTimer(&sis);
    setPatternParam(&pp, DAY_TURN_ON, true, true);
    (this->*pattern_table[pp.pattern].pat)(&pp);

    while(true) {
        checkMode(&mode);
        switch(mode){
        case MODE_DEFAULT:
            chekcPattern(&pp, &sis);
#if SIGNAL_IN_DEBUG
            showPatternParam(&pp);
#endif
            (this->*pattern_table[pp.pattern].pat)(&pp);
            break;

        case MODE_DEBUG:
            int loop_num = 0;
            if (debug_mode(&pp, &loop_num)) {
                printf("pattern is %s\n", getName(pp.pattern));
                for (int i = 0; i < loop_num; i++) {
                    printf("loop %d\n", i);
                    (this->*pattern_table[pp.pattern].pat)(&pp);
                    sleep_ms(10);
                }
            } else {
                mode = MODE_DEFAULT;
            }
            break;
        }
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

typedef struct debug_param_t {
    char argc[32];
    int argv[3];
}DEBUG_PARAM_T;
#include <cstdio>
int TURN_SIGNAL::inputCommand(PATTERN_PARAM_T *pp_p, int *ln) {
    char commandStr[256] = {'\0'};
    int c = -1;
    printf("command:");
    while(true) {
        c = getchar_timeout_us(0);
        if (c >= 0) {
            if (c != '\n') {
                strcat(commandStr, (char*)(&c));
                //printf("str:%s\n",commandStr);
                printf("%s", (char*)(&c));
            } else {
                printf("\n");
                char cmd[32] = {0};
                char argc[32] = {0};
                int argv[4] = {0};
                //sscanf_s(commandStr, "%s %c %d %d %d",
                sscanf(commandStr, "%s %s %d %d %d",
                                //cmd, sizeof(cmd),
                                //argc, sizeof(argc),
                                //&(argv[0]), sizeof(int),
                                //&(argv[1]), sizeof(int),
                                //&(argv[2]), sizeof(int));
                                cmd, argc,
                                &(argv[0]),
                                &(argv[1]),
                                &(argv[2]));
                printf("%s\n", commandStr);
                printf("cmd:%s\n", cmd);
                printf("argc:%s\n", argc);
                printf("argv:%d %d %d\n", argv[0], argv[1], argv[2]);

                for (int i = 0; i < DEBUG_COMMAND_MAX_NUM; i++) {
                    //if (strcmp(commandStr, debug_command[i]) == 0) {
                    if (strcmp(cmd, debug_command[i]) == 0) {
                        switch (i) {
                        case DEBUG_PATTERN_SET:
                            printf("set! now!\n");
                            for (int j = 0; j < PATTERN_MAX_NUM; j++) {
                                if (strcmp(argc, pattern_table[j].name) == 0) {
                                    printf("match %s\n", pattern_table[j].name);
                                    pp_p->pattern = j;
                                    pp_p->right = (bool)argv[0];
                                    pp_p->left = (bool)argv[1];
                                    *ln = argv[2];
                                }
                            }
                            break;
                        default:
                            break;
                        }
                        for (int j=0; j<256; j++)
                            commandStr[j] = '\0';
                        printf("debug command is %s\n", debug_command[i]);
                        return i;
                    }
                }
                printf("comannd is not match\n");
                printf("command:");
                for (int i=0; i<256; i++)
                    commandStr[i] = '\0';
            }
        }
    }
}

int TURN_SIGNAL::debug_mode(PATTERN_PARAM_T *pp_p, int *ln) {
    switch(inputCommand(pp_p, ln)) {
    case DEBUG_PATTERN_SET:
        printf("debug pattern set\n");
        //pp_p->pattern = SNAKES;
        return DEBUG_PATTERN_SET;
    case DEBUG_EXIT:
    default:
        return DEBUG_EXIT;
    }
}
