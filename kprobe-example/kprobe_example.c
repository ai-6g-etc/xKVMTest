
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kvm_host.h>

// 定义一个kprobe结构体
static struct kprobe kp;

// kprobe的处理函数
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    struct kvm_vcpu *vcpu = (struct kvm_vcpu *)regs->di;
    pr_info("VCPU created: %d\n", vcpu->vcpu_id);
    return 0;
}

// 初始化模块
static int __init kprobe_example_init(void)
{
    int ret;

    // 初始化kprobe结构体
    kp.pre_handler = handler_pre;
    kp.symbol_name = "kvm_arch_vcpu_create";

    // 注册kprobe
    ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("register_kprobe failed, returned %d\n", ret);
        return ret;
    }

    pr_info("Planted kprobe at %p\n", kp.addr);

    return 0;
}

// 清理模块
static void __exit kprobe_example_exit(void)
{
    // 取消注册kprobe
    unregister_kprobe(&kp);
    pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_example_init)
module_exit(kprobe_example_exit)
MODULE_LICENSE("GPL");
