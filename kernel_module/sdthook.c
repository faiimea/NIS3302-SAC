#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

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
typedef asmlinkage long (*orig_openat_t)(struct pt_regs *regs);

demo_sys_call_ptr_t *get_syscall_table(void);


int AuditOpenat(struct pt_regs *, char * pathname, int ret);
int Auditfinitmodule(char * pathname);
int Auditdeletemodule(char * modulename);

void netlink_release(void);
void netlink_init(void);



demo_sys_call_ptr_t * sys_call_table = NULL;
orig_openat_t orig_openat=NULL,orig_finitmodule=NULL,orig_deletemodule=NULL;

unsigned int level;
pte_t *pte;


asmlinkage long hacked_openat(struct pt_regs *regs)
{

    long ret;
    char buffer[PATH_MAX];
    long nbytes;
   
    nbytes=strncpy_from_user(buffer,(char*)regs->si,PATH_MAX);

    if(nbytes>0){
        if(regs->si!=1){
            //printk(KERN_ERR "pathname inn hacked_openat: %lu\nand buffer now is:%s\n",regs->bx,buffer);
            //printk("Info: 1  hooked sys_openat(), file name:%s(%ld bytes)\n",buffer,nbytes);
        }
        ret = orig_openat(regs);

        if(strlen(buffer)!=0){
            AuditOpenat(regs,buffer,ret);}
    }
    else{
        ret = orig_openat(regs);
    }
    
    return ret;
}


int get_info_from_fd(unsigned int fd, unsigned long * ino, uid_t * uid, int *type,char* buf)
{
    struct file *file_p = NULL;
    struct inode * f_inode = NULL;
    struct fd f;
    char* buff;
      
  
    // 获取 uid	
    *uid = current_uid().val;   // unsigned int
    // 获取fd对应的file struct
    // file_p = fget_raw(fd);
    f = fdget(fd);
    file_p = f.file;
    if (!file_p)
    {
        // 获取失败
        return -1;
    }
    
    // 获取 ino
    f_inode = file_p->f_inode;
    *ino = f_inode->i_ino;
    
    //get path
    
    buff=d_path(&file_p->f_path, buf, PATH_MAX);
    strcpy(buf,buff);
    
    return 0;
}

asmlinkage long hacked_finitmodule(struct pt_regs *regs)
{
    long ret;
    char buffer[PATH_MAX];
    unsigned long ino = 0;
    uid_t uid = 0;
    int f_type = 0;
    
    get_info_from_fd(regs->di, &ino, &uid, &f_type,buffer);  
    
    if(strlen(buffer)>0){
        printk("ModuleInstalled:%s\n",buffer);
        Auditfinitmodule(buffer);
    }    
    ret = orig_finitmodule(regs);
    
    return ret;
}

asmlinkage long hacked_deletemodule(struct pt_regs *regs){
    long ret;
    char buffer[PATH_MAX];
   
    
    strncpy_from_user(buffer,(char*)regs->di,PATH_MAX);
    
    if(strlen(buffer)>0){
        printk("ModuleDeleted:%s\n",buffer);
        Auditdeletemodule(buffer);
    }
    else{printk(KERN_ERR "ModuleDeletedFailedtoResolve\n");}
    ret = orig_deletemodule(regs);
    return ret;
}



static int __init audit_init(void)
{
	printk(KERN_ERR "start init\n");
	sys_call_table = get_syscall_table();
	if (sys_call_table==NULL){
		printk(KERN_ERR "FATAL ERROR\n");
		return -1;
	}
	
	printk("Info: sys_call_table found at %lx\n",(unsigned long)sys_call_table) ;

        //Hook Sys Call Openat
	orig_openat = (orig_openat_t) sys_call_table[__NR_openat];
	printk("orginal openat:%lx\n",(long)orig_openat);
	orig_finitmodule = (orig_openat_t) sys_call_table[__NR_finit_module];
	printk("orginal finitmodule:%lx\n",(long)orig_finitmodule);
	orig_deletemodule = (orig_openat_t) sys_call_table[__NR_delete_module];
	printk("orginal deletemodule:%lx\n",(long)orig_deletemodule);

	pte = lookup_address((unsigned long) sys_call_table, &level);
        // Change PTE to allow writing
        set_pte_atomic(pte, pte_mkwrite(*pte));
        printk("Info: Disable write-protection of page with sys_call_table\n");

        sys_call_table[__NR_openat] = (demo_sys_call_ptr_t) hacked_openat;
        printk("__NR_openat changed\n");
	sys_call_table[__NR_finit_module] = (demo_sys_call_ptr_t) hacked_finitmodule;
	printk("__NR_finit_module changed\n");
	sys_call_table[__NR_delete_module] = (demo_sys_call_ptr_t) hacked_deletemodule;
	printk("__NR_delete_module changed\n");
	
	
        set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
        printk("Info: sys_call_table hooked!\n");

        netlink_init();
        return 0;
}


static void __exit audit_exit(void)
{

    pte = lookup_address((unsigned long) sys_call_table, &level);
    set_pte_atomic(pte, pte_mkwrite(*pte));

	sys_call_table[__NR_openat] = (demo_sys_call_ptr_t)orig_openat;
	sys_call_table[__NR_finit_module] = (demo_sys_call_ptr_t)orig_finitmodule;
	sys_call_table[__NR_delete_module] = (demo_sys_call_ptr_t)orig_deletemodule;
	
	set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));


    netlink_release();
}

module_init(audit_init);
module_exit(audit_exit);
