/*
 *Author: Jonathan Lee Drouillard
 *Description: Given a set of instructions, predict which
               data structures are following the set of rules.
 *Date Created: 20120825
 *Date Last Modified: 20120826

 *Data Abstraction
    Creates integer method for storing current instruction type.
    Creates integer n to store the number of instructions to follow.
    Creates DATASTRUCTURE ds to generate our test data structures and perform methods.
    Creates XY, pair of ints, to store the x and y following the instruction.
    Creates queue of strings to store the various results.
 *Input
    Read in the amount of instruction lines as an integer.
    Read in a method (insert/remove).
    Read in a point through a x coordinate and y coordinate.
    Continue this process until no new instruction lines are read in.
 *Process/Implementation
    User inputs amount of n instruction lines, followed by n instructions in the
    format of (method) (x) (y).  The method instructs the program to insert into
    a data structure or remove from a data structure.  On an insert, the x y pair
    is simply placed into all 3 data structures of interest, (stack, queue, and
    priority queue).  On a remove method, the data structures perform their
    various methods to get the front/top of the data structure.  The goal at this
    point is to match up the x,y point to the method's remove point.  If our point
    matches then remove method's point, then, we have a match.  If a data structure
    makes it through the whole set with no mismatches, then, we assume that data
    structure followed the set of methods/instructions!
    Once we have read the end of the file, the program should explain to the user
    which data structures it has encountered.  It has the choices of priority queue,
    queue, or stack if and only if one of these choices were the only data structure
    encountered.  If the program encountered two or more data structures, the program
    will explain that it is 'not sure'.  If we could not determine any logical
    data structures for the set of methods/instructions, the program will explain
    that is it 'impossible.'

    For each iteration of a set of instructions, we must clear the 3 data structures
    of interest, as well as reset the booleans which maintain if the methods
    follow the rules of the data structure.

    Upon an insert instruction, the program will simply insert the point into all
    3 data structures of interest.

    Upon removal of a point from the 3 data structures, we will remove the point,
    and check it again the method's expected values.  If the values do not match up,
    we will simply toggle a boolean to state 'it is not this data structure'.

    A simple function was created to output the current state of the DATASTRUCTURE
    structure at the end of execution for the set of methods.
 *Output
    "priority queue" if the instructions resemble _only_ a priority queue.
    "queue" if the instructions resemble _only_ a queue.
    "stack" if the instructions resemble _only_ a stack.
    "not sure" if the instructions resemble two or more data structures.
    "impossible" if the instructions do not resemble any of the data structures.

 *Assumptions
    Assumes the user follows all input methods in order, ie:
    -#1- n instructions as an integer (1...999)
    -#2- n method as an integer (1,2)
    -#3- n x coordinate (1...100)
    -#4- n y coordinate (1...100)
    Any deviations from this input/order will output unexpected results.
**/

#include <iostream> //cout, cin
#include <queue>    //priority_queue, queue
#include <stack>    //stack
#include <utility>  //pair
using namespace std;

#define INSERT 1 //Insert method for instructions
#define REMOVE 2 //Remove method for instructions
typedef pair<int, int> XY; //Point

/*
 *Description:
 *    Functions as a comparator for the priority queue of points.
 *    Overloaded operator to prove that one object is of greater
 *    or equal value to another.  The Manhattan distance formula
 *    was used to determine equality.  ABS(X_1+Y_1) < ABS(X_2+Y_2)
 *    (Did not use abs value because reqs doc states coords (1...100).)
**/
struct COMPARATOR {
    //************************************************************
    // description: Manhattance Distance based comparator.       *
    // return: True: pq1 < pq2,   False: pq1 >= pq2              *
    // pre:  pq1 and pq2 contain non-negative points.            *
    // post: N/A                                                 *
    //************************************************************
    bool operator() (const XY& pq1, const XY& pq2) {
        return (pq1.first + pq1.second < pq2.first + pq2.second);
    }
};

/*
 *Description:
 *    Functions as a storage unit for our 3 data structures of interest.
 *    Performs the INSERT and REMOVE methods and checks expected vs
 *    results in terms of our data structures and the instructions from
 *    the user.
 *    Produces the various outputs for the program.
**/
struct DATASTRUCTURE {
    priority_queue<XY, vector<XY>, COMPARATOR> p;
    queue<XY> q;
    stack<XY> s;
    bool isP, isQ, isS; //Instructions match the data structure.

    //************************************************************
    // description: Resets 3 data structures and their booleans. *
    // return: None.                                             *
    // pre: None.                                                *
    // post: All data structures reset and booleans set to true. *
    //************************************************************
    void empty() {
        p = priority_queue<XY, vector<XY>, COMPARATOR>();
        q = queue<XY>();
        s = stack<XY>();
        isP = true;
        isQ = true;
        isS = true;
    }

    //************************************************************
    // description: Checks if the remove/expected command match  *
    //              any of the behaviors found in the data       *
    //              structures.  If they do not match, the data  *
    //              structure's corresponding bool is set false. *
    // return: N/A                                               *
    // pre:  Other contains non-negative coordinates.            *
    //       Data structures contain data.                       *
    // post: Data structures have top/front removed.             *
    //       If results are not expected, set the data structure *
    //       boolean to false.                                     *
    //************************************************************
    void remove(const XY& other) {
        XY self;  //Store what we pop.
        self = p.top();
        if(self.first != other.first || self.second != other.second) {
            isP = false;
        }
        self = q.front();
        if(self.first != other.first || self.second != other.second) {
            isQ = false;
        }
        self = s.top();
        if(self.first != other.first || self.second != other.second) {
            isS = false;
        }
        p.pop();
        q.pop();
        s.pop();
    }

    //************************************************************
    // description: Pushes a coordinate pair onto all 3 data     *
    //              structures.                                  *
    // return: N/A                                               *
    // pre: Other contains non-negative coordinates.             *
    // post: Data Structures contain the coordinate pair.        *
    //************************************************************
    void insert(const XY& other) {
        p.push(other);
        q.push(other);
        s.push(other);
    }

    //************************************************************
    // description: Based on the data structure's boolean,       *
    //              determines which type of data structure      *
    //              follows the set of instructions.             *
    // return: String.                                           *
    // pre: N/A                                                  *
    // post: N/A                                                 *
    //************************************************************
    string status() {
        if((isP && (isQ || isS)) || isQ && (isS || isP)) {
            return "not sure";
        } else if(!isP && !isQ && !isS) {
            return "impossible";
        } else if (isQ) {
            return "queue";
        } else {
            return (isP ? "priority queue" : "stack");
        }
    }
};

int main() {
    int method;                //Which Method, INSERT or REMOVE (user input).
    int n;                    //Amount of Instructions in this test case. (user input)
    DATASTRUCTURE ds;        //Storage/functional unit for data and data structures.
    XY in_xy;                //Points from the user's methods (user input)
    queue<string> result;    //Storage for our output.
    while(cin >> n) { //Until we run out of cases...
        ds.empty();  //Clear out all data structures.
        while(n != 0) { //Until we run out of instructions...
            cin >> method >> in_xy.first >> in_xy.second; //Read in insert/remove command, x, y.
            if (method == INSERT) {
                ds.insert(in_xy);
            } else if (method == REMOVE) {
                ds.remove(in_xy);
            } else { //Unknown method.
                return 1;
            }
            n--; //Decrement amount of expected instructions.
        }
        result.push(ds.status()); //Store the result.
    }

    while(!result.empty()) { //Print all of the results.
        cout << result.front().c_str() << endl;
        result.pop();
    }
    return 0;
}

