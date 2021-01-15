

#include <vector>
#include <iostream>
#include <algorithm>


static constexpr int NONE = -1;
static constexpr int INF = 1000 * 1000 * 1000;


struct Edge {
    int to;
    int flow;
    int capacity;
};


int source = 0;
int sink = 0;


std::vector<int> head;
std::vector<int> next;
std::vector<bool> used;
std::vector<Edge> edges;


void add_edge(int vertex, Edge edge) {
    edges.push_back(edge);
    int current = edges.size() - 1;
    next[current] = head[vertex];
    head[vertex] = current;
}

void make_edge(int begin, int end, int capacity) {
    Edge edge = {end, 0, capacity};
    Edge rev_edge = {begin, 0, 0};
    add_edge(begin, edge);
    add_edge(end, rev_edge);
}


int ford(int vertex, int min, int min_weight) {
    used[vertex] = true;
    if (vertex == sink) {
        return min;
    }
    for (int id = head[vertex]; id != NONE; id = next[id]) {
        Edge &edge = edges[id];
        if (edge.flow + min_weight <= edge.capacity && !used[edge.to]) {
            int delta = ford(edge.to, std::min(min, edge.capacity - edge.flow), min_weight);
            if (delta > 0) {
                edge.flow += delta;
                edges[id ^ 1].flow -= delta;
                return delta;
            }
        }
    }
    return 0;
}


int main() {


    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);


    int cntV = 0, cntE = 0;
    std::cin >> cntV >> cntE;


    head.resize(cntV, NONE);
    used.resize(cntV, false);
    next.resize(2 * cntE, NONE);


    std::cin >> source >> sink;
    source -= 1; sink -= 1;


    for (int i = 0; i < cntE; i++) {
        int first = 0, second = 0, capacity = 0;
        std::cin >> first >> second >> capacity;
        make_edge(first - 1, second - 1, capacity);
    }


    for (int min_weight = 1 << 30; min_weight >= 1; min_weight /= 2) {
        while (ford(source, INF, min_weight)) {
            std::fill(used.begin(), used.end(), false);
        }
        std::fill(used.begin(), used.end(), false);
    }


    int max_flow = 0;
    for (int id = head[source]; id != NONE; id = next[id]) {
        max_flow += edges[id].flow;
    }
    std::cout << max_flow << '\n';


    return 0;
}