//
//  dfa.cpp
//  RegExpWithFiniteAutomata
//
//  Created by IRAN CHOUDHURY on 18/09/23.
//

#include <stdio.h>
#include <map>
#include <set>
#include <iostream>
#include "../special-symbol.h"
#include "../nfa/nfa.cpp"
using namespace std;

extern map<unsigned long int,map<string,unsigned long int>> create_dfa_minimize_table(map<unsigned long int,map<string,unsigned long int>> final_dfa,set<string> alphabets,set<unsigned long int> dfa_accepted_state_num, set<State*> _nfa_accepting_states);
extern set<unsigned long int> min_dfa_acceting_state;

class DFA : public NFA
{

public:
    set<unsigned long int> dfa_accepted_state_num = set<unsigned long int>();
    map<string,unsigned long int> _mapping = map<string,unsigned long int>();
    map<unsigned long int,map<string,unsigned long int>> dfa_minimize_table = map<unsigned long int,map<string,unsigned long int>>();
    
    map<unsigned long int,map<string,unsigned long int>> minimizedTable = map<unsigned long int,map<string,unsigned long int>>();
    set<string> _dfa_accepted_state = set<string>();
    
    DFA(State* start)
    {
        NFA::setTransitionTable(start);
    }
    void create_dfa_table()
    {
        setEpsilonClosure();
        set<unsigned long int> starting_state = set<unsigned long int>();
        map<string,set<unsigned long int>> it = _nfa_table->at(1);
        starting_state = it.at(EPSILON_CLOSURE);
        set<unsigned long int> nfa_state_nums = getAcceptingStateNumbers();
         this->_alphabets = this->getAlphabet();
        set<unsigned long int> alphabetsSet;
        map<string,map<string,string>> dfaTable = map<string,map<string,string>>();
        map<string,set<unsigned long int>> transitionset = map<string,set<unsigned long int>>();
        vector<set<unsigned long int>> working_list = {starting_state};
        while(working_list.size()>0)
        {
            set<unsigned long int> states = working_list[0];
            working_list.erase(working_list.begin());
            bool accepting = false;
            string dfaStateLabel = "";
            for(auto s :states)
            {
                if(nfa_state_nums.contains(s))
                {
                    accepting = true;
                }
                dfaStateLabel+="_"+to_string(s);
            }
            if(accepting){
                _dfa_accepted_state.insert(dfaStateLabel);
                accepting = false;
            }
            dfaTable[dfaStateLabel] = {};
        for(string alpha : _alphabets)
        {
            
            set<unsigned long int> on_symbol = set<unsigned long int>();
            for(auto st:states)
            {
                
                auto it = _nfa_table->at(st).find(alpha);
                if(it != _nfa_table->at(st).end())
                {
                    auto nit = _nfa_table->at(st).find(alpha);
                    if(nit != _nfa_table->at(st).end())
                    {
                        alphabetsSet  = _nfa_table->at(st).at(alpha);
                        for(unsigned long int l : alphabetsSet)
                        {
                            set<unsigned long int> closerSet =  _nfa_table->at(l).at(EPSILON_CLOSURE);
                            
                            for(unsigned long int closure : closerSet)
                            {
                                on_symbol.insert(closure);
                            }
                        }
                        
                    }else
                    {
                        continue;
                    }
                  
                }
                else
                {
                    continue;
                }
                
            }
            if(on_symbol.size()>0)
            {
                string key="";
                for(auto s :on_symbol)
                {
                    key+="_"+to_string(s);
                }
                auto it = dfaTable.find(dfaStateLabel);
                if(it != dfaTable.end())
                {
                    it->second.insert(make_pair(alpha, key));
                }
                
                auto dit = dfaTable.find(key);
                if(dit == dfaTable.end())
                {
                    working_list.push_back(on_symbol);
                }
            }
          }
        }
        remap_dfa_table(&dfaTable);
    }
    void remap_dfa_table(map<string,map<string,string>>* dfaTable)
    {
        map<unsigned long int,map<string,unsigned long int>> final_dfa = map<unsigned long int,map<string,unsigned long int>>();
        //map<string,unsigned long int> mapping = map<string,unsigned long int>();
        unsigned long int increment = 1;
       // set<string> alphabets = getAlphabet();
        set<unsigned long int> nfa_state_nums = getAcceptingStateNumbers();

        for(auto transition : *dfaTable)
        {
            _mapping[transition.first] = increment;
            increment++;
        }
        for(auto transition : *dfaTable)
        {
            for(auto alpha: this->_alphabets)
            {
                auto its = transition.second.find(alpha);
                if(its != transition.second.end())
                {
                    map<string,unsigned long int> pair;
                    pair.insert({alpha,_mapping[transition.second.at(alpha)]});
                    auto it = final_dfa.find(_mapping[transition.first]);
                    if(it != final_dfa.end())
                    {
                        it->second.insert({alpha,_mapping[transition.second.at(alpha)]});
                    } else
                        final_dfa.insert({_mapping[transition.first],pair});
                }else
                {
                    final_dfa.insert({_mapping[transition.first],{}});
                }
            }
        }
        /// dfa accepting table mapping
        for(string accept_state : _dfa_accepted_state)
        {
            dfa_accepted_state_num.insert(_mapping[accept_state]);
        }
        minimizedTable =  create_dfa_minimize_table(final_dfa, _alphabets, dfa_accepted_state_num, _nfa_accepting_states);
    
    }
    bool matches_min_dfa(string _string)
    {
        unsigned long int state = 1;
        unsigned long int i = 0;
        while (!_string.substr(i,1).empty()) {
           auto it = minimizedTable.at(state).find(_string.substr(i++,1));
            if(it == minimizedTable.at(state).end())
            {
                return false;
            } else
            {
                state = it->second;
            }
        }
        set<unsigned long int> min_dfa_accepting_states = min_dfa_acceting_state;
        if(!min_dfa_accepting_states.contains(state))
        {
            return false;
        }
        return true;
    }

};
