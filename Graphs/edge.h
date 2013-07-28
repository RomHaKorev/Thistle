#ifndef EDGE_H
#define EDGE_H

class Node;

class Edge {
public:
  Edge( Node* n1, Node* n2 );
  Node* node1;
  Node* node2;
};

#endif // EDGE_H
