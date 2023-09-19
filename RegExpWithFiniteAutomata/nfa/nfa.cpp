//
//  nfa.cpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 04/09/23.
//
#ifndef nfa_h
#define nfa_h
#include <stdio.h>
#include "../state.cpp"
#include <map>
#include <set>
#include <iostream>
#include "special-symbol.h"
using namespace std;

class NFA
{
   
public:
    State* in;
    State* out;
    NFA(){}
    NFA(State* inState, State* outState)
    {
        this->in = inState;
        this->out = outState;
    }
    bool matches(string str) {
      return this->in->match(str,this->in);
    }
    set<State*> _nfa_accepting_states = set<State*>();
    map<unsigned long int,map<string,set<unsigned long int>>> *_nfa_table = new (map<unsigned long int,map<string,set<unsigned long int>>>);
    set<State*> _visited_set;
       
   
    set<string> _alphabets = set<string>();

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
               for(auto v_state: _visited_set)
               {
                   _nfa_table->at(v_state->num).erase(EPSILON);
                   auto closer_set = getClosure(v_state);
                   set<unsigned long int> c_set = {};
                   for(State* sset : closer_set)
                   {
                       c_set.insert(sset->num);
                   }
                   _nfa_table->find(v_state->num)->second.insert(make_pair(EPSILON_CLOSURE, c_set));
               }
    }
    set<string> getAlphabet()
    {
        set<string> alphabets = set<string>();
        for(auto transition : *_nfa_table)
        {
            for(auto list_tran : transition.second)
            {
                if(list_tran.first != EPSILON_CLOSURE)
                {
                    alphabets.insert(list_tran.first);
                }
            }
        }
        return alphabets;
    }
    set<unsigned long int> getAcceptingStateNumbers()
    {
        set<unsigned long int> setWithAcceptingStaeNumbers = set<unsigned long int>();
        for(auto s : _nfa_accepting_states)
        {
            setWithAcceptingStaeNumbers.insert(s->num);
        }
        return  setWithAcceptingStaeNumbers;
    }
   void setTransitionTable(State* start)
    {
            if(_visited_set.contains(start))
            {
                return;
            }
            _visited_set.insert(start);
       
       
            start->num = _visited_set.size();
       if(start->accepting == true)
       {
           _nfa_accepting_states.insert(start);
       }
            _nfa_table->insert({start->num,{}});
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
                                
               auto it = _nfa_table->find(start->num);
                if(it != _nfa_table->end())
                {
                    for(auto item : *combine)
                    {
                      auto nextit =  it->second.find(item.first);
                        if(nextit != it->second.end())
                        {
                            for(auto itm: item.second)
                            {
                                nextit->second.insert(itm);
                            }
                        } else
                        {
                           it->second.insert(make_pair(item.first, item.second));
                        }
                    }
                }
                else
                {
                    _nfa_table->insert({start->num, *combine});
                }
            }
    }
    
};
#endif
