#ifndef LL_BUTTON_H
#define LL_BUTTON_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifndef __weak
#define __weak __attribute__((__weak__))
#endif

/* ll button operations callback function */
typedef void (*ll_btn_ops_cb)(void*);
/* ll button level get callback function */
typedef uint32_t (*ll_btn_level_get_cb)(void*);

/* button state */
typedef enum
{
    LL_BTN_INIT = 0,
    LL_BTN_DOWN,
    LL_BTN_PRESSED,
    LL_BTN_PRESSEND,
    LL_BTN_UP,
    LL_BTN_RELEASED,
} ll_btn_state;

/* button state callback function preinstall, add custom function you need above *LL_BTN_IDX_END* */
typedef enum
{
    LL_BTN_DOWN_CB = 0,
    LL_BTN_UP_CB,
    LL_BTN_LONG_PRESS_START_CB,
    LL_BTN_LONG_PRESS_DURING_CB,
    LL_BTN_LONG_PRESS_STOP_CB,
    LL_BTN_TAP_CB,
    LL_BTN_CB_IDX_END
} ll_btn_cb_type;

/* ll button object */
typedef struct LL_BTN_OBJ
{
    volatile uint32_t    tick;
    uint32_t             active_level : 1;
    volatile uint32_t    btn_level : 1;
    uint32_t             state : 3;
    uint32_t             debounce_term : 8;
    uint32_t             debounce_again : 4;
    uint32_t             debounce_cnt : 4;
    uint32_t             is_debouncing : 1;
    uint32_t             idx : 10;
    uint32_t             long_press_term : 16;
    uint32_t             tapping_term : 10;
    uint32_t             tapping_cnt : 6;
    const char          *name;
    ll_btn_ops_cb        cb[LL_BTN_CB_IDX_END];
    struct LL_BTN_OBJ   *next;
} ll_btn_obj_t;


#define LL_BTN_TRUE  1
#define LL_BTN_FALSE 0

/* Using debug set 1, no debug set 0 */
#define LL_BTN_DEBUG_ON 0
#define LL_BTN_DEBUG_NAME(__info, ...) do { \
                                          printf("LL_BTN_DEBUG(idx:%d, name:%s)::", btn->idx, btn->name); \
                                          printf(__info, ##__VA_ARGS__); \
                                        } while(0)
#if LL_BTN_DEBUG_ON
#define LL_BTN_DEBUG(__info, ...) LL_BTN_DEBUG_NAME(__info, ##__VA_ARGS__)
#else
#define LL_BTN_DEBUG(__info, ...)
#endif

/* Get 2 tick difference(32-bit) */
#define LL_BTN_TICK_DIFF_32(curr, last) ((curr) > (last) ? (uint32_t)((curr) - (last)) : (UINT32_MAX) - (last) + (curr) + 1)
#define LL_BTN_TICK_DIFF(curr, last)  LL_BTN_TICK_DIFF_32(curr, last)

/* ================================= TIPS ================================== */
/* 1.TIME RULE
 * debouce_time < tap_dance_interval_time << long_press_interval_time
 */

/* Default setting for using, time unit: ms */
/* default active level */
#define LL_BTN_DEFAULT_ACTIVE_LEVEL 1
/* debounce term */
#define LL_BTN_DEFAULT_DEBOUNCE_TERM 5
/* do debounce again times */
#define LL_BTN_DEFAULT_DEBOUNCE_AGAIN 0
/* max 65535ms */
#define LL_BTN_DEFAULT_LONG_PRESS_TERM 500
/* 70 ~ 120ms better */
#define LL_BTN_DEFAULT_TAPPING_TERM 100

/* ========================== LL BUTTON FUNCTIONS ========================== */
/**
 * @brief Dynamic creat a ll button, only need set 2 params, other is default
 *
 * @param name          Button name
 * @param active_level  Button active level
 */
void ll_btn_dynamic_creat_simple(const char *name,
                                 uint32_t active_level);

/**
 * @brief Dynamic creat a ll button, need set all params
 *
 * @param name                      Button name
 * @param active_level              Button active level
 * @param debounce_term             Button debounce term
 * @param debounce_again            Button debounce again times
 * @param long_press_term           Button long press term
 * @param tapping_term              Button tapping term
 */
void ll_btn_dynamic_creat(const char *name,
                          uint32_t active_level,
                          uint32_t debounce_term,
                          uint32_t debounce_again,
                          uint32_t long_press_term,
                          uint32_t tapping_term);

/**
 * @brief Static creat a ll button, only need set button object and 2 params, other is default
 *
 * @param btn           Button obeject
 * @param name          Button name
 * @param active_level  Button active level
 */
void ll_btn_static_creat_simple(ll_btn_obj_t *btn,
                                const char   *name,
                                uint32_t      active_level);

/**
 * @brief Static creat a ll button, need set all params
 *
 * @param btn                       Button obeject
 * @param name                      Button name
 * @param active_level              Button active level
 * @param debounce_term             Button debounce term
 * @param debounce_again            Button debounce again times
 * @param long_press_term           Button long press term
 * @param tapping_term              Button tapping term
 */
void ll_btn_static_creat(ll_btn_obj_t *btn,
                         const char   *name,
                         uint32_t      active_level,
                         uint32_t      debounce_term,
                         uint32_t      debounce_again,
                         uint32_t      long_press_term,
                         uint32_t      tapping_term);

/**
 * @brief Print all created ll buttons
 *
 * @return None
 */
void ll_btn_print_list(void);

/**
 * @brief A button attach a callback funciton, this is used for button which is created by dynamic.
 *
 * @param idx        The index by dynamic created button
 * @param cb_type    The callback function type in enum(ll_btn_cb_type)
 * @param cb         Corresponding callback function
 */
void ll_btn_attach_by_idx(uint32_t idx, uint8_t cb_type, ll_btn_ops_cb cb);

/**
 * @brief A button attach a callback funciton, this is used for button which is created by dynamic.
 *
 * @param idx        The index by dynamic created button
 * @param cb_type    The callback function type in enum(ll_btn_cb_type)
 */
void ll_btn_detach_by_idx(uint32_t idx, uint8_t cb_type);

/**
 * @brief A button attach a callback funciton, this is used for button which is created by static.
 *
 * @param btn        Button object
 * @param cb_type    The callback function type in enum(ll_btn_cb_type)
 * @param cb         Corresponding callback function
 */
void ll_btn_attach_by_obj(ll_btn_obj_t *btn, uint8_t cb_type, ll_btn_ops_cb cb);

/**
 * @brief A button detach a callback funciton, this is used for button which is created by static.
 *
 * @param btn        Button object
 * @param cb_type    The callback function type in enum(ll_btn_cb_type)
 */
void ll_btn_detach_by_obj(ll_btn_obj_t *btn, uint8_t cb_type);

/**
 * @brief This is ll button core, need call in a endless loop.
 *
 */
void ll_btn_task(void);

#ifdef __cplusplus
}
#endif

#endif
