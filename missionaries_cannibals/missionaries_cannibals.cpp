/*
* CPSC 3750
* Artificial Intelligence
* Missionaries and Cannibals problem
* Author: Luke Leontowich
*/

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

//  State of the system
typedef struct{
    //  LHS[0] : number of missionaries on the Left Hand Side
    //  LHS[1] : number of cannibals on the Left Hand Side
    int LHS[2];

    //  RHS[0] : number of missionaries on the Right Hand Side
    //  RHS[1] : number of cannibals on the Right Hand Side
    int RHS[2];

    //  inboat[0] : number of missionaries on the boat
    //  inboat[1] : number of cannibals on the boat
    int inboat[2];

    //  the total distance left to travel from the LHS to the RHS
    int distance;

    //  the sum of the absolute value of the difference between
    //  the cannibals and missionaries on the LHS, boat, and RHS
    int heurstic;

    //  f = distance + heuristic
    int f;
} Node;


//  Display the Node
void printNode(const Node & node) {
    cout << "LHS  BOAT RHS  f\n";
    cout << "m c  m c  m c  " << node.f << "\n";
    cout << node.LHS[0] << " " << node.LHS[1] << "  ";
    cout << node.inboat[0] << " " << node.inboat[1] << "  ";
    cout << node.RHS[0] << " " << node.RHS[1] << "\n\n";
}


//  Comparison for the priority queue
//  the lowest f value has the highest priority
struct compareNodes {
    bool operator()(const Node& n1, const Node& n2) {
        return n1.f > n2.f;
    }
};


//  There are ten different actions we can take
//  The actions are outlined within the function
constexpr int NUM_ACTIONS = 10;
void transition(const Node& parent, Node& child, const int& action) {

        //  initialize the child node infomation
        child.LHS[0] = parent.LHS[0];
        child.LHS[1] = parent.LHS[1];
        child.inboat[0] = parent.inboat[0];
        child.inboat[1] = parent.inboat[1];
        child.RHS[0] = parent.RHS[0];
        child.RHS[1] = parent.RHS[1];

        switch (action) {
            //  take 1 missionary from LHS and put on boat
            case 0: child.LHS[0] = parent.LHS[0] - 1;
                    child.inboat[0] = parent.inboat[0] + 1; 
                    break;
            
            //  Take 2 misisonaries from LHS and put on boat
            case 1: child.LHS[0] = parent.LHS[0] - 2; 
                    child.inboat[0] = parent.inboat[0] + 2; 
                    break;

            //  Take 1 Cannibals from LHS and put on boat
            case 2: child.LHS[1] = parent.LHS[1] - 1; 
                    child.inboat[1] = parent.inboat[1] + 1; 
                    break;

            //  Take 2 Cannibals from LHS and put on boat
            case 3: child.LHS[1] = parent.LHS[1] - 2; 
                    child.inboat[1] = parent.inboat[1] + 2; 
                    break;

            //  Take 1 Missionary and 1 Cannibals from LHS and put on boat
            case 4: child.LHS[0] = parent.LHS[0] - 1; 
                    child.inboat[0] = parent.inboat[0] + 1;
                    child.LHS[1] = parent.LHS[1] - 1; 
                    child.inboat[1] = parent.inboat[1] + 1;
                    break;

            //  Take 1 missionary off the boat and unto the RHS
            case 5: child.inboat[0] = parent.inboat[0] - 1; 
                    child.RHS[0] = parent.RHS[0] + 1; 
                    break;

            //  Take 2 missionaries off the boat and unto the RHS
            case 6: child.inboat[0] = parent.inboat[0] - 2; 
                    child.RHS[0] = parent.RHS[0] + 2; 
                    break;


            //  Take 1 Cannibals off the boat and unto the RHS
            case 7: child.inboat[1] = parent.inboat[1] - 1; 
                    child.RHS[1] = parent.RHS[1] + 1; 
                    break;

            //  Take 2 Cannibals off the boat and unto the RHS
            case 8: child.inboat[1] = parent.inboat[1] - 2; 
                    child.RHS[1] = parent.RHS[1] + 2; 
                    break;

            //  Take 1 Missionary and 1 Cannibals off the baot and unto the RHS
            case 9: child.inboat[0] = parent.inboat[0] - 1; 
                    child.RHS[0] = parent.RHS[0] + 1; 
                    child.inboat[1] = parent.inboat[1] - 1; 
                    child.RHS[1] = parent.RHS[1] + 1; 
                    break;

            default: break;
        }
}

