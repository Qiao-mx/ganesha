# NFS-Ganesha 数据结构模板文件归类

本文档对 nfs-ganesha 项目中可复用的数据结构实现文件进行归类整理，便于 C++转C 项目复用。

---

## 1. 红黑树 (Red-Black Tree) - STL改编版

基于 GNU ISO C++ Library STL 改编的纯C红黑树实现，使用宏定义方式实现所有操作。

| 文件 | 原始路径 | 说明 |
|------|----------|------|
| `rbt_node.h` | `src/include/rbt_node.h` | 红黑树节点结构定义，定义 `struct rbt_head`（树头）和 `struct rbt_node`（节点），使用 `rbt_value` (uint64_t) 排序 |
| `rbt_tree.h` | `src/include/rbt_tree.h` | 红黑树操作宏集合，包含 `RBT_INSERT`、`RBT_UNLINK`、`RBT_FIND`、`RBT_FIND_LEFT`、`RBT_ROTATE_LEFT/RIGHT`、`RBT_INCREMENT/DECREMENT`、`RBT_VERIFY` 等 |

**设计模式**: 非侵入式设计，`rbt_node` 嵌入到业务结构中，通过 `rbt_opaq` 指针关联外部对象。

**使用示例**:
```c
struct rbt_head tree;
RBT_HEAD_INIT(&tree);

struct rbt_node *found;
RBT_FIND(&tree, found, key_value);
RBT_INSERT(&tree, new_node, found);
RBT_UNLINK(&tree, node_to_remove);
```

---

## 2. 红黑树 (Red-Black Tree) - libtree版

基于 libtree 库实现的红黑树，使用函数（非宏）接口，支持父指针。

| 文件 | 原始路径 | 说明 |
|------|----------|------|
| `avltree.h` (rbtree部分) | `src/include/avltree.h` | `struct rbtree_node`、`struct rbtree` 定义及函数声明 |
| `rb.c` | `src/avl/rb.c` | 红黑树完整函数实现：`rbtree_insert`、`rbtree_remove`、`rbtree_lookup`、`rbtree_next/prev` 等 |

**设计模式**: 侵入式设计，通过 `rbtree_container_of()` 宏从节点获取外层容器。

---

## 3. AVL 树

基于 libtree 库的带父指针 AVL 树实现，自动平衡。

| 文件 | 原始路径 | 说明 |
|------|----------|------|
| `avltree.h` (avltree部分) | `src/include/avltree.h` | `struct avltree_node`、`struct avltree` 定义，内联 `avltree_lookup`、`avltree_insert`、`avltree_do_lookup` |
| `avl.c` | `src/avl/avl.c` | AVL 树函数实现：`avltree_do_insert`、`avltree_remove`、`avltree_next/prev`、`avltree_inf/sup` 等 |
| `test_avl.c` | `src/test/test_avl.c` | AVL 树单元测试 |
| `test_mh_avl.c` | `src/test/test_mh_avl.c` | AVL 树多头（multi-head）测试 |

**设计模式**: 侵入式设计，通过 `avltree_container_of()` 宏获取容器。使用平衡因子 `balance` 维护树平衡。

---

## 4. 二叉搜索树 (Binary Search Tree) - 线程化

基于 libtree 库的线程化二叉搜索树实现。

| 文件 | 原始路径 | 说明 |
|------|----------|------|
| `avltree.h` (bstree部分) | `src/include/avltree.h` | `struct bstree_node`、`struct bstree` 定义 |
| `bst.c` | `src/avl/bst.c` | BST 函数实现：`bstree_insert`、`bstree_remove`、`bstree_lookup`、`bstree_next/prev` 等 |

**设计模式**: 使用 `uintptr_t` 技巧在指针低位存储线程标志（threaded flag），节省空间。

---

## 5. Splay 树 (自适应伸展树)

基于 libtree 库的自上而下线程化 Splay 树实现。

| 文件 | 原始路径 | 说明 |
|------|----------|------|
| `avltree.h` (splaytree部分) | `src/include/avltree.h` | `struct splaytree_node`、`struct splaytree` 定义 |
| `splay.c` | `src/avl/splay.c` | Splay 树函数实现：`splaytree_insert`、`splaytree_remove`、`splaytree_lookup` 等 |

**设计模式**: 自适应结构，最近访问的节点被旋转到根部，适合局部性访问模式。

---

## 6. 双向链表 (Doubly Linked List)

Ganesha 项目自有的侵入式双向循环链表实现，类似 Linux 内核 `list_head`。

