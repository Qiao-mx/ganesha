#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "data_structures/rbt_node.h"
#include "data_structures/rbt_tree.h"
#include "data_structures/gsh_list.h"

/* ========== 辅助结构定义 ========== */

/* 用于 list 演示：字符串节点，嵌入 glist_head */
struct string_node {
    char str[64];
    struct glist_head node;
};

/* 用于 map 演示：学生条目，嵌入 rbt_node */
struct student_entry {
    struct rbt_node rbt;
    char name[64];
};

/* ========== 辅助函数 ========== */

/* 创建独立的红黑树整数节点（用于 vector 演示） */
static struct rbt_node *create_int_node(uint64_t value)
{
    struct rbt_node *n = (struct rbt_node *)calloc(1, sizeof(struct rbt_node));
    if (!n) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    n->rbt_value = value;
    n->rbt_opaq = NULL;
    return n;
}

/* 释放红黑树中所有独立节点 */
static void free_rbt_int_nodes(struct rbt_head *head)
{
    while (head->leftmost) {
        struct rbt_node *n = head->leftmost;
        RBT_UNLINK(head, n);
        free(n);
    }
}

/* 创建字符串链表节点 */
static struct string_node *create_string_node(const char *str)
{
    struct string_node *n = (struct string_node *)calloc(1, sizeof(struct string_node));
    if (!n) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    strncpy(n->str, str, sizeof(n->str) - 1);
    n->str[sizeof(n->str) - 1] = '\0';
    return n;
}

/* 创建学生条目（rbt_node 嵌入其中） */
static struct student_entry *create_student_entry(uint64_t id, const char *name)
{
    struct student_entry *e = (struct student_entry *)calloc(1, sizeof(struct student_entry));
    if (!e) {
        fprintf(stderr, "内存分配失败\n");
        exit(1);
    }
    e->rbt.rbt_value = id;
    e->rbt.rbt_opaq = e;  /* rbt_opaq 指回外层结构，便于检索 */
    strncpy(e->name, name, sizeof(e->name) - 1);
    e->name[sizeof(e->name) - 1] = '\0';
    return e;
}

/* 字符串节点排序比较函数（用于 glist_insert_sorted） */
static int string_node_compare(struct glist_head *a, struct glist_head *b)
{
    struct string_node *na = glist_entry(a, struct string_node, node);
    struct string_node *nb = glist_entry(b, struct string_node, node);
    return strcmp(na->str, nb->str);
}

/* ========== 函数声明 ========== */
void demonstrateVector(void);
void demonstrateList(void);
void demonstrateMap(void);

/* ========== 主函数 ========== */

int main(void)
{
    printf("========== C 数据结构演示（基于 nfs-ganesha 红黑树和链表）==========\n\n");

    /* 演示红黑树替代 vector（动态数组） */
    printf("1. 演示红黑树替代 std::vector 功能：\n");
    demonstrateVector();
    printf("\n----------------------------------------\n");

    /* 演示双向链表替代 list（双向链表） */
    printf("2. 演示双向链表替代 std::list 功能：\n");
    demonstrateList();
    printf("\n----------------------------------------\n");

    /* 演示红黑树替代 map（键值对映射） */
    printf("3. 演示红黑树替代 std::map 功能：\n");
    demonstrateMap();
    printf("\n========== 演示结束 ==========\n");

    return 0;
}

/* ========== 演示红黑树替代 std::vector<int> ========== */
/*
 * std::vector 是动态数组，支持随机访问。
 * 红黑树天然有序，替代后自动排序，但不支持索引访问。
 * 对应操作映射：
 *   push_back / insert  → RBT_FIND + RBT_INSERT
 *   遍历（有序）        → RBT_LOOP + RBT_INCREMENT
 *   查找                → RBT_FIND
 *   删除                → RBT_UNLINK
 *   排序                → 红黑树天然有序，无需额外操作
 */
