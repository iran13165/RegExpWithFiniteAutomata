//
//  main.cpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 04/09/23.
//

#include <iostream>
#include "state.cpp"
#include "builders.cpp"
using namespace std;

int main(int argc, const char * argv[]) {
//    State *s1 = new State(false);
//    State *s2 = new State(false);
//    State *s3 = new State(true);
//
//    s1->addTransitionOnSymbol("a", s2);
//    s1->addTransitionOnSymbol("b", s3);
//
//    cout<<s1->getTransitionOnSymbol("a").size()<<endl;
    Builders build;
    NFA* a = build._char("a");
    NFA* b = build._char("b");
    NFA* c = build._char("c");
    //problem occurs when call build.alt(build.charc("a"),build.charc("a"),build.charc("a"));
    build._or(a,b,c);

    cout<<a->matches("ab")<<endl;
    cout<<b->matches("b")<<endl;
    cout<<c->matches("c")<<endl;
    cout<<a->matches("d")<<endl;
    return 0;
}
