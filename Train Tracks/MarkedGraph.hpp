//
//  MarkedGraph.hpp
//  Train Tracks
//
//  Created by Alex Lowen on 12/30/18.
//  Copyright © 2018 Alex Lowen. All rights reserved.
//

#ifndef MarkedGraph_hpp
#define MarkedGraph_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <map>

class MarkedGraph{
public:
    std::vector<std::set<int>>* Graph;
    size_t edge_count;
    
    MarkedGraph(std::vector<std::string>*, size_t);
    MarkedGraph(MarkedGraph*);
    void DisplayMarkedGraph();
};

MarkedGraph partial_fold(MarkedGraph, std::pair<int,int>);
MarkedGraph left_improper_full_fold(MarkedGraph, std::pair<int,int>);
MarkedGraph right_improper_full_fold(MarkedGraph, std::pair<int,int>);
MarkedGraph proper_full_fold(MarkedGraph, std::pair<int,int>);

#endif /* MarkedGraph_hpp */
