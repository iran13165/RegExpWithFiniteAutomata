//
//  builders.cpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 04/09/23.
//
#include <cstdarg>
#include "state.cpp"
#include "nfa.cpp"

using namespace std;
#include "NFAState.cpp"
using namespace std;


class Builders
{
public:
    NFA* _char(string c)
    {
        State* inState = new NFAState(false);
        State* outState = new NFAState(true);

        return new NFA(inState->addTransitionOnSymbol(c, outState),outState);
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
  //  template<typename T, typename ... Args>
    NFA* alt(NFA* first,...)
    {
        va_list args;
        va_start(args, first);
        NFA* start = first;
        while (1) {
            NFA* frag = va_arg(args, NFA*);

            if(frag == NULL)

            if(frag == NULL || frag->in == NULL || frag->out == NULL)
                break;
            start = altPair(start, frag);
            first++;
        }
        va_end(args);
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
    NFA* _or(NFA* first,...)
    {
        va_list args;
        va_start(args, first);
        NFA* start = first;
        while (1) {
            NFA* frag = va_arg(args, NFA*);
            if(frag == NULL)
                break;
            start = orPair(start, frag);
            first++;
        }
        va_end(args);
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

