#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <set>
#include <climits>

template <typename T>
class Vertex
//Хранит имя вершины, путь до нее и поток по этому пути
{
public:
    T name;
    std::string path;
    int flow_for_edge;
    int flow_for_path = 0;
    Vertex(T name_, std::string path_, int flow_f_e, int flow_f_p) 
        : name(name_), path(path_), flow_for_edge(flow_f_e), flow_for_path(flow_f_p) {}
};

template <typename T>
class Graph
{
private:
    int size; //кол-во рёбер
    T start;  //нач. вершина
    T end;    //кон. вершниа
    std::map<T, std::map<T, int>> const_edges; //Мапа рёбер (неизменяется, нужна для вывода)
    std::map<T, std::map<T, int>> edges;       //Мапа рёбер, также хранит обратные рёбра (изменяется)
public:
    Graph() = default;
    void init();
    void print();
    int ff_alg(); //Форд Фалкерсон
};

template <typename T>
void Graph<T>::init()
//Читаем кол-во рёбер, старт, конец и заполняем мапу рёбер
{
    T from, to;
    int weight;
    std::cin >> size;
    std::cin >> start >> end;

    for (int i=0; i<size; i++)
    {
        std::cin >> from >> to >> weight;
        const_edges[from][to] = weight;
        //Мапу для вывода заполняем всеми данными в тесте рёбрами

        if (to == start || from == end) 
        //В мапу для алгоритма кладём все рёбра, кроме ведущих в начало и исходящие из конца
            continue;

        if (edges[to].count(from) == 0)
        {
            edges[from][to] = 0;
        }
        if (edges[from].count(to) == 0)
        {
            edges[from][to] += weight;
        } else {
            edges[from][to] = weight;
        }
    }
}

template <typename T>
void Graph<T>::print()
{
    for (const auto var : const_edges)
    {
        for (const auto var2 : var.second)
        {
            int f = const_edges[var.first][var2.first] - edges[var.first][var2.first];
            if (f < 0) f = 0;
            std::cout << var.first << " " << var2.first << " " << 
                    f << std::endl;
        }
    }
}

template <typename T>
struct set_cmp
//кмп для set'a сортируем сначала по потоку, потом по имени вершины
{
    bool operator() (Vertex<T> a, Vertex<T> b)
    {
        if (a.flow_for_edge == b.flow_for_edge)
            return a.name < b.name;
        return a.flow_for_edge < b.flow_for_edge;
    }
};

template <typename T>
int Graph<T>::ff_alg()
{
    int max_flow = 0; //макс поток
    std::set<Vertex<T>, set_cmp<T>> open; //вершины, доступные для посещения
        //Хранит имя вершины, путь до нее, поток по последнему ребру и мин. поток на пути
    std::string close; //хранит имена закрытых вершин

    open.insert(Vertex<T>(start, "", INT_MAX, INT_MAX));


    while (!open.empty())
    {
        Vertex<T> curr = *(--open.end());
        std::cout << "Open list:" << std::endl;
        for (auto var : open)
        {
            std::cout << var.path << "->" << var.name << "; Flow for path = " << 
                        var.flow_for_path << "; Flow for last edge = " << 
                        var.flow_for_edge << ";" << std::endl; 
        }
        std::cout << "Edge selected: " << curr.path << "->" << curr.name << " " << 
                        "; Flow for path = " << curr.flow_for_path << 
                        "; Flow for last edge = " << curr.flow_for_edge << 
                        ";" << std::endl; 
        open.erase(--open.end());

        if(curr.name == end)
        // если пришли в конец
        {
            curr.path += curr.name;
            T from, to;
            for (int i=0; i< curr.path.length() - 1; i++)
            {
                from = curr.path[i];
                to = curr.path[i+1];
                edges[from][to] -= curr.flow_for_path;
                edges[to][from] += curr.flow_for_path;
            }
            max_flow += curr.flow_for_path;

            std::cout << "Reached the end! Path: " << curr.path << "; Flow for path = " <<
                        curr.flow_for_path << "; Summ flow for graph = " << max_flow << std::endl;

            close.clear();
            open.clear();
            open.insert(Vertex<T>(start, "", INT_MAX, INT_MAX));
            continue;
        }
        close.push_back(curr.name);
        std::cout << "Close list: " << close << ";" << std::endl;

        for (auto var : edges[curr.name])
        {
            if (close.find(var.first) != std::string::npos || var.second <= 0)
            {
                continue; // если вершина находится в списке закрытых или по ней нельзя пустить поток
            }
            if (var.first == start) //пропускаем вершины, ведущие в начало
                continue;
            open.insert(Vertex<T>(var.first, curr.path + curr.name, var.second,
                                    std::min(curr.flow_for_path, var.second)));

            std::cout << "Edge {" << curr.path + curr.name << "->" << var.first <<
                        "; " << var.second << "; " << std::min(curr.flow_for_path, var.second) <<
                        "} add to open list" << std::endl;
        }
    }
    return max_flow;
}

int main()
{
    Graph<char> graph;
    graph.init();
    std::cout << graph.ff_alg() << std::endl;
    graph.print();

    return 0;
}