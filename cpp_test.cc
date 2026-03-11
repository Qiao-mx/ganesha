#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>  // 用于vector的查找、排序等操作

// 函数声明，用于封装不同数据结构的演示逻辑
void demonstrateVector();
void demonstrateList();
void demonstrateMap();

int main() {
    std::cout << "========== C++ 常用数据结构演示 ==========\n\n";

    // 演示vector（动态数组）
    std::cout << "1. 演示 std::vector 功能：\n";
    demonstrateVector();
    std::cout << "\n----------------------------------------\n";

    // 演示list（双向链表）
    std::cout << "2. 演示 std::list 功能：\n";
    demonstrateList();
    std::cout << "\n----------------------------------------\n";

    // 演示map（键值对映射，红黑树实现）
    std::cout << "3. 演示 std::map 功能：\n";
    demonstrateMap();
    std::cout << "\n========== 演示结束 ==========\n";

    return 0;
}

// 演示vector：动态数组，随机访问效率高
void demonstrateVector() {
    // 1. 初始化和添加元素
    std::vector<int> vec = {10, 20, 30};
    vec.push_back(40);  // 尾部添加
    vec.insert(vec.begin() + 2, 25);  // 在索引2位置插入25

    // 2. 遍历输出
    std::cout << "   初始化后的vector：";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // 3. 访问和修改元素（随机访问）
    vec[1] = 15;  // 修改索引1的元素为15
    std::cout << "   修改索引1后的vector：";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // 4. 查找和删除元素
    auto it = std::find(vec.begin(), vec.end(), 30);
    if (it != vec.end()) {
        vec.erase(it);  // 删除找到的元素30
    }
    std::cout << "   删除元素30后的vector：";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << "\n";

    // 5. 排序
    std::sort(vec.begin(), vec.end());
    std::cout << "   排序后的vector：";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << "\n";
}

// 演示list：双向链表，插入/删除效率高（无随机访问）
void demonstrateList() {
    // 1. 初始化和添加元素
    std::list<std::string> strList = {"apple", "banana"};
    strList.push_front("orange");  // 头部添加
    strList.push_back("grape");    // 尾部添加

    // 2. 遍历输出
    std::cout << "   初始化后的list：";
    for (const std::string& str : strList) {
        std::cout << str << " ";
    }
    std::cout << "\n";

    // 3. 插入元素（在banana前插入pear）
    auto it = std::find(strList.begin(), strList.end(), "banana");
    if (it != strList.end()) {
        strList.insert(it, "pear");
    }
    std::cout << "   在banana前插入pear后的list：";
    for (const std::string& str : strList) {
        std::cout << str << " ";
    }
    std::cout << "\n";

    // 4. 删除元素（删除orange）
    strList.remove("orange");
    std::cout << "   删除orange后的list：";
    for (const std::string& str : strList) {
        std::cout << str << " ";
    }
    std::cout << "\n";

    // 5. 排序
    strList.sort();
    std::cout << "   排序后的list：";
    for (const std::string& str : strList) {
        std::cout << str << " ";
    }
    std::cout << "\n";
}

// 演示map：键值对映射，按键有序排列，键唯一
void demonstrateMap() {
    // 1. 初始化和添加元素
    std::map<int, std::string> studentMap;
    studentMap[101] = "Zhang San";  // 直接赋值添加
    studentMap[102] = "Li Si";
    studentMap.insert(std::make_pair(103, "Wang Wu"));  // insert方式添加

    // 2. 遍历输出（默认按键升序）
    std::cout << "   初始化后的map：\n";
    for (const auto& pair : studentMap) {
        std::cout << "      学号：" << pair.first << "，姓名：" << pair.second << "\n";
    }

    // 3. 修改元素（修改102的姓名）
    studentMap[102] = "Li Si (modified)";
    std::cout << "   修改102姓名后的map：\n";
    for (const auto& pair : studentMap) {
        std::cout << "      学号：" << pair.first << "，姓名：" << pair.second << "\n";
    }

    // 4. 查找元素（查找学号103）
    auto it = studentMap.find(103);
    if (it != studentMap.end()) {
        std::cout << "   找到学号103：" << it->second << "\n";
    }

    // 5. 删除元素（删除学号101）
    studentMap.erase(101);
    std::cout << "   删除学号101后的map：\n";
    for (const auto& pair : studentMap) {
        std::cout << "      学号：" << pair.first << "，姓名：" << pair.second << "\n";
    }
}