| 文件 | 原始路径 | 说明 |
|------|----------|------|
| `gsh_list.h` | `src/include/gsh_list.h` | 完整链表实现（纯头文件），定义 `struct glist_head`，提供全部操作 |
| `test_glist.c` | `src/test/test_glist.c` | 链表单元测试 |

**核心API**:
- `glist_init()` - 初始化链表头
- `glist_add()` / `glist_add_tail()` - 头部/尾部添加
- `glist_del()` - 删除节点
- `glist_move_tail()` - 移动节点到尾部
- `glist_empty()` / `glist_null()` - 判空
- `glist_splice_tail()` - 链表合并
- `glist_split()` - 链表拆分
- `glist_swap_lists()` - 链表交换
- `glist_insert_sorted()` - 排序插入
- `glist_for_each()` / `glist_for_each_safe()` - 遍历宏
- `glist_entry()` / `glist_first_entry()` / `glist_last_entry()` - 获取容器结构
- `container_of()` - 通用容器宏

---

## 7. 哈希表 (Hash Table)

基于红黑树分区的并发哈希表实现，支持线程安全。

| 文件 | 原始路径 | 说明 |
|------|----------|------|
| `hashtable.h` | `src/include/hashtable.h` | 哈希表接口定义，`struct hash_table`、`struct hash_partition`、`struct hash_param` 等 |
| `hashtable.c` | `src/hashtable/hashtable.c` | 完整哈希表实现：创建、销毁、查找、插入、删除、遍历 |

**核心API**:
- `hashtable_init()` - 创建哈希表
- `hashtable_destroy()` - 销毁
- `hashtable_getlatch()` - 带锁查找
- `hashtable_setlatched()` - 带锁设置
- `hashtable_deletelatched()` - 带锁删除
- `HashTable_Get()` / `HashTable_Set()` / `HashTable_Del()` - 简化包装

**依赖**: `rbt_node.h`, `rbt_tree.h`, `abstract_mem.h`

---

## 8. 队列 (Queue)

| 文件 | 原始路径 | 说明 |
|------|----------|------|
| `9p_req_queue.h` | `src/include/9p_req_queue.h` | 9P协议请求队列，基于 `gsh_list` 实现的 LIFO 队列，支持生产者-消费者模式 |
| `gsh_wait_queue.h` | `src/include/gsh_wait_queue.h` | Pthread 等待队列，`wait_entry_t` 基于互斥锁+条件变量 |

---

## 9. 基础设施/辅助文件

| 文件 | 原始路径 | 说明 |
|------|----------|------|
| `abstract_mem.h` | `src/include/abstract_mem.h` | 内存分配抽象层：`gsh_malloc`、`gsh_calloc`、`gsh_realloc`、`gsh_free`、`pool_t` 对象池 |
| `abstract_atomic.h` | `src/include/abstract_atomic.h` | 原子操作抽象层：为 int8/16/32/64、uint8/16/32/64、size_t 等类型提供原子 fetch/store/add/sub/inc/dec 操作 |
| `CMakeLists.txt` | `src/avl/CMakeLists.txt` | libtree 树库的 CMake 构建配置 |

---

## 文件依赖关系

```
avltree.h  (libtree 统一头文件)
  ├── avl.c     (AVL树实现)
  ├── rb.c      (红黑树实现)
  ├── bst.c     (BST实现)
  └── splay.c   (Splay树实现)

rbt_node.h + rbt_tree.h  (STL红黑树)
  └── hashtable.h + hashtable.c  (哈希表)
        └── abstract_mem.h  (内存管理)

gsh_list.h  (双向链表)
  ├── gsh_wait_queue.h  (等待队列)
  │     └── 9p_req_queue.h  (请求队列)
  └── hashtable.c  (哈希表内部使用)

abstract_atomic.h  (原子操作，被多处引用)
```

---

## 复用建议

1. **双向链表 (`gsh_list.h`)**: 纯头文件实现，最容易复用，无外部依赖。可直接 `#include` 使用。
2. **AVL树 (`avltree.h` + `avl.c`)**: 需同时包含头文件和编译源文件。比较独立，仅依赖标准库。
3. **STL红黑树 (`rbt_node.h` + `rbt_tree.h`)**: 纯宏实现，无需编译源文件。适合嵌入式场景。
4. **哈希表**: 依赖较多（红黑树 + 内存管理 + 日志），复用需要适配依赖。
5. **libtree 系列** (`avl.c`, `rb.c`, `bst.c`, `splay.c`): 共享同一头文件 `avltree.h`，可按需选择编译。
