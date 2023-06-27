#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <asm/pgtable.h>
#include <asm/uaccess.h>
#include <asm/ptrace.h>

/*
** module macros
*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("infosec-sjtu");
MODULE_DESCRIPTION("hook sys_call_table");


typedef void (* demo_sys_call_ptr_t)(void);
//asmlinkage 
//pt_regs store the regs_info in stack
//function pointer and the para is regs(pt_regs)
typedef asmlinkage long (*orig_openat_t)(struct pt_regs *regs);
typedef asmlinkage long (*orig_read_t)(struct pt_regs *regs);

demo_sys_call_ptr_t *get_syscall_table(void);


int AuditOpenat(struct pt_regs *, char * pathname, int ret);

void netlink_release(void);
void netlink_init(void);



demo_sys_call_ptr_t * sys_call_table = NULL;
orig_openat_t orig_openat=NULL;

unsigned int level;
pte_t *pte;


asmlinkage long hacked_openat(struct pt_regs *regs)
{
	
	long ret;
	char buffer[PATH_MAX];
	unsigned long user_addr = regs->bx;
    long nbytes;

    // regs->bx --> buffer --> pathname --> fullname
  	nbytes=strncpy_from_user(buffer,(char*)regs->bx,PATH_MAX);
  	//printk("Buffer content2: %s\n", buffer);
   	if(nbytes!=-14){printk("Info:   hooked sys_openat(), file name:%s(%ld bytes)",buffer,nbytes);}


	ret = orig_openat(regs);
	//用于记录openat系统调用信息的函数
	AuditOpenat(regs,buffer,ret);

  	return ret;
}





static int __init audit_init(void)
{
	
	sys_call_table = get_syscall_table();
	
	printk("Info: sys_call_table found at %lx\n",(unsigned long)sys_call_table) ;
	// HERE!!!!
    // Hook Sys Call Openat
    // 将sys_call_table中的openat系统调用的原始函数保存到orig_openat变量中。
	orig_openat = (orig_openat_t) sys_call_table[__NR_openat];
	printk("Info:  orginal openat:%lx\n",(long)orig_openat);

	pte = lookup_address((unsigned long) sys_call_table, &level);
        // 查找sys_call_table对应的页表项，并将其设置为可写。
        set_pte_atomic(pte, pte_mkwrite(*pte));
        printk("Info: Disable write-protection of page with sys_call_table\n");
	// 将sys_call_table中的openat系统调用的函数指针设置为指向钩子函数hacked_openat
        sys_call_table[__NR_openat] = (demo_sys_call_ptr_t) hacked_openat;
	// 将页表项恢复为只读。
        set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
        printk("Info: sys_call_table hooked!\n");
	// 初始化一个用于通信的Netlink套接字。
        netlink_init();
        return 0;
}


static void __exit audit_exit(void)
{
    //查找sys_call_table对应的页表项，并将其设置为可写。
    pte = lookup_address((unsigned long) sys_call_table, &level);
    set_pte_atomic(pte, pte_mkwrite(*pte));
//将sys_call_table中的openat系统调用的函数指针恢复为指向原始函数。
	sys_call_table[__NR_openat] = (demo_sys_call_ptr_t)orig_openat;
//将页表项恢复为只读。
	set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
//释放Netlink套接字。
    netlink_release();
}

// 模块的初始化函数+清理函数。
module_init(audit_init);
module_exit(audit_exit);
