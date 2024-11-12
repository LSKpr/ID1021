#include <bits/stdc++.h>
using namespace std;
//  //include <linux/time.h>
long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}
struct city;

struct connection
{
    int destination;
    int cost;
    connection(int _cost, int _destination)
    {
        cost = _cost;
        destination = _destination;
    }
};

struct city
{
    int id;
    string name;
    city(string _name, int _id)
    {
        name = _name;
        id = _id;
    };
};
unordered_map<string, int> name_id;
unordered_map<int, string> id_name;

struct hash_map
{
    int id_cnt = 0;
    int name_to_id(string s)
    {
        if (name_id.count(s) == 0)
        {
            name_id[s] = id_cnt;
            id_name[id_cnt] = s;
            return id_cnt++;
        }
        else
        {
            return name_id[s];
        }
    }
    string id_to_name(int id)
    {
        if (id_name.count(id) == 0)
            return "";
        return id_name[id];
    }
};
unordered_set<int> path;
struct cities_map
{
    hash_map hashmap;
    vector<city> cities;

    vector<connection> connections[100]; // 100 is the maximum number of cities
    int lookup(string s)
    {
        int id = hashmap.name_to_id(s);
        if (cities.size() <= id)
        {
            cities.push_back(city(s, id));
            return id;
        }
        else
            return id;
    }
    void connect(int src, int dst, int cost)
    {

        connections[src].push_back(connection(cost, dst));
        connections[dst].push_back(connection(cost, src));
        // cout << this->cities[src].name << " and " << this->cities[dst].name << " connected \n";
    }

    int shortest(int from, int to, int left)
    {
        if (from == to)
        {
            return 0;
        }
        int sofar = -1;
        for (connection next : this->connections[from])
        {
            if (next.cost <= left)
            {
                int d = shortest(next.destination, to, left - next.cost);
                if (d >= 0 && ((sofar == -1) || (d + next.cost) < sofar))
                    sofar = (d + next.cost);
            }
        }
        return sofar;
    }
    int shortest_path(int from, int to)
    {
        if (from == to)
        {
            return 0;
        }
        path.insert(from);
        int sofar = -1;
        for (connection next : this->connections[from])
        {
            if (path.count(next.destination) == 0)
            {
                int d = shortest_path(next.destination, to);
                if (d >= 0 && ((sofar == -1) || (d + next.cost) < sofar))
                    sofar = (d + next.cost);
            }
        }
        path.erase(from);
        return sofar;
    }
    int shortest_path_turbo(int from, int to,int curr_time , int* limit)
    {
        if (from == to)
        {
            *limit = curr_time;
            return 0;
        }
        path.insert(from);
        int sofar = -1;
        for (connection next : this->connections[from])
        {
            if(*limit!=-1 && (curr_time + next.cost > *limit)) continue; 
            if (path.count(next.destination) == 0)
            {
                int d = shortest_path_turbo(next.destination, to, curr_time + next.cost, limit);
                if (d >= 0 && ((sofar == -1) || (d + next.cost) < sofar))
                    sofar = (d + next.cost);
            }
        }
        path.erase(from);
        return sofar;
    }
};

cities_map *graph(string filename)
{
    ifstream file("tra");
    cities_map *ret = (cities_map *)malloc(sizeof(cities_map));
    // Check if the file opened successfully
    if (!file.is_open())
    {
        cout << "Could not open the file " << filename << endl;
    }
    // cout << ret << "\n";
    string line;
    while (getline(file, line))
    {
        string city1, city2, distStr;
        unsigned int dist;

        istringstream lineStream(line);
        // Parse city1, city2, and dist from each line
        if (getline(lineStream, city1, ',') &&
            getline(lineStream, city2, ',') &&
            getline(lineStream, distStr))
        {
            dist = stoi(distStr); // Convert distance to integer
            int src = ret->lookup(city1);
            int dst = ret->lookup(city2);
            // after creating city2 the pointer of city1 changed
            ret->connect(src, dst, dist);
            // cout << "City1: " << city1 << ", City2: " << city2 << ", Distance: " << dist << " km" << endl;
        }
    }

    file.close();
    return ret;
}
void print_id()
{
    for (int i = 0; i < 52; i++)
    {
        cout << i << " " << id_name[i] << "\n";
    }
}
int bench(int n, int loop, int random)
{
    struct timespec t_start, t_stop;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    // for (int i = 0; i < loop; i++)
    // {
    // }
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    return wall;
}
void small_main()
{
    cities_map *trains = graph("trains.cvs");
    cout << "doggo";
}
int main(int argc, char *argv[])
{
    int start_cities[] = {
        10, // Malmö
        11, // Göteborg
        10, // Malmö
        0,  // Stockholm
        0,  // Stockholm
        11, // Göteborg
        30, // Sundsvall
        33, // Umeå
        11, // Göteborg
        10  // Malmo
    };

    // Destination city IDs
    int destination_cities[] = {
        11, // Göteborg
        0,  // Stockholm
        0,  // Stockholm
        30, // Sundsvall
        33, // Umeå
        30, // Sundsvall
        33, // Umeå
        11, // Göteborg
        33,  // Umeå
        36  // kiruna
    };
    cities_map *trains = graph("trains.csv");
    print_id();

    // if (argc < 4)
    // {
    //     printf("usage: %s <from> <to> <limit>\n", argv[0]);
    //     exit(1);
    // }
    // int from = trains->lookup(argv[1]);
    // int to = trains->lookup(argv[2]);
    int from;
    int to;
    // int limit = atoi(argv[3]);
    for (int i = 0; i < 10; i++)
    {
        from = start_cities[i];
        to = destination_cities[i];
        cout << id_name[from] << " " << id_name[to] << "\n";
        int limit = -1;
        struct timespec t_start, t_stop;
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        // int s = trains->shortest_path_turbo(from, to,0,&limit);
        int s = trains->shortest_path(from,to);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        if (s > 0)
            printf("shortest path %d found in %.2fms\n", s, ((double)wall) / 1000000);
        else
            printf("no path found\n");
    }
}