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
#include <map>
#include <set>
#include <iostream>
#include "constants.h"
using namespace std;

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
    set<State*> acceptingStates = set<State*>();
    map<unsigned long int,map<string,set<unsigned long int>>> *transitionTable = new (map<unsigned long int,map<string,set<unsigned long int>>>);
    set<State*> visitedSet;
    set<unsigned long int> dfa_accepted_state_num = set<unsigned long int>();
    set<string> dfa_accepted_state = set<string>();
    map<string,unsigned long int> mapping = map<string,unsigned long int>();
    set<string> alphabets = set<string>();
    set<State*> getClosure(State* v_state)
    {
        auto st = v_state->getTransitionOnSymbol(EPSILON);
       // transitionTable[v_state->num] = {};
        set<State*> closure = set<State*>();
        closure.insert(v_state);
        for(auto nextState : st)
        {
            if(!closure.contains(nextState))
            {
                closure.insert(nextState);
                auto nextClosure = getClosure(nextState);
                for(auto a : nextClosure)
                {
                    closure.insert(a);
                }
            }
        }
        return closure;

    }
    void setEpsilonClosure()
    {
               for(auto v_state: visitedSet)
               {
                   transitionTable->at(v_state->num).erase(EPSILON);
                   auto closer_set = getClosure(v_state);
                   set<unsigned long int> c_set = {};
                   for(State* sset : closer_set)
                   {
                       c_set.insert(sset->num);
                   }
                   transitionTable->find(v_state->num)->second.insert(make_pair(EPSILON_CLOSURE, c_set));
               }
    }
    set<string> getAlphabet()
    {
        set<string> alphabets = set<string>();
        for(auto transition : *transitionTable)
        {
            for(auto list_tran : transition.second)
            {
                if(list_tran.first != EPSILON_CLOSURE)
                {
                    alphabets.insert(list_tran.first);
                }
            }
        }
        return alphabets;
    }
    set<unsigned long int> getAcceptingStateNumbers()
    {
        set<unsigned long int> setWithAcceptingStaeNumbers = set<unsigned long int>();
        for(auto s : acceptingStates)
        {
            setWithAcceptingStaeNumbers.insert(s->num);
        }
        return  setWithAcceptingStaeNumbers;
    }
   void setTransitionTable(State* start)
    {
            if(visitedSet.contains(start))
            {
                return;
            }
            visitedSet.insert(start);
       
       
            start->num = visitedSet.size();
       if(start->accepting == true)
       {
           acceptingStates.insert(start);
       }
            transitionTable->insert({start->num,{}});
            map<string,set<State*>> transitions = start->getTransitions();
            for(auto next : transitions)
            {
               map<string, set<unsigned long int>> *combine = new map<string, set<unsigned long int>>();
                for(auto nextState : next.second)
                {
                    setTransitionTable(nextState);
                    auto it = combine->find(next.first);
                    if(it != combine->end())
                    {
                        it->second.insert(nextState->num);
                    }
                    else
                    {
                        combine->insert({next.first,{nextState->num}});
                    }
                }
                                
               auto it = transitionTable->find(start->num);
                if(it != transitionTable->end())
                {
                    for(auto item : *combine)
                    {
                      auto nextit =  it->second.find(item.first);
                        if(nextit != it->second.end())
                        {
                            for(auto itm: item.second)
                            {
                                nextit->second.insert(itm);
                            }
                        } else
                        {
                           it->second.insert(make_pair(item.first, item.second));
                        }
                    }
                }
                else
                {
                    transitionTable->insert({start->num, *combine});
                }
            }
    }
    
    void create_dfa_table()
    {
        this->setEpsilonClosure();
        set<unsigned long int> starting_state = set<unsigned long int>();
        map<string,set<unsigned long int>> it = transitionTable->at(1);
        starting_state = it.at(EPSILON_CLOSURE);
        set<unsigned long int> nfa_state_nums = getAcceptingStateNumbers();
         this->alphabets = this->getAlphabet();
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
                dfa_accepted_state.insert(dfaStateLabel);
                accepting = false;
            }
            dfaTable[dfaStateLabel] = {};
        for(string alpha : alphabets)
        {
            
            set<unsigned long int> on_symbol = set<unsigned long int>();
            for(auto st:states)
            {
                
                auto it = transitionTable->at(st).find(alpha);
                
                //set<unsigned long int> tset = set<unsigned long int>();
                if(it != transitionTable->at(st).end())
                {
                    auto nit = transitionTable->at(st).find(alpha);
                    if(nit != transitionTable->at(st).end())
                    {
                        alphabetsSet  = transitionTable->at(st).at(alpha);
                        for(unsigned long int l : alphabetsSet)
                        {
                            set<unsigned long int> closerSet =  transitionTable->at(l).at(EPSILON_CLOSURE);
                            
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
            mapping[transition.first] = increment;
            increment++;
        }
        for(auto transition : *dfaTable)
        {
            for(auto alpha: this->alphabets)
            {
                auto its = transition.second.find(alpha);
                if(its != transition.second.end())
                {
                    map<string,unsigned long int> pair;
                    pair.insert({alpha,mapping[transition.second.at(alpha)]});
                    auto it = final_dfa.find(mapping[transition.first]);
                    if(it != final_dfa.end())
                    {
                        it->second.insert({alpha,mapping[transition.second.at(alpha)]});
                    } else
                        final_dfa.insert({mapping[transition.first],pair});
                }else
                {
                    final_dfa.insert({mapping[transition.first],{}});
                }
            }
        }
        /// dfa accepting table mapping
        for(string accept_state : dfa_accepted_state)
        {
            dfa_accepted_state_num.insert(mapping[accept_state]);
        }
        create_dfa_minimize_table(final_dfa);
    
    }
    map<unsigned long int,set<unsigned long int>> current_transition_map = map<unsigned long int,set<unsigned long int>>();

    void create_dfa_minimize_table(map<unsigned long int,map<string,unsigned long int>> final_dfa)
    {
        map<unsigned long int,map<string,unsigned long int>> dfa_minimize_table = map<unsigned long int,map<string,unsigned long int>>();
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
        if(acceptingStates.size()>0)
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
        map<unsigned long int,map<string,unsigned long int>> minimizedTable = map<unsigned long int,map<string,unsigned long int>>();
        unsigned long int idx=1;
        for(auto _set: current_state)
        {
            remap.insert({_set,idx});
            idx++;
        }
        for(auto entry: remap)
        {
            minimizedTable.insert({entry.second,{}});
            for(auto symbol: this->alphabets)
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
        
    }
    bool areEquivalent(unsigned long int h_state,unsigned long int rest_state,map<unsigned long int,map<string,unsigned long int>> final_dfa)
    {
       
        for(auto alpha : this->alphabets)
        {
            if(!is_two_states_equivalent(h_state,rest_state,final_dfa,alpha))
           {
               return false;
           }
        }
        return true;
    }
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
};
#endif
