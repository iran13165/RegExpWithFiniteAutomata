//
//  main.cpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 04/09/23.
//

#include <iostream>
#include "state.cpp"
#include "nfa/builders.cpp"
#include "dfa/dfa.cpp"
#include <memory>
using namespace std;

int main(int argc, const char * argv[]) {

    Builders build;
    NFA* a = build._char("a");
    NFA* b = build._char("b");
    NFA* c = build._char("c");
    NFA* aStar= build.repExplicit(a);

    //NFA* d = build._char("d");
   // problem occurs when call build.alt(build.charc("a"),build.charc("a"),build.charc("a"));
    vector<NFA*> nfs;
    nfs.push_back(aStar);
    nfs.push_back(b);
    nfs.push_back(c);
    NFA* in =build._or(nfs);
    //or(alt(char('a'),repExplicit(char('a'))), char('b'),char('c'));
//    build.alt(a,b,c);
//    build.alt(a,b,c,d);
   // build.alt(a,b);

    in->setTransitionTable(in->in);
    in->setEpsilonClosure();
    in->getAcceptingStateNumbers();
    in->getAlphabet();
    DFA dfa(in->in);
    dfa.create_dfa_table();
    //dfa().setTransitionTable(in->in);
//    in->create_dfa_table();
//    
//    cout<<in->matches("abc")<<endl;
    cout<<dfa.matches_min_dfa("abc")<<endl;
//    
//    cout<<in->matches("b")<<endl;
    cout<<dfa.matches_min_dfa("b")<<endl;
//
//    cout<<in->matches("c")<<endl;
    cout<<dfa.matches_min_dfa("c")<<endl;
//
//    cout<<in->matches("d")<<endl;
    cout<<dfa.matches_min_dfa("d")<<endl;

    return 0;
}
