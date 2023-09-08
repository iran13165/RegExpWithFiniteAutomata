//
//  nfa.cpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 04/09/23.
//
#ifndef nfa_h
#define nfa_h
#include <stdio.h>
#include "state.cpp"
#include <map>
#include <set>
#include <iostream>
#include "constants.h"
using namespace std;

class NFA
{
   
public:
    State* in;
    State* out;
    NFA(State* inState, State* outState)
    {
        this->in = inState;
        this->out = outState;
    }
    bool matches(string str) {
      return this->in->match(str,this->in);
    }
//    map<int,map<string,set<string>>> getTransitionTable()
//    {
//        map<int,map<string,set<string>>>_transitionTable = {};
//        set<State*> visitedSet = {};
//       // setTransitionTable(&_transitionTable,&visitedSet);
//
//
//        return _transitionTable;
//    }
    map<unsigned long int,map<string,set<unsigned long int>>> *transitionTable = new (map<unsigned long int,map<string,set<unsigned long int>>>);
    set<State*> visitedSet;
    set<State*> getClosure(State* v_state)
    {
        auto st = v_state->getTransitionOnSymbol(EPSILON);
       // transitionTable[v_state->num] = {};
        set<State*> closure = set<State*>();
        closure.insert(v_state);
        for(auto nextState : st)
        {
            if(!closure.contains(nextState))
            {
                closure.insert(nextState);
                auto nextClosure = getClosure(nextState);
                for(auto a : nextClosure)
                {
                    closure.insert(a);
                }
            }
        }
        return closure;

    }
    void setEpsilonClosure()
    {
               for(auto v_state: visitedSet)
               {
                   transitionTable->at(v_state->num).erase(EPSILON);
                   auto closer_set = getClosure(v_state);
                   set<unsigned long int> c_set = {};
                   for(State* sset : closer_set)
                   {
                       c_set.insert(sset->num);
                   }
                   transitionTable->find(v_state->num)->second.insert(make_pair(EPSILON_CLOSURE, c_set));
               }
    }
   void setTransitionTable(State* start)
    {
            if(visitedSet.contains(start))
            {
                return;
            }
            visitedSet.insert(start);
            start->num = visitedSet.size();
            transitionTable->insert({start->num,{}});
            map<string,set<State*>> transitions = start->getTransitions();
            for(auto next : transitions)
            {
               map<string, set<unsigned long int>> *combine = new map<string, set<unsigned long int>>();
                for(auto nextState : next.second)
                {
                    setTransitionTable(nextState);
                    auto it = combine->find(next.first);
                    if(it != combine->end())
                    {
                        it->second.insert(nextState->num);
                    }
                    else
                    {
                        combine->insert({next.first,{nextState->num}});
                    }
                }
                                
               auto it = transitionTable->find(start->num);
                if(it != transitionTable->end())
                {
                    for(auto item : *combine)
                    {
                        it->second.insert(make_pair(item.first, item.second));
                    }
                }
                else
                {
                    transitionTable->insert({start->num, *combine});
                }
            }

    }
};
#endif
