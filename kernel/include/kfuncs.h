#pragma once

void memory_init(void *mb2_addr);
void gdt_install();
void interrupts_install();
void drivers_install();
void disk_check();