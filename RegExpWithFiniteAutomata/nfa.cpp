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
            if(start->accepting == true)
            {
                acceptingStates.insert(start);
            }
            start->num = visitedSet.size();
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
                        it->second.insert(make_pair(item.first, item.second));
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
        set<unsigned long int> starting_state = set<unsigned long int>();
        map<string,set<unsigned long int>> it = transitionTable->at(1);
        starting_state = it.at(EPSILON_CLOSURE);
        
        set<string> alphabets = getAlphabet();
        set<unsigned long int> alphabetsSet;
        map<string,map<string,set<unsigned long int>>> dfaTable = map<string,map<string,set<unsigned long int>>>();
        map<string,set<unsigned long int>> transitionset = map<string,set<unsigned long int>>();
        for(string alpha : alphabets)
        {
           
            
          for(auto st:starting_state)
          {
              
             auto it = transitionTable->at(st).find(alpha);
            
              set<unsigned long int> tset = set<unsigned long int>();
              if(it != transitionTable->at(st).end())
              {
                  alphabetsSet  = transitionTable->at(st).at(alpha);
                  for(unsigned long int l : alphabetsSet)
                  {
                    set<unsigned long int> closerSet =  transitionTable->at(l).at(EPSILON_CLOSURE);
                      
                      for(unsigned long int closure : closerSet)
                      {
                          tset.insert(closure);
                      }
                  }
               }
              if(!tset.empty())
              {
                  auto it = transitionset.find(alpha);
                  if(it != transitionset.end())
                  {
                      for(auto item:it->second)
                      {
                          it->second.insert(item);
                      }
                  } else
                  {
                      transitionset.insert({alpha,tset});
                  }
                                  
              }
          }
          
        
        }
        string key = "";
        for(auto s :starting_state)
        {
            key+="_"+to_string(s);
        }
          auto itdfa = dfaTable.find(key);
          if(itdfa != dfaTable.end())
          {
              for(auto item : itdfa->second)
              {
                  itdfa->second.insert(make_pair(item.first, item.second));
              }
          } else
          {
              dfaTable.insert({key,transitionset});
          }
        loop_for_dfa_table(&dfaTable, key);
        remap_dfa_table(&dfaTable);
        
    }
    void loop_for_dfa_table(map<string,map<string,set<unsigned long int>>>* dfaTable, string last_key)
    {
        set<string> alphabets = getAlphabet();
        for(string alph:alphabets)
        {
            set<unsigned long int> starting_state = set<unsigned long int>();
            auto it = dfaTable->at(last_key).find(alph);
            if(it !=  dfaTable->at(last_key).end())
            {
                 starting_state = it->second;
//                map<string,set<unsigned long int>> it = transitionTable->at(1);
//                starting_state = it.at(EPSILON_CLOSURE);
                
               
                set<unsigned long int> alphabetsSet;
                map<string,set<unsigned long int>> transitionset = map<string,set<unsigned long int>>();
                for(string alpha : alphabets)
                {
                   
                    
                  for(auto st:starting_state)
                  {
                      
                     auto it = transitionTable->at(st).find(alpha);
                    
                      set<unsigned long int> tset = set<unsigned long int>();
                      if(it != transitionTable->at(st).end())
                      {
                          alphabetsSet  = transitionTable->at(st).at(alpha);
                          for(unsigned long int l : alphabetsSet)
                          {
                            set<unsigned long int> closerSet =  transitionTable->at(l).at(EPSILON_CLOSURE);
                              
                              for(unsigned long int closure : closerSet)
                              {
                                  tset.insert(closure);
                              }
                          }
                       }
                      if(!tset.empty())
                      {
                          auto it = transitionset.find(alpha);
                          if(it != transitionset.end())
                          {
                              for(auto item:it->second)
                              {
                                  it->second.insert(item);
                              }
                          } else
                          {
                              transitionset.insert({alpha,tset});
                          }
                                          
                      }
                  }
                  
                
                }
                string key = "";
                for(auto s :starting_state)
                {
                    key+="_"+to_string(s);
                }
                  auto itdfa = dfaTable->find(key);
                  if(itdfa != dfaTable->end())
                  {
                      for(auto item : itdfa->second)
                      {
                          itdfa->second.insert(make_pair(item.first, item.second));
                      }
                  } else
                  {
                      dfaTable->insert({key,transitionset});
                  }
                loop_for_dfa_table(dfaTable,key);
            }
            
        }
        
        
        
       
       // unsigned long size =  dfaTable.size();
    }
    void remap_dfa_table(map<string,map<string,set<unsigned long int>>>* dfaTable)
    {
        map<unsigned long int,map<string,unsigned long int>> final_dfa = map<unsigned long int,map<string,unsigned long int>>();
        map<string,unsigned long int> mapping = map<string,unsigned long int>();
        unsigned long int increment = 1;
        set<string> alphabets = getAlphabet();
        for(auto transition : *dfaTable)
        {
            mapping[transition.first] = increment;
            increment++;
        }
        for(auto transition : *dfaTable)
        {
            for(auto alpha: alphabets){
                 auto it = dfaTable->at(transition.first).find(alpha);
                if(it != dfaTable->at(transition.first).end()){
                    set<unsigned long int> sets = dfaTable->at(transition.first).at(alpha);
                     string key = "";
                     for(auto set : sets)
                     {
                         key+="_"+to_string(set);
                     }
                     final_dfa[mapping[transition.first]].insert({alpha,mapping[key]});

                }
                else{
                    final_dfa[mapping[transition.first]].insert({});
                }
            }
        }
        
    }
};
#endif
