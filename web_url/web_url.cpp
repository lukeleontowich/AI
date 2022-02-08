/*
* CPSC 3750
* Artificial Intelligence
* Web Url Problem
* Author: Luke Leontowich
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <utility>
#include <map>
#include <queue>

using namespace std;

//  the string is the url
//  the node is the heuristic which is the
//  number of '/' which is how I'm determining depth
typedef pair<string, int> Node;

//  Class that represents the url strings
class Graph {
public:
    Graph() {}
    ~Graph() {}

    //  Add a link between a source and destination url
    void addEdge(const string& src, const string& dest) {
        Node s;
        Node d;
        s.first = src;
        d.first = dest;
        calcHeuristic(s);
        calcHeuristic(d);

        adj_list[s.first].push_back(d);
        adj_list[d.first].push_back(s);
    }



    //  Search for a path, return an empty vector if nothing is found
    vector<string> search(const string& src, const string& dest) {
        //  we don't even need a queue since we are just backing up one folder
        //  and checking if the two sides are equal
        //  we will use the heuristic of the depth of the file path as our guide
        //  If both sides (src & dest) end up at "https://" then there is no common
        //  path
        vector<string> result_src;
        vector<string> result_dest;

        Node current_src;
        current_src.first = src;
        calcHeuristic(current_src);

        Node current_dest;
        current_dest.first = dest;
        calcHeuristic(current_dest);


        //  add results
        result_src.push_back(parseNode(current_src));
        result_dest.push_back(parseNode(current_dest));
    
        bool keep_going = true;
        while (keep_going) {
            //  check if we are at "https://";
            if (current_src.second <= 2 || current_dest.second <= 2) {
                vector<string> empty_set;
                return empty_set;
            }


            //  case that the src is deeper and needs to backtrack
            if (current_src.second > current_dest.second) {
                while (current_src.second > current_dest.second) {
                    //   move src up one
                    Node parent;
                    getParent(current_src, parent);
                    current_src = parent;

                    //  store the result
                    result_src.push_back(parseNode(current_src));

                }
                //  check if the two url's are the same
                if (current_src.first == current_dest.first) {
                    keep_going = false;
                }

            //  case that the dest is deeper and needs to backtrack
            } else if (current_dest.second > current_src.second) {
                while (current_dest.second > current_src.second) {
                    //  move dest up one
                    Node parent;
                    getParent(current_dest, parent);
                    current_dest = parent;

                    //  store the result
                    result_dest.push_back(parseNode(current_dest));

                }
                //  check if the two url's are the same
                if (current_src.first == current_dest.first) {
                    keep_going = false;
                }

            //  case that the src and dest already have the same depth
            } else {
                if (current_src.first == current_dest.first) {
                    keep_going = false;

                //  we need to move both the src and dest up one 
                } else {
                    //   move src up one
                    Node parent;
                    getParent(current_src, parent);
                    current_src = parent;

                    //  store the result
                    result_src.push_back(parseNode(current_src));

                    //  move dest up one
                    getParent(current_dest, parent);
                    current_dest = parent;

                    //  store the result
                    result_dest.push_back(parseNode(current_dest));
                }
            
            }
        }

        //  prepare the result
        //  remove the last element of the dest result since it already exists in src
        result_dest.erase(--result_dest.end());

        //  now reverse the dest string
        reverse(result_dest.begin(), result_dest.end());
        
        //  attach the dest to the src
        for (auto x : result_dest) {
            result_src.push_back(x);
        }
        return result_src;
    }

private:

    unordered_map<string, vector<Node>> adj_list;

    //  heuristic is simply the depth which can be found be counting the '/'
    void calcHeuristic(Node& node) {
        node.second = count(node.first.begin(), node.first.end(), '/');
    }

    //  returns the parent, given a child
    void getParent(const Node& child, Node& parent) {
        for (auto it = adj_list[child.first].begin();
                  it != adj_list[child.first].end();
                  ++it) {

            if (it->second < child.second) {
                //  there will be only one parent node
                parent = *it;
            }
        }
    }

    //  parse the node so that just the last part of the url is returned
    string parseNode(const Node& node) {
        string temp = node.first.substr(node.first.find_last_of('/')+1);
        return temp;
    }
};




int main () {
    Graph network;

    network.addEdge("https://", "https://nhl.com");

    network.addEdge("https://nhl.com", "https://nhl.com/schedule");
    network.addEdge("https://nhl.com", "https://nhl.com/news");
    network.addEdge("https://nhl.com", "https://nhl.com/teams");

    network.addEdge("https://nhl.com/teams", "https://nhl.com/teams/sharks");
    network.addEdge("https://nhl.com/teams", "https://nhl.com/teams/flames");
    network.addEdge("https://nhl.com/teams", "https://nhl.com/teams/redwings");

    network.addEdge("https://nhl.com/teams/sharks", 
                    "https://nhl.com/teams/sharks/stats");
    network.addEdge("https://nhl.com/teams/sharks", 
                    "https://nhl.com/teams/sharks/tickets");

    network.addEdge("https://nhl.com/teams/sharks/stats", 
                    "https://nhl.com/teams/sharks/stats/player");
    network.addEdge("https://nhl.com/teams/sharks/stats", 
                    "https://nhl.com/teams/sharks/stats/goalie");

    network.addEdge("https://nhl.com/teams/sharks/tickets", 
                    "https://nhl.com/teams/sharks/tickets/premium");
    network.addEdge("https://nhl.com/teams/sharks/tickets", 
                    "https://nhl.com/teams/sharks/tickets/sale");

    network.addEdge("https://nhl.com/teams/flames", 
                    "https://nhl.com/teams/flames/stats");
    network.addEdge("https://nhl.com/teams/flames", 
                    "https://nhl.com/teams/flames/tickets");

    network.addEdge("https://nhl.com/teams/flames/stats", 
                    "https://nhl.com/teams/flames/stats/player");

    network.addEdge("https://nhl.com/teams/flames/stats", 
                    "https://nhl.com/teams/flames/stats/goalie");

    network.addEdge("https://nhl.com/teams/flames/tickets", 
                    "https://nhl.com/teams/flames/tickets/premium");

    network.addEdge("https://nhl.com/teams/flames/tickets", 
                    "https://nhl.com/teams/flames/tickets/sale");

       
    network.addEdge("https://nhl.com/teams/redwings", 
                    "https://nhl.com/teams/redwings/stats");
    network.addEdge("https://nhl.com/teams/redwings", 
                    "https://nhl.com/teams/redwings/tickets");

    network.addEdge("https://nhl.com/teams/redwings/stats", 
                    "https://nhl.com/teams/redwings/stats/player");

    network.addEdge("https://nhl.com/teams/redwings/stats", 
                    "https://nhl.com/teams/redwings/stats/goalie");

    network.addEdge("https://nhl.com/teams/redwings/tickets", 
                    "https://nhl.com/teams/redwings/tickets/premium");
    network.addEdge("https://nhl.com/teams/redwings/tickets", 
                    "https://nhl.com/teams/redwings/tickets/sale");      

    
    string url1 = "https://nhl.com/teams/redwings/stats";
    string url2 = "https://nhl.com/teams/sharks/tickets";
    cout << "searching on:\n";
    cout << url1 << "\n";
    cout << url2 << "\n";
    auto result = network.search("https://nhl.com/teams/redwings/stats", 
                                 "https://nhl.com/teams/sharks/tickets");

    if (!result.empty()) {
        cout << "\nfile path:\n";
        for (unsigned i = 0; i < result.size()-1; ++i) {
            cout << result[i] << "-->";
        }
        cout << result[result.size()-1];
        cout << "\n";
    } else {
        cout << "no path found\n";
    }
    return 0;
}