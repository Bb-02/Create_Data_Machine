/**
 * template.cpp — 数据生成模板（输入 + 输出）
 *
 * 用法：
 *   1. 改 g_problem_id
 *   2. 写 generate_input()   — 造输入数据
 *   3. 写 solve(in, out)     — 题解代码
 *   4. 编译：g++ -std=c++17 -O2 template.cpp -o gen
 *   5. 生成输入：./gen [seed]
 *   6. 生成输出：./gen out
 *
 * 输入文件 -> Data/{id}_Data/001.in, 002.in ...
 * 输出文件 -> Data/{id}_Data/001.out, 002.out ...
 */

#include "gen_lib.h"
using namespace std;

// ============================================================
// ★ 改这里 — 题目 ID
// ============================================================
string g_problem_id = "my_problem";

// ============================================================
// ★ 步骤 1：造输入数据 (.in 文件)
// ============================================================
void generate_input() {
    DataWriter dw; // 001.in, 002.in, 003.in ...

    // --- 第 1 组：样例 ---
    dw.next([](ostream &o) {
        o << "3 5\n";
        o << "1 2 3\n";
    });

    // --- 小数据 ---
    for (int i = 0; i < 3; i++) {
        dw.next([&](ostream &o) {
            int n = rnd->next(1, 10);
            int m = rnd->next(1, 10);
            o << n << ' ' << m << '\n';
            print_vec(o, gen_array(n, 1, 100));
        });
    }

    // --- 极限数据 ---
    for (int i = 0; i < 3; i++) {
        dw.next([&](ostream &o) {
            int n = 100000;
            int m = rnd->next(1, 1'000'000'000);
            o << n << ' ' << m << '\n';
            print_vec(o, gen_array(n, 1, 1'000'000'000));
        });
    }

    cerr << "Input done! " << dw.counter << " files -> Data/"
         << g_problem_id << "_Data/\n";
}

// ============================================================
// ★ 步骤 2：题解代码（读入 -> 计算 -> 输出答案）
// ============================================================
void solve(istream &in, ostream &out) {
    // 示例：求数组和（换成你真正的题解）
    int n;
    long long m;
    in >> n >> m;

    long long sum = 0;
    for (int i = 0; i < n; i++) {
        long long x;
        in >> x;
        sum += x;
    }

    // 输出答案
    out << sum % m << '\n';
}

// ============================================================
int main(int argc, char *argv[]) {
    // 判断模式：./gen out  -> 生成输出；其他 -> 生成输入
    bool out_mode = false;
    uint64_t seed = 0;
    bool seed_set = false;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "out") {
            out_mode = true;
        } else {
            seed = stoull(arg);
            seed_set = true;
        }
    }

    rnd = new Random(seed_set ? seed : 0);
    cerr << "Problem: " << g_problem_id << '\n';
    if (seed_set) cerr << "Seed: " << seed << '\n';

    if (out_mode) {
        // 生成输出：读取 .in 文件，运行题解，写出 .out
        gen_output(solve);
    } else {
        // 生成输入
        generate_input();
    }

    delete rnd;
    return 0;
}
