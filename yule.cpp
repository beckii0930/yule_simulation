/**
 * AUTHOR: Yibei Jiang
 * PID: A91076677
 * LOGIN: cs100vba
 * Date: Aug 14th
*/

// #include "Node.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <fstream>
#include <string>
using namespace std;
#define MAX 999999

/**
 * Parsing input class
 */
class ParseInput {

public:
  ParseInput (int &argc, char **argv) {
    for (int i=0; i<argc; i++) {
      this -> inputs.push_back(std::string(argv[i]));
    }
  }

  /**
   * check if the option exists in the command
   * @param: the option to check for
   * @return: true if the command exists, false otherwise
   */
  bool optExist(const std::string &opt) const {
    std::vector<std::string>::const_iterator iter2;
    iter2 = std::find(this->inputs.begin(), this->inputs.end(), opt);
    return iter2 != this->inputs.end();
  }

  /**
   * get the input after the option
   * @param: the option to look for
   * @return: the string after the option
   */
  const std::string& getOption(const std::string &opt) const {

    // create an iterator to iterate through the input and find the option
    std::vector<std::string>::const_iterator iter;
    iter = std::find(this->inputs.begin(), this->inputs.end(), opt);

    //return the information after the option
    if(iter != this->inputs.end() && ++iter != this->inputs.end()) {
      return *iter;
    }
    return "";
  }

private:
  std::vector<std::string> inputs;
};

//  --------------------------- Node starts here --------------------------------
/**
 * Node Class
 */
class Node {

public:

  Node* left;
  Node* right;
  Node* parent;
  bool active;
  double mytime;
  double depth;
  int id;

  Node (int numNode) : left(0), right(0), mytime(0), id(numNode), active(false)
  {}

  bool isActive() const {
    return active;
  }
  void markActive() {
    active = true;
  }

  Node* getLeft() const{
    return left;
  }
  Node* getRight() const{
    return right;
  }
  double getTime() const{
    return mytime;
  }
  /*bool operator ==(const Node & obj) const
  {
   if (splitTime == obj.splitTime)
      return true;
   else
      return false;
  }*/

  /* Function to print level order traversal a tree*/
  /*void printLevelOrder(struct Node* root)
  {
      int h = height(root);
      int i;
      for (i=1; i<=h; i++)
      {
          printGivenLevel(root, i);
          printf("\n");
      }
  }*/

    /* Print Nodes at a given level */
  /*void printGivenLevel(struct Node* root, int level)
  {
      if (root == NULL)
          return;
      if (level == 1)
          printf("%f ", root->splitTime);
      else if (level > 1)
      {
          printGivenLevel(root->left, level-1);
          printGivenLevel(root->right, level-1);
      }
  }*/
  /* Compute the "height" of a tree -- the number of
    nodes along the longest path from the root node
    down to the farthest leaf node.*/
  /*int height(struct Node* node)
  {
      if (node==NULL)
          return 0;
      else
      {
          /* compute the height of each subtree */
  //         int lheight = height(node->left);
  //         int rheight = height(node->right);

  //          use the larger one 
  //         if (lheight > rheight)
  //             return(lheight+1);
  //         else return(rheight+1);
  //     }
  // }*/
};

// --------------------------- Node ends here --------------------------------
// ------------------------------ Newick -----------------------------------
/** output tree (or subtree) in Newick format */
string newick(Node* n)
{
  // if leaves
  if (n->getLeft() == 0 && n->getRight() == 0)
    return "L" + to_string(n->id);

  // if internal nodes
  else {

    string lLen;
    string rLen;

    lLen = to_string(n->getLeft()->getTime() - n->getTime());
    string lStr = newick(n->getLeft());

    rLen = to_string(n->getRight()->getTime() - n->getTime());
    string rStr = newick(n->getRight());

    string out = "(" + lStr + ":" + lLen + "," + rStr + ":" + rLen + ")";

    if (n->parent == 0)
        out += ";";
    return out;
  }
}

//--------------------------- Tree implementation ------------------------------

void yule(double &rate, int & endn, double & endt) {

  // cerr <<"given rate"<<endl;
  double t = 0;
  int numNode = 0;
  std::vector<Node*> nodes;
  std::vector<Node*> leaves;

  // add root
  Node* root = new Node(numNode++);
  root->mytime = 0;
  nodes.push_back(root);
  leaves.push_back(root);

  std::random_device rd; 
  std::default_random_engine generator(rd());
  // propagate till # leaves are met
  while (leaves.size() < endn) {

    // random number generator for next splitting time

    std::exponential_distribution<double> distribution1(rate * (leaves.size()));
    double deltaT = distribution1(generator);
    t += deltaT;

    if (t >= endt) {
      break;
    }

    // generate random number of node to pop from the set
    std::uniform_int_distribution<int> distribution2(0,(leaves.size()-1));
    int leafToPop = distribution2(generator);

    // delete a leaf node and propagate with children
    std::vector<Node*>::iterator leafIt;
    leafIt = leaves.begin();
    for (int i = 0; i<leafToPop; i++) {
      leafIt++;
    }
    Node* node = (*leafIt);
    node->mytime = t;
    leaves.erase(leafIt);

    //create 2 child node for the node poped and add new nodes to nodes
    Node* left = new Node(numNode++);
    Node* right = new Node(numNode++);

    // the time line up until the parent 
    left->mytime = t;
    right->mytime = t;

    // set connection between parent and children
    node->left = left;
    node->right = right;
    left->parent = node;
    right->parent = node;

    leaves.push_back(left);
    leaves.push_back(right);
    nodes.push_back(left);
    nodes.push_back(right);

    // cerr << "the total time updated to: "<< t <<endl;
    // root->printLevelOrder(root);
  }

  // +++++++++++++++++++++++ update time once more +++++++++++++++++++++++++++
  if (leaves.size() == endn) {
    std::random_device rd;
    std::default_random_engine generator(rd());

    std::exponential_distribution<double> distribution1(rate * (nodes.size()+1));
    double deltaT = distribution1(generator);
    t += deltaT;

  }

  //+++++++++++++++++++++ uodate all edges to current ++++++++++++++++++++

  // update leaf times
  std::vector<Node*>::iterator iter2;
  iter2 = leaves.begin();
  while (iter2 != leaves.end()) {
    (*iter2)->mytime = t;
    iter2++;
  }
  // ++++++++++++++++++++++++ print in Newick format +++++++++++++++++++++++++++
  cout << newick(root)<<endl;
}

int main(int argc, char * argv[])
{

  // Parse the input first
  ParseInput input(argc, argv);
  double rate;
  double endt;
  int endn;
  int max_int = 99999;
  double max_double = 99999;

  // the option has rate and time only
  if(input.optExist("-r") && input.optExist("-t") && !input.optExist("-n"))
  {
    rate = std::stoi(input.getOption("-r"));
    endt = std::stoi(input.getOption("-t"));

    yule(rate, max_int, endt);
  }

  // the option has rate and num of leaves only
  else if(input.optExist("-r") && input.optExist("-n") && !input.optExist("-t"))
  {
    rate = std::stoi(input.getOption("-r"));
    endn = std::stoi(input.getOption("-n"));
    cerr<<"detected rate: "<< rate << endl;
    cerr<<"detected nume of leaves: "<<endn <<endl;
    yule(rate, endn, max_double);
  }
  else
    cerr << "option not valid"<<endl;

}