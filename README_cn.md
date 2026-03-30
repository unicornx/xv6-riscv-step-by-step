**循序渐进，学习开发 xv6-riscv**

<!-- TOC -->

- [1. 课程简介](#1-课程简介)
- [2. 课程在线资源](#2-课程在线资源)
	- [2.1. 课程代码仓库以及课件等](#21-课程代码仓库以及课件等)
	- [2.2. 在线慕课 B 站地址](#22-在线慕课-b-站地址)
	- [2.3. QQ 学习群](#23-qq-学习群)
- [3. 课程内容](#3-课程内容)
- [4. 使用说明](#4-使用说明)
	- [4.1. 下载本仓库](#41-下载本仓库)
	- [4.2. 仓库内容简介](#42-仓库内容简介)
	- [4.3. 开发环境搭建](#43-开发环境搭建)
	- [4.4. 构建和运行](#44-构建和运行)
- [5. 参考资料](#5-参考资料)

<!-- /TOC -->

# 1. 课程简介

本课程是 [《循序渐进，学习开发一个 RISC-V 上的操作系统》][1] （下文简称 “前置课程”）的进阶课程。前置课程中采用循序渐进的方式为大家介绍了如何从零开始徒手打造一个用于教学目的的、支持 RISC-V 架构的简易实时操作系统 RVOS。该课程自 2021 年春上线以来受到广大读者学生好评，并提出了很多宝贵改进意见，特别是建议增强 RVOS 的支持功能，最好能加上对虚拟内存，文件系统等高校操作系统课程中都会涉及的内容的介绍。本课程尝试响应热心读者的建议，但并没有在原 RVOS 的基础上重新开发，而是借用了 [xv6-riscv][2] 这个广受好评的开源教学用 Unix-like 系统来作为本门进阶课程的讲授内容。

[xv6-riscv][2] 是 MIT 开发的一个用于服务于操作系统课程教学（MIT 的 6.1810 课程）的操作系统，它将经典的 UNIX Version 6 移植到现代的 RISC-V 处理器架构上。[xv6-riscv][2] 使用 ANSI C 编写，支持 RISC-V 多核处理器；在设计上沿用了经典的 UNIX 哲学，内核提供了文件、进程、管道的抽象，以及熟悉的 Shell 环境，和 RVOS 类似，[xv6-riscv][2] 直接通过 QEMU 模拟器运行，方便学生在自己的电脑上调试运行。笔者窃以为将 [xv6-riscv][2] 直接拿来作为 RVOS 的进阶版本是自然且再好不过的了。

本课程在教学设计上依然沿袭了 “前置课程” 所引入的 “循序渐进” 风格特色，试图讲解如何从零开始逐步打造出一个完整的 xv6 系统，并在每一步开发设计中融入对操作系统以及 RISC-V 相关背景知识的介绍。笔者坚信在面对复杂的知识体系面前，陡峭的学习路径很容易让人在初期的挫败感中放弃尝试，而平滑的学习路径则是改善这一体验的绝佳实践。所以本课程经过精心设计后，尝试围绕操作系统（也是 [xv6-riscv][2] 力图实现的）的三大核心能力：虚拟化（Virtualization）、并发（Concurrency）和持久性（Persistence），将 xv6 的代码彻底拆解后重新带领大家从零将其组装一遍，通过这个过程希望能够让大家深入地体会操作系统到底是什么、它实现了哪些功能以及它是如何实现这些功能的。

# 2. 课程在线资源

## 2.1. 课程代码仓库以及课件等

<https://gitee.com/unicornx/xv6-riscv-step-by-step>

或者：

<https://github.com/unicornx/xv6-riscv-step-by-step>

## 2.2. 在线慕课 B 站地址

- [《循序渐进，学习开发 xv6-riscv (Part 1 多任务调度)》](https://www.bilibili.com/video/BV1tK576AEQ8)

- [《循序渐进，学习开发 xv6-riscv (Part 2 进程)》](https://www.bilibili.com/video/BV1oRuy61ERc)

- 《循序渐进，学习开发 xv6-riscv (Part 3 文件系统)》  : 敬请期待 ...

欢迎扫码关注慕课 B 站主页： 

![](./b.png)

## 2.3. QQ 学习群

欢迎加入本课程的 QQ 学习群 (群号：799027025，沿用自前置课程)，边学习边讨论：

![](./qq-group-2.png)

# 3. 课程内容

基于以上目的，本课程分为三大阶段，分别关注 《Operating Systems: Three Easy Pieces》中所提到的操作系统核心特征如下：

- “多任务调度” 部分：关注操作系统的 “CPU 虚拟化”。
- “进程” 部分：关注操作系统的 “内存虚拟化”。
- “文件系统” 部分：关注操作系统的 “持久性”。

而操作系统中 “并发与同步” 的问题在三个阶段中都会涉及。

课程从进程部分开始将引入用户态和内核态的划分，所以从进程部分开始我们将引入系统调用的概念并逐步实现 xv6 中提供的 21 个系统调用，其中进程部分实现一部分，文件系统部分实现剩余部分。

上述三大阶段依次叠加实现，每个部分又各自围绕自己的主题递进展开，代码仓库的提交历史也按照 “循序渐进” 的原则展开对 xv6 的开发，下面列出代码提交历史和 MOOC 视频集数之间的对应关系（代码提交历史即在代码仓库路径下执行 `git log --oneline --reverse` 从上往下顺序显示所列出的 commit 的 标题文本）。

“多任务调度” 部分：

| Commit 标题                            | 慕课视频集标题                     |
|----------------------------------------|-------------------------------------|
| Initial version                        |                                     |
| Boot from machine to supervisor.       | 第 2 章 启动（Boot）                |
| Print hello by UART.                   | 第 3 章 Hello xv6                   |
| Page-based physical memory management. | 第 4 章 物理内存管理                |
| First kernel task/proc.                | 第 5 章 第一个任务 part1/part2      |
| Cooperative multi-tasks.               | 第 6 章 协作式多任务                |
| Kernel trap.                           | 第 7 章 陷阱（Trap）part1/part2     |
| External interrupt.                    | 第 8 章 外部设备中断 part1/part2    |
| Hardware timer interrupt.              | 第 9 章 硬件定时器中断              |
| Preemptive multi-tasks.                | 第 10 章 抢占式多任务               |
| Spinlock & example 1.                  | 第 11 章 自旋锁 part1/part2/part3   |
| Spinlock example 2                     | 第 11 章 自旋锁 part3               |
| Spinlock example 3                     | 第 11 章 自旋锁 part3               |
| Support SMP.                           | 第 12 章 多核处理 part1/part2/part3 |
| RVOS2                                  | 第 13 章 RVOS2                      |

“进程” 部分：

| Commit 标题                            | 慕课视频集标题                     |
|----------------------------------------|-------------------------------------|
| Create kernel address space.           | 第 14 章 页表 part4/part5           |
| Process: have its own page table       | 第 15 章 进程 part1                 |
| Process: handle trap from user mode    | 第 15 章 进程 part1/part2           |
| Process: first user-mode process       | 第 15 章 进程 part3                 |
| Process: improve the build of initcode.| 第 15 章 进程 part3                 |
| Syscall: infrastructure                | 第 16 章 系统调用                   |
| Syscall: sys_write                     | 第 16 章 系统调用                   |
| Syscall: automatically generate usys.S | 第 16 章 系统调用                   |
| Syscall: sys_getpid                    | 第 16 章 系统调用                   |
| Sleep & Wakeup: infrastructure         | 第 17 章 进程的睡眠与唤醒 part1     |
| Syscall: sys_pause & sys_uptime        | 第 17 章 进程的睡眠与唤醒 part1     |
| Syscall: sys_read                      | 第 17 章 进程的睡眠与唤醒 part2     |
| A shell in user mode                   | 第 17 章 进程的睡眠与唤醒 part2     |
| Syscall: sys_exit                      | 第 18 章 进程的创建执行和回收 part1 |
| Syscall: sys_exec (I)                  | 第 18 章 进程的创建执行和回收 part2 |
| Syscall: sys_exec (II)                 | 第 18 章 进程的创建执行和回收 part1 |
| Syscall: sys_fork                      | 第 18 章 进程的创建执行和回收 part3 |
| Syscall: sys_wait                      | 第 18 章 进程的创建执行和回收 part3 |
| Syscall: sys_kill                      | 第 18 章 进程的创建执行和回收 part3 |
| Syscall: sys_sbrk                      | 第 19 章 进程和堆-part1             |
| Syscall: enhanced sys_sbrk             | 第 19 章 进程和堆-part2             |
| Implement a simple ram filesystem      | 第 20 章 基于内存的文件系统         |
| Add more user applications to ramfs.   | 第 20 章 基于内存的文件系统         |


**注**：由于时间有限（本课程完全是笔者工作业余时间开发），所以目前只公开发布了 “多任务调度” 和 “进程” 部分的内容（包括课件，代码以及视频）。“文件系统” 部分何时完成，还是个未知数，对此笔者深表歉意，敬请大家耐心等待 :-)。

本课程代码目前对齐 [Github 上 xv6-riscv 代码仓库][3] 的 commit 为：`5474d4b "Makefile: discover riscv64-none-elf- toolchain (#439)"`（基本一致，除了一些用户态的测试代码外）。大致和 6.1810 这门课的 2025 年秋季班内容对齐。

# 4. 使用说明

## 4.1. 下载本仓库

本仓库地址：<https://gitee.com/unicornx/xv6-riscv-step-by-step>

第一次下载本仓库：`git clone git@gitee.com:unicornx/xv6-riscv-step-by-step.git`。

**注意**：后续更新已有仓库时不能使用常用的 pull。这是因为本仓库代码出于演示 Step by Step 的修改过程的目的，main 分支的 commit 仅用于演示步骤而不反应实际 git 提交历史，远程上游仓库有代码更新时 main 分支会被强制更新（`push -f`）。所以建议采用直接强制重置到远程最新状态的方式对已有仓库进行更细。

首先确定自己在本地的 main 分支上没有需要保留的修改（自己的修改建议都放在自己新建的开发分支上），然后执行如下命令：

```shell
# 确保当前工作在 main 分支
git checkout main

# 获取远程最新信息
git fetch origin

# 将本地 main 分支强制指向远程 main
git reset --hard origin/main
```

执行完后，本地 main 就和远程完全一致了。

当然也可以删除原仓库目录，然后按照第一次下载的方式直接再完整 clone 一次。

## 4.2. 仓库内容简介

本仓库的目录安排和 [xv6-riscv 代码仓库][3] 基本保持一致。只是在它的基础上增加了以下目录和文件：

- `references`: 本课程涉及的参考资料，其中子目录 `6.1810` 下的内容来自 MIT 6.828 官网 <https://pdos.csail.mit.edu/6.828/> 的 “References”。子目录 `riscv-spec` 的内容是一些课程用到的 RISC-V 的 specification 规范文档。
- `slides`: 本课程在线慕课讲授时用到的课件。大家在学习慕课时请先对照看一下该目录下的勘误文档 `errata.pdf`。
- `README_cn.md`: 即本文件。

## 4.3. 开发环境搭建

本课程代码在 Ubuntu 24.04.4 LTS 上验证通过。

```shell
$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 24.04.4 LTS
Release:	24.04
Codename:	noble
```

需要安装以下软件包和开发工具：

| 软件包    | 安装命令 |
|-----------|----------|
| Make      | sudo apt install build-essential |
| Toolchain | sudo apt install gcc-riscv64-unknown-elf |
| QEMU      | sudo apt install qemu-system-misc |
| GDB       | sudo apt install gdb-multiarch |

更多环境的搭建问题请阅读课件的 ["第一章 建立开发环境"](./slides/ch01-setup-environment.pdf)。

## 4.4. 构建和运行

本课程的代码构建和运行方式和 [xv6-riscv 代码仓库][3] 的方式基本一致。常用命令如下：

- `make` ：构建内核 kernel

- `make qemu` : 确保构建完整系统（不同的步骤可能包括 kernel、文件系统 fs.img 以及用户态程序中的一部分乃至全部）并启动 QEMU 运行系统。

- `make qemu-gdb`: 确保构建完整系统并启动 QEMU 调试系统。

- `make clean`: 清理工作环境。

# 5. 参考资料

- MIT 6.1810: Operating System Engineering: <https://pdos.csail.mit.edu/6.828/>。部分课程资料和本课程涉及的 RISC-V 标准规范文件已下载备份在本课程的 references 目录下。
- 《Operating Systems: Three Easy Pieces》：<https://pages.cs.wisc.edu/~remzi/OSTEP/>。
- 《循序渐进，学习开发一个 RISC-V 上的操作系统》：<https://gitee.com/unicornx/riscv-operating-system-mooc>。
- 《xv6: a simple, Unix-like teaching operating system 中文翻译》: <https://gitee.com/unicornx/xv6-riscv-book-cn>。

再次感谢 MIT 的 PDOS 实验室为大家带来的 6.1810 精彩课程以及网络上的开源资源！

祝大家学习愉快，学有所成！

本课程内容如有错误都由本人负责，任何课程相关问题欢迎联系我：

- 姓名：汪辰
- 电子邮箱：unicorn_wang@outlook.com

或者给本课程 [提 issue][4]。


[1]: https://gitee.com/unicornx/riscv-operating-system-mooc
[2]: https://pdos.csail.mit.edu/6.828/
[3]: https://github.com/mit-pdos/xv6-riscv
[4]: https://gitee.com/unicornx/xv6-riscv-step-by-step/issues