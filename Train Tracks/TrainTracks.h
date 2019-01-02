//
//  TrainTracks.h
//  Train Tracks
//
//  Created by Alex Lowen on 10/15/18.
//  Copyright (c) 2018 Alex Lowen. All rights reserved.
//

#ifndef __Train_Tracks__TrainTracks__
#define __Train_Tracks__TrainTracks__

#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "MarkedGraph.hpp"

struct Nielsen {
    std::string top;
    std::string bot;
    void Fold_Left();
    void Fold_Right();
    void DisplayNielsen();
};

struct node {
    size_t component;
    //size_t circ_len;
};

struct DistToMe {
    size_t dist;
    size_t me;
};


class TopRep{
public:
    std::vector<std::vector<int>> mat;
    TopRep(const std::vector<std::string>&);
    TopRep(size_t, size_t);
    void Display();
    void DisplayMap();
    
    std::vector<int> Map_of_Turns;
    void DisplayMoT();

    std::vector<std::vector<std::set<int>>>* Gates;
    size_t DistToBase(size_t,int);
    void CreateGates();
    void DisplayGates();
    
    std::vector<int>* Components_Marked_By_Base;
    std::map<size_t, int>* Component;
    void CreateComponent(size_t);
    std::map<size_t, size_t>* circ_len;
    void DisplayCircLen();
    
    void fold(std::string, std::string); // needs work
    
    std::map<std::pair<int,int>, std::set<std::pair<int,int>>*>* illegal;
    void Illegal_Turns_Crossed();
    void DisplayIllegal();
    
    int IndexToInt(size_t);
    size_t IntToIndex(int);
    
    TopRep straighten();
    
    TopRep(const std::vector<std::vector<int>>);
    //TopRep operator =(const TopRep);
    
    std::set<std::pair<int,int>>* Internal_Illegal_Turns_Crossed;
    void Find_Internal_Illegal_Turns_Crossed();
    
    void DisplayInteralIllegal();
    
    MarkedGraph fold_turn(MarkedGraph, std::pair<int,int>);
};

// want to do TopRep(1,

#endif /* defined(__Train_Tracks__TrainTracks__) */
