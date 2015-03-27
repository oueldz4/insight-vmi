# Introduction #

This page shows a collection of mean pointer tricks that the kernel uses. All of these examples cause some headaches for InSight.


# Pointers as error codes #

http://lxr.linux.no/#linux+v2.6.32.58/mm/shmem.c#L2626

Global variable `shm_mnt` of type `struct vfsmount *` is returned as an error code (checked by macro `IS_ERR(shm_mnt)`):

As a result, a candidate is detected for `shm_mnt` that should not be there:
```
Found variable with name shm_mnt:
  ID:             0xb4d49b
  Name:           shm_mnt
  Address:        0xc1496cc0
  Type:           0xb3e347   struct vfsmount *
              <1> 0xa5d4     struct file *: (struct vfsmount *)
  Source file:    mm/shmem.c:34
```

# Type cast based on internal flag #

http://lxr.linux.no/#linux+v3.4.4/mm/rmap.c#L1002

Base on the result of function [PageAnon](http://lxr.linux.no/linux+v3.4.4/include/linux/mm.h#L806), `page.mapping` can be used as `struct anon_vma` or `struct address_space`:

```
Found type with name page:
  ID:             0x6043
  Name:           struct page
  Type:           Struct
  Size:           32
  Source file:    arch/x86/kernel/init_task.c:18
  Members:        6
    0x0c                0xa377     union (anon.)
    {
      0x0c              0xa24f     struct (anon.)
      {
        ...
        0x10  mapping:  0xa371     struct address_space *
                    <1> 0xdd13     struct inode *: ((struct page *)->mapping - 176)
                    <2> 0x1c47c    struct anon_vma *: ((struct page *)->mapping - 1)
                    <3> 0x603d     struct page *: (struct page *)->mapping
      }
      ...
    }
   ...
```

**Type rule handling this case:**
```
<rule priority="100">
	<name>page.mapping</name>
	<description>
		<!-- http://lxr.linux.no/#linux+v3.6.6/include/linux/mm.h#L769 -->
		On an anonymous page mapped into a user virtual memory area,
		page->mapping points to its anon_vma, not to a struct address_space;
		with the PAGE_MAPPING_ANON bit set to distinguish it.  See rmap.h.
	</description>
	<filter>
		<datatype>struct</datatype>
		<typename>page</typename>
		<members>
			<member></member>
			<member></member>
			<member>mapping</member>
		</members>
	</filter>
	<action type="inline">
		var mapping = arguments[0].mapping;
		if (!mapping.IsNull()) {
			var PAGE_MAPPING_ANON = 1;
			var PAGE_MAPPING_FLAGS = 3;
			var addr = mapping.AddressLow();
			if (addr &amp; PAGE_MAPPING_ANON)
				mapping.ChangeType("anon_vma");
			mapping.SetAddressLow(addr &amp; ~PAGE_MAPPING_FLAGS);
		}
		return mapping;
	</action>
</rule>
```

# Fields initialized with wrong types #

http://lxr.linux.no/#linux+v2.6.32.58/fs/file.c#L294

In this example, `fdtable.open_fds` is of type `fd_set*`. Both `fd_set` and `embedded_fd_set` have only one field, namely `fds_bits`. However, one is declared as `long[1]`, the other as `long[32]`:

```
>>> show files_struct
Found type with name files_struct:
  ID:             0x4ed474
  Name:           struct files_struct
  Type:           Struct
  Size:           224
  Source file:    kernel/fork.c:1027
  Members:        8
    ...
    0x4c  open_fds_init:      0x4f0c19 struct embedded_fd_set
    0x50  fd_array:           0x4f0ca6 struct file *[32]
```

```
>>> show files_struct.open_fds_init
Found type with name files_struct, showing files_struct.open_fds_init:
  ID:             0x4f0c19
  Name:           struct embedded_fd_set
  Type:           Struct
  Size:           4
  Source file:    kernel/fork.c:27
  Members:        1
    0x0  fds_bits: 0x4e6d4a long unsigned int[1]
```

```
>>> show fdtable.open_fds
Found type with name fdtable, showing fdtable.open_fds:
  ID:             0x102666
  Name:           fd_set *
  Type:           Pointer
  Size:           4
  Hash:           0xcd5932
  1. Ref. type:   0xf8818    fd_set
  2. Ref. type:   0xf8733    __kernel_fd_set
  3. Ref. type:   0xf870c    struct (anon.)
  Members:        1
    0x0  fds_bits: 0xf8723 long unsigned int[32]
```

The code in [line 313](http://lxr.linux.no/#linux+v2.6.32.58/fs/file.c#L313) assigns the 1-array-field to a 32-array-pointer. However, if we were to access `new_fdt->open_fds->fds_bits[1]` afterwards, we would not be accessing `newf->open_fds_init[1]`, but instead `newf->fd_array[0]`.

# Implicitely included data structures #

http://lxr.linux.no/#linux+v2.6.32/include/linux/prio_tree.h#L14

```
struct raw_prio_tree_node {
    struct prio_tree_node   *left;
    struct prio_tree_node   *right;
    struct prio_tree_node   *parent;
};

struct prio_tree_node {
    struct prio_tree_node   *left;
    struct prio_tree_node   *right;
    struct prio_tree_node   *parent;
    unsigned long           start;
    unsigned long           last; 
}
```

In the above example, `struct prio_tree_node` implicitely includes `struct raw_prio_tree_node`, but not as a field directly, but only implicitely through by starting with the same three members as the other type.

# Pointer derference based on flag (radix tree) #

http://lxr.linux.no/#linux+v2.6.32.58/lib/radix-tree.c#L102

Based on the last bit of the pointer that points to the next node in a radix tree, the pointer may either be directly dereferenceable or not.

```
static void *radix_tree_lookup_element(struct radix_tree_root *root,
                               unsigned long index, int is_slot)
{
        unsigned int height, shift;
        struct radix_tree_node *node, **slot;

        node = rcu_dereference(root->rnode);
        if (node == NULL)
                return NULL;

        if (!radix_tree_is_indirect_ptr(node)) {
               if (index > 0)
                        return NULL;
               return is_slot ? (void *)&root->rnode : node;
        }
       node = radix_tree_indirect_to_ptr(node);
 
       ...
```

In case the last bit is set, the pointer must be dereferenced using the following method:

```
static inline void *radix_tree_indirect_to_ptr(void *ptr)
{
       return (void *)((unsigned long)ptr & ~RADIX_TREE_INDIRECT_PTR);
}
```

**Type rule handling this case:**
```
<rule priority="101">
	<name>radix_tree_root.rnode</name>
	<description>
		Based on the last bit of the pointer that points to the next
		node in a radix tree, the pointer may either be directly
		dereferenceable or not.
	</description>
	<filter>
		<datatype>struct</datatype>
		<typename>radix_tree_root</typename>
		<members>
			<member>rnode</member>
		</members>
	</filter>
	<action type="inline">
		var RADIX_TREE_INDIRECT_PTR = 1;
		var root = arguments[0];
		var rnode = root.rnode;
		if (!rnode.IsNull()) {
			var addr_low = rnode.AddressLow();
			if (addr_low &amp; RADIX_TREE_INDIRECT_PTR)
				rnode.SetAddressLow(addr_low &amp; ~RADIX_TREE_INDIRECT_PTR);
		}
		return rnode;
	</action>
</rule>
```

# Percpu Variables #
http://lxr.linux.no/#linux+v2.6.32.58/include/linux/percpu.h#L191

Percpu variables are pointers that are not valid by themselves. Instead the pointer value that a percpu variable contains must be added to the the correct offset for the cpu that tries to access this variable. This offset is stored in the global array `__per_cpu_offset`. This gets especially nasty if a variable within a struct is not defined as a percpu variable, but is initialized as one as in the case of vfsmount.mnt\_writers:

```
struct vfsmount {
        struct list_head mnt_hash;
        struct vfsmount *mnt_parent;    /* fs we are mounted on */
        struct dentry *mnt_mountpoint;  /* dentry of mountpoint */
        struct dentry *mnt_root;        /* root of the mounted tree */
        struct super_block *mnt_sb;     /* pointer to superblock */
        struct list_head mnt_mounts;    /* list of children, anchored here */
        struct list_head mnt_child;     /* and going through their mnt_child */
        int mnt_flags;
        /* 4 bytes hole on 64bits arches */
        const char *mnt_devname;        /* Name of device e.g. /dev/dsk/hda1 */
        struct list_head mnt_list;
        struct list_head mnt_expire;    /* link in fs-specific expiry list */
        struct list_head mnt_share;     /* circular list of shared mounts */
        struct list_head mnt_slave_list;/* list of slave mounts */
        struct list_head mnt_slave;     /* slave list entry */
        struct vfsmount *mnt_master;    /* slave is on master->mnt_slave_list */
        struct mnt_namespace *mnt_ns;   /* containing namespace */
        int mnt_id;                     /* mount identifier */
        int mnt_group_id;               /* peer group identifier */
        /*
         * We put mnt_count & mnt_expiry_mark at the end of struct vfsmount
         * to let these frequently modified fields in a separate cache line
         * (so that reads of mnt_flags wont ping-pong on SMP machines)
         */
        atomic_t mnt_count;
        int mnt_expiry_mark;            /* true if marked for expiry */
        int mnt_pinned;
        int mnt_ghosts;
#ifdef CONFIG_SMP
        int *mnt_writers;
#else
        int mnt_writers;
#endif
};
```

mnt\_writers is defined as `*int`. But it is in fact a percpu variable:

http://lxr.linux.no/#linux+v2.6.32.58/fs/namespace.c#L128

```
struct vfsmount *alloc_vfsmnt(const char *name)
{
        struct vfsmount *mnt = kmem_cache_zalloc(mnt_cache, GFP_KERNEL);
        if (mnt) {
                int err;

                err = mnt_alloc_id(mnt);
                if (err)
                        goto out_free_cache;

                if (name) {
                        mnt->mnt_devname = kstrdup(name, GFP_KERNEL);
                        if (!mnt->mnt_devname)
                                goto out_free_id;
                }

                atomic_set(&mnt->mnt_count, 1);
                INIT_LIST_HEAD(&mnt->mnt_hash);
                INIT_LIST_HEAD(&mnt->mnt_child);
                INIT_LIST_HEAD(&mnt->mnt_mounts);
                INIT_LIST_HEAD(&mnt->mnt_list);
                INIT_LIST_HEAD(&mnt->mnt_expire);
                INIT_LIST_HEAD(&mnt->mnt_share);
                INIT_LIST_HEAD(&mnt->mnt_slave_list);
                INIT_LIST_HEAD(&mnt->mnt_slave);
#ifdef CONFIG_SMP
                mnt->mnt_writers = alloc_percpu(int);
                if (!mnt->mnt_writers)
                       goto out_free_devname;
#else
                mnt->mnt_writers = 0;
#endif
        }
...
```

More examples:
  * hd\_struct.dkstats: http://lxr.linux.no/#linux+v2.6.32.58/include/linux/genhd.h#L252
  * percpu\_counter.counters: http://lxr.linux.no/#linux+v2.6.32.58/lib/percpu_counter.c#L29


**Type rules handling these cases:**
```
<rule priority="90">
	<name>per_cpu__*</name>
	<description>Returns the first entry of a per-cpu variable for x86.</description>
	<filter>
		<variablename match="wildcard">per_cpu__*</variablename>
	</filter>
	<action type="inline">
		var inst = arguments[0];
		var offsets = new Instance("__per_cpu_offset");
		// Is this an SMP system?
		if (offsets.IsValid()) {
			var offset = offsets.ArrayElem(0);
			inst.AddToAddress(offset.toULong(16));
		}
		return inst;
	</action>
</rule>

<rule priority="101">
	<name>vfsmount.mnt_writers</name>
	<description>Member "mnt_writers" is a per-cpu variable.</description>
	<filter>
		<datatype>struct</datatype>
		<typename>vfsmount</typename>
		<members>
			<member>mnt_writers</member>
		</members>
	</filter>
	<action type="inline">
		var inst = arguments[0];
		var members = arguments[1];
		inst = inst.Member(members[0]);
		var offsets = new Instance("__per_cpu_offset");
		// Is this an SMP system?
		if (offsets.IsValid()) {
			var offset = offsets.ArrayElem(0);
			inst.AddToAddress(offset.toULong(16));
		}
		return inst;
	</action>
</rule>

<rule priority="101">
	<name>percpu_counter.counters</name>
	<description>Member "counters" is a per-cpu variable.</description>
	<filter>
		<datatype>struct</datatype>
		<typename>percpu_counter</typename>
		<members>
			<member>counters</member>
		</members>
	</filter>
	<action type="inline">
		var inst = arguments[0];
		var members = arguments[1];
		inst = inst.Member(members[0]);
		var offsets = new Instance("__per_cpu_offset");
		// Is this an SMP system?
		if (offsets.IsValid()) {
			var offset = offsets.ArrayElem(0);
			inst.AddToAddress(offset.toULong(16));
		}
		return inst;
	</action>
</rule>
```