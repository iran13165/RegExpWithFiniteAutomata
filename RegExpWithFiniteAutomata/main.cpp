//
//  main.cpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 04/09/23.
//

#include <iostream>
#include "state.cpp"
#include "builders.cpp"
#include <memory>
using namespace std;


int main(int argc, const char * argv[]) {
    Builders build;
    NFA* a = build._char("a");
    NFA* b = build._char("b");
    NFA* c = build._char("c");
//    NFA* d = build._char("d");
   // problem occurs when call build.alt(build.charc("a"),build.charc("a"),build.charc("a"));
     NFA* in = build._or(a,b,c);
//    build.alt(a,b,c);
//    build.alt(a,b,c,d);
   // build.alt(a,b);

    in->setTransitionTable(in->in);
    in->setEpsilonClosure();
    in->getAcceptingStateNumbers();
    in->getAlphabet();
    in->create_dfa_table();
    cout<<in->matches("abc")<<endl;
    cout<<in->matches("b")<<endl;
    cout<<in->matches("c")<<endl;
    cout<<in->matches("d")<<endl;
    return 0;
}
