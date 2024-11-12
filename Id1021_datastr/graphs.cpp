#include <bits/stdc++.h>
using namespace std;
// #include <linux/time.h>
long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
    return (t_stop->tv_nsec - t_start->tv_nsec) +
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000;
}
struct city;

struct connection
{
    city *destination;
    int cost;
    connection(int _cost, city *_destination)
    {
        cost = _cost;
        destination = _destination;
    }
};

struct city
{
    int id;
    string name;
    vector<connection> connsetions;
    city(string _name, int _id)
    {
        name = _name;
        id = _id;
    };
    void add_connection(city *dst, int cost)
    {
        connsetions.push_back(connection(cost, dst));
    }
};

struct hash_map
{
    unordered_map<string, int> name_id;
    unordered_map<int, string> id_name;
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

struct city_map
{
    hash_map hashmap;
    vector<city> cities;

    city *lookup(string s)
    {
        int id = hashmap.name_to_id(s);
        if (cities.size() <= id)
        {
            cities.push_back(city(s, id));
            return &cities[id];
        }
        else
            return &cities[id];
    }
};
void connect(city *src, city *dst, int dist)
{
    src->add_connection(dst, dist);
    dst->add_connection(src, dist);

    cout<< src->name <<" and "<<dst->name<<" connected \n";
}

city_map *graph(string filename)
{
    cout<<"sas\n";
    ifstream file("/home/lskpr/coding/Id1021_datastr/" + filename);
    city_map* ret = (city_map*)malloc(sizeof(city_map));
    // Check if the file opened successfully
    if (!file.is_open()) {
        cout << "Could not open the file " << filename << endl;
    }
    cout<<ret<<"\n";
    string line;
    while (getline(file, line))
    {
        string city1, city2, distStr;
        int dist;

        istringstream lineStream(line);
        cout<<"i ";
        // Parse city1, city2, and dist from each line
        if (getline(lineStream, city1, ',') &&
            getline(lineStream, city2, ',') &&
            getline(lineStream, distStr))
        {
            dist = stoi(distStr); // Convert distance to integer
            city * src = ret->lookup(city1);
            city * dst = ret->lookup(city2);
            connect(src , dst, dist);
             cout << "City1: " << city1 << ", City2: " << city2 << ", Distance: " << dist << " km" << endl;

        }
    }

    file.close();
    return ret;
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
    city_map* trains = graph("trains.cvs");
    cout<<"doggo";
}
int main()
{
    small_main();
    return 0;
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000, (1 << 9) * 1000, (1 << 10) * 1000};
    int k = 1;
    int loop = 1; // keep it 1
    for (int i = 0; i < 20; i++)
    {
        int n = 10000 + (i) * 5000;
        long min = 2147483647;
        for (int i = 0; i < k; i++)
        {
            long wall = bench(n, loop, 0);
            if (wall < min)
                min = wall;
        }
        // printf("%d %f ns\n", n, (double)min / loop);
        printf("%d %0.2f\n", n, bench(n, loop, 0));
    }
    // int n = 100;
    // int* arr = sorted(n);
    // for (int i = 0; i < n; i++)
    // {
    //     printf("%d \n", arr[i]);
    // }

    // printf("\n%d ",binary_search(arr,n,121));
    // printf("\nRANDOM WALK:\n");
    // for (int i = 0; i < 6; i++)
    // {
    //     int n = sizes[i];
    //     long min = 2147483647;
    //     for (int i = 0; i < k; i++)
    //     {
    //         long wall = bench(n, loop, 1);
    //         if (wall < min)
    //             min = wall;
    //     }
    //     printf("%d %0.2f ns\n", n, (double)min / loop);
    // }
}