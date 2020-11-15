

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>


static constexpr int NONE = -1;
static constexpr int INF = 1000 * 1000 * 1000;


struct Edge {
    int from;
    int to;
    int flow;
    int capacity;
    int cost;
};


int count_vertexes = 0;
int count_edges = 0;

int source = 0;
int sink = 0;


std::vector<int> dist;
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

void make_edge(int begin, int end, int capacity, int cost) {
    Edge edge = {begin, end, 0, capacity, cost};
    Edge rev_edge = {end, begin, 0, 0, -cost};
    add_edge(begin, edge);
    add_edge(end, rev_edge);
}


bool ford_bellman() {
    std::fill(dist.begin(), dist.end(), INF);
    std::fill(from.begin(), from.end(), NONE);

    dist[source] = 0;

    bool flag = true;

    while (flag) {
        flag = false;
        for (int id = 0; id < edges.size(); id++) {
            Edge edge = edges[id];
            if (edge.flow < edge.capacity && dist[edge.from] + edge.cost < dist[edge.to]) {
                dist[edge.to] = dist[edge.from] + edge.cost;
                from[edge.to] = id;
                flag = true;
            }
        }
    }

    return dist[sink] != INF;
}


unsigned long long min_cost_max_flow() {
    unsigned long long min_cost = 0;

    while (ford_bellman()) {
        int flow = INF;
        for (int cur = from[sink]; cur != NONE; cur = from[edges[cur].from]) {
            flow = std::min(flow, edges[cur].capacity - edges[cur].flow);
        }
        for (int cur = from[sink]; cur != NONE; cur = from[edges[cur].from]) {
            min_cost += 1ULL * flow * edges[cur].cost;
            edges[cur].flow += flow;
            edges[cur ^ 1].flow -= flow;
        }
    }

    return min_cost;
}


int main() {


    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);


    std::cin >> count_vertexes >> count_edges;


    dist.resize(count_vertexes, INF);
    from.resize(count_vertexes, NONE);
    head.resize(count_vertexes, NONE);
    next.resize(2 * count_edges, NONE);


    std::cin >> source >> sink;


    for (int i = 0; i < count_edges; i++) {
        int first = 0, second = 0, capacity = 0, cost = 0;
        std::cin >> first >> second >> capacity >> cost;
        first -= 1; second -= 1;
        make_edge(first, second, capacity, cost);
    }


    std::cout << min_cost_max_flow() << '\n';


    return 0;
}