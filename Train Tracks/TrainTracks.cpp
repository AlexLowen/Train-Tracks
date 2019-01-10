    //
//  TrainTracks.cpp
//  Train Tracks
//
//  Created by Alex Lowen on 10/15/18.
//  Copyright (c) 2018 Alex Lowen. All rights reserved.
//

#include "TrainTracks.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>

std::vector<int>* reverse(std::vector<int>* vec) {
    std::vector<int>* ret=new std::vector<int>;
    for(size_t i=0; i<vec->size(); i++) {
        ret->push_back(-vec->at(vec->size()-i-1));
    }
    return ret;
}


int TopRep::IndexToInt(size_t index) {
    int Int;
    if(index>=Map_of_Turns.size()/2) Int=static_cast<int>(Map_of_Turns.size()/2-index-1);
    else Int=static_cast<int>(index+1);
    return Int;
}

size_t TopRep::IntToIndex(int Int) {
    size_t index;
    if(Int<0) index=Map_of_Turns.size()/2-Int-1;
    else index=Int-1;
    return index;
}

void TopRep::CreateComponent(size_t starting_vertex) {
    //Creates a map of turns component given a starting_vertex. If the starting_vertex is in an existing component, then it will append the existing component instead.
    
    size_t current_vertex;
    current_vertex=starting_vertex;
    
    int Image_of_current_vertex;
    
    //while we still haven't found a loop in the component
    //It is possible that the loop stops after discovering a loop we accounted for earlier. Thus the chain is part of an earlier component.
    while (!Component->count(current_vertex)) {
        Image_of_current_vertex=Map_of_Turns[current_vertex];
        
        //This insert is temporary. Ultimately, we need to insert current_vertex with the base point of the component. But we can't know apriori that this insert isn't correct.
        Component->insert(std::pair<size_t,int>(current_vertex,Image_of_current_vertex));
        
        //++
        current_vertex=IntToIndex(Image_of_current_vertex);
    }
    
    //Now that the loop is found, we must identify everything we've seen so far as part of the same component.
    //Due to the previous loop, current_vertex is at a base vertex of the circuit. Either the closest rel starting_vertex, or an
    size_t base_size_t=current_vertex;
    int base_int=IndexToInt(current_vertex);

    current_vertex=starting_vertex;
    
    //Since we have a base vertex, We will also be able to compute the length of the circuit.
    size_t length=0;
    
    //This will be a signal that will increase everytime our current_vertex becomes the base_vertex.
    //At most this will be two by going around the circuit once.
    //The purpose of this is to have every vertex encountered identified to base_size_t in the Component array.
    size_t flag=0;
    
    //the count method checks if circ_len has a key equal to base_size_t.
    //If TRUE then the component already exists.
    if(circ_len->count(base_size_t)) {
        length=circ_len->at(base_size_t);
        
        //temp will store the old current_vertex while current_vertex is incremented to the new current_vertex.
        //that is, we must change what current_vertex maps to without losing what it currently maps to.
        size_t temp;
        
        //we don't need to go around the circuit since we did in a previous call to this function.
        //that is, everything in the circuit already is identified to some base_vertex.
        while (flag<1) {
            if (Component->at(current_vertex)==base_int) flag++;
            circ_len->insert(std::pair<size_t,size_t>(current_vertex,length));
            temp=current_vertex;
            Image_of_current_vertex=Map_of_Turns[current_vertex];
            
            
            current_vertex=IntToIndex(Image_of_current_vertex);
            
            Component->at(temp)=Component->at(base_size_t);
        }
        return;
    }
    Components_Marked_By_Base->push_back(base_int);
    flag=0;
    current_vertex=starting_vertex;
    

    while (flag<2) {
        if (current_vertex==base_size_t) flag++;
        if (flag && flag<2) length++;
        Image_of_current_vertex=Map_of_Turns[current_vertex];
        
        current_vertex=IntToIndex(Image_of_current_vertex);
    }
    flag=0;
    current_vertex=starting_vertex;
    size_t temp2;
    while (flag<2) {
        if (Component->at(current_vertex)==base_int) flag++;
        circ_len->insert(std::pair<size_t,size_t>(current_vertex,length));
        temp2=current_vertex;
        Image_of_current_vertex=Map_of_Turns[current_vertex];
        current_vertex=IntToIndex(Image_of_current_vertex);

        Component->at(temp2)=base_int;
    }
}



