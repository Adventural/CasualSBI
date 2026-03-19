#ifndef _SBI_PLIC_H_
#define _SBI_PLIC_H_

#include <stdint.h>

#define PLIC_PRIORITY_BASE      0x000000  // 中断源优先级 (每个源 4 字节)
#define PLIC_PENDING_BASE       0x001000  // 中断待处理位 (位掩码)
#define PLIC_ENABLE_BASE        0x002000  // 中断使能 (每个 Context 间隔 0x80)
#define PLIC_CONTEXT_BASE       0x200000  // 阈值与 Claim/Complete (每个 Context 间隔 0x1000)

// 每个 Context 内部的偏移
#define PLIC_CONTEXT_THRESHOLD  0x00
#define PLIC_CONTEXT_CLAIM      0x04

typedef struct {
    uint64_t base;
} plic_t;

// 设置特定中断源的优先级 (1-7)
void plic_set_priority(plic_t *plic, uint32_t irq, uint32_t priority);
// 设置特定 Context 的屏蔽阈值
void plic_set_threshold(plic_t *plic, uint32_t context, uint32_t threshold);
void plic_irq_enable(plic_t *plic, uint32_t context, uint32_t irq);
// 获取当前挂起的最高优先级中断号
uint32_t plic_claim(plic_t *plic, uint32_t context);
// 通知 PLIC 中断已处理完成
void plic_complete(plic_t *plic, uint32_t context, uint32_t irq);
void plic_init(plic_t *plic);

#endif