void demonstrateVector(void)
{
    struct rbt_head tree;
    struct rbt_node *found, *it, *new_node;
    int i;

    RBT_HEAD_INIT(&tree);

    /* 1. 初始化并添加元素：10, 20, 30 */
    {
        uint64_t init_vals[] = {10, 20, 30};
        for (i = 0; i < 3; i++) {
            new_node = create_int_node(init_vals[i]);
            RBT_FIND(&tree, found, init_vals[i]);
            RBT_INSERT(&tree, new_node, found);
        }
    }

    /* push_back(40) - 尾部添加 */
    new_node = create_int_node(40);
    RBT_FIND(&tree, found, (uint64_t)40);
    RBT_INSERT(&tree, new_node, found);

    /* insert(25) - 插入25 */
    new_node = create_int_node(25);
    RBT_FIND(&tree, found, (uint64_t)25);
    RBT_INSERT(&tree, new_node, found);

    /* 2. 遍历输出（红黑树天然有序） */
    printf("   初始化后的红黑树（自动排序）：");
    RBT_LOOP(&tree, it) {
        printf("%llu ", (unsigned long long)RBT_VALUE(it));
        RBT_INCREMENT(it);
    }
    printf("\n");

    /* 3. 修改元素：将20替换为15（模拟 vec[1] = 15） */
    RBT_FIND(&tree, found, (uint64_t)20);
    if (found && RBT_VALUE(found) == 20) {
        RBT_UNLINK(&tree, found);
        free(found);
    }
    new_node = create_int_node(15);
    RBT_FIND(&tree, found, (uint64_t)15);
    RBT_INSERT(&tree, new_node, found);

    printf("   将20替换为15后的红黑树：");
    RBT_LOOP(&tree, it) {
        printf("%llu ", (unsigned long long)RBT_VALUE(it));
        RBT_INCREMENT(it);
    }
    printf("\n");

    /* 4. 查找并删除元素30 */
    RBT_FIND(&tree, found, (uint64_t)30);
    if (found && RBT_VALUE(found) == 30) {
        RBT_UNLINK(&tree, found);
        free(found);
    }
    printf("   删除元素30后的红黑树：");
    RBT_LOOP(&tree, it) {
        printf("%llu ", (unsigned long long)RBT_VALUE(it));
        RBT_INCREMENT(it);
    }
    printf("\n");

    /* 5. 排序：红黑树天然有序 */
    printf("   红黑树天然有序，无需额外排序操作\n");

    /* 清理 */
    free_rbt_int_nodes(&tree);
}

/* ========== 演示双向链表替代 std::list<std::string> ========== */
/*
 * std::list 是双向链表，gsh_list 也是双向循环链表。
 * 对应操作映射：
 *   push_front  → glist_add
 *   push_back   → glist_add_tail
 *   insert      → __glist_add
 *   remove      → glist_del
 *   sort        → glist_insert_sorted (重建)
 *   遍历        → glist_for_each
 */
void demonstrateList(void)
{
    struct glist_head head;
    struct glist_head *node_ptr, *safe_ptr;
    struct string_node *sn;

    glist_init(&head);

    /* 1. 初始化：添加 "apple", "banana" */
    sn = create_string_node("apple");
    glist_add_tail(&head, &sn->node);
    sn = create_string_node("banana");
    glist_add_tail(&head, &sn->node);

    /* push_front "orange" - 头部添加 */
    sn = create_string_node("orange");
    glist_add(&head, &sn->node);

    /* push_back "grape" - 尾部添加 */
    sn = create_string_node("grape");
    glist_add_tail(&head, &sn->node);

    /* 2. 遍历输出 */
    printf("   初始化后的list：");
    glist_for_each(node_ptr, &head) {
        sn = glist_entry(node_ptr, struct string_node, node);
        printf("%s ", sn->str);
    }
    printf("\n");

    /* 3. 在 banana 前插入 pear */
    glist_for_each(node_ptr, &head) {
        sn = glist_entry(node_ptr, struct string_node, node);
        if (strcmp(sn->str, "banana") == 0) {
            struct string_node *pear = create_string_node("pear");
            __glist_add(node_ptr->prev, node_ptr, &pear->node);
            break;
        }
    }
    printf("   在banana前插入pear后的list：");
    glist_for_each(node_ptr, &head) {
        sn = glist_entry(node_ptr, struct string_node, node);
        printf("%s ", sn->str);
    }
    printf("\n");

    /* 4. 删除 orange */
    glist_for_each_safe(node_ptr, safe_ptr, &head) {
        sn = glist_entry(node_ptr, struct string_node, node);
        if (strcmp(sn->str, "orange") == 0) {
            glist_del(node_ptr);
            free(sn);
            break;
        }
    }
    printf("   删除orange后的list：");
    glist_for_each(node_ptr, &head) {
        sn = glist_entry(node_ptr, struct string_node, node);
        printf("%s ", sn->str);
    }
    printf("\n");

    /* 5. 排序：取出所有节点，用 glist_insert_sorted 重新插入 */
    {
        struct glist_head sorted_head;
        glist_init(&sorted_head);

        while (!glist_empty(&head)) {
            node_ptr = head.next;
            glist_del(node_ptr);
            glist_insert_sorted(&sorted_head, node_ptr, string_node_compare);
        }
        glist_splice_tail(&head, &sorted_head);
    }

    printf("   排序后的list：");
    glist_for_each(node_ptr, &head) {
        sn = glist_entry(node_ptr, struct string_node, node);
        printf("%s ", sn->str);
    }
    printf("\n");

    /* 清理 */
    glist_for_each_safe(node_ptr, safe_ptr, &head) {
        sn = glist_entry(node_ptr, struct string_node, node);
        glist_del(node_ptr);
        free(sn);
    }
}