void TopRep::CreateGates() {
    size_t k=Map_of_Turns.size();
    
    Component=new std::map<size_t,int>;
    circ_len=new std::map<size_t,size_t>;
    Components_Marked_By_Base=new std::vector<int>;
    
    for(size_t i=0; i<k; i++) {
        if(!Component->count(i)) CreateComponent(i);
        else if(!circ_len->count(i)) {
            int base=Component->at(i);
            size_t bas=IntToIndex(base);
            circ_len->insert(std::pair<size_t,size_t>(i, circ_len->at(bas)));
        }
    }
    Gates=new std::vector<std::vector<std::set<int>>>;
    Gates->resize(Components_Marked_By_Base->size());

    size_t tempsz;
    int tempint;
    for(size_t i=0; i<Gates->size(); i++) {
        Gates->at(i)=*new std::vector<std::set<int>>;
        tempint=Components_Marked_By_Base->at(i);
        tempsz=IntToIndex(tempint);
        size_t t= circ_len->at(tempsz);
        Gates->at(i).resize(t);
        for(size_t j=0; j<Gates->at(i).size(); j++) Gates->at(i).at(j)=*new::std::set<int>;
    }
    tempint=0;
    size_t iter;
    for(size_t i=0; i<k; i++) {
        tempint= i>=mat.size() ? mat.size()-i-1:i+1;
        size_t d=DistToBase(i, Component->at(i));
        d=d%circ_len->at(i);
        iter=0;
        while(Components_Marked_By_Base->at(iter)!=Component->at(i)) iter++;
        Gates->at(iter).at(d).insert(tempint);
    }
}

void Nielsen::Fold_Left() {
    char o=bot[0];
    char n=top[0];
    top=top.substr(1,top.size());
    std::string::iterator one=top.begin();
    std::string::iterator two=++bot.begin();
    while(one!=top.end() || two !=bot.end()) {
        if(one!=top.end() && *one==o) {top.insert(one++, n); one++;}
        else if(two!= top.end() && *two==o) {bot.insert(two++, n); two++;}
        else if(one==top.end()) two++;
        else if(two==bot.end()) one++;
        else {one++; two++;}
    }
    return;
}

void Nielsen::Fold_Right() {
    char o=top[0];
    char n=bot[0];
    bot=bot.substr(1,bot.size());
    std::string::iterator one=++top.begin();
    std::string::iterator two=bot.begin();
    while(one!=top.end() || two !=bot.end()) {
        if(one!=top.end() && *one==o) {top.insert(one++, n); one++;}
        else if(two!= top.end() && *two==o) {bot.insert(two++, n); two++;}
        else if(one==top.end()) two++;
        else if(two==bot.end()) one++;
        else {one++; two++;}
    }
    return;
}

void Nielsen::DisplayNielsen() {
    size_t i=0;
    while(i<top.size())
        std::cout<<top[i++];
    std::cout<<std::endl;
    i=0;
    while(i<bot.size())
        std::cout<<bot[i++];
    
}


size_t TopRep::DistToBase(size_t t, int s) {
    size_t dist=0;
    int tempint = t>=mat.size() ? mat.size()-t-1:t+1;
    size_t tempsiz=t;
    while(tempint!=s){
        dist++;
        tempint=Map_of_Turns[tempsiz];
        if(tempint<0) tempsiz=mat.size()-tempint-1;
        else tempsiz=tempint-1;
        if(dist>Map_of_Turns.size()) break;
    }
        return dist;
}


/*DistToMe TopRep::find_circuit_len(size_t t){
    std::vector<size_t> memory;
    size_t i=Map_of_Turns[t]-1;
    memory.push_back(i);
    bool flag=true;
    while(flag) {
        if(Map_of_Turns[i]<0) i=mat.size()-Map_of_Turns[i]-1;
        else i=Map_of_Turns[i]-1;
        flag=!WasCrossed(memory,i);
        memory.push_back(i);
        
    }
    size_t base=memory[memory.size()-1];
    size_t len=0;
    while(memory[len]!=base) len++;
    DistToMe DTM=*new DistToMe;
    DTM.dist=memory.size()-len-1;
    DTM.me=base;
    return DTM;
}
 */




