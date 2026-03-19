#include "plic.h"
#include <arch/riscv/arch.h>

// 设置特定中断源的优先级 (1-7)
void plic_set_priority(plic_t *plic, uint32_t irq, uint32_t priority) 
{
    *(volatile uint32_t*)(plic->base + PLIC_PRIORITY_BASE + irq * 4) = priority;
}

// 设置特定 Context 的屏蔽阈值
void plic_set_threshold(plic_t *plic, uint32_t context, uint32_t threshold) 
{
    uintptr_t addr = plic->base + PLIC_CONTEXT_BASE + context * 0x1000;
    *(volatile uint32_t*)(addr + PLIC_CONTEXT_THRESHOLD) = threshold;
}

void plic_irq_enable(plic_t *plic, uint32_t context, uint32_t irq) 
{
    uintptr_t addr = plic->base + PLIC_ENABLE_BASE + context * 0x80 + (irq / 32) * 4;
    *(volatile uint32_t*)addr |= (1 << (irq % 32));
}

// 获取当前挂起的最高优先级中断号
uint32_t plic_claim(plic_t *plic, uint32_t context)
{
    uintptr_t addr = plic->base + PLIC_CONTEXT_BASE + context * 0x1000;
    return *(volatile uint32_t*)(addr + PLIC_CONTEXT_CLAIM);
}

// 通知 PLIC 中断已处理完成
void plic_complete(plic_t *plic, uint32_t context, uint32_t irq)
{
    uintptr_t addr = plic->base + PLIC_CONTEXT_BASE + context * 0x1000;
    *(volatile uint32_t*)(addr + PLIC_CONTEXT_CLAIM) = irq;
}

void plic_init(plic_t *plic) 
{
    uint64_t base = plic->base;

    for (int i = 1; i < 1024; i++) {
        *(volatile uint32_t*)(base + PLIC_PRIORITY_BASE + i * 4) = 0;
    }
}
