
class Node {

public:
  Node () : left(0), right(0), splitTime(0){  }

  Node* left;
  Node* right;
  Node* parent
  int splitTime;
  Node* getLeft() {
    return left;
  }
  Node* getRight() {
    return right;
  }
  int getSplitTime() {
    return splitTime;
  }
  bool operator ==(const Node & obj) const
  {
   if (splitTime == obj.splitTime)
     return true;
   else
     return false;
  }

};


namespace std
{
  template<>
    struct hash<Node>
    {
      size_t
      operator()(const Node & obj) const
      {
        return hash<int>()(obj.getSplitTime());
      }
    };
}

