//
//  state.hpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 04/09/23.
//

#ifndef state_hpp
#define state_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <stdbool.h>

using namespace std;

class State
{
  
public:
    bool accepting;
    map<string,set<State*>> _transition;
    unsigned long int num;
    State(bool accepting)
    {
        this->accepting = accepting;
        this->_transition = map<string,set<State*>>();
    }
    
    map<string,set<State*>> getTransitions()
    {
        return this->_transition;
    }
    State* addTransitionOnSymbol(string symbol, State *state)
    {
        auto it = this->_transition.find(symbol);
        if(it != this->_transition.end())
        {
            this->_transition.find(symbol)->second.insert(state);
        }
        else
        {
            this->_transition.insert({symbol,{state}});
        }
        return this;
    }
    set<State*> getTransitionOnSymbol(string symbol)
    {
        auto it = this->_transition.find(symbol);
        return (it != this->_transition.end()) ? it->second : set<State*>();
    }
    virtual bool match(string str,State* startState, set<State*> visited = {}) = 0;
};


#endif /* state_hpp */
