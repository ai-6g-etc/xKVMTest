#include <sys/eventfd.h>
#include <linux/kvm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main() {
    int kvm_fd, vm_fd, vcpu_fd;
    struct kvm_irqfd irqfd = {
        .fd = eventfd(0, EFD_NONBLOCK),
        .flags = KVM_IRQFD_FLAG_DEASSIGN,
        .gsi = 0,
    };

    kvm_fd = open("/dev/kvm", O_RDWR);
    if (kvm_fd < 0) {
        perror("open /dev/kvm");
        return 1;
    }

    vm_fd = ioctl(kvm_fd, KVM_CREATE_VM, 0);
    if (vm_fd < 0) {
        perror("KVM_CREATE_VM");
        close(kvm_fd);
        return 1;
    }

    vcpu_fd = ioctl(vm_fd, KVM_CREATE_VCPU, 0);
    if (vcpu_fd < 0) {
        perror("KVM_CREATE_VCPU");
        close(vm_fd);
        close(kvm_fd);
        return 1;
    }

    if (ioctl(vm_fd, KVM_IRQFD, &irqfd) < 0) {
        perror("KVM_IRQFD");
        close(vcpu_fd);
        close(vm_fd);
        close(kvm_fd);
        return 1;
    }

    // Rest of your code

    close(irqfd.fd);
    close(vcpu_fd);
    close(vm_fd);
    close(kvm_fd);

    return 0;
}
