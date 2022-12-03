#include <system.h>
#include <stdint.h>

// This will keep track of how many ticks that he system has been running for.
int timer_tick = 0;
int color = 1;

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
void timer_handler(struct regs *r)
{
    // Increment our tick count
    timer_tick++;
}

/* This will continuously loop until the given time has
*  been reached */
void sleep(int ticks)
{
    unsigned long eticks;

    eticks = timer_tick + ticks;
    while(timer_tick < eticks);
}

// Sets up the system clock by installing the timer handler into irq0
void timer_install(void)
{
    // installs 'timer_handler' to irq0
    irq_install_handler(0, timer_handler);
}