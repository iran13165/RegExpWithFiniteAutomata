////
////  dfa-minimizer.cpp
////  RegExpWithFiniteAutomata
////
////  Created by IRAN CHOUDHURY on 19/09/23.
////
#ifndef dfa_min_hpp
#define dfa_min_hpp
#include <stdio.h>
#include <set>
#include <map>
#include <iostream>
#include "../state.cpp"

using namespace std;
set<unsigned long int> min_dfa_acceting_state;
map<unsigned long int,set<unsigned long int>> current_transition_map = map<unsigned long int,set<unsigned long int>>();
map<unsigned long int,map<string,unsigned long int>> minimizedTable = map<unsigned long int,map<string,unsigned long int>>();
set<string> _alphabets;
bool is_two_states_equivalent(unsigned long int s1,unsigned long int s2,map<unsigned long int,map<string,unsigned long int>> final_dfa, string alpha)
{
    auto s1_state = current_transition_map.find(s1);
    auto s2_state = current_transition_map.find(s2);
    if(s1_state != current_transition_map.end() || s2_state != current_transition_map.end())
    {
        return false;
    }
    auto originalTransitionS1 = final_dfa.at(s1).find(alpha);
    auto originalTransitionS2 = final_dfa.at(s2).find(alpha);
    if(originalTransitionS1 == final_dfa.at(s1).end() && originalTransitionS2 == final_dfa.at(s2).end())
    {
        return true;
    }
    // Otherwise, check if they are in the same sets.
     return current_transition_map.at(s1).contains(originalTransitionS1->second) &&
    current_transition_map[s2].contains(originalTransitionS2->second);
}
bool isEqual(vector<set<unsigned long int>> current_state,vector<set<unsigned long int>> previous_state)
{
    if(current_state.size() != previous_state.size())
    {
        return false;
    }
    long int n = previous_state.size();
    for(int i = 0; i<n; i++)
    {
       if(previous_state[i].size() != current_state[i].size())
       {
           return false;
       }
       for(auto el : previous_state[i])
       {
           if(!current_state[i].contains(el))
           {
               return false;
           }
       }
        for(auto el : current_state[i])
        {
            if(!previous_state[i].contains(el))
            {
                return false;
            }
        }
    }
    
    return true;
}
bool areEquivalent(unsigned long int h_state,unsigned long int rest_state,map<unsigned long int,map<string,unsigned long int>> final_dfa)
{
   
    for(auto alpha : _alphabets)
    {
        if(!is_two_states_equivalent(h_state,rest_state,final_dfa,alpha))
       {
           return false;
       }
    }
    return true;
}

map<unsigned long int,map<string,unsigned long int>> create_dfa_minimize_table(map<unsigned long int,map<string,unsigned long int>> final_dfa,set<string> alphabets,set<unsigned long int> dfa_accepted_state_num, set<State*> _nfa_accepting_states)
{
    _alphabets=alphabets;
    set<unsigned long int> accepting_states = dfa_accepted_state_num;
    set<unsigned long int> non_accepting_states = set<unsigned long int>();
    for(auto st: final_dfa)
    {
        auto it = dfa_accepted_state_num.find(st.first);
        if(it == dfa_accepted_state_num.end())
        {
            
            non_accepting_states.insert(st.first);
            current_transition_map.insert({st.first,non_accepting_states});
        } else
        {
            current_transition_map.insert({st.first,accepting_states});
        }
    }
   
   vector<vector<set<unsigned long int>>> all = vector<vector<set<unsigned long int>>>();
    vector<set<unsigned long int>> sAll = vector<set<unsigned long int>>();
    if(non_accepting_states.size()>0)
    sAll.push_back(non_accepting_states);
    if(_nfa_accepting_states.size()>0)
    sAll.push_back(accepting_states);
    all.push_back(sAll);
    vector<set<unsigned long int>> previous_state = vector<set<unsigned long int>>();
    vector<set<unsigned long int>> current_state = all[all.size()-1];
    vector<set<unsigned long int>> combine_sets = vector<set<unsigned long int>>();
    while(!isEqual(current_state,previous_state))
    {
        map<unsigned long int,set<unsigned long int>> new_transition_map = map<unsigned long int,set<unsigned long int>>();
        for(auto s:current_state)
        {
           map<unsigned long int, set<unsigned long int>> handleStates = map<unsigned long int, set<unsigned long int>>();
            //unsigned long int::iterator it;
            auto it = s.begin();
            handleStates.insert({*it,s});
            set<unsigned long int> rests = set<unsigned long int>();
            //advance(it, 1);
            for(auto _s:s)
            {
                if(_s != *it)
                rests.insert(_s);
            }
           //
            for(auto state : rests)
            {
                for(auto handledState : handleStates)
                {
                    // This and some previously handled state are equivalent --
                            // just append this state to the same set.
                            if (areEquivalent(state, handledState.first, final_dfa))
                            {
                                set<unsigned long int> new_states=set<unsigned long int>();
                                auto it = handleStates.find(handledState.first);
                                if(it != handleStates.end())
                                {
                                    handleStates.at(handledState.first).insert(state);
                                }else{
                                    new_states.insert(state);
                                    handleStates.insert({handledState.first, new_states});
                                }
                                auto s_it = handleStates.find(state);
                                if(s_it == handleStates.end())
                                {
                                    handleStates.insert({state,handleStates.at(handledState.first)});
                                } else
                                {
                                    handleStates.at(state).insert(handledState.first);
                                }
                                continue;
                                
                            } else
                            {
                                handleStates.insert({state, {state}});
                            }
                }
            }
          //objects.assign
            for(auto handleState: handleStates)
            {
                new_transition_map.insert({handleState.first,handleState.second});
            }
        }
        current_transition_map = new_transition_map;
        set<set<unsigned long int>> new_sets = set<set<unsigned long int>>();
        for(auto s:new_transition_map)
        {
            new_sets.insert(s.second);
        }
        //all.push([...newSets]);
        vector<set<unsigned long int>> ssAll = vector<set<unsigned long int>>();
        
       // all.push_back(sAll);
        for(auto s: new_sets)
        {
            ssAll.push_back(s);
        }
        all.push_back(ssAll);

          // Top of the stack is the current.
          current_state = all[all.size() - 1];

          // Previous set.
          previous_state = all[all.size() - 2];
    }
    //REMAP
    map<set<unsigned long int>,unsigned long int> remap =  map<set<unsigned long int>,unsigned long int>();

    unsigned long int idx=1;
    for(auto _set: current_state)
    {
        remap.insert({_set,idx});
        idx++;
    }
    for(auto entry: remap)
    {
        if(dfa_accepted_state_num.contains(entry.second))
        {
            min_dfa_acceting_state.insert(entry.second);
        }
        minimizedTable.insert({entry.second,{}});
        for(auto symbol: _alphabets)
        {
            unsigned long int original_transition=0;
            for(auto originalState: entry.first)
            {
                auto it = final_dfa.at(originalState).find(symbol);
                if(it != final_dfa.at(originalState).end())
                {
                    original_transition=it->second;
                    break;
                }
            }
            if(original_transition)
            {
                map<string,unsigned long int> pair=map<string,unsigned long int>();
                pair.insert({symbol,remap.at(current_transition_map.at(original_transition))});
               auto it = minimizedTable.find(entry.second);
                if(it != minimizedTable.end())
                {
                    minimizedTable.at(entry.second).insert({symbol,remap.at(current_transition_map.at(original_transition))});
                } else
                {
                    minimizedTable.insert({entry.second,pair});
                }
                    
            }
        }
        
    }
    return minimizedTable;
    
}
#endif
