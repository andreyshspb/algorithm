

#include <vector>
#include <queue>
#include <iostream>


static constexpr int NONE = -1;
static constexpr int INF = 1000 * 1000 * 1000;


struct Edge {
    int to;
    double flow;
    double capacity;
};


int source = 0;
int sink = 0;


std::vector<int> mark;
std::vector<int> from;
std::vector<int> head;
std::vector<int> next;
std::vector<Edge> edges;


void add_edge(int vertex, Edge edge) {
    edges.push_back(edge);
    int current = edges.size() - 1;
    next[current] = head[vertex];
    head[vertex] = current;
}

void make_edge(int begin, int end, double capacity) {
    Edge edge = {end, 0.0, capacity};
    Edge rev_edge = {begin, 0.0, 0.0};
    add_edge(begin, edge);
    add_edge(end, rev_edge);
}


std::vector<int> path;

bool make_path() {
    path = {};
    std::fill(from.begin(), from.end(), NONE);
    std::fill(mark.begin(), mark.end(), NONE);

    std::queue<int> queue;
    queue.push(source);
    mark[source] = 0;

    while (!queue.empty()) {
        int cur = queue.front();
        queue.pop();
        for (int id = head[cur]; id != NONE; id = next[id]) {
            Edge edge = edges[id];
            if (mark[edge.to] == NONE && edge.flow < edge.capacity) {
                mark[edge.to] = mark[cur] + 1;
                from[edge.to] = id;
                queue.push(edge.to);
            }
        }
    }

    if (mark[sink] == NONE) {
        return false;
    }

    int cur = sink;
    while (cur != source) {
        path.push_back(from[cur]);
        cur = edges[from[cur] ^ 1].to;
    }

    return true;

}


double max_flow() {
    while (make_path()) {
        double min = 1.0 * INF;
        for (int id : path) {
            min = std::min(min, edges[id].capacity - edges[id].flow);
        }
        for (int id : path) {
            edges[id].flow += min;
            edges[id ^ 1].flow -= min;
        }
    }

    double flow = 0;
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
    from.resize(count_vertexes, NONE);
    head.resize(count_vertexes, NONE);
    next.resize(2 * count_edges, NONE);


    std::cin >> source >> sink;
    source -= 1; sink -= 1;


    for (int i = 0; i < count_edges; i++) {
        int first = 0, second = 0;
        double capacity = 0;
        std::cin >> first >> second >> capacity;
        first -= 1; second -= 1;
        make_edge(first, second, capacity);
    }


    std::cout << max_flow() << '\n';


    return 0;
}