//We construct an automorphism of F_n using a random composition of elementary neilsen transformations followed by a relabling.
//n is the rank of the free group and d is the number of random elementary neilsen transformations we compose.
TopRep::TopRep(size_t n, size_t d) {
    //This is reseading the randomness each call to this function. I ultimately should call this once but it might be confusing to the user to call it themselves.
    srand(time(NULL));
    
    //ultimately, we represent the random transvection of the form a->ab as dice_roll1->dice_roll1 * dice_rull2
    //temp_dice_roll2 represents the raw roll but the 2nd roll needs to represent a transvection that isn't of the form a^2 or a -a.
    size_t dice_roll1, dice_roll2, temp_dice_roll2;
    
    this->mat.resize(n);
    this->Map_of_Turns.resize(2 * n);
    
    //This is the identity automorphism that we start with.
    for(size_t i=0; i<mat.size(); i++) mat[i].push_back(static_cast<int>(i+1));
    
    //We iterate our compositions here.
    for (size_t i=0; i<d; i++) {
        
        // random number between 0 and n-1. Note that where inverses elements map is induced by where elements map.
        dice_roll1=static_cast<size_t>(rand()%n);
        
        // random number between 0 and 2n-2. Note that the transvection a->ab is different than a->a -b. So its a larger range than the first die roll.
        temp_dice_roll2=static_cast<size_t>(rand()%(2*n-2));
        
        // we construct the needed second die roll from the raw roll
        if(temp_dice_roll2<dice_roll1) dice_roll2=temp_dice_roll2;
        else if(temp_dice_roll2<dice_roll1+n-1) dice_roll2=temp_dice_roll2+1;
        else dice_roll2=temp_dice_roll2+2;
        
        std::cout<<"fold: "<<i+1<<std::endl;
        char letter = 'a';
        std::cout<<static_cast<char>(dice_roll1+letter)<<": "<<static_cast<char>(dice_roll1+letter);
        if(IndexToInt(dice_roll2)<0)std::cout<<'-'<<static_cast<char>(letter-IndexToInt(dice_roll2)-1);
        else std::cout<<static_cast<char>(dice_roll2+letter);
        std::cout<<std::endl;
        
        //we replace every dice_roll1 in the image with dice_roll1 * dice_roll2 here
        //we replace every -dice_roll1 in the image with -dice_roll2 * -dice_roll1
        for(size_t j=0; j<mat.size(); j++) {
            std::vector<int>::iterator iter=mat[j].begin();
            while(iter!=mat[j].end()) {
                if (*iter>0 && IntToIndex(*iter)==dice_roll1) {
                    iter=mat[j].insert(++iter,IndexToInt(dice_roll2));
                    iter++;
                    }
                else if(IntToIndex(*iter)-n==dice_roll1) {
                    iter=mat[j].insert(iter++,-IndexToInt(dice_roll2));
                    iter++; iter++;
                }
                else iter++;
            }
        }
        std::cout<<std::endl;
        DisplayMap();
        std::cout<<std::endl;
        std::cout<<std::endl;
    }
    


    //labels contains the letters in the codomain that have not been used yet via the partially constructed relabelling.
    std::vector<int> labels=*new std::vector<int>;
    for( size_t i=0; i<n; i++) labels.push_back(IndexToInt(i));
    
    
    std::vector<int>::iterator iter;
    char letter='a';
    std::vector<std::vector<int>> temp=mat;
    std::cout<<"Homeomorphism: "<<std::endl;
    for(size_t i=0; i<n; i++) {
        char letter='a';
        std::cout<<static_cast<char>(letter+i)<<" -> ";
        dice_roll1=static_cast<size_t>(rand()%(n-i));
        dice_roll2=static_cast<size_t>(rand()%2);
        if(dice_roll2==1) std::cout<<'-';
        
        iter=labels.begin();
        
        //after iterating, we find the letter in labels that that we assign i to in the domain.
        for(size_t j=0; j<dice_roll1; j++) iter++;
        std::cout<<static_cast<char>(letter+ *iter-1)<<std::endl;
        
        for(size_t j=0; j<mat.size(); j++) {
            for(size_t k=0; k<mat[j].size(); k++) {
                
                //for every letter in the codomain, if
                if(mat[j][k]>0 && i==IntToIndex(mat[j][k])) dice_roll2==1 ? temp[j][k]=-*iter:temp[j][k]=*iter;
                else if(i==IntToIndex(mat[j][k])-n) dice_roll2==1 ? temp[j][k]=*iter:temp[j][k]=-*iter;
            }
        }
        
        
        
        
        
        
        
        
        
        /////////HAVING TROUBLE HERE!!!! implimentation and the fact that I'm not getting negative relabelings. Also a->ab followed by a->a-b is not ideal.
        /////////Most important fix is to figure out what the hell is going on with the illegal turn function. Its driving me insane!
        
        labels.erase(iter);
    }
    std::cout<<std::endl;
    mat=temp;
    
    for(size_t i=0; i<n; i++) {
        Map_of_Turns[i]=mat[i][0];
        Map_of_Turns[i+n]=-mat[i][mat[i].size()-1];
    }


    CreateGates();
    Illegal_Turns_Crossed();
}


TopRep::TopRep(const std::vector<std::string>& v) {
    this->mat.resize(v.size());
    this->Map_of_Turns.resize(2 * v.size());
    for(size_t i=0; i < v.size(); i++){
        if(v[i][0]=='-') Map_of_Turns[i]=(-(v[i][1]-96));
        else Map_of_Turns[i]=v[i][0]-96;
        int k;
        for (size_t j=0; j<v[i].size(); j++) {
            k=v[i][j];
            if (k==45) this->mat[i].push_back(96-v[i][++j]);
            else this->mat[i].push_back(k-96);
        }
    }
    for(size_t i=0; i<v.size(); i++) {
        if(v[i][v[i].size()-2]=='-') Map_of_Turns[i+v.size()]=v[i][v[i].size()-1]-96;
        else Map_of_Turns[i+v.size()]=-(v[i][v[i].size()-1]-96);
    }
    //circuit_len=find_circuit_len(0).dist;
    CreateGates();
    Illegal_Turns_Crossed();
}

void TopRep::Display() {
    std::cout<<std::endl;
    for( size_t i=0; i < mat.size(); i++) {
        for( size_t j=0; j < mat[i].size(); j++ ){
            std::cout<<mat[i][j]<<' ';
        }
        std::cout<<std::endl;
    }
    return;
}
void TopRep::DisplayMap() {
    std::cout<<std::endl;
    for( size_t i=0; i < mat.size(); i++) {
        char letter = 'a';
        std::cout << static_cast<char>(letter + i) <<" -> ";
        for (size_t j=0; j<mat[i].size(); j++) {
            int k=mat[i][j];
            if(k<0) std::cout<<'-'<<static_cast<char>(letter-k-1);
            else std::cout<<static_cast<char>(letter+k-1);
        }
        std::cout<<std::endl;
    }
    return;
}

