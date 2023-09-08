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
//    NFA* c = build._char("c");
//    NFA* d = build._char("d");
   // problem occurs when call build.alt(build.charc("a"),build.charc("a"),build.charc("a"));
    build.altPair(a,b);
//    build.alt(a,b,c);
//    build.alt(a,b,c,d);
   // build.alt(a,b);

    a->setTransitionTable(a->in);
    a->setEpsilonClosure();

    cout<<a->matches("abc")<<endl;
    cout<<a->matches("b")<<endl;
    cout<<a->matches("c")<<endl;
    cout<<a->matches("d")<<endl;
    return 0;
}
