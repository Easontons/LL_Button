/* include lib header file */
#include "ll_button.h"

/* define your button index */
enum llb_idx
{
    BTN1 = 0,
    BTN2 = 1,
    BTN3 = 2,
};

enum llb_active_level
{
    LOW_ACTIVE,
    HIGH_ACTIVE
};

/* static creat use button object */
ll_btn_obj_t btn_1;
ll_btn_obj_t btn_2;
ll_btn_obj_t btn_3;

void btn1_down_cb(void *btn)
{
    /* do some button down thing */
}
void btn2_down_cb(void *btn)
{
    /* do some button down thing */
}
void btn3_down_cb(void *btn)
{
    /* do some button down thing */
}
void btn3_tap_cb(void *btn)
{
    ll_btn_obj_t *button = btn;
    if (button->tapping_cnt == 1)  /* tap once */
    {
        /* do some button tap(1) thing */
    }
    else if (button->tapping_cnt == 2)  /* tap twice */
    {
        /* do some button tap(2) thing */
    }
}

void static_creat_button(void)
{
    /** Simple creat(using some default setting in ll_button.h) or full param setting creat.
     *  Created button will get their unique idx param by order,
     *  you can use idx to attach button callback.
     */
    ll_btn_static_creat_simple(&btn_1, "btn1", HIGH_ACTIVE);
    ll_btn_static_creat_simple(&btn_1, "btn2", HIGH_ACTIVE);
    ll_btn_static_creat(&btn_3
                        "btn3",
                         HIGH_ACTIVE,
                         5   /* debounce term */,
                         1   /* debounce time, 0 is once */,
                         600 /* long press term */,
                         250 /* tap term */);

    /* Static creat can use idx and object attach callback */
    ll_btn_attach_by_idx(BTN1, LL_BTN_DOWN_CB, btn1_down_cb);
    ll_btn_attach_by_obj(&btn_2, LL_BTN_DOWN_CB, btn2_down_cb);
    ll_btn_attach_by_obj(&btn_3, LL_BTN_DOWN_CB, btn3_down_cb);
    ll_btn_attach_by_obj(&btn_3, LL_BTN_TAP_CB, btn3_tap_cb);
}

/** !!! need provide button level get function in your project,
 *      using the same name in ll_button.c, because this is a
 *      weak function.
 */
uint32_t ll_btn_level_get(ll_btn_obj_t *btn)
{
    /* you can use idx to do, like this */
    switch (btn->idx)
    {
    case BTN1:
        /* level_get function is depend on your project */
        return level_get(BTN1);
    case BTN2:
        return level_get(BTN2);
    case BTN1:
        return level_get(BTN3);
    default:
        /* default return is not active */
        return (1 - btn->active_level);
    }
}

/** !!! you also need provide system tick get function in your project,
 *      this function need return 32-bit system tick, usually a microsecond
 *      tick. using the same name in ll_button.c, because this is a
 *      weak function.
 */
uint32_t ll_btn_tick_get(void)
{
    return system_tick;
}

void main(void)
{
    static_creat_button();

    while (1)
    {
        /* always call in endless loop */
        ll_btn_task();
    }
}
