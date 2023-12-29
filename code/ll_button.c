#include "ll_button.h"

/* ll button head */
static ll_btn_obj_t ll_btn_head = {0};

/* Configure ll button */
static void ll_btn_conf(ll_btn_obj_t *btn,
                        const char   *name,
                        uint32_t      active_level,
                        uint32_t      debounce_term,
                        uint32_t      debounce_again,
                        uint32_t      long_press_term,
                        uint32_t      tapping_term)
{
    memset(btn, 0, sizeof(ll_btn_obj_t));
    btn->name = name;
    btn->active_level = active_level;
    btn->debounce_term = debounce_term;
    btn->debounce_again = debounce_again;
    btn->long_press_term = long_press_term;
    btn->tapping_term = tapping_term;
    btn->state = LL_BTN_INIT;
    /**
     *  Set button level to deactive level
     *  Explain:
     *  If button active level is 0, and button default level(btn_level) is 0 in initialization,
     *  than button will go to active state, but this is not expected,
     *  so set button level to deactive level in conf() to avoid this happening.
     */
    btn->btn_level = 1 - btn->active_level;
}

/* Insert a ll button to linked list */
static void ll_btn_insert(ll_btn_obj_t *btn)
{
    ll_btn_obj_t *target = &ll_btn_head;
    uint16_t idx = 0;
    while (target->next != NULL)
    {
        target = target->next;
        idx++;
    }
    target->next = btn;
    btn->idx = idx;
}

void ll_btn_dynamic_creat_simple(const char   *name,
                                 uint32_t      active_level)
{
    ll_btn_obj_t *btn = (ll_btn_obj_t *)LLB_MALLOC(sizeof(ll_btn_obj_t));
    if (btn == NULL)
    {
        return;
    }

    ll_btn_conf(btn,
                name,
                active_level,
                LL_BTN_DEFAULT_DEBOUNCE_TERM,
                LL_BTN_DEFAULT_DEBOUNCE_AGAIN,
                LL_BTN_DEFAULT_LONG_PRESS_TERM,
                LL_BTN_DEFAULT_TAPPING_TERM
                );
    ll_btn_insert(btn);
}

void ll_btn_dynamic_creat(const char  *name,
                          uint32_t     active_level,
                          uint32_t     debounce_term,
                          uint32_t     debounce_again,
                          uint32_t     long_press_term,
                          uint32_t     tapping_term)
{
    ll_btn_obj_t *btn = (ll_btn_obj_t *)LLB_MALLOC(sizeof(ll_btn_obj_t));
    if (btn == NULL)
    {
        return;
    }

    ll_btn_conf(btn,
                name,
                active_level,
                debounce_term,
                debounce_again,
                long_press_term,
                tapping_term);
    ll_btn_insert(btn);
}

void ll_btn_static_creat_simple(ll_btn_obj_t *btn,
                                const char   *name,
                                uint32_t      active_level)
{
    ll_btn_conf(btn,
                name,
                active_level,
                LL_BTN_DEFAULT_DEBOUNCE_TERM,
                LL_BTN_DEFAULT_DEBOUNCE_AGAIN,
                LL_BTN_DEFAULT_LONG_PRESS_TERM,
                LL_BTN_DEFAULT_TAPPING_TERM
                );
    ll_btn_insert(btn);
}

void ll_btn_static_creat(ll_btn_obj_t *btn,
                          const char  *name,
                          uint32_t     active_level,
                          uint32_t     debounce_term,
                          uint32_t     debounce_again,
                          uint32_t     long_press_term,
                          uint32_t     tapping_term)
{
    ll_btn_conf(btn,
                name,
                active_level,
                debounce_term,
                debounce_again,
                long_press_term,
                tapping_term);
    ll_btn_insert(btn);
}

/* Weak function, need give a same name function in your project,
 * and this functon need provide every button level by button index */
__weak uint32_t ll_btn_level_get(ll_btn_obj_t *btn)
{
    return 0;
}

/* Weak function ,need give a 32-bit global tick in your project */
__weak uint32_t ll_btn_tick_get(void)
{
    return 0;
}

static void ll_btn_reset(ll_btn_obj_t *button)
{
    button->state = LL_BTN_IDLE;
    button->tick = 0;
}

