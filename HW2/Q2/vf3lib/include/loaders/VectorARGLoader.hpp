/**
 * @file FastStreamARGLoader.hpp
 * @author P. Foggia (pfoggia\@unisa.it)
 * @date May 2014
 * @brief A fast implementation of the ARGLoader interface for
 *        reading from a text istream
 */

#ifndef VECTORARGLOADER_HPP
#define VECTORARGLOADER_HPP

#include <iostream>
#include <vector>
#include <map>

#include <Error.hpp>

#include "ARGraph.hpp"

namespace vflib {

template <typename Node, typename Edge>
class VectorARGLoader: public ARGLoader<Node, Edge> {
    public:
        /**
         * @brief Reads the graph data from a text istream
         * @param in The input stream
         * @param undirected If true, the graph is undirected
         */
        VectorARGLoader(std::vector<int> &v_nodes, std::vector<std::tuple<int,int,int>> &v_edges, bool undirected=false);
        virtual uint32_t NodeCount() const;
        virtual Node GetNodeAttr(nodeID_t node);
        virtual uint32_t OutEdgeCount(nodeID_t node) const;
        virtual nodeID_t GetOutEdge(nodeID_t node, uint32_t i, Edge *pattr);

    private:
        uint32_t node_count;
        std::vector<Node> nodes;
        std::vector< std::map<nodeID_t, Edge> > edges;
        typename std::map<nodeID_t, Edge>::iterator edge_iterator;
        nodeID_t last_edge_node;
        uint32_t last_edge_index;
};



template <typename Node, typename Edge>
VectorARGLoader<Node,Edge>
::VectorARGLoader(std::vector<int> &v_nodes, std::vector<std::tuple<int,int,int>> &v_edges, bool undirected) {
    last_edge_node = NULL_NODE;

    node_count = v_nodes.size();
    nodes.reserve(node_count);
    nodes.insert(nodes.end(),v_nodes.begin(),v_nodes.end());

    edges.resize(node_count);
    uint32_t edge_count;
    nodeID_t i, j, n1, n2;

    edge_count = v_edges.size();
    for(std::tuple<nodeID_t,nodeID_t,int> x : v_edges) {
        n1 = std::get<0>(x);
        n2 = std::get<1>(x);
        edges[n1][n2] = std::get<2>(x);
        if (undirected)
            edges[n2][n1] = edges[n1][n2];
    }
}

template <typename Node, typename Edge>
uint32_t
VectorARGLoader<Node,Edge>
::NodeCount() const {
    return node_count;
}

template <typename Node, typename Edge>
Node
VectorARGLoader<Node,Edge>
::GetNodeAttr(nodeID_t node) {
    return nodes[node];
}

template <typename Node, typename Edge>
uint32_t
VectorARGLoader<Node,Edge>
::OutEdgeCount(nodeID_t node) const {
    return edges[node].size();
}

template <typename Node, typename Edge>
nodeID_t
VectorARGLoader<Node,Edge>
::GetOutEdge(nodeID_t node, uint32_t i, Edge *pattr) {
    assert (i<OutEdgeCount(node));

    if (node != last_edge_node) {
        last_edge_node = node;
        edge_iterator = edges[node].begin();
        last_edge_index = 0;
    }
    while (last_edge_index < i) {
        edge_iterator ++;
        last_edge_index ++;
    }

    while (last_edge_index > i) {
        edge_iterator --;
        last_edge_index --;
    }

    *pattr = edge_iterator->second;
    return edge_iterator->first;
}



} // End namespace vflib

#endif
