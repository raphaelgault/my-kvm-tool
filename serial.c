#include <stdio.h>
#include <linux/kvm.h>
#include "serial.h"

void init_uart_regs(struct uart_regs *regs)
{
    regs->lsr = (1 << 6) | (1 << 5);
}

void serial_uart_handle_io(struct uart_regs *regs,
                           struct io *io,
                           struct kvm_run *run_data)
{
    char *data = (char *)(run_data) + io->data_offset;

    int index = io->port - SERIAL_UART_BASE_ADDR;
    __u8 *uart = (__u8*)regs;

    switch(index) {
        case 0:
            if (io->direction == KVM_EXIT_IO_IN) {
                *data = regs->rbr;
            }
            else {
                regs->thr = *data;
                putchar(*data);
            }
            break;
        case 1:
            break;
        case 2:
            if (io->direction == KVM_EXIT_IO_IN)
                *data = regs->iir;
            else
                regs->fcr = *data;
            break;
        case 3:
            if (io->direction == KVM_EXIT_IO_IN)
                *data = regs->lcr;
            else
                regs->lcr = *data;
            break;
        case 4:
            if (io->direction == KVM_EXIT_IO_IN)
                *data = regs->mcr;
            else
                regs->mcr = *data;
            break;
        case 5:
            if (io->direction == KVM_EXIT_IO_IN)
                *data = regs->lsr;
            break;
        case 6:
            if (io->direction == KVM_EXIT_IO_IN)
                *data = regs->msr;
            break;
        case 7:
            if (io->direction == KVM_EXIT_IO_IN)
                *data = regs->sr;
            else
                regs->sr = *data;
            break;
    }
}
