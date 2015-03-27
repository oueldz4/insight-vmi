# About #



# What is InSight? #

InSight is a memory analysis tool for the IA-32 (x86) and AMD64 platform that makes all kernel objects of an operating system available for further analysis. It operates on physical memory dumps stored on disk or on the guest physical memory of a running virtual machine that a hypervisor makes available. Therefore InSight lends itself to applications of virtual machine introspection (VMI), digital forensic analysis, malware analysis, as well as kernel debugging.

InSight is written in C++ using the [Qt libraries](http://qt.nokia.com/) and runs on Linux. It supports memory analysis of Linux kernels and will be extended for Windows kernels in the future (see RoadMap). The source code is licensed under the terms and conditions of the <a href='http://www.gnu.org/licenses/old-licenses/gpl-2.0.html'>GNU GPL v2</a>.

This tool is developed by <a href='http://www.sec.in.tum.de/christian-schneider/'>Christian Schneider</a> and other contributors as part of <a href='http://www.sec.in.tum.de/leveraging-virtualization-techniques-for-system-security/'>their research</a> at the <a href='http://portal.mytum.de/welcome/?set_language=en'>Technische Universität München</a>, Munich, Germany. Together with his colleges at the <a href='http://www.sec.in.tum.de/'>Chair for IT Security</a>, his interest lies in the field of virtual machine introspection and how this can be used in novel ways to improve current intrusion detection methods.

# Background #

Operating systems organize their state in specific data structures within the **kernel memory**. The instances of such data structures represent the **kernel objects**. Kernel objects contain a number members, each of which has one of the following type:

  * a primitive type, such as an integer or a floating point value,
  * a composite type, such as another structure or union,
  * a pointer to a primitive or composite type,
  * a pointer to a function.

The values of the members with primitive types and function pointers actually represent the system state and steer the behavior of the kernel while the pointers and nested compositions link objects together and logically group the state information. This results in a **graph** of kernel objects as illustrated in the following figure:

![https://insight-vmi.googlecode.com/svn/wiki/images/kernel_graph.png](https://insight-vmi.googlecode.com/svn/wiki/images/kernel_graph.png)

The goal of InSight is to make this information available to applications that analyze the state of an operating system. The challenge of this task is that the layout and organization of the kernel memory within the physical memory varies among different operating systems and their particular versions. Without detailed knowledge about the operating system that is being analyzed, the physical memory of a machine only consists of zeros and ones, but has no meaning to an application. This problem has become well-known as the **semantic gap** in the field of virtual machine introspection.

InSight brings back the semantics into the zeros and ones by applying knowledge about the operating system to the raw physical memory. In doing so, InSight is able to re-create the kernel objects from the memory as the kernel would see them, including their values and their pointer linkage.

# Features #

InSight runs on Linux and provides the following functionality:

  * analysis of physical **memory dumps** stored on disk as well as
  * analysis of **live** physical memory of a **virtual machine** (if the hypervisor provides access to the guest's memory)
  * support for **32-bit** and **64-bit** addressing schemes with and without PAE
  * re-creation of **kernel objects** for the Linux kernel
  * automatic **de-referencing of pointers** to futher objects
  * application of **type casts** and **pointer arithmetic** as the kernel would do
  * an **[interactive shell](InSightShell.md)** for manual analysis of kernel objects
  * a **[JavaScript engine](ScriptingEngine.md)** for automated analysis of repeating or complex tasks

# How it works #

In order to extract kernel objects out of a machine's pyhsical memory, InSight requires and applies the following information to it:

  1. layout of the kernel's virtual memory
  1. layout and size of kernel data structures
  1. location of kernel objects in virtual memory
  1. dynamic type and pointer manipulations through the kernel
  1. function of the virtual-to-physical address translation

For a Linux system, InSight obtains (2) and (3) from the [debugging symbols](LinuxDebugSymbols.md) of the kernel begin analyzed. While the basic layout of the Linux kernel's address space (1) is public knowlege, the exact addresses and offsets of certain memory areas differ among kernel versions and addressing schemes. InSight extracts the exact locations from three data sources: the `System.map` file, the kernel header files, and the debugging symbols.

To capture the dynamic type and pointer manipulations (4) the kernel performs at runtime (type casts and pointer arithmetic), InSight performs a static analysis of the pre-processed source code of the kernel. This knowledge is then applied to derive the correct object types and addresses when objects are read, allowing InSight to read objects pointed to by `void*` pointers and following pointers stored as integer types.

The virtual-to-physical address translation (5) itself is specified by the Intel IA-32 and the AMD64 architecture specification which is publicly available.

With this information at hand, kernel objects can be read from physical memory. For example, when an application requests the object `init_task`, InSight performs the following steps:

  1. looking up `init_task` in the list of global variables
  1. finding the virtual adress of `init_task` to be _x_
  1. finding `init_task` to be of type `struct task_struct`
  1. creating an `Instance` object of type `struct task_struct` at the virtual address _x_

An application may now inspect the members of the `Instance` object and may choose to read their values. Suppose the value of member `pid` is to be read, InSight continues as follows:

  1. finding member `pid` to be of type `pid_t`
  1. resolving `pid_t` to be an alias for `unsigned int`
  1. calculating the address _y_ of member `pid` as the base address of the `Instance` object (which still holds the address _x_ of variable `init_task`) plus the offset of member `pid` within `struct init_task`
  1. translating the calcualted virtual address _y_ to the corresponding physical address _z_
  1. reading `sizeof(pid_t)` bytes from physical memory at address _z_
  1. interpreting it as type `unsigned int`

This basic mechanism enables InSight to follow any member, dereference any pointer and output any kernel object.