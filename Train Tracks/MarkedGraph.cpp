//
//  MarkedGraph.cpp
//  Train Tracks
//
//  Created by Alex Lowen on 12/30/18.
//  Copyright © 2018 Alex Lowen. All rights reserved.
//

#include "MarkedGraph.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdlib>

int CharToInt(const char ch){
    char letter='a';
    return static_cast<int>(ch-letter+1);
}

std::string IntToChar(const int n) {
    std::string ret;
    char letter='a';
    if(n>0) {
        ret+=(letter+n-1);
    }
    else {
        ret+='-';
        ret+=(letter-n-1);
    }
    return ret;
}

MarkedGraph::MarkedGraph(std::vector<std::string>* graph, size_t edg_cnt) {
    Graph=new std::vector<std::set<int>>;
    Graph->resize(graph->size());
    for(size_t vertex=0; vertex < graph->size(); vertex++) {
        for(size_t i=0; i<graph->at(vertex).size(); i++) {
            if(graph->at(vertex).at(i)=='-') {
                Graph->at(vertex).insert(-CharToInt((graph->at(vertex).at(++i))));
            }
            else Graph->at(vertex).insert(CharToInt(graph->at(vertex).at(i)));
        }
    }
    edge_count=edg_cnt;
}

MarkedGraph::MarkedGraph(MarkedGraph* graph){
    this->Graph=graph->Graph;
    this->edge_count=graph->edge_count;
}

MarkedGraph partial_fold(MarkedGraph graph, std::pair<int,int> turn) {
    for(size_t init_vertex=0; init_vertex<graph.Graph->size(); init_vertex++) {
        
        if(graph.Graph->at(init_vertex).count(turn.first) && !graph.Graph->at(init_vertex).count(turn.second)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.second) && !graph.Graph->at(init_vertex).count(turn.first)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.first) && graph.Graph->at(init_vertex).count(turn.second)) {
            MarkedGraph quotient_graph=graph;
            quotient_graph.edge_count++;
            
            int new_edge=static_cast<int>(quotient_graph.edge_count);
            int new_vertex_temp[]={turn.first, turn.second, -new_edge};
            std::set<int> new_vertex (new_vertex_temp,new_vertex_temp+3);
            
            quotient_graph.Graph->push_back(std::set<int>(new_vertex));
            
            quotient_graph.Graph->at(init_vertex).erase(turn.first);
            quotient_graph.Graph->at(init_vertex).erase(turn.second);
            quotient_graph.Graph->at(init_vertex).insert(new_edge);
            
            return quotient_graph;
        }
    }
    return graph;
}

MarkedGraph right_improper_full_fold(MarkedGraph graph, std::pair<int,int> turn) {
    for(size_t init_vertex=0; init_vertex<graph.Graph->size(); init_vertex++) {
        
        //folding over a loop is a homeomorphism up to homotopy.
        if(graph.Graph->at(init_vertex).count(turn.second) && graph.Graph->at(init_vertex).count(-turn.second)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.first) && !graph.Graph->at(init_vertex).count(turn.second)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.second) && !graph.Graph->at(init_vertex).count(turn.first)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.first) && graph.Graph->at(init_vertex).count(turn.second)) {
            MarkedGraph quotient_graph=graph;
            for(size_t term_vertex=0; term_vertex<graph.Graph->size(); term_vertex++) {
                if(graph.Graph->at(term_vertex).count(-turn.second)) {
                    quotient_graph.Graph->at(term_vertex).insert(turn.first);
                    quotient_graph.Graph->at(init_vertex).erase(turn.first);
                    return quotient_graph;
                }
            }
        }
    }
    return graph;
}

MarkedGraph left_improper_full_fold(MarkedGraph graph, std::pair<int,int> turn) {
    for(size_t init_vertex=0; init_vertex<graph.Graph->size(); init_vertex++) {
        
        //folding over a loop is a homeomorphism up to homotopy.
        if(graph.Graph->at(init_vertex).count(turn.first) && graph.Graph->at(init_vertex).count(-turn.first)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.second) && !graph.Graph->at(init_vertex).count(turn.first)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.first) && !graph.Graph->at(init_vertex).count(turn.second)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.second) && graph.Graph->at(init_vertex).count(turn.first)) {
            MarkedGraph quotient_graph=graph;
            for(size_t term_vertex=0; term_vertex<graph.Graph->size(); term_vertex++) {
                if(graph.Graph->at(term_vertex).count(-turn.first)) {
                    quotient_graph.Graph->at(term_vertex).insert(turn.second);
                    quotient_graph.Graph->at(init_vertex).erase(turn.second);
                    return quotient_graph;
                }
            }
        }
    }
    return graph;
}

