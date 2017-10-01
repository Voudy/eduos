#ifndef EDUOS_OS_IRQ_H
#define EDUOS_OS_IRQ_H

#define IRQ_MAX_SHIFT 32
#define IRQ_ALL ((1 << IRQ_MAX_SHIFT) - 1)

typedef unsigned long irqmask_t;

extern irqmask_t irq_disable(void);
extern void irq_enable(irqmask_t mask);

extern int irq_init(void);

extern void (*irq_hnd)(void);

#endif /* EDUOS_OS_IRQ_H */


