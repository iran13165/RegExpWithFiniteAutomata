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
    map<string,set<State*>> _transitions;
    unsigned long int num;
    State(bool accepting)
    {
        this->accepting = accepting;
        this->_transitions = map<string,set<State*>>();
    }
    
    map<string,set<State*>> getTransitions()
    {
        return this->_transitions;
    }
    State* addTransitionOnSymbol(string symbol, State *state)
    {
//        auto it = this->_transitions.find(symbol);
//        if(it != this->_transitions.end())
//        {
//            this->_transitions.find(symbol)->second.insert(state);
//        }
//        else
//        {
//            this->_transitions.insert({symbol,{state}});
//        }
       
        this->getTransitionOnSymbol(symbol);
        //this->_transition.insert(make_pair({symbol,state}));
        this->_transitions.at(symbol).insert(state);
        return this;
    }
    set<State*> getTransitionOnSymbol(string symbol)
    {
          auto it = this->_transitions.find(symbol);
          set<State*> transitions;
          if (it == this->_transitions.end()) {
            transitions = set<State*>();
            this->_transitions.insert({symbol, transitions});
          }
           else
           {
               transitions = it->second;
           }

          return transitions;
    }
    virtual bool match(string str,State* startState, set<State*> visited = {}) = 0;
};


#endif /* state_hpp */
