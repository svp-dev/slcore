
/* uTC Family creation template code, defines a family *
 * and create for a family with $n arguments.          */
$template_def
class Family_$n : public FamilyBase
{
public:
    typedef void (*ThreadFunc)($template_ref_typedef);

private:
    struct Thread : public ThreadInfo
    {
    	ThreadFunc m_func;
        $template_params
		
        void run()
        {
            EnableCancellation();
            DPRINT("+ Entering User code...");
            (*m_func)($template_invoke);
            DPRINT("- Exiting User code...");
            DisableCancellation();
            $template_block;
        }
        
        template <typename T>
        static T* get_prev(const ThreadCreateInfo& tci, T Thread::*arg)
        {
        	return (tci.prev == NULL) ? NULL : &(static_cast<Thread*>(tci.prev)->*arg);
        }
	    
        Thread(const ThreadCreateInfo& tci, ThreadFunc func $template_ctor_ref)
            : ThreadInfo(tci), m_func(func) $template_thread_init
        {
        }
    };

    ThreadFunc m_func;
    $template_args

    ThreadInfo* create_thread(const ThreadCreateInfo& tci)
    {
        return new Thread(tci, m_func $template_thread_invoke);
    }
	
public:
    Family_$n(place place_id, bool root, bool nosync, long start, long end, long step, unsigned long blockSize, ThreadFunc func $template_ctor_ref)
        : FamilyBase(start, end, step, blockSize, place_id, root, nosync, $template_isshared), m_func(func) $template_init
    {
    }
};

$template_def_break
class Family_B$n : public Family_$n $template_bare_nobreak, public BreakableFamily<BreakT>
{
public:
    Family_B$n(BreakT* pBreakVal, place place_id, bool root, bool nosync,  long start, long end, long step, unsigned long blockSize, typename Family_$n $template_bare_nobreak :: ThreadFunc func $template_ctor_ref)
        : Family_$n $template_bare_nobreak(place_id, root, nosync, start, end, step, blockSize, func $template_passon), BreakableFamily<BreakT>(pBreakVal)
    {
    }
};

$template_def_break
static inline void create(family& f, place place_id, bool root, bool nosync, long start, long end, long step, unsigned long blockSize, BreakT& pBreakVal, void (*func)($template_ref_typedef) $template_ctor)
{
    // Don't allow kills when we create a family; and don't create families when a kill is in progress.
    // Otherwise, the possibility exists that a kill never catches up to the creates.
    ProtectKill();
    f = new Family_B$n $template_bare(&pBreakVal, place_id, root, nosync, start, end, step, blockSize, func $template_passon);

    f->start_creating();

    UnprotectKill();
}

$template_def
static inline void create(family& f, place place_id, bool root, bool nosync, long start, long end, long step, unsigned long blockSize, void (*func)($template_ref_typedef) $template_ctor)
{
    // Don't allow kills when we create a family; and don't create families when a kill is in progress.
    // Otherwise, the possibility exists that a kill never catches up to the creates.
    ProtectKill();
    f = new Family_$n $template_bare_nobreak(place_id, root, nosync, start, end, step, blockSize, func $template_passon);

    f->start_creating();
    
    UnprotectKill();
}

