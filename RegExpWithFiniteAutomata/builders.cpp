//
//  builders.cpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 04/09/23.
//
#include <utility>
#include <cstdarg>
#include "state.cpp"
#include "nfa.cpp"
#include <iostream>
using namespace std;
#include "NFAState.cpp"


class Builders
{
public:
    NFA* _char(string c)
    {
        State* inState = new NFAState(false);
        State* outState = new NFAState(true);
        State* t = inState->addTransitionOnSymbol(c, outState);
        return new NFA(t,outState);
    }
    NFA* e()
    {
        return _char(EPSILON);
    }
    NFA* altPair(NFA* first, NFA* second)
    {
        first->out->accepting = false;
        second->out->accepting = true;
        first->out->addTransitionOnSymbol(EPSILON, second->in);
        return new NFA(first->in, second->out);
    }
   //template<typename T,typename... Param>
    NFA* alt(vector<NFA*> nfs)
    {
        NFA* start = nfs[0];
        for(int i = 1; i<nfs.size(); i++)
        {
            start = orPair(start, nfs[i]);
        }
        
        return start;

    }

    NFA* orPair(NFA* first, NFA* second)
    {
        State *inState = new NFAState(false);
        State* outState = new NFAState(true);
        
        inState->addTransitionOnSymbol(EPSILON, first->in);
        inState->addTransitionOnSymbol(EPSILON, second->in);
        
        first->out->accepting = false;
        second->out->accepting = false;
        first->out->addTransitionOnSymbol(EPSILON, outState);
        second->out->addTransitionOnSymbol(EPSILON, outState);
        return new NFA(inState, outState);
        
    }
    NFA* _or(vector<NFA*> nfs)
    {
        
        NFA* start = nfs[0];
        for(int i = 1; i<nfs.size(); i++)
        {
            start = orPair(start, nfs[i]);
        }
        
        return start;
    }

    // -----------------------------------------------------------------------------
    // Kleene-closure

    /**
     * Kleene star/closure.
     *
     * a*
     */
    NFA* repExplicit(NFA* fragment) {
      State* inState = new NFAState(false);
      State* outState = new NFAState(true);

      // 0 or more.
      inState->addTransitionOnSymbol(EPSILON, fragment->in);
      inState->addTransitionOnSymbol(EPSILON, outState);

      fragment->out->accepting = false;
      fragment->out->addTransitionOnSymbol(EPSILON, outState);
      outState->addTransitionOnSymbol(EPSILON, fragment->in);

      return new NFA(inState, outState);
    }

    /**
     * Optimized Kleene-star: just adds ε-transitions from
     * input to the output, and back.
     */
    NFA* rep(NFA* fragment) {
      fragment->in->addTransitionOnSymbol(EPSILON, fragment->out);
      fragment->out->addTransitionOnSymbol(EPSILON, fragment->in);
      return fragment;
    }

    /**
     * Optimized Plus: just adds ε-transitions from
     * the output to the input.
     */
    NFA* plusRep(NFA* fragment) {
      fragment->out->addTransitionOnSymbol(EPSILON, fragment->in);
      return fragment;
    }

    /**
     * Optimized ? repetition: just adds ε-transitions from
     * the input to the output.
     */
    NFA* questionRep(NFA* fragment) {
      fragment->in->addTransitionOnSymbol(EPSILON, fragment->out);
      return fragment;
    }
};

