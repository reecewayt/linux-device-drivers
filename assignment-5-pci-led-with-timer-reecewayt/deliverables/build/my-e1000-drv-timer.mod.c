#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
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
	{ 0xdc658e53, "module_layout" },
	{ 0x14632f1f, "param_ops_int" },
	{ 0xbfeffc40, "device_destroy" },
	{ 0x8c422527, "pci_unregister_driver" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x6f021618, "class_destroy" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xc6e5a5ee, "__pci_register_driver" },
	{ 0x954ad0e5, "cdev_del" },
	{ 0xef4c61eb, "device_create" },
	{ 0xfbb5c7d2, "cdev_add" },
	{ 0xf4837e68, "cdev_init" },
	{ 0xe05148c6, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xa78af5f3, "ioread32" },
	{ 0xdc0e4855, "timer_delete" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x4a453f53, "iowrite32" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xedc03953, "iounmap" },
	{ 0x2654cfa7, "pci_release_selected_regions" },
	{ 0x674f386f, "pci_disable_device" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0xde80cd09, "ioremap" },
	{ 0xf4b07c4d, "pci_request_selected_regions" },
	{ 0xef044e40, "pci_select_bars" },
	{ 0xc2b1c71c, "pci_enable_device" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v00008086d0000100Esv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "33B0CD5A7DA008E5F9A34E3");
