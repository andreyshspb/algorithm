#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>


static constexpr int NONE = -1;
static constexpr long long INF = 1000 * 1000 * 1000;


struct Edge {
    int from;
    int to;
    long long flow;
    long long capacity;
    long long cost;
};


int count_vertexes = 0;
int count_edges = 0;

int source = 0;
int sink = 0;


std::vector<long long> phi;
std::vector<long long> dist;
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

void make_edge(int begin, int end, long long capacity, long long cost) {
    Edge edge = {begin, end, 0, capacity, cost};
    Edge rev_edge = {end, begin, 0, 0, -cost};
    add_edge(begin, edge);
    add_edge(end, rev_edge);
}


void ford_bellman() {
    std::fill(phi.begin(), phi.end(), INF);
    phi[source] = 0;

    bool flag = true;

    while (flag) {
        flag = false;
        for (int id = 0; id < edges.size(); id++) {
            Edge edge = edges[id];
            if (edge.flow < edge.capacity && phi[edge.from] + edge.cost < phi[edge.to]) {
                phi[edge.to] = phi[edge.from] + edge.cost;
                flag = true;
            }
        }
    }
}


bool dijkstra() {
    std::fill(dist.begin(), dist.end(), INF);
    std::fill(from.begin(), from.end(), NONE);
    dist[source] = 0;

    std::priority_queue<std::pair<long long, int>> queue;
    queue.push({0, source});

    while (!queue.empty()) {
        int cur = queue.top().second;
        queue.pop();
        for (int id = head[cur]; id != NONE; id = next[id]) {
            int to = edges[id].to;
            long long weight = edges[id].cost + phi[cur] - phi[to];
            if (edges[id].flow < edges[id].capacity && dist[cur] + weight < dist[to]) {
                dist[to] = dist[cur] + weight;
                from[to] = id;
                queue.push({-dist[to], to});
            }
        }
    }

    return dist[sink] != INF;
}


long long min_cost_max_flow() {
    long long min_cost = 0;

    ford_bellman();

    while (dijkstra()) {
        long long flow = INF;
        for (int cur = from[sink]; cur != NONE; cur = from[edges[cur].from]) {
            flow = std::min(flow, edges[cur].capacity - edges[cur].flow);
        }
        for (int cur = from[sink]; cur != NONE; cur = from[edges[cur].from]) {
            min_cost += flow * edges[cur].cost;
            edges[cur].flow += flow;
            edges[cur ^ 1].flow -= flow;
        }

        for (int v = 0; v < count_vertexes; v++) {
            phi[v] += dist[v];
        }
    }

    return min_cost;
}



int main() {


    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);


    std::cin >> count_vertexes >> count_edges;


    phi.resize(count_vertexes, INF);
    dist.resize(count_vertexes, INF);
    from.resize(count_vertexes, NONE);
    head.resize(count_vertexes, NONE);
    next.resize(2 * count_edges, NONE);


    std::cin >> source >> sink;


    for (int i = 0; i < count_edges; i++) {
        int first = 0, second = 0;
        long long capacity = 0, cost = 0;
        std::cin >> first >> second >> capacity >> cost;
        first -= 1; second -= 1;
        make_edge(first, second, capacity, cost);
    }


    std::cout << min_cost_max_flow() << '\n';


    return 0;
}