void TopRep::DisplayMoT() {
    std::cout<<std::endl;
    for(size_t i=0; i<Map_of_Turns.size(); i++) {
        char letter = 'a';
        if (i<Map_of_Turns.size()/2) {
            std::cout<<static_cast<char>(letter+i)<<" -> ";
        }
        else {
            std::cout<<'-'<<static_cast<char>(letter+(i-Map_of_Turns.size()/2))<<" -> ";
        }
        int j=Map_of_Turns[i];
        if(j>0) std::cout<<static_cast<char>(letter+j-1);
        else std::cout<<'-'<<static_cast<char>(letter-j-1);
        std::cout<<std::endl;
    }
    return;
}

void TopRep::DisplayGates() {
    std::cout<<std::endl;
    for(size_t i=0; i<Gates->size(); i++) {
        std::cout<<"Component "<<i<<std::endl;
        for(size_t j=0; j<Gates->at(i).size(); j++) {
            std::cout<<"{";
            std::set<int>::iterator temp;
            for(std::set<int>::iterator k=Gates->at(i).at(j).begin(); k!=Gates->at(i).at(j).end(); k++) {
                if(*k<0) std::cout<<'-'<<static_cast<char>(96-*k);
                else std::cout<<static_cast<char>(96+*k);
                temp=k;
                temp++;
                if(temp != Gates->at(i).at(j).end()) std::cout<<',';
            }
            std::cout<<"}"<<std::endl;
        }
    }
}

void TopRep::DisplayCircLen() {
    auto iter=circ_len->begin();
    while (iter!=circ_len->end()) {
        std::cout<<iter->first<<" has circuit length "<<iter->second<<std::endl;
        iter++;
    }
}

void TopRep::fold(std::string left, std::string right) {
    
    
    /*
    if(left.size()<right.size()) {
        if(left.find(right)<left.size()) {
            std::vector<std::vector<int>>::iterator iter=mat.begin();
            while(iter!=mat.end()) {
                std::vector<int>::iterator iti=iter->begin();
                if (right.size()==2 && *iti == 45) {
                    if (0-*(iti)==right[1]) {
                        if(left[0]==45) {
                            mat.resize(mat.size()+1);
                            iter->insert(iti, - static_cast<int>(left[1]));
                            iti++; iti++;
                        }
                        else {
                            mat.resize(mat.size()+1);
                            iter->insert(iti, static_cast<int>(left[0]));
                            iti++; iti++;
                        }
                    }
                    else {
                        iti++; iti++;
                    }
                }
                else {
                    
                }
            }
     
            return;
        }
    }
    else if(right.size()<left.size()) {
        if(right.find(left)<right.size()) {
            
            return;
        }
    }
     */
    return; //partial fold implimentation here.
}

