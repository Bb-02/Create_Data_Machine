/**
 * example_graph.cpp — 完整示例：最短路径题目的输入+输出数据生成
 *
 * 生成输入：./gen_graph
 * 生成输出：./gen_graph out
 */

#include "gen_lib.h"
using namespace std;

string g_problem_id = "shortest_path";

// ============================================================
// 步骤 1：造输入数据
// ============================================================
void generate_input() {
    DataWriter dw;

    vector<TestCase> cases = {
        {"small_01",   5,       6,        10},
        {"small_02",   5,       10,       10},
        {"small_03",   8,       20,       100},
        {"medium_01",  100,     500,      1000},
        {"medium_02",  1000,    5000,     10000},
        {"large_01",   100000,  200000,   1'000'000'000LL},
        {"max",        200000,  200000,   1'000'000'000LL},
    };

    for (auto &tc : cases) {
        int n = tc.n, m = tc.m;
        long long max_w = tc.max_val;

        auto edges = gen_graph_connected(n, m);
        int s = rnd->next(1, n);
        int t = rnd->next(1, n);

        dw.next([&](ostream &o) {
            o << n << ' ' << edges.size() << ' ' << s << ' ' << t << '\n';
            for (auto [u, v] : edges) {
                long long w = rnd->next(1, max_w);
                o << u << ' ' << v << ' ' << w << '\n';
            }
        });

        cerr << "  [" << tc.name << "] n=" << n << " m=" << edges.size() << '\n';
    }

    cerr << "Input done! " << dw.counter << " files -> Data/"
         << g_problem_id << "_Data/\n";
}

// ============================================================
// 步骤 2：题解（Dijkstra 最短路径）
// ============================================================
using ll = long long;
const ll INF = 1e18;

void solve(istream &in, ostream &out) {
    int n, m, s, t;
    in >> n >> m >> s >> t;

    vector<vector<pair<int, ll>>> g(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v; ll w;
        in >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }

    vector<ll> dist(n + 1, INF);
    dist[s] = 0;
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    pq.push({0, s});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        for (auto [v, w] : g[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    out << (dist[t] == INF ? -1 : dist[t]) << '\n';
}

// ============================================================
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
    cerr << "Problem: " << g_problem_id << '\n';
    if (seed_set) cerr << "Seed: " << seed << '\n';

    if (out_mode) gen_output(solve);
    else generate_input();

    delete rnd;
    return 0;
}
