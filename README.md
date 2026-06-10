# 算法题数据生成框架

## 这玩意干嘛的？

写算法题的时候，你需要造测试数据（输入文件 `.in` 和输出文件 `.out`）。这个框架帮你干两件事：

1. **造输入数据** — 随机生成各种规模、各种结构的 `.in` 文件
2. **造输出数据** — 读入 `.in`，跑一遍标准题解，生成对应的 `.out` 文件

## 目录长这样

```
Create_Data/
├── gen_lib.h              ← 核心库（不用改）
├── template.cpp           ← 模板（复制这个开始写新题）
├── example_graph.cpp      ← 完整示例
├── README.md              ← 你在看的这个
└── Data/                  ← 所有题目的数据都在这
    ├── sum_Data/          ← 题目 "sum" 的数据
    │   ├── 001.in
    │   ├── 001.out
    │   ├── 002.in
    │   ├── 002.out
    │   └── ...
    └── shortest_path_Data/ ← 题目 "shortest_path" 的数据
        ├── 001.in
        ├── 001.out
        └── ...
```

---

# 快速开始（5 分钟上手）

## 第 1 步：复制模板

```bash
cp template.cpp my_problem.cpp
```

## 第 2 步：改题目 ID

打开 `my_problem.cpp`，找到这一行：

```cpp
string g_problem_id = "my_problem";  // 改成你的题目名，比如 "sum"、"tree" 等
```

**这个 ID 决定了数据存到哪个文件夹。** 比如你写 `"sum"`，数据就去 `Data/sum_Data/`。

## 第 3 步：写造输入数据的代码

在 `generate_input()` 函数里写：

```cpp
void generate_input() {
    DataWriter dw; // 自动创建 Data/sum_Data/ 目录

    // 每调用一次 dw.next(...) = 生成一个 .in 文件
    // o 就是文件流，当成 cout 用就行
    dw.next([](ostream &o) {
        o << "3 5\n";                  // 第一行：n m
        o << "1 2 3\n";               // 第二行：数组
    });
    // ↑ 这个生成 001.in

    dw.next([](ostream &o) {
        int n = 10;
        o << n << '\n';
        auto arr = gen_array(n, 1, 100);  // 10 个 [1,100] 的随机数
        print_vec(o, arr);                 // 输出到文件
    });
    // ↑ 这个生成 002.in

    // 用循环批量生成
    for (int i = 0; i < 5; i++) {
        dw.next([&](ostream &o) {
            int n = rnd->next(10, 100);     // 随机 10~100
            o << n << '\n';
            print_vec(o, gen_array(n, 1, 1'000'000));
        });
    }
    // ↑ 生成 003.in ~ 007.in
}
```

## 第 4 步：写题解代码

在 `solve()` 函数里写：

```cpp
void solve(istream &in, ostream &out) {
    // 从 in 读入（当成 cin 用）
    int n, m;
    in >> n >> m;

    long long sum = 0;
    for (int i = 0; i < n; i++) {
        long long x;
        in >> x;
        sum += x;
    }

    // 输出答案到 out（当成 cout 用）
    out << sum % m << '\n';
}
```

## 第 5 步：编译运行

```bash
# 编译
g++ -std=c++17 -O2 my_problem.cpp -o gen

# 生成输入数据（.in 文件）
./gen

# 生成输出数据（读 .in + 跑题解 → .out 文件）
./gen out

# 固定随机种子（让数据可复现）
./gen 12345
```

---

# 核心概念详解

## DataWriter — 怎么写 .in 文件

```cpp
DataWriter dw;              // 文件名叫 001.in, 002.in, 003.in ...
DataWriter dw("in");        // 文件名叫 in_001.in, in_002.in ...
DataWriter dw("in", "ans"); // 文件名叫 in_001.ans, in_002.ans ...
```

**写数据只有一种方式：`dw.next( lambda )`**

```cpp
dw.next([](ostream &o) {
    // o 就是文件流，用 << 往里写
    o << "hello world\n";
    o << 42 << ' ' << 99 << '\n';
});
```

lambda 里可以用 `[&]` 捕获外面的变量：

```cpp
int n = rnd->next(1, 100);
dw.next([&](ostream &o) {   // [&] 让 lambda 能访问 n
    o << n << '\n';
});
```

## print_vec — 输出数组的一行

```cpp
// 输出到文件流 o
print_vec(o, arr);          // 空格分隔：1 2 3 4 5
print_vec(o, arr, "\n");    // 换行分隔：1\n2\n3\n4\n5

// 输出到屏幕（调试用）
print_vec(arr);
```

## println — 输出一行到屏幕

```cpp
println("hello");              // hello
println("n =", n, "m =", m);   // n = 5 m = 10
println();                     // 空行
```

---

# 随机数引擎

全局变量 `rnd` 就是随机数引擎，直接用。

| 函数 | 说明 | 示例 |
|------|------|------|
| `rnd->next(l, r)` | [l, r] 区间随机整数 | `rnd->next(1, 100)` → 1~100 |
| `rnd->next(r)` | [0, r] 区间随机整数 | `rnd->next(100)` → 0~100 |
| `rnd->next_n(n)` | [0, n) 区间随机整数 | `rnd->next(5)` → 0~4 |
| `rnd->next_real()` | [0, 1) 随机浮点数 | |
| `rnd->chance(p)` | 以概率 p 返回 true | `rnd->chance(0.3)` → 30% 概率 |
| `rnd->shuffle(v)` | 原地打乱数组 | |
| `rnd->pick(v)` | 随机选一个元素 | `rnd->pick(v)` |
| `rnd->sample(v, k)` | 不放回选 k 个 | `rnd->sample(v, 3)` |
| `rnd->distinct(k, l, r)` | [l,r] 中取 k 个不同的数 | `rnd->distinct(5, 1, 100)` |