/* Print all ll buttons */
void ll_btn_print_list(void)
{
    ll_btn_obj_t *btn = &ll_btn_head;
    while (btn->next != NULL)
    {
        btn = btn->next;
        LL_BTN_DEBUG_NAME("\r\n");
    }
}

void ll_btn_attach_by_idx(uint32_t idx, uint8_t cb_type, ll_btn_ops_cb cb)
{
    ll_btn_obj_t *btn = ll_btn_head.next;
    uint32_t count = idx;
    while (count > 0)
    {
        btn = btn->next;
        count--;
    }
    if (cb_type >= LL_BTN_CB_IDX_END)
    {
        LL_BTN_DEBUG("this type callback function does not preinstall.\r\n");
        return;
    }
    btn->cb[cb_type] = cb;
    if (btn->cb[cb_type] == NULL)
    {
        LL_BTN_DEBUG("callback function idx %d attach NULL.\r\n", cb_type);
    }
    else
    {
        LL_BTN_DEBUG("callback function idx %d attach success.\r\n", cb_type);
    }
}
void ll_btn_detach_by_idx(uint32_t idx, uint8_t cb_type)
{
    ll_btn_obj_t *btn = ll_btn_head.next;
    uint32_t count = idx;
    while (count > 0)
    {
        btn = btn->next;
        count--;
    }
    if (cb_type >= LL_BTN_CB_IDX_END)
    {
        LL_BTN_DEBUG("this type callback function do not preinstall.\r\n");
        return;
    }
    btn->cb[cb_type] = NULL;
}

void ll_btn_attach_by_obj(ll_btn_obj_t *btn, uint8_t cb_type, ll_btn_ops_cb cb)
{
    if (cb_type >= LL_BTN_CB_IDX_END)
    {
        LL_BTN_DEBUG("this type callback function does not preinstall.\r\n");
        return;
    }
    btn->cb[cb_type] = cb;
    if (btn->cb[cb_type] == NULL)
    {
        LL_BTN_DEBUG("callback function idx %d attach NULL.\r\n", cb_type);
    }
    else
    {
        LL_BTN_DEBUG("callback function idx %d attach success.\r\n", cb_type);
    }
}

void ll_btn_detach_by_obj(ll_btn_obj_t *btn, uint8_t cb_type)
{
    if (cb_type >= LL_BTN_CB_IDX_END)
    {
        LL_BTN_DEBUG("this type callback function do not preinstall.\r\n");
        return;
    }
    btn->cb[cb_type] = NULL;
}

void ll_btn_debounce(ll_btn_obj_t *btn, uint32_t curr_tick, uint32_t curr_level)
{
    uint32_t tick_diff;
    /* no need debounce or debounce method external */
    if (btn->debounce_term == 0)
    {
        btn->btn_level = curr_level;
        return;
    }

    if (curr_level == btn->btn_level)
    {
        btn->is_debouncing = LL_BTN_FALSE;
    }
    else if ((curr_level != btn->btn_level) && (btn->is_debouncing == LL_BTN_FALSE))
    {
        btn->is_debouncing = LL_BTN_TRUE;
        btn->debounce_tick = curr_tick;
        LL_BTN_DEBUG("debounce start, tick is %ld.\r\n", btn->debounce_tick);
    }

    if (btn->is_debouncing == LL_BTN_TRUE)
    {
        tick_diff = LL_BTN_TICK_DIFF(curr_tick, btn->debounce_tick);
        if (tick_diff >= btn->debounce_term)
        {
            btn->debounce_cnt++;
            LL_BTN_DEBUG("debouncing, debounce cnt is %d.\r\n", btn->debounce_cnt);
            if (btn->debounce_cnt > btn->debounce_again)
            {
                btn->is_debouncing = LL_BTN_FALSE;
                btn->btn_level = curr_level;
                btn->debounce_tick = curr_tick;
                btn->debounce_cnt = 0;
                LL_BTN_DEBUG("debounce end, tick is %ld.\r\n", btn->debounce_tick);
            }
            else
            {
                btn->debounce_tick = curr_tick;
            }
        }
    }
}

/**
 * @brief ll button finite state machine
 * @param btn ll button object
 * @param curr_tick current tick
 */
