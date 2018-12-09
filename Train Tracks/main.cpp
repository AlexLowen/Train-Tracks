//
//  main.cpp
//  Train Tracks
//
//  Created by Alex Lowen on 10/15/18.
//  Copyright (c) 2018 Alex Lowen. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include "TrainTracks.h"

int main(int argc, const char * argv[]) {
    //std::vector<std::string> Map={"a-c","-dcc-ac","-b-c", "c"};
    //std::vector<std::string> Map={"ae", "-dcec-ace", "-b", "ce", "ec"};
    //std::vector<std::string> Map={"b","c", "d-a", "-d-c"};
    //std::vector<std::string> Map={"b","c","ca"};
//    std::vector<std::string> Map={"afceda","bcedaf", "cedafc", "ced", "edab", "fce"};
    //std::vector<std::string> Map={"ba", "bba"};
    //std::vector<std::string> Map={"b","c","d","-a-d-c-b"};
    //std::vector<std::string> Map={"abcbcb", "bcb", "cbabc"};
    //std::vector<std::string> Map={"abcbcb","bcb","cbabc"};
    //std::vector<std::string> Map={"-cbdbbd","-a-d","bd","-b"};
    //TopRep TpRp(Map);
    //TpRp.DisplayGates();
    //TpRp.DisplayIllegal();
    TopRep TpRp(10,30);
    //TpRp.Display();
    std::cout<<"Before we straighten: "<<std::endl;
    TpRp.DisplayMap();
    std::cout<<std::endl<<std::endl;
    TopRep St=TpRp.straighten();
    std::cout<<"Straightened to: "<<std::endl;
    St.DisplayMap();
    std::cout<<std::endl<<std::endl;
    std::cout<<"The Gates are: "<<std::endl;
    St.DisplayGates();
    std::cout<<std::endl<<std::endl;
    std::cout<<"The illegal turns crossed are: "<<std::endl;
    St.DisplayIllegal();
    St.Find_Internal_Illegal_Turns_Crossed();
    St.DisplayInteralIllegal();
    
    //TpRp.DisplayGates();
    //TpRp.DisplayMoT();
    //TpRp.DisplayIllegal();
    //std::vector<std::string> Map={"b","c","d","-a-d-c-b"};
/*    Nielsen p={"dabced", "cedafc"};
    TopRep TpRp(Map);
    //TpRp.Display();
    TpRp.DisplayMap();
    std::cout<<std::endl;
    TpRp.DisplayMoT();
    std::cout<<std::endl;
    TpRp.DisplayGates();
    std::cout<<std::endl;
    p.DisplayNielsen();
    p.top.resize(50);
    p.bot.resize(50);
    std::cout<<std::endl;
    std::cout<<std::endl;
    p.Fold_Right();
    p.Fold_Right();
    
    p.Fold_Left();
    std::cout<<"Fold Left"<<std::endl;
    p.Fold_Right();
    std::cout<<"Fold Right"<<std::endl;
    p.Fold_Left();
    std::cout<<"Fold Left"<<std::endl;
    p.Fold_Right();
    std::cout<<"Fold Right"<<std::endl;
    p.Fold_Right();
    
    std::cout<<"Fold Right"<<std::endl;
    p.Fold_Left();
    std::cout<<"Fold Left"<<std::endl;
    p.Fold_Right();
    std::cout<<"Fold Right"<<std::endl;
    p.Fold_Left();
    std::cout<<"Fold Left"<<std::endl;
    p.Fold_Right();
    std::cout<<"Fold Right"<<std::endl;
    size_t start=0;
    size_t end=1;
    size_t term=8*end;
    while(start++<term) p.Fold_Left();
    std::cout<<"Fold Left "<<term<<" times"<<std::endl;
    p.Fold_Left();
    std::cout<<"Fold Left"<<std::endl;
    p.Fold_Left();
    std::cout<<"Fold Left"<<std::endl;
    p.Fold_Right();
    std::cout<<"Fold Right"<<std::endl; 
 
 
    std::cout<<std::endl;
    
    p.DisplayNielsen();
    std::cout<<std::endl;
 */
    return 0;
}
