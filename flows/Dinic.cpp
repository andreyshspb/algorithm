#include <vector>
#include <queue>
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


std::vector<int> mark;
std::vector<int> head;
std::vector<int> buff;
std::vector<int> next;
std::vector<Edge> edges;


void add_edge(int vertex, Edge edge) {
    edges.push_back(edge);
    int current = edges.size() - 1;
    next[current] = head[vertex];
    head[vertex] = current;
    buff[vertex] = current;
}

void make_edge(int begin, int end, int capacity) {
    Edge edge = {end, 0, capacity};
    Edge rev_edge = {begin, 0, 0};
    add_edge(begin, edge);
    add_edge(end, rev_edge);
}


bool bfs(int min_weight) {
    std::fill(mark.begin(), mark.end(), NONE);

    std::queue<int> queue;
    queue.push(source);
    mark[source] = 0;

    while (!queue.empty()) {
        int cur = queue.front();
        queue.pop();
        for (int id = head[cur]; id != NONE; id = next[id]) {
            Edge edge = edges[id];
            if (mark[edge.to] == NONE && edge.flow + min_weight <= edge.capacity) {
                mark[edge.to] = mark[cur] + 1;
                queue.push(edge.to);
            }
        }
    }

    return mark[sink] != NONE;

}


int dfs(int vertex, int can, int min_weight) {
    if (vertex == sink) {
        return can;
    }
    while (head[vertex] != NONE) {
        Edge &edge = edges[head[vertex]];
        if (mark[edge.to] == mark[vertex] + 1 && edge.flow + min_weight <= edge.capacity) {
            int res = dfs(edge.to, std::min(can, edge.capacity - edge.flow), min_weight);
            if (res > 0) {
                edge.flow += res;
                edges[head[vertex] ^ 1].flow -= res;
                return res;
            }
        }
        head[vertex] = next[head[vertex]];
    }
    return 0;
}


long long max_flow() {
    for (int min_weight = 1 << 30; min_weight >= 1; min_weight /= 2) {
        while (bfs(min_weight)) {
            while (dfs(source, INF, min_weight) > 0) {}
            std::copy(buff.begin(), buff.end(), head.begin());
        }
    }

    long long flow = 0;
    for (int id = head[source]; id != NONE; id = next[id]) {
        flow += edges[id].flow;
    }

    return flow;
}


int main() {


    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);


    int count_vertexes = 0, count_edges = 0;
    std::cin >> count_vertexes >> count_edges;


    mark.resize(count_vertexes, NONE);
    head.resize(count_vertexes, NONE);
    buff.resize(count_vertexes, NONE);
    next.resize(2 * count_edges, NONE);


    std::cin >> source >> sink;
    source -= 1; sink -= 1;


    for (int i = 0; i < count_edges; i++) {
        int first = 0, second = 0, capacity = 0;
        std::cin >> first >> second >> capacity;
        first -= 1; second -= 1;
        make_edge(first, second, capacity);
    }


    std::cout << max_flow() << '\n';


    return 0;
}