void ll_btn_fsm(ll_btn_obj_t *btn, uint32_t curr_tick)
{
    uint32_t tick_diff = LL_BTN_TICK_DIFF(curr_tick, btn->tick);
    switch (btn->state)
    {
    case LL_BTN_INIT:
        if (btn->btn_level == btn->active_level)
        {
            btn->state = LL_BTN_DOWN;
        }
        else
        {
            /* if button at deactive state in start, do button up callback once */
            if (btn->cb[LL_BTN_UP_CB])
            {
                btn->cb[LL_BTN_UP_CB](btn);
            }
            btn->state = LL_BTN_IDLE;
        }
        break;
    case LL_BTN_IDLE:
        if (btn->btn_level == btn->active_level)
        {
            btn->state = LL_BTN_DOWN;
        }
        break;
    case LL_BTN_DOWN:
        if (btn->cb[LL_BTN_DOWN_CB])
        {
            btn->cb[LL_BTN_DOWN_CB](btn);
        }
        btn->state = LL_BTN_PRESSED;
        btn->tick = curr_tick;
        LL_BTN_DEBUG("button down, tick is %lu.\r\n", btn->tick);
        break;
    case LL_BTN_PRESSED:
        if (btn->btn_level != btn->active_level)
        {
        	btn->state = LL_BTN_UP;
        }
        else if ((btn->btn_level == btn->active_level) && (tick_diff > btn->long_press_term))
        {
            if (btn->cb[LL_BTN_LONG_PRESS_START_CB])
            {
                btn->cb[LL_BTN_LONG_PRESS_START_CB](btn);
            }
            /* If long press trigger, tap dance cnt need clear,
             * it means that long press trigger no tap trigger */
            btn->tapping_cnt = 0;
            btn->state = LL_BTN_PRESSEND;
            LL_BTN_DEBUG("button long press start, tick is %lu.\r\n", btn->tick + btn->long_press_term);
        }
        break;
    case LL_BTN_PRESSEND:
        if (btn->btn_level == btn->active_level)
        {
            if (btn->cb[LL_BTN_LONG_PRESS_DURING_CB])
            {
                btn->cb[LL_BTN_LONG_PRESS_DURING_CB](btn);
            }
        }
        else if (btn->btn_level != btn->active_level)
        {
            if (btn->cb[LL_BTN_LONG_PRESS_STOP_CB])
            {
                btn->cb[LL_BTN_LONG_PRESS_STOP_CB](btn);
            }
            btn->state = LL_BTN_UP;
        }
        break;
    case LL_BTN_UP:
    	if (tick_diff <= btn->tapping_term)
    	{
    		btn->tapping_cnt++;
    	}
        if (btn->cb[LL_BTN_UP_CB])
        {
            btn->cb[LL_BTN_UP_CB](btn);
        }
        btn->tick = curr_tick;
        LL_BTN_DEBUG("button up, tick is %lu.\r\n", btn->tick);
        btn->state = LL_BTN_RELEASED;
        break;
    case LL_BTN_RELEASED:
        if (btn->btn_level == btn->active_level)
        {
            btn->state = LL_BTN_DOWN;
        }
        else if (tick_diff > btn->tapping_term)
        {
            if (btn->cb[LL_BTN_TAP_CB] && (btn->tapping_cnt > 0))
            {
                btn->cb[LL_BTN_TAP_CB](btn);
                LL_BTN_DEBUG("button tap, tap cnt is %d.\r\n", btn->tapping_cnt);
                btn->tapping_cnt = 0;
            }
            LL_BTN_DEBUG("button released, tick is %lu.\r\n", btn->tick);
            ll_btn_reset(btn);
        }
        break;
    default:
        break;
    }
}

/* LL button task, need call in your project loop */
void ll_btn_task(void)
{
    ll_btn_obj_t *target = &ll_btn_head;
    uint32_t curr_tick;
    while (target->next != NULL)
    {
        curr_tick = ll_btn_tick_get();
        target = target->next;
        uint32_t level = ll_btn_level_get(target);
        ll_btn_debounce(target, curr_tick, level);
        ll_btn_fsm(target, curr_tick);
    }
}

