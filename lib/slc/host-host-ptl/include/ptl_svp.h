// ptl_svp.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#ifndef UTC_PTL_CPP_PTL_SVP_H
# define UTC_PTL_CPP_PTL_SVP_H

#include "ptl_vars.h"

namespace uTC
{

#ifndef UTC_CONFIG_NO_CANCELLATION
 void __DisableCancellation(const char* file, int line);
 void __EnableCancellation(const char* file, int line);

// End #ifndef UTC_CONFIG_NO_CANCELLATION
#endif

 void WaitForCancellation();


    void ProtectKill();
    void UnprotectKill();

    //
    // uTC types
    //
    enum ExitCode
    {
        EXIT_NORMAL,
        EXIT_BREAK,
        EXIT_KILL,
        EXIT_SQUEEZE
    };

    class FamilyBase;
    class ThreadInfo;
    typedef FamilyBase* family;

    struct PlaceInfo
    {
        int             n_procs;         // Number of processors in this place
        bool            exclusive_flag;  // Flag to indicate if this place is mutually exclusive
        pthread_mutex_t exclusion_mutex; // Mutex to lock this place for mutual exclusion
        bool            exclusion_lock;  // Is place locked for mutual exclusion?
        pthread_cond_t  exclusion_cond;  // Conditional to signal unlocked
        FamilyBase*     exclusion_f;     // Family that locked this place
    };

    typedef PlaceInfo* place;

    extern PlaceInfo __builtin_PLACE_GROUP;

    static const place PLACE_LOCAL = NULL;
    static const place PLACE_GROUP = &__builtin_PLACE_GROUP;

    class SharedBase
    {
        friend class ThreadInfo;

        SharedBase* m_prev; // Previous shared in the family
        SharedBase* m_next; // Next shared in the thread
		void read();
        void kill();

    protected:
        bool        m_cached;
        bool        m_killed;
        bool        m_read;
        bool        m_written;

        ThreadInfo*     m_info;
        pthread_mutex_t m_mutex;
        pthread_cond_t  m_rcond;
        pthread_cond_t  m_wcond;

        SharedBase(ThreadInfo* info, SharedBase* prev);
        virtual ~SharedBase();

	public:
        void block();
    };

    // Returns the ThreadInfo structure for this thread
    static inline ThreadInfo* GetThreadInfo()
    {
        extern pthread_key_t key_thread_info;
        return (ThreadInfo*)pthread_getspecific(key_thread_info);
    }

    struct ThreadCreateInfo
    {
        int         index;
        bool        first;
        bool        last;
        FamilyBase* f;
        ThreadInfo* prev;
    };

    class ThreadInfo : public ThreadCreateInfo
    {
    	friend class SharedBase;

        SharedBase*  m_shareds;

    public:
        // Friendly readable FamilyID
        DEXEC(std::string m_id);

        pthread_t   m_tid;
        bool        m_cleanupSelf;

        void        kill_shareds();

        // Runs the actual thread
        virtual void run() = 0;

        ThreadInfo(const ThreadCreateInfo& tci);
        virtual ~ThreadInfo();
    };

    template <typename T> class shared;

    // Base family class. Implements all family operations not dependent
    // on the type of arguments.
    class FamilyBase
    {
        template <typename T>
            friend class shared;
        friend class ThreadInfo;
        friend void kill(family f);
        template <typename BreakT>
        friend void Break(const BreakT& val);
        friend void ProtectKill();
        friend void WaitForCancellation();

        // Child family IDs
        unsigned long   m_nextChildId;

        // Family parameters
        int             m_start, m_end, m_step, m_index;
        unsigned int    m_blockSize;

        // Family tree
        FamilyBase	   *m_parent;
        FamilyBase 	   *m_firstChild, *m_nextSibling, *m_prevSibling;

        ExitCode			       m_exitCode;
        ThreadInfo*                m_lastCreated;
        std::map<int, ThreadInfo*> m_threads;
        bool				       m_allCreated;
        bool				       m_killed;
        bool                       m_killbusy;

        pthread_mutex_t m_data_mutex;
        pthread_cond_t  m_thread_completed;
        pthread_cond_t  m_created;
        pthread_cond_t  m_child_cleanup;
        pthread_cond_t	m_termination;
        pthread_cond_t  m_thread_cleanup;
        int             m_nThreadsRunning;  // Must be signed!

        static void* thread_func(void* param);
        static void* thread_creation_func(void* param);

        void create_iteration(int index, bool first, bool last);
        void set_last_created(ThreadInfo* ti);
        void create_loop();
        void destroy(bool lock = true);

        // This is overloaded by the template-specific superclass
        virtual ThreadInfo* create_thread(const ThreadCreateInfo& tci) = 0;

        protected:

        virtual ~FamilyBase();
        public:
    	static void  thread_cleanup(void* arg);

        // Friendly readable Family ID
        DEXEC(std::string		m_id);