void TopRep::Illegal_Turns_Crossed() {
    // illegal is a map that takes an order pair (illegal turn) to a vector of ordered pairs (every pair of edges that will cross said illegal turn after some iterations.)
    illegal=new std::map<std::pair<int,int>, std::set<std::pair<int,int>>*>;
    std::vector<std::pair<int,int>>::iterator iter;
    
    //for every gate component
    for (size_t i=0; i<Gates->size(); i++) {
        
        //for every gate
        for(size_t j=0; j<Gates->at(i).size(); j++) {
            
            //for each choice of our first element in said gate - ill1
            for(std::set<int>::iterator first=Gates->at(i).at(j).begin(); first!=Gates->at(i).at(j).end(); first++) {
                int ill1 = *first;
                
                //for each choice of our second element in said gate - ill2
                for(std::set<int>::iterator second=Gates->at(i).at(j).begin(); second!=Gates->at(i).at(j).end(); second++) {
                    int ill2 = *second;
                    
                    
                    //we have that -ill1 concatenated with ill2 forms an illegal turn.
                    //since our map is locally injective on the interior of edges for all powers of our map, we need only check that the illegal turns aren't being created at the end of one edge followed by the beginning of the second edge.
                    
                    //for each potential first edge that could lead to the illegal turn. (called mapfirst)
                    for(size_t mapfirst=0; mapfirst<Map_of_Turns.size(); mapfirst++) {
                        
                        //temp1 represents the image of a small segment of the end of the first edge.
                        int temp1= mapfirst>=mat.size() ? -Map_of_Turns[mapfirst-mat.size()] : -Map_of_Turns[mapfirst+mat.size()];
                        
                        //int rep for the first edge.
                        int mapfirstint= mapfirst>=mat.size() ? mat.size()-mapfirst-1 : mapfirst+1;
                        
                        //for each potential second edge that could lead to the illegal turn. (called mapsecond)
                        for(size_t mapsecond=0; mapsecond<Map_of_Turns.size(); mapsecond++) {
                            
                            //temp2 represents the image of a small segment of the beginning of the second edge.
                            int temp2=Map_of_Turns[mapsecond];
                            
                            //int rep for the second edge.
                            int mapsecondint=mapsecond>=mat.size() ? mat.size()-mapsecond-1 : mapsecond+1;
                            
                            //if our potential 2 edges will cross an illegal turn,
                            if(mapfirstint+mapsecondint!=0 && -temp1==ill1 && temp2==ill2) {
                                
                                //If this is the first 2 edges that cross this illegal turn,
                                if(illegal->count(std::pair<int,int>(ill1,ill2))==false) {
                                    std::pair < std::pair<int,int>, std::set<std::pair<int,int>>* > w = *new std::pair < std::pair<int,int>, std::set<std::pair<int,int>>* >;
                                    w.first=std::pair<int,int>(ill1,ill2);
                                    w.second=new std::set<std::pair<int,int>>;
                                    w.second->insert(std::pair<int,int>(mapfirstint,mapsecondint));
                                    illegal->insert(w);
                                }
                                //If this isn't the first 2 edges that cross this illegal turn,
                                else {
                                    illegal->at(std::pair<int,int>(ill1,ill2))->insert(std::pair<int,int>(mapfirstint,mapsecondint));
                                    /*
                                    iter = illegal->at(std::pair<int,int>(ill1,ill2))->begin();
                                    while (iter!=illegal->at(std::pair<int,int>(ill1,ill2))->end()) {
                                        
                                        //If we already accounted for these 2 edges crossing this illegal turn, then we break to avoid redundancy.
                                        if(iter->first==mapfirstint && iter->second==mapsecondint) break;
                                        iter++;
                                        
                                        //If we didn't account for these 2 edges, then we will instert it in.
                                        if(iter==illegal->at(std::pair<int,int>(ill1,ill2))->end()) {
                                        illegal->at(std::pair<int,int>(ill1,ill2))->push_back(std::pair<int,int>(mapfirstint,mapsecondint));
                                            break;
                                     }
                                     */
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}

void TopRep::DisplayIllegal() {
    std::cout<<std::endl;
    auto iter=illegal->begin();
    std::set<std::pair<int,int>>::iterator second_iter;
    size_t ill=1;
    while(iter!=illegal->end()) {
        std::cout<<ill++<<"th Illegal Turn ";
        
        if(iter->first.first<0) std::cout<<'-'<<static_cast<char>(96-iter->first.first);
        else std::cout<<static_cast<char>(96+iter->first.first);
        std::cout<<" then ";
        
        if(iter->first.second<0) std::cout<<'-'<<static_cast<char>(96-iter->first.second);
        else std::cout<<static_cast<char>(96+iter->first.second);
        
        std::cout<<" happens at "<<std::endl;
        second_iter=iter->second->begin();
        while(second_iter!=iter->second->end()) {
            std::cout<<"g(";
            if(second_iter->first<0) std::cout<<'-'<<static_cast<char>(96-second_iter->first);
            else std::cout<<static_cast<char>(96+second_iter->first);
            
            if(second_iter->second<0) std::cout<<'-'<<static_cast<char>(96-second_iter->second)<<')'<<std::endl;
            else std::cout<<static_cast<char>(96+second_iter->second)<<')'<<std::endl;
            second_iter++;
        }
        std::cout<<std::endl;
        std::cout<<std::endl;
        iter++;
    }

}

TopRep::TopRep(std::vector<std::vector<int>> m) {
    this->mat=m;
    this->mat.resize(mat.size());
    this->Map_of_Turns.resize(2 * mat.size());
    for(size_t i=0; i < mat.size(); i++){
        if(mat[i][0]=='-') Map_of_Turns[i]=(-(mat[i][1]));
        else Map_of_Turns[i]=mat[i][0];
    }
    for(size_t i=0; i<mat.size(); i++) {
        if(mat[i][mat[i].size()-2]=='-') Map_of_Turns[i+mat.size()]=mat[i][mat[i].size()-1];
        else Map_of_Turns[i+mat.size()]=-(mat[i][mat[i].size()-1]);
    }
    CreateGates();
    Illegal_Turns_Crossed();
}

TopRep TopRep::straighten() {
    std::vector<std::vector<int>> ret=this->mat;
    std::vector<std::vector<int>>::iterator i=ret.begin();
    std::vector<int>::iterator j;
    std::vector<int>::iterator iter_placeholder;
    while(i!=ret.end()) {
        j=i->begin();
        while(j!=i->end()) {
            if(j+1!=i->end() && *j==-(*(j+1))) {
                iter_placeholder=j;
                i->erase(j);
                j=iter_placeholder;
                i->erase(j);
                j=iter_placeholder;
                if(j!=i->begin()) j--;
            }
            else j++;
        }
        i++;
    }
    TopRep r(ret);
    return r;
}

void TopRep::Find_Internal_Illegal_Turns_Crossed() {
    Internal_Illegal_Turns_Crossed=new std::set<std::pair<int,int>>;
    //ill1 and ill2 iteratorate through the image of TpRp (mat).
    std::vector<int>::iterator ill1, ill2;
    //for each letter assignment (image)
    size_t assignment_count=0;
    
    //this boolean exists to help us terminate iteration once we discover
    bool flag=false;
    while(assignment_count<mat.size()) {
        
        //no reason to check if there isn't a turn
        if(mat[assignment_count].size()>1) {
            ill1=mat[assignment_count].begin();
            ill2=ill1;
            ill2++;
            
            while(ill2!=mat[assignment_count].end()) {
                
                if(Internal_Illegal_Turns_Crossed->count(std::pair<int,int>(-*ill1,*ill2)) || (Internal_Illegal_Turns_Crossed->count(std::pair<int,int>(*ill1,-*ill2))));
                                                                                    
                else {
                    //for each gate component
                    for(size_t i=0; i<Gates->size(); i++) {
                    
                        //If we already found what we are looking for,
                        if(flag==true) break;
                
                        //for each gate in said component
                        for(size_t j=0; j<Gates[i].size(); j++) {
                    
                            //If -*ill1 is in said gate
                            if(Gates->at(i).at(j).count(-*ill1)) {
                        
                                //If an illegal turn was crossed.
                                if(Gates->at(i).at(j).count(*ill2)) {
                                    Internal_Illegal_Turns_Crossed->insert(std::pair<int,int>(-*ill1,*ill2));
                                    flag=true;
                                    break;
                                }
                                //Otherwise it wasn't and we should move on. This means iterating ill1 and ill2 if possible.
                                else {
                                    flag=true;
                                    break;
                                }
                            }
                        //else -*ill1 isn't in said gate and we should increment j.
                        }
                    }
                }
                ill1++;
                ill2++;
                flag=false;
            }
        }
    assignment_count++;
    }
    return;
}

void TopRep::DisplayInteralIllegal() {
    std::cout<<std::endl;
    
    if(Internal_Illegal_Turns_Crossed->empty()) std::cout<<"This is a train track map!";
        else {
        char letter='a';
        int first;
        int second;
        size_t i=0;
    
        for(std::set<std::pair<int,int>>::iterator iter=Internal_Illegal_Turns_Crossed->begin(); iter!=Internal_Illegal_Turns_Crossed->end() ; iter++) {
            i++;
            std::cout<<"Internal illegal crossed "<<i<<": ";
            first=iter->first;
            second=iter->second;
            if(first<0) std::cout<<static_cast<char>(letter-first-1);
            else std::cout<<'-'<<static_cast<char>(letter+first-1);
            if(second>0) std::cout<<static_cast<char>(letter+second-1);
            else std::cout<<'-'<<static_cast<char>(letter-second-1);
            std::cout<<std::endl;
        }
    }
    std::cout<<std::endl;
    return;
}

std::pair<TopRep, MarkedGraph> fold_turn(TopRep f, MarkedGraph graph, std::pair<int,int> turn) {
    
    /////////NEED TO WORK ON NEGATIVE INTS. mat is too small.
    
    std::vector<int> first_turn_image;
    std::vector<int> second_turn_image;
    
    if(turn.first<0) first_turn_image=*reverse(&f.mat.at(f.IntToIndex(-turn.first)));
    else first_turn_image=f.mat.at(f.IntToIndex(turn.first));
    
    if(turn.second<0) second_turn_image=*reverse(&f.mat.at(f.IntToIndex(-turn.second)));
    else second_turn_image=f.mat.at(f.IntToIndex(turn.second));
    
    
    std::vector<int>::iterator i=first_turn_image.begin();
    std::vector<int>::iterator j=second_turn_image.begin();
    if(*i!=*j) return std::pair<TopRep, MarkedGraph>{f, graph};
    std::vector<int> new_edge;
    size_t new_edge_size=0;
    
    while(i!=first_turn_image.end()) {
        
        //situation when the left edge is larger than the right. Need to perform a left improper full fold.
        if(j==second_turn_image.end()) {
            //new_edge.resize(f.mat.at(f.IntToIndex(turn.first)).size()-new_edge_size);
            while(i!=first_turn_image.end()) {
                new_edge.push_back(*i); i++;
            }
            std::vector<std::vector<int>> ret;
            //ret.resize(f.mat.size());
            for(size_t k=0; k<f.mat.size(); k++) {
                std::vector<int> new_vec;
                ret.push_back(new_vec);
                if(k!=abs(turn.first)-1) {
                    ret.at(k)=f.mat.at(k);
                    for(std::vector<int>::iterator l=ret.at(k).begin(); l!=ret.at(k).end(); l++) {
                        if(*l==turn.first) {
                            l=ret.at(k).insert(l, turn.second);
                            l++;
                        }
                        if(*l==-turn.first) {
                            l++;
                            l=ret.at(k).insert(l, -turn.second);
                        }
                    }
                }
                else {
                    if(turn.first<0) ret.at(k)=*reverse(&new_edge);
                    else ret.at(k)=new_edge;
                    for(std::vector<int>::iterator l=ret.at(k).begin(); l!=ret.at(k).end(); l++) {
                        if(*l==turn.first) {
                            l=ret.at(k).insert(l, turn.second);
                            l++;
                        }
                        if(*l==-turn.first) {
                            l++;
                            l=ret.at(k).insert(l, -turn.second);
                        }
                    }
                }
            }
            MarkedGraph quotient_graph=left_improper_full_fold(graph, turn);
            TopRep new_top_rep=ret;
            new_top_rep=new_top_rep.straighten();
            return std::pair<TopRep, MarkedGraph>{new_top_rep, quotient_graph};
        }
        else if(*i==*j){
            new_edge_size++;
            i++;
            j++;
        }
        
        //situation when a partial fold needs to occur.
        else {
            //new_edge.resize(new_edge_size);
            for(std::vector<int>::iterator k=first_turn_image.begin(); k!=i; k++) {
                new_edge.push_back(*k);
            }
            std::vector<std::vector<int>> ret;
            //ret.resize(f.mat.size()+1);
            for(size_t k=0; k<f.mat.size()+1; k++) {
                std::vector<int> new_vec;
                ret.push_back(new_vec);
                if(k!=f.mat.size()&&k!=abs(turn.first)-1&&k!=abs(turn.second)-1) {
                    ret.at(k)=f.mat.at(k);
                    for(std::vector<int>::iterator l=ret.at(k).begin(); l!=ret.at(k).end(); l++) {
                        if(*l==turn.first) {
                            l=ret.at(k).insert(l, static_cast<int>(f.mat.size())+1);
                            l++;
                        }
                        else if(*l==-turn.first) {
                            l++;
                            l=ret.at(k).insert(l, static_cast<int>(-f.mat.size())-1);
                        }
                        else if(*l==turn.second) {
                            l=ret.at(k).insert(l, static_cast<int>(f.mat.size())+1);
                            l++;
                        }
                        else if(*l==-turn.second) {
                            l++;
                            l=ret.at(k).insert(l, static_cast<int>(-f.mat.size())-1);
                        }
                    }
                }
                else if(k!=f.mat.size()&&k!=abs(turn.first)-1&&k==abs(turn.second)-1) {
                    if(turn.second<0) {
                        for(size_t m=0; m<second_turn_image.size()-new_edge_size; m++) {
                            ret.at(k).push_back(f.mat.at(abs(turn.second)-1).at(m));
                        }
                    }
                    else {
                        for(size_t m = new_edge_size; m<second_turn_image.size(); m++) {
                            ret.at(k).push_back(f.mat.at(abs(turn.second)-1).at(m));
                        }
                    }
                    for(std::vector<int>::iterator l=ret.at(k).begin(); l!=ret.at(k).end(); l++) {
                        if(*l==turn.first) {
                            l=ret.at(k).insert(l, static_cast<int>(f.mat.size())+1);
                            l++;
                        }
                        else if(*l==-turn.first) {
                            l++;
                            l=ret.at(k).insert(l, static_cast<int>(-f.mat.size())-1);
                        }
                        else if(*l==turn.second) {
                            l=ret.at(k).insert(l, static_cast<int>(f.mat.size())+1);
                            l++;
                        }
                        else if(*l==-turn.second) {
                            l++;
                            l=ret.at(k).insert(l, static_cast<int>(-f.mat.size())-1);
                        }
                    }
                }
                else if(k!=f.mat.size()&&k!=abs(turn.second)-1&&k==abs(turn.first)-1) {
                    if(turn.first<0) {
                        for(size_t m=0; m<first_turn_image.size()-new_edge_size; m++) {
                            ret.at(k).push_back(f.mat.at(abs(turn.first)-1).at(m));
                        }
                    }
                    else {
                        for(size_t m = new_edge_size; m<first_turn_image.size(); m++) {
                            ret.at(k).push_back(f.mat.at(abs(turn.first)-1).at(m));
                        }
                    }
                    for(std::vector<int>::iterator l=ret.at(k).begin(); l!=ret.at(k).end(); l++) {
                        if(*l==turn.first) {
                            l=ret.at(k).insert(l, static_cast<int>(f.mat.size())+1);
                            l++;
                        }
                        if(*l==-turn.first) {
                            l++;
                            l=ret.at(k).insert(l, static_cast<int>(-f.mat.size())-1);
                        }
                        if(*l==turn.second) {
                            l=ret.at(k).insert(l, static_cast<int>(f.mat.size())+1);
                            l++;
                        }
                        if(*l==-turn.second) {
                            l++;
                            l=ret.at(k).insert(l, static_cast<int>(-f.mat.size())-1);
                        }
                    }
                }
                else {
                    ret.at(k)=new_edge;
                    for(std::vector<int>::iterator l=ret.at(k).begin(); l!=ret.at(k).end(); l++) {
                        if(*l==turn.first) {
                            l=ret.at(k).insert(l, static_cast<int>(f.mat.size()+1));
                            l++;
                        }
                        if(*l==-turn.first) {
                            l++;
                            l=ret.at(k).insert(l, static_cast<int>(-f.mat.size()-1));
                        }
                        if(*l==turn.second) {
                            l=ret.at(k).insert(l, static_cast<int>(f.mat.size())+1);
                            l++;
                        }
                        if(*l==-turn.second) {
                            l++;
                            l=ret.at(k).insert(l, static_cast<int>(-f.mat.size())-1);
                        }
                    }
                }
            }
            MarkedGraph quotient_graph=partial_fold(graph, turn);
            TopRep new_top_rep=ret;
            new_top_rep=new_top_rep.straighten();
            return std::pair<TopRep, MarkedGraph>{new_top_rep, quotient_graph};
        }
    }
    if(i==first_turn_image.end()) {
        
        //situation when the right edge has the same image as the left. Need to perform a proper full fold.
        if(j==second_turn_image.end()) {
            bool second=false;
            for(size_t init_vertex=0; init_vertex<graph.Graph->size(); init_vertex++) {
                
                if(graph.Graph->at(init_vertex).count(turn.first)) second=true;
                //if the left is a loop, we just identify the right to the left.
                if(graph.Graph->at(init_vertex).count(turn.first)) {
                    if (graph.Graph->at(init_vertex).count(-turn.first)) {
                        std::vector<std::vector<int>> ret;
                        //ret.resize(f.mat.size()-1);
                        for(size_t i=0; i<f.mat.size(); i++) {
                            if(i!=abs(turn.second)-1) {
                                ret.push_back(f.mat.at(i));
                            }
                        }
                        for(size_t i=0; i<ret.size(); i++) {
                            for(size_t j=0; j<ret.at(i).size(); j++) {
                                if(ret.at(i).at(j)==turn.second) {
                                    ret.at(i).at(j)=turn.first;
                                }
                                if(ret.at(i).at(j)==-turn.second) {
                                    ret.at(i).at(j)=-turn.first;
                                }
                            }
                        }
                        MarkedGraph quotient_graph=proper_full_fold(graph, turn);
                        TopRep new_top_rep=ret;
                        new_top_rep.straighten();
                        return std::pair<TopRep, MarkedGraph>{new_top_rep, quotient_graph};
                    }
                    else break;
                }
            }
            //if the right is a loop, we just identify the left to the right.
            if(second==true) {
                std::vector<std::vector<int>> ret;
                //ret.resize(f.mat.size()-1);
                for(size_t i=0; i<f.mat.size(); i++) {
                    if(i!=abs(turn.first)-1) {
                        ret.push_back(f.mat.at(i));
                    }
                }
                for(size_t i=0; i<ret.size(); i++) {
                    for(size_t j=0; j<ret.at(i).size(); j++) {
                        if(ret.at(i).at(j)==turn.first) {
                            ret.at(i).at(j)=turn.second;
                        }
                        if(ret.at(i).at(j)==-turn.first) {
                            ret.at(i).at(j)=-turn.second;
                        }
                    }
                }
                MarkedGraph quotient_graph=proper_full_fold(graph, turn);
                TopRep new_top_rep=ret;
                new_top_rep=new_top_rep.straighten();
                return std::pair<TopRep, MarkedGraph>{new_top_rep, quotient_graph};
            }
        }
        //situation when the right edge is larger than the left. Need to perform a right improper full fold.
        else {
            //new_edge.resize(f.mat.at(f.IntToIndex(turn.second)).size()-new_edge_size);
            while(j!=second_turn_image.end()) {
                new_edge.push_back(*j); j++;
            }
            std::vector<std::vector<int>> ret;
            //ret.resize(f.mat.size());
            for(size_t k=0; k<f.mat.size(); k++) {
                std::vector<int>* temp=new std::vector<int>;
                ret.push_back(*temp);
                if(k!=abs(turn.second)-1) {
                    ret.at(k)=f.mat.at(k);
                    for(std::vector<int>::iterator l=ret.at(k).begin(); l!=ret.at(k).end(); l++) {
                        if(*l==turn.second) {
                            l=ret.at(k).insert(l, turn.first);
                            l++;
                        }
                        if(*l==-turn.second) {
                            l++;
                            l=ret.at(k).insert(l, -turn.first);
                        }
                    }
                }
                else {
                    if(turn.second<0) ret.at(k)=*reverse(&new_edge);
                    else ret.at(k)=new_edge;
                    for(std::vector<int>::iterator l=ret.at(k).begin(); l!=ret.at(k).end(); l++) {
                        if(*l==turn.second) {
                            l=ret.at(k).insert(l, turn.first);
                            l++;
                        }
                        if(*l==-turn.second) {
                            l++;
                            l=ret.at(k).insert(l, -turn.first);
                        }
                    }
                }
            }
            MarkedGraph quotient_graph=left_improper_full_fold(graph, turn);
            TopRep new_top_rep=ret;
            new_top_rep=new_top_rep.straighten();
            return std::pair<TopRep, MarkedGraph>{new_top_rep, quotient_graph};
        }
    }
    
    return std::pair<TopRep, MarkedGraph>{f,graph};
}

std::pair<TopRep,MarkedGraph> fold_turn(TopRep f, MarkedGraph graph, std::string left, std::string right) {
    char letter='a';
    if(left[0]=='-') {
        if(right[0]=='-') return fold_turn(f, graph, std::pair<int,int>{-static_cast<int>(left[1]-letter+1), -static_cast<int>(right[1]-letter+1)});
        else return fold_turn(f, graph, std::pair<int,int>{-static_cast<int>(left[1]-letter+1), static_cast<int>(right[0]-letter+1)});
    }
    else {
        if(right[0]=='-') return fold_turn(f, graph, std::pair<int,int>{static_cast<int>(left[0]-letter+1), -static_cast<int>(right[1]-letter+1)});
        else return fold_turn(f, graph, std::pair<int,int>{static_cast<int>(left[0]-letter+1), static_cast<int>(right[0]-letter+1)});
    }
}


