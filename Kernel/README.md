Kernel0x1 (100 points)
----------------------------------------
I have been eagerly waiting the day when we replace hard drives with Non-volatile RAMs to achieve lightning-fast computing. So I tried to created a trivial kernel simulator with a simple shell interface to try this capability by creating and reading small files.

`nc pwn.hackerspacejust.com 9966`

`nc pwn.hackerspacejust.com 9967`

----------------------------------------

[![kernel][1]][1_url]

[1]: https://img.shields.io/badge/-kernel-blue?style=for-the-badge&logo=

[1_url]: kernel


<br>
<br>
<br>

### compiled with :

```bash
$ gcc -m32 kernel.c -o kernel
```