---

# 内置生成器

## 数组

```cpp
// n 个 [l, r] 的随机数
auto a = gen_array(n, l, r);

// 1~n 的随机排列
auto p = gen_permutation(n);

// 非降序数组
auto a = gen_sorted_array(n, l, r);

// 将 n 分成 k 个正整数（每份大小随机，和 = n）
auto parts = gen_partition(n, k);
```

## 字符串

```cpp
// 长度为 n 的随机小写字母串
string s = gen_string(n);

// 长度为 n，字符来自指定字符集
string s = gen_string(n, "ACGT");  // 只含 A C G T
string s = gen_string(n, "01");    // 随机 01 串
```

## 树

```cpp
// 完全随机树（Prufer 序列，n 个节点）
auto edges = gen_tree_prufer(n);

// 菊花树（1 号节点是中心）
auto edges = gen_tree_star(n);

// 链
auto edges = gen_tree_chain(n);

// 每个节点度数不超过 d 的随机树
auto edges = gen_tree_deg_capped(n, d);
```

树的返回值是 `vector<pair<int, int>>`，每条边 `u < v`。

输出到文件：
```cpp
auto edges = gen_tree_prufer(n);
print_edges(o, edges); // 每行输出 "u v"
```

## 图

```cpp
// n 个节点 m 条边的连通无向图
auto edges = gen_graph_connected(n, m);

// n 个节点 m 条边的有向无环图
auto edges = gen_dag(n, m);
```

---

# gen_output — 生成 .out 文件

```cpp
// 写一个 solve 函数
void solve(istream &in, ostream &out) {
    // in 当成 cin，out 当成 cout
    int n; in >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) in >> a[i];

    // ... 你的题解 ...

    out << ans << '\n';
}
```

然后：

```cpp
// 自动扫描 Data/{id}_Data/ 下所有 .in 文件
// 逐个读入、跑 solve、写出 .out
gen_output(solve);
```

它会：
1. 找到所有 `.in` 文件（按文件名排序）
2. 对每个 `001.in` → 读入 → `solve(...)` → 写出 `001.out`
3. 在终端打印进度

---

# 实战：从零写一个 A+B 的数据

```cpp
#include "gen_lib.h"
using namespace std;

string g_problem_id = "aplusb";

void generate_input() {
    DataWriter dw;

    // 样例
    dw.next([](ostream &o) { o << "1 2\n"; });

    // 随机 10 组
    for (int i = 0; i < 10; i++) {
        dw.next([&](ostream &o) {
            int a = rnd->next(-100, 100);
            int b = rnd->next(-100, 100);
            o << a << ' ' << b << '\n';
        });
    }
}

void solve(istream &in, ostream &out) {
    int a, b;
    in >> a >> b;
    out << a + b << '\n';
}

int main(int argc, char *argv[]) {
    bool out_mode = false;
    uint64_t seed = 0;
    bool seed_set = false;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "out") out_mode = true;
        else seed = stoull(arg), seed_set = true;
    }
    rnd = new Random(seed_set ? seed : 0);
    if (out_mode) gen_output(solve);
    else generate_input();
    delete rnd;
    return 0;
}
```

编译运行：

```bash
g++ -std=c++17 -O2 aplusb.cpp -o gen
./gen        # 生成 Data/aplusb_Data/001.in ~ 011.in
./gen out    # 生成 Data/aplusb_Data/001.out ~ 011.out
```

---

# 常用技巧

## 让数据可复现

```bash
./gen 12345       # 用种子 12345 生成输入
./gen 12345 out   # 用同样的种子生成输出（如果输出也用了随机的话无所谓）
```

## 写一个文件多组询问

```cpp
dw.next([&](ostream &o) {
    int t = rnd->next(1, 10);
    o << t << '\n';                    // T 组询问
    while (t--) {
        int n = rnd->next(1, 100);
        o << n << '\n';
        print_vec(o, gen_array(n, 1, 1000));
    }
});
```

## 造极端数据

```cpp
// 全是一样的数
dw.next([](ostream &o) {
    int n = 100000;
    o << n << '\n';
    for (int i = 0; i < n; i++) {
        if (i) o << ' ';
        o << 1;
    }
    o << '\n';
});

// 递增/递减
auto a = gen_sorted_array(n, 1, 1'000'000'000);  // 递增
reverse(a.begin(), a.end());                       // 递减
```

## 不同规模的数据用 TestCase 管理

```cpp
vector<TestCase> cases = {
    {"sample",  5,   10,      100},
    {"small",   20,  100,     1000},
    {"medium",  1000, 10000,  100000},
    {"large",   100000, 500000, 1'000'000'000LL},
};

for (auto &tc : cases) {
    // tc.n, tc.m, tc.max_val 就是这组数据的参数
    dw.next([&](ostream &o) { ... });
}
```

---

# 命令行参考

| 命令 | 效果 |
|------|------|
| `./gen` | 生成输入数据（随机种子） |
| `./gen 12345` | 生成输入数据（种子=12345，可复现） |
| `./gen out` | 生成输出数据 |
| `./gen 12345 out` | 生成输出数据（指定种子） |
