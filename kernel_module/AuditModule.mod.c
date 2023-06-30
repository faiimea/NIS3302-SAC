#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe914e41e, "strcpy" },
	{ 0x6f49ea72, "__nlmsg_put" },
	{ 0x69acdf38, "memcpy" },
	{ 0xb8048c60, "__netlink_kernel_create" },
	{ 0x8b9200fd, "lookup_address" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x92997ed8, "_printk" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x296695f, "refcount_warn_saturate" },
	{ 0xa916b694, "strnlen" },
	{ 0x5c4b2dd9, "__alloc_skb" },
	{ 0x61651be, "strcat" },
	{ 0x68031039, "init_net" },
	{ 0xdfb6d37d, "netlink_unicast" },
	{ 0x55cbf8b0, "kfree_skb_reason" },
	{ 0xb20b227e, "kern_path" },
	{ 0xf9c0b663, "strlcat" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xfe0edd20, "pv_ops" },
	{ 0x5eb9726b, "sock_release" },
	{ 0xfcca5424, "register_kprobe" },
	{ 0x63026490, "unregister_kprobe" },
	{ 0x754d539c, "strlen" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x541a6db8, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "1604098250DDA7EF6259E3C");
