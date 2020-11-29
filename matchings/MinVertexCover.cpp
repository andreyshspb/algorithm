
#include <vector>
#include <iostream>
#include <algorithm>


static constexpr int NONE = -1;


std::vector<bool> usedL;
std::vector<bool> usedR;
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
    std::fill(usedR.begin(), usedR.end(), false);
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


void dfs(int left) {
    if (left == NONE || usedL[left]) {
        return;
    }
    usedL[left] = true;
    for (int right : graph[left]) {
        if (pair[right] != left) {
            usedR[right] = true;
            dfs(pair[right]);
        }
    }
}

void vertex_cover() {
    matching();
    std::fill(usedL.begin(), usedL.end(), false);
    std::fill(usedR.begin(), usedR.end(), false);
    for (int left = 0; left < cntL; left++) {
        if (!covered[left]) {
            dfs(left);
        }
    }
}


int main() {


    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);


    std::cin >> cntL >> cntR;


    usedL.resize(cntL, false);
    usedR.resize(cntR, false);
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


    vertex_cover();


    std::vector<int> coverL, coverR;
    for (int i = 0; i < cntL; i++) {
        if (!usedL[i]) {
            coverL.push_back(i + 1);
        }
    }
    for (int i = 0; i < cntR; i++) {
        if (usedR[i]) {
            coverR.push_back(i + 1);
        }
    }


    std::cout << coverL.size() + coverR.size() << '\n';
    std::cout << coverL.size() << ' ';
    for (int v : coverL) {
        std::cout << v << ' ';
    }
    std::cout << '\n';
    std::cout << coverR.size() << ' ';
    for (int v : coverR) {
        std::cout << v << ' ';
    }
    std::cout << '\n';


    return 0;
}