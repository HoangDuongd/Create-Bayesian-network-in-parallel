#ifndef METHOD_H
#define METHOD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>
#include <algorithm>
#include "Node.h"
#include "Edge.h"
class Node;

std::unordered_map<std::string, double> probabilityDistribution(const std::vector<Node>& nodes);
std::pair<bool, std::vector<Node>> CI_test(const std::vector<Node>& Nodes, Edge edge, int degree, double threshold);
#endif // METHOD_H