        void     kill(ExitCode code);
        int      squeeze();
        ExitCode sync();
        void     unlink();
        void     start_creating();
        place           m_place;
        int*    m_pSqueezeValue;

	public:
        FamilyBase(int start, int end, int step, unsigned int blockSize, place place_id, int* pSqueezeVal);
    };

    template <typename T>
    class shared : public SharedBase
    {
        T       m_value;
        T       m_cache;
        T*      m_parent;
        shared* m_prev;

        T read()
        {
            LOCK(&m_mutex);
            // Wait until it has been written
            while (!m_killed && !m_written)
            {
                CONDWAIT(&m_wcond, &m_mutex);
                DPRINT("shared::read() woken up; killed: " << m_killed << " (" << &m_killed << " of sharedbase " << this << "), written: " << m_written);
            }

            if (m_killed)
            {
                UNLOCK(&m_mutex);
                WaitForCancellation();
            }

            // Mark it as read
            m_read  = true;
            T value = m_value;

            pthread_cond_signal(&m_rcond);
            UNLOCK(&m_mutex);
            return value;
        }

    public:
        shared& operator=(const T& value)
        {
            if (m_info->last)
            {
                // We're the last thread, write back to the parent shareds
                *m_parent = value;
            }
            else
            {
                DisableCancellation();
                LOCK(&m_mutex);

                // Mark it as written as well
                m_value   = value;
                m_written = true;

                pthread_cond_signal(&m_wcond);
                UNLOCK(&m_mutex);
                EnableCancellation();
            }
            return *this;
        }

        shared& operator=(shared& s)
        {
            return *this = (T)s;
        }

        T operator->()
        {
            return (T)*this;
        }

        operator T()
        {
            T value;
            if (m_prev == NULL)
            {
                // Read shared from parent
                value = *m_parent;
            }
            else
            {
                // Read from previous shared
                if (!m_cached)
                {
                    // First time, cache the value
                    DisableCancellation();
                    m_cache  = m_prev->read();
                    m_cached = true;
                    EnableCancellation();
                }
                value = m_cache;
            }
            return value;
        }

        shared(ThreadInfo* info, T* &parent, shared* prev)
        	: SharedBase(info, prev), m_parent(parent), m_prev(prev)
        {
        }
    };

	template <typename T>
	class global
	{
		T m_value;

    public:
		void block() {}

        operator T()                      { return m_value; }
        global& operator=(const T& value) { m_value = value; return *this; }
        global& operator=(global& g)      { return *this = (T)g; }
        T  operator->()                   { return (T)*this; }

        global(ThreadInfo* info, global<T> parent, global* prev) : m_value(parent.m_value) {}
        global(ThreadInfo* info, T         parent, global* prev) : m_value(parent) {}
	};

    // Index variable class
    class index
    {
        ThreadInfo* m_info;

        // Make the assignment operators private so they can't be used
        index& operator=(const int i)    { return *this; }
        index& operator=(const index& i) { return *this; }
        public:

        operator int() const
        {
            // Simply return the index of this thread
            return m_info->index;
        }

        index()
        {
            m_info = GetThreadInfo();
        }
    };

    template <typename BreakT>
    class BreakableFamily
    {
    public:
        BreakT* m_pBreakValue;

        BreakableFamily(BreakT* pBreakVal) : m_pBreakValue(pBreakVal) {}
    };

    //
    // uTC functions
    //
    ExitCode sync(family f);
    void kill(family f);
    void squeeze(family f);

    template <typename BreakT>
    void Break(const BreakT& breakval)
    {
        FamilyBase* f = GetThreadInfo()->f;
        BreakableFamily<BreakT>* bf = dynamic_cast< BreakableFamily<BreakT>* >(f);
        if (bf == NULL)
        {
            // We're breaking with mismatched types
            fputs("Thread attempted to break with mismatched type\n", stderr);

            // Abandon ship!
            abort();
        }
        else
        {
            // Lock the global kill mutex so we only have one kill at a time.
            // This lock is also held by the family creation process.
            ProtectKill();

            // Set the break value before killing
            *bf->m_pBreakValue = breakval;

            ThreadInfo* ti = GetThreadInfo();
            if (ti->f != f)
            {
                ti->m_cleanupSelf = true;
            }

            f->kill(EXIT_BREAK);

            UnprotectKill();

            if (ti->f->m_killed)
            {
                // We were killed ourselves

                #ifdef UTC_CONFIG_NO_CANCELLATION
                 // Cancellation support is not present, cleanup manually.
                 FamilyBase::thread_cleanup(ti->f);
                #else

                 DisableCancellation();
                 // Cancellation support is present, so will be cleaned
                 // up by cleanup function on cleanup stack.
                 pthread_exit(NULL);
                #endif
            }
            else
            {
                ti->m_cleanupSelf = false;
            }
        }
    }


    // Import the template header
#include "ptl_create.h"

}
#endif // ! UTC_PTL_CPP_PTL_SVP_H
