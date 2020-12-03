
#include <vector>
#include <iostream>
#include <algorithm>


static constexpr int NONE = -1;


std::vector<bool> usedL;
std::vector<int> pair;
std::vector<bool> covered;
std::vector<std::vector<int>> graph;


int cntL = 0;
int cntR = 0;


bool kuhn(int left) {
    usedL[left] = true;
    for (int right : graph[left]) {
        if (pair[right] == NONE || (!usedL[pair[right]] && kuhn(pair[right]))) {
            covered[left] = true;
            pair[right] = left;
            return true;
        }
    }
    return false;
}

void matching() {
    std::fill(usedL.begin(), usedL.end(), false);
    bool flag = true;
    while (flag) {
        std::fill(usedL.begin(), usedL.end(), false);
        flag = false;
        for (int i = 0; i < cntL; i++) {
            if (!covered[i] && kuhn(i)) {
                flag = true;
            }
        }
    }
}


int main() {


    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);


    std::cin >> cntL >> cntR;


    usedL.resize(cntL, false);
    pair.resize(cntR, NONE);
    covered.resize(cntL, false);
    graph.resize(cntL);


    for (int i = 0; i < cntL; i++) {
        int cnt = 0;
        std::cin >> cnt;
        for (int j = 0; j < cnt; j++) {
            int to = 0;
            std::cin >> to;
            graph[i].push_back(to - 1);
        }
    }


    matching();

    for (int i = 0; i < cntR; i++) {
        if (pair[i] != NONE) {
            std::cout << pair[i] + 1 << ' ' << i + 1 << '\n';
        }
    }


    return 0;
}