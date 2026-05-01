#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <random>

using namespace std;
typedef long long ll;

ll nodes_visited = 0;

bool backtrack(const vector<ll>& nums, const vector<ll>& suffix_sums, ll target, int idx, ll current) {
    nodes_visited++;
    if (current == target) return true;
    if (idx >= nums.size() || current > target || current + suffix_sums[idx] < target) return false;
    if (backtrack(nums, suffix_sums, target, idx + 1, current + nums[idx])) return true;
    return backtrack(nums, suffix_sums, target, idx + 1, current);
}

int main() {
    const int N = 34;
    const int TRIALS = 300;
    ofstream file("amdb_research_data.csv");
    file << "Trial,Bits,Density,Type,Time_ms,Nodes,Status" << endl;

    mt19937_64 rng(1337);
   
vector<int> bit_configs = {20, 25, 30, 35, 40, 45, 50};



    cout << "🚀 Starting Phase Transition Research (2400 runs)..." << endl;

    for (int bits : bit_configs) {
        for (string type : {"Random", "Structured"}) {
            cout << "Testing " << type << " at " << bits << " bits..." << endl;
            for (int t = 1; t <= TRIALS; ++t) {
                vector<ll> nums(N);
                if (type == "Random") {
                    uniform_int_distribution<ll> dist(1, (1LL << bits));
                    for (int i = 0; i < N; ++i) nums[i] = dist(rng);
                } else {
                    for (int i = 0; i < N; ++i) nums[i] = (1LL << (bits - N + i)) + (rng() % 100);
                }

                sort(nums.rbegin(), nums.rend());
                vector<ll> suffix_sums(N + 1, 0);
                for (int i = N - 1; i >= 0; --i) suffix_sums[i] = suffix_sums[i + 1] + nums[i];

                ll target = suffix_sums[0] / 2;
                nodes_visited = 0;

                auto start = chrono::high_resolution_clock::now();
                bool found = backtrack(nums, suffix_sums, target, 0, 0);
                auto end = chrono::high_resolution_clock::now();

                double time_ms = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;

                file << t << "," << bits << "," << fixed << setprecision(3) << (double)N/bits << ","
                     << type << "," << time_ms << "," << nodes_visited << ","
                     << (found ? "YES" : "NO") << endl;
            }
        }
    }
    file.close();
    cout << "✅ Research Complete! Data saved to amdb_research_data.csv" << endl;
    return 0;
}
