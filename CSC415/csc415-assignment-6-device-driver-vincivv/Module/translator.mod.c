#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe8213e80, "_printk" },
	{ 0x578d7688, "__register_chrdev" },
	{ 0x316d45e6, "class_create" },
	{ 0x52b15b3b, "__unregister_chrdev" },
	{ 0x298b19e0, "device_create" },
	{ 0x39797361, "device_destroy" },
	{ 0xe7907964, "class_destroy" },
	{ 0xc609ff70, "strncpy" },
	{ 0x3e565e15, "strcmp" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xaa47b76e, "__arch_copy_from_user" },
	{ 0x0e9cab28, "memset" },
	{ 0x9c4ed43a, "alt_cb_patch_nops" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x30c65558, "strnlen" },
	{ 0xfbe7861b, "memcpy" },
	{ 0xe54e0a6b, "__fortify_panic" },
	{ 0xaa47b76e, "__arch_copy_to_user" },
	{ 0xdc1bea0d, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xe8213e80,
	0x578d7688,
	0x316d45e6,
	0x52b15b3b,
	0x298b19e0,
	0x39797361,
	0xe7907964,
	0xc609ff70,
	0x3e565e15,
	0x90a48d82,
	0xaa47b76e,
	0x0e9cab28,
	0x9c4ed43a,
	0xd272d446,
	0x546c19d9,
	0xa61fd7aa,
	0x30c65558,
	0xfbe7861b,
	0xe54e0a6b,
	0xaa47b76e,
	0xdc1bea0d,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"_printk\0"
	"__register_chrdev\0"
	"class_create\0"
	"__unregister_chrdev\0"
	"device_create\0"
	"device_destroy\0"
	"class_destroy\0"
	"strncpy\0"
	"strcmp\0"
	"__ubsan_handle_out_of_bounds\0"
	"__arch_copy_from_user\0"
	"memset\0"
	"alt_cb_patch_nops\0"
	"__stack_chk_fail\0"
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"strnlen\0"
	"memcpy\0"
	"__fortify_panic\0"
	"__arch_copy_to_user\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B12A21AD510A8C492D64219");