/* ========== 演示红黑树替代 std::map<int, std::string> ========== */
/*
 * std::map 底层就是红黑树，rbt_tree.h 提供完全对应的能力。
 * 对应操作映射：
 *   map[key] = value   → RBT_FIND + RBT_INSERT / 修改 rbt_opaq
 *   insert(pair)       → RBT_FIND + RBT_INSERT
 *   find(key)          → RBT_FIND
 *   erase(key)         → RBT_FIND + RBT_UNLINK
 *   遍历（按键有序）    → RBT_LOOP + RBT_INCREMENT
 */
void demonstrateMap(void)
{
    struct rbt_head tree;
    struct rbt_node *found, *it;
    struct student_entry *entry;

    RBT_HEAD_INIT(&tree);

    /* 1. 初始化并添加元素 */
    entry = create_student_entry(101, "Zhang San");
    RBT_FIND(&tree, found, (uint64_t)101);
    RBT_INSERT(&tree, &entry->rbt, found);

    entry = create_student_entry(102, "Li Si");
    RBT_FIND(&tree, found, (uint64_t)102);
    RBT_INSERT(&tree, &entry->rbt, found);

    entry = create_student_entry(103, "Wang Wu");
    RBT_FIND(&tree, found, (uint64_t)103);
    RBT_INSERT(&tree, &entry->rbt, found);

    /* 2. 遍历输出（默认按键升序） */
    printf("   初始化后的map：\n");
    RBT_LOOP(&tree, it) {
        entry = (struct student_entry *)RBT_OPAQ(it);
        printf("      学号：%llu，姓名：%s\n",
               (unsigned long long)RBT_VALUE(it), entry->name);
        RBT_INCREMENT(it);
    }

    /* 3. 修改元素（修改102的姓名） */
    RBT_FIND(&tree, found, (uint64_t)102);
    if (found && RBT_VALUE(found) == 102) {
        entry = (struct student_entry *)RBT_OPAQ(found);
        strncpy(entry->name, "Li Si (modified)", sizeof(entry->name) - 1);
        entry->name[sizeof(entry->name) - 1] = '\0';
    }
    printf("   修改102姓名后的map：\n");
    RBT_LOOP(&tree, it) {
        entry = (struct student_entry *)RBT_OPAQ(it);
        printf("      学号：%llu，姓名：%s\n",
               (unsigned long long)RBT_VALUE(it), entry->name);
        RBT_INCREMENT(it);
    }

    /* 4. 查找元素（查找学号103） */
    RBT_FIND(&tree, found, (uint64_t)103);
    if (found && RBT_VALUE(found) == 103) {
        entry = (struct student_entry *)RBT_OPAQ(found);
        printf("   找到学号103：%s\n", entry->name);
    }

    /* 5. 删除元素（删除学号101） */
    RBT_FIND(&tree, found, (uint64_t)101);
    if (found && RBT_VALUE(found) == 101) {
        entry = (struct student_entry *)RBT_OPAQ(found);
        RBT_UNLINK(&tree, found);
        free(entry);
    }
    printf("   删除学号101后的map：\n");
    RBT_LOOP(&tree, it) {
        entry = (struct student_entry *)RBT_OPAQ(it);
        printf("      学号：%llu，姓名：%s\n",
               (unsigned long long)RBT_VALUE(it), entry->name);
        RBT_INCREMENT(it);
    }

    /* 清理 */
    while (tree.leftmost) {
        struct rbt_node *n = tree.leftmost;
        entry = (struct student_entry *)RBT_OPAQ(n);
        RBT_UNLINK(&tree, n);
        free(entry);
    }
}
