Kernel0x1 (Easy)
----------------------------------------
I have been eagerly waiting the day when we replace hard drives with Non-volatile RAMs to achieve lightning-fast computing. So I tried to created a trivial kernel simulator with a simple shell interface to try this capability by creating and reading small files.

nc pwn.hackerspacejust.com 9966

nc pwn.hackerspacejust.com 9967

----------------------------------------
kernel



gcc -m32 kernel.c -o kernel