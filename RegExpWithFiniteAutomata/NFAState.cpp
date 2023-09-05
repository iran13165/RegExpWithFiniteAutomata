//
//  NFAState.cpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 05/09/23.
//

#include <stdio.h>
#include "state.cpp"
using namespace std;
static const string EPSILON = "ε";

        
class NFAState : public State
{
public:
    NFAState(bool accepting):State(accepting){}
    bool match(string str,State* startState, set<State*> visited = {})
    {
        if(visited.contains(startState))
            return false;
        visited.insert(startState);
        
        if(str.length() == 0)
        {
            if(startState->accepting == true)
            {
                return true;
            }
            for(State* nextState: startState->getTransitionOnSymbol(EPSILON))
            {
                if(match("", nextState,visited))
                {
                    return true;
                }
            }
            return false;
        }
        string symbol = str.substr(0,1);
        string rest = str.substr(1, str.length()-1);
        
          set<State*> symbolTransitions = startState->getTransitionOnSymbol(symbol);
           for (State* nextState : symbolTransitions) {
             if (match(rest,nextState)) {
               return true;
             }
           }

           // If we couldn't match on symbol, check still epsilon-transitions
           // without consuming the symbol (i.e. continue from `string`, not `rest`).
        for (State* nextState : startState->getTransitionOnSymbol(EPSILON)) {
            if (match(str,nextState, visited)) {
                return true;
            }
        }
        
        
        return false;
    }
};
