#!/bin/bash

# 检查是否支持 KVM
if ! grep -q vmx /proc/cpuinfo; then
    echo "CPU不支持 KVM,无法创建虚拟机"
    exit 1
fi

# 创建虚拟机磁盘
qemu-img create -f qcow2 vm.qcow2 20G

# 启动虚拟机
qemu-system-x86_64 \
    -enable-kvm \
    -m 4096 \
    -cpu host \
    -smp 4 \
    -drive file=vm.qcow2,format=qcow2 \
    -cdrom ubuntu-20.04.6-desktop-amd64.iso \
    -boot d \
    -display gtk \
    -vnc :0 \
    -monitor stdio

# 连接到虚拟机控制台
while true; do
    read -p "输入命令 (help查看帮助): " cmd
    case "$cmd" in
        help)
            echo "可用命令:"
            echo "  stop - 停止虚拟机"
            echo "  pause - 暂停虚拟机"
            echo "  resume - 恢复虚拟机"
            echo "  info - 显示虚拟机信息"
            echo "  quit - 退出"
            ;;
        stop)
            echo "正在停止虚拟机..."
            quit
            ;;
        pause)
            echo "正在暂停虚拟机..."
            (echo "stop") >&3
            ;;
        resume)
            echo "正在恢复虚拟机..."
            (echo "cont") >&3
            ;;
        info)
            (echo "info") >&3
            ;;
        quit)
            echo "退出中..."
            exit 0
            ;;
        *)
            echo "未知命令,请重试"
            ;;
    esac
done 3<&0
