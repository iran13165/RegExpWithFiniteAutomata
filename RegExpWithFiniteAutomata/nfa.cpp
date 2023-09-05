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
};
#endif