//  Check that a node is valid
bool isValidInfo(const Node& node) {
    //  check lowert bounds 
    if (node.RHS[0] < 0 || node.RHS[1] < 0 || 
        node.inboat[0] < 0 || node.inboat[1] < 0 ||
        node.LHS[0] < 0 || node.LHS[1] < 0) {
            
        return false;
    }

    //  check upper bounds
    if (node.RHS[0] > 3 || node.RHS[1] > 3 || 
        node.inboat[0] > 2 || node.inboat[1] > 2 ||
        node.LHS[0] > 3 || node.LHS[1] > 3) {
            
        return false;
    }

    // check that there are two or less people in the boat
    if (node.inboat[0] + node.inboat[1] > 2) {
        return false;
    }


    //  check for 3 missionaries and 3 cannibals
    if (node.RHS[0] + node.inboat[0] + node.LHS[0] != 3 ||
        node.RHS[1] + node.inboat[1] + node.LHS[1] != 3) {
        return false;
    }

    //  we need one person in the boat unless we are in the start or end state
    if (node.RHS[0] != 3 || node.RHS[1] != 3) {
        if (node.inboat[0] + node.inboat[1] <= 0) {
            return false;
        }
    }

    //  make sure is never mor cannibals than missionaries
    if ((node.RHS[0] != 0 && node.RHS[0] < node.RHS[1]) || 
       (node.inboat[0] != 0 && node.inboat[0] < node.inboat[1]) ||
       (node.LHS[0] != 0 && node.LHS[0] < node.LHS[1])){
            return false;
    }
    return true;
}


//  Calculate the heuristic
//  the sum of the absolute value of the difference between
//  the cannibals and missionaries on the LHS, boat, and RHS
void calcHeuristic(Node& node) {
   node.heurstic = abs(node.LHS[0] - node.LHS[0]) +
                   abs(node.inboat[0] - node.inboat[1]) +
                   abs(node.RHS[0] - node.RHS[1]);
}

//  Calculate the distance
//  the total distance left to travel from the LHS to the RHS
void calcDistance(Node& node) {
    node.distance = 2*node.LHS[0] + 2*node.LHS[1] + 
                    node.inboat[0] + node.inboat[1];
}


//  Converts a node to a unique integer value
//  This value is needed for the checked bool array
//  that will check if we have already processed a certain node
//  or not
int nodeToInt(const Node& node) {
    int x = 0;
    x += node.RHS[0] * pow(4, 5);
    x += node.RHS[1] * pow(4,4);
    x += node.inboat[0] * pow(4, 3);
    x += node.inboat[1] * pow(4,2);
    x += node.LHS[0] * 4;
    x += node.LHS[1];
    return x;
}

int main () {
    priority_queue<Node, vector<Node>, compareNodes> pq;

    //  the index value can be at most 3*4^5 + 3*4^4 = 3840
    bool checked[4000];
    fill(checked, checked + 4000, false);

    //  initialize root node
    Node root;
    root.LHS[0] = 3;
    root.LHS[1] = 3;
    root.inboat[0] = 0;
    root.inboat[1] = 0;
    root.RHS[0] = 0;
    root.RHS[1] = 0;
    calcDistance(root);
    calcHeuristic(root);
    root.f = root.distance + root.heurstic;

    //  push root into priority queue
    pq.push(root);

    //  current node
    Node current = root;

    //  mark that we've seen the root node
    checked[nodeToInt(root)] = true;

    //  if current.RHS[0] = current.RHS[1] = 3 then everyone has reached
    //  the other side
    while (current.RHS[0] != 3 || current.RHS[1] != 3) {
        current = pq.top();
        pq.pop();

        //  print the order of the nodes
        printNode(current);

        //  go through each action and check if it is a valid node
        for (int i = 0; i < NUM_ACTIONS; ++i) {
            Node child;
            transition(current, child, i);

            //  if valid then load child unto the priority queue
            if (isValidInfo(child)) {
                int index = nodeToInt(child);
                if (!checked[index]) {
                    calcHeuristic(child);
                    calcDistance(child);
                    child.f = child.distance + child.heurstic;
                    pq.push(child);
                    checked[index] = true;
                }
            } 
        }
    }
    return 0;
}