MarkedGraph proper_full_fold(MarkedGraph graph, std::pair<int,int> turn) {
    for(size_t init_vertex=0; init_vertex<graph.Graph->size(); init_vertex++) {
        if(graph.Graph->at(init_vertex).count(turn.second) && !graph.Graph->at(init_vertex).count(turn.first)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.first) && !graph.Graph->at(init_vertex).count(turn.second)) {
            return graph;
        }
        if(graph.Graph->at(init_vertex).count(turn.second) && graph.Graph->at(init_vertex).count(turn.first)) {
            if(graph.Graph->at(init_vertex).count(-turn.first) && graph.Graph->at(init_vertex).count(-turn.second)) {
                for(size_t del_vertex=0; del_vertex<graph.Graph->size(); del_vertex++) {
                    if(graph.Graph->at(del_vertex).count(-turn.second)) {
                        MarkedGraph quotient_graph=graph;
                        quotient_graph.edge_count--;
                        for(std::set<int>::iterator copy=graph.Graph->at(del_vertex).begin(); copy!=graph.Graph->at(del_vertex).end(); copy++) {
                            quotient_graph.Graph->at(init_vertex).insert(*copy);
                        }
                        
                        //slick way to delete
                        quotient_graph.Graph->at(init_vertex).erase(turn.second);
                        quotient_graph.Graph->at(init_vertex).erase(-turn.second);
                        quotient_graph.Graph->at(del_vertex)=graph.Graph->at(graph.Graph->size()-1);
                        quotient_graph.Graph->pop_back();
                        return quotient_graph;
                    }
                }
            }
            if(graph.Graph->at(init_vertex).count(-turn.first)) {
                for(size_t del_vertex=0; del_vertex<graph.Graph->size(); del_vertex++) {
                    if(graph.Graph->at(del_vertex).count(-turn.second)) {
                        MarkedGraph quotient_graph=graph;
                        quotient_graph.edge_count--;
                        for(std::set<int>::iterator copy=graph.Graph->at(del_vertex).begin(); copy!=graph.Graph->at(del_vertex).end(); copy++) {
                            quotient_graph.Graph->at(init_vertex).insert(*copy);
                        }
                        
                        //slick way to delete
                        quotient_graph.Graph->at(init_vertex).erase(turn.second);
                        quotient_graph.Graph->at(init_vertex).erase(-turn.second);
                        quotient_graph.Graph->at(del_vertex)=graph.Graph->at(graph.Graph->size()-1);
                        quotient_graph.Graph->pop_back();
                        return quotient_graph;
                    }
                }
            }
            if(graph.Graph->at(init_vertex).count(-turn.second)) {
                for(size_t del_vertex=0; del_vertex<graph.Graph->size(); del_vertex++) {
                    if(graph.Graph->at(del_vertex).count(-turn.first)) {
                        MarkedGraph quotient_graph=graph;
                        quotient_graph.edge_count--;
                        for(std::set<int>::iterator copy=graph.Graph->at(del_vertex).begin(); copy!=graph.Graph->at(del_vertex).end(); copy++) {
                            quotient_graph.Graph->at(init_vertex).insert(*copy);
                        }
                        
                        //slick way to delete
                        quotient_graph.Graph->at(init_vertex).erase(turn.first);
                        quotient_graph.Graph->at(init_vertex).erase(-turn.first);
                        quotient_graph.Graph->at(del_vertex)=graph.Graph->at(graph.Graph->size()-1);
                        quotient_graph.Graph->pop_back();
                        return quotient_graph;
                    }
                }
            }
            if(!graph.Graph->at(init_vertex).count(-turn.first) && !graph.Graph->at(init_vertex).count(-turn.second)) {
                for(size_t del_vertex=0; del_vertex<graph.Graph->size(); del_vertex++) {
                    if(graph.Graph->at(del_vertex).count(-turn.second)) {
                        MarkedGraph quotient_graph=graph;
                        quotient_graph.edge_count--;
                        size_t term_vertex=0;
                        while(!graph.Graph->at(term_vertex).count(-turn.first)) term_vertex++;
                        for(std::set<int>::iterator copy=graph.Graph->at(del_vertex).begin(); copy!=graph.Graph->at(del_vertex).end(); copy++) {
                            quotient_graph.Graph->at(term_vertex).insert(*copy);
                        }
                        
                        //slick way to delete
                        quotient_graph.Graph->at(init_vertex).erase(turn.second);
                        quotient_graph.Graph->at(term_vertex).erase(-turn.second);
                        quotient_graph.Graph->at(del_vertex)=graph.Graph->at(graph.Graph->size()-1);
                        quotient_graph.Graph->pop_back();
                        return quotient_graph;
                    }
                }
            }
        }
    }
    return graph;
}

void MarkedGraph::DisplayMarkedGraph(){
    std::cout<<std::endl;
    std::cout<<std::endl;
    for(size_t vertex=0; vertex<Graph->size(); vertex++) {
        std::cout<<"vertex v"<<vertex<<": ";
        for(std::set<int>::iterator edge=Graph->at(vertex).begin(); edge!=Graph->at(vertex).end(); edge++) {
            std::cout<<IntToChar(*edge)<<' ';
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}


/*
 for(size_t term_vertex=0; term_vertex<graph.Graph->size(); term_vertex++) {
 if(graph.Graph->at(term_vertex).count(-turn.first) && !graph.Graph->at(term_vertex).count(-turn.second)) {
 
 }
 if(graph.Graph->at(term_vertex).count(-turn.second) && !graph.Graph->at(term_vertex).count(-turn.first)) {
 
 }
 if(graph.Graph->at(term_vertex).count(-turn.first) && graph.Graph->at(term_vertex).count(-turn.second)) {
 
 }
 }
 */
