#ifndef FINITSTATEMACHINE_H_
#define FINITSTATEMACHINE_H_

#include <map>
#include <boost/shared_ptr.hpp>

namespace FSM{

    template < typename EI, typename KEY, typename PARAM >
        class State
    {
    public:
        virtual ~State(){}
        virtual void Enter(KEY &next_key, PARAM &p) { next_key = "";}
        virtual void Leave(){}
    };

    template < typename EI, typename KEY, typename PARAM >
        class FiniteStateMachine : public State<EI, KEY, PARAM>{
    public:
        FiniteStateMachine() : active_state_(0){}
        virtual FiniteStateMachine(){}

        virtual void leave(){
            if(active_state_) active_state_->leave();
        }
        
        State<EI, KEY, PARAM> *active_state(void) { return active_state_;}
        State<EI, KEY, PARAM> *get_state(const KEY& k){
            typename std::map<KEY, boost::shared_ptr< State<EI, KEY, PARAM> > >::const_iterator iter;
            iter = states_.find(k);
            assert(iter != states_.end());
            return iter->second();
        }

    protected:
        virtual bool transition(KEY &k, PARAM &p){
            if(EI::no_transition(k)) return false;
            typename std::map<KEY, boost::shared_ptr< State<EI, KEY, PARAM> > >::const_iterator iter;
            iter = states_.find(k);
            assert( iter != states_.end());
            if(active_state_) active_state_->leave();
            active_state_->enter(k, p);
            return true;
        }

        typedef typename std::map<KEY, boost::shared_ptr< State<EI, KEY, PARAM> > > state_containter;
        state_containter states_;
    private:
        State<EI, KEY, PARAM> *active_state_;
    };
}

#endif
