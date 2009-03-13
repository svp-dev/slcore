// ptl_svp.cc: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#include "ptl_svp.h"

using namespace std;

// Program entry point function
void t_main(void);

namespace uTC
{
    //
    // These functions register and destroy the key used to
    // store thread-specific information.
    //
    static pthread_attr_t  g_thread_attr;
           pthread_key_t   key_thread_info;
    static pthread_mutex_t g_kill_mutex;
    static pthread_cond_t  g_kill_done;
    static bool            g_kill_busy;

    static pthread_mutex_t g_killers_mutex;
    static pthread_cond_t  g_killers_done;
    static unsigned int    g_killers_count;


    PlaceInfo __builtin_PLACE_GROUP;

#ifndef UTC_CONFIG_NO_CANCELLATION
    // These functions are only defined when Cancellation is configured
    // and available
    void __DisableCancellation(const char* file, int line)
    {
        pthread_testcancel();
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        DPRINT(file << ":" << line << " Cancellation disabled");
    }

    void __EnableCancellation(const char* file, int line)
    {
        DPRINT(file << ":" << line << " Cancellation enabled");
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_testcancel();
    }

    void WaitForCancellation()
    {
        // We've been killed, wait until we get cancelled
        EnableCancellation();
        while (true)
        {
            pthread_testcancel();
            UTC_YIELD();
        }
    }

// Else, if UTC_CONFIG_NO_CANCELLATION is defined
#else
    void WaitForCancellation()
    {
        ThreadInfo* ti = GetThreadInfo();

        // Make sure our family is not creating new threads anymore
        // before we are entering cleanup.
        LOCK(&ti->f->m_data_mutex);
        while (!ti->f->m_allCreated)
        {
            CONDWAIT(&ti->f->m_termination, &ti->f->m_data_mutex);
        }
        UNLOCK(&ti->f->m_data_mutex);

        FamilyBase::thread_cleanup(ti->f);
    }
// End #ifndef UTC_CONFIG_NO_CANCELLATION
#endif


#ifdef UTC_CONFIG_SIGNALS
    extern "C" void segfault_handler(int sig)
    {
        signal(SIGSEGV, NULL);

        DPRINT("Segmentation fault");

        // Stop the execution
        abort();
    }

    extern "C" void abort_handler(int sig)
    {
        signal(SIGABRT, NULL);

        DPRINT("Aborted");

        // Stop the execution
        abort();
    }
// End #ifdef UTC_CONFIG_SIGNALS
#endif

#ifdef UTC_CONFIG_NPTL_PTHREAD_EXIT_BUG
    static void *init_nptl(void *ptr)
    {
        DPRINT("NPTL Init complete");
        pthread_exit(NULL);

        // Even though pthread_exit is defined as a function that
        // never returns, add a return against compiler warnings
        return NULL;
    }
// End #ifdef UTC_CONFIG_NPTL_PTHREAD_EXIT_BUG
#endif

    static void init()
    {
#ifdef UTC_CONFIG_SIGNALS
        signal(SIGSEGV, segfault_handler);
        signal(SIGABRT, abort_handler);
// End #ifdef UTC_CONFIG_SIGNALS
#endif

#ifdef SOLARIS_8
        // Disable Alarm Signal on Solaris 8, workaround for a bug in
        // the threading implementation
        signal(SIGALRM, SIG_IGN);
#endif

#ifdef UTC_CONFIG_DEBUG
        debug_init();
#endif
        pthread_key_create(&key_thread_info, NULL);
        pthread_mutex_init(&g_kill_mutex, NULL);
        pthread_cond_init(&g_kill_done, NULL);
        g_kill_busy = false;
        pthread_mutex_init(&g_killers_mutex, NULL);
        pthread_cond_init(&g_killers_done, NULL);
        g_killers_count = 0;

        // All threads are created detached and with minimum stack
        pthread_attr_init(&g_thread_attr);

#ifdef UTC_CONFIG_SET_STACKSIZE
        // When setting the stacksize is enabled, check if the requested stacksize
        // is not lower then the system's PTHREAD_STACK_MIN limit.
        // This has to be done here instead of in dependencies.h as PTHREAD_STACK_MIN
        // macro might not expand to a value (e.g. on Solaris)
        pthread_attr_setstacksize(&g_thread_attr, (UTC_CONFIG_DEFAULT_STACKSIZE<PTHREAD_STACK_MIN?PTHREAD_STACK_MIN:UTC_CONFIG_DEFAULT_STACKSIZE));
#endif

        pthread_attr_setdetachstate(&g_thread_attr, PTHREAD_CREATE_DETACHED);

#ifdef UTC_CONFIG_NPTL_PTHREAD_EXIT_BUG
        // Insert a workaround against possible pthread_exit segfault bug
        // that is present in NPTL
        // See also: http://bugs.mysql.com/bug.php?id=24507 where a similar
        // problem is described

        // Actual glibc bugreport (for Redhat)
        // http://sourceware.org/bugzilla/show_bug.cgi?id=2644

        // Apparently it is a problem that only can occur during the first
        // call to pthread_exit, while racing with some other pthread call

        // Bug workaround: before any utc-ptl threads are run, create and
        // pthread_exit a thread to prepare some NPTL internal datastructures

        pthread_t NPTL_t;
        pthread_attr_t NPTL_thread_attr;

        // Set thread attributes, make it joinable
        pthread_attr_init(&NPTL_thread_attr);
        pthread_attr_setdetachstate(&NPTL_thread_attr, PTHREAD_CREATE_JOINABLE);

        // Create and wait on our init thread
        pthread_create(&NPTL_t, &NPTL_thread_attr, init_nptl, NULL);
        pthread_join(NPTL_t, NULL);

        // Clean up the attribute datastructure
        pthread_attr_destroy(&NPTL_thread_attr);

// End #ifdef UTC_CONFIG_NPTL_PTHREAD_EXIT_BUG
#endif

    }

    static void cleanup()
    {
        // Wait for all 'rogue' self-killer threads to finish
        LOCK(&g_killers_mutex);
        while (g_killers_count > 0)
        {
            DPRINT("Waiting for killers (" << g_killers_count << ")");
            CONDWAIT(&g_killers_done, &g_killers_mutex);
        }
        UNLOCK(&g_killers_mutex);

        pthread_cond_destroy(&g_killers_done);
        DESTROY(&g_killers_mutex);
        pthread_attr_destroy(&g_thread_attr);
        pthread_cond_destroy(&g_kill_done);
        DESTROY(&g_kill_mutex);
        pthread_key_delete(key_thread_info);

#ifdef  UTC_CONFIG_DEBUG
        debug_cleanup();
#endif
    }

    void ProtectKill()
    {
        DisableCancellation();
        LOCK(&g_kill_mutex);
        while (g_kill_busy)
        {
            // Check if we've been killed in the mean time
            ThreadInfo* ti = GetThreadInfo();
            if (ti->f->m_killed)
            {
                // We've been killed, be a sitting duck
                UNLOCK(&g_kill_mutex);
                WaitForCancellation();
            }

            // Wait for a signal that the situation changed
            CONDWAIT(&g_kill_done, &g_kill_mutex);
        }

        g_kill_busy = true;
        UNLOCK(&g_kill_mutex);
    }

    void UnprotectKill()
    {
        LOCK(&g_kill_mutex);
        g_kill_busy = false;
        pthread_cond_broadcast(&g_kill_done);
        UNLOCK(&g_kill_mutex);
        EnableCancellation();
    }

    ExitCode sync(family f)
    {
        // If we attempt to sync on a family that is a continuation,
        // we sync immediately with a normal exit code. Also since the
        // creatorthread syncs on it automatically, and we dont want a 
        // race-condition of sync that both try to access and delete FB!
        if (f->is_continuation())
        {
            return EXIT_NORMAL;
        }

        ExitCode res = EXIT_NORMAL;
        if (f == NULL)
        {
            fputs("Thread attempted to sync on invalid family\n", stderr);
            abort();
        }
        else
        {
            res = f->sync();
        }
        return res;
    }

    void squeeze(family f)
    {
        // Check if family is valid and if
        if (f == NULL || f->m_pSqueezeValue == NULL)
        {
            fputs("Thread attempted to squeeze an invalid family\n", stderr);
            abort();
        }
        else
        {
            *(f->m_pSqueezeValue) = f->squeeze();
        }
        return;
    }

    void kill(family f)
    {
        if (f == NULL)
        {
            fputs("Thread attempted to kill an invalid family\n", stderr);
            abort();
        }
        else
        {
            // Lock the global kill mutex so we only have one kill at a time.
            // This lock is also held by the family creation process.
            ProtectKill();

            ThreadInfo* ti = GetThreadInfo();
            if (ti->f != f)
            {
                // If we're not the top-level killed family, cleanup the family.
                // Otherwise, sync() will clean it up.
                ti->m_cleanupSelf = true;
            }

            f->kill(EXIT_KILL);

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

    // ********************************************************
    // SharedBase class
    // ********************************************************

    void SharedBase::read()
    {
        // Mark it as read
        LOCK(&m_mutex);
        m_read  = true;
        pthread_cond_signal(&m_rcond);
        UNLOCK(&m_mutex);
    }

    SharedBase::SharedBase(ThreadInfo* info, SharedBase* prev)
        : m_prev(prev)
    {
        m_info    = info;
        m_read    = false;
        m_cached  = false;
        m_written = false;
        m_killed  = false;

        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_rcond, NULL);
        pthread_cond_init(&m_wcond, NULL);

        // Link this shared into the thread's shareds list
        m_next = m_info->m_shareds;
        m_info->m_shareds = this;
    }

    void SharedBase::block()
    {
        DPRINT("+ block: mutex = " << &m_mutex << " this: " << this);

        if (m_prev != NULL && !m_cached)
        {
            // Read the previous shared so it can die.
            // Don't block while reading.
            m_prev->read();
        }

        if (!m_info->last)
        {
            DPRINT("* block; waiting to be read or killed in shared: " << this);
            // Wait until we've been read. Only then can we die.
            LOCK(&m_mutex);
            while (!m_killed && !m_read)
            {
                CONDWAIT(&m_rcond, &m_mutex);
            }
            UNLOCK(&m_mutex);
        }

        if (m_killed)
        {
            DPRINT("* block: we're killed!");
            WaitForCancellation();
        }
    }

    SharedBase::~SharedBase()
    {
        pthread_cond_destroy(&m_wcond);
        pthread_cond_destroy(&m_rcond);
        DESTROY(&m_mutex);

        DPRINT("- shared::~shared() this: " << this);
    }

    void SharedBase::kill()
    {
        LOCK(&m_mutex);
        m_killed = true;
        DPRINT("Set killed flag @ " << &m_killed << " of sharedbase " << this);
        pthread_cond_signal(&m_wcond);
        pthread_cond_signal(&m_rcond);
        UNLOCK(&m_mutex);
    }

    // ********************************************************
    // ThreadInfo class
    // ********************************************************

    // Wake up any threads blocked on this thread's shareds
    void ThreadInfo::kill_shareds()
    {
        DPRINT("Killing shareds in thread " << this);

        for (SharedBase* cur = m_shareds; cur != NULL; cur = cur->m_next)
        {
            DPRINT("Killing shared " << cur << " in " << this);
            cur->kill();
        }
    }

    ThreadInfo::ThreadInfo(const ThreadCreateInfo& tci) : ThreadCreateInfo(tci)
    {
        m_tid         = pthread_self();
        m_cleanupSelf = false;
        m_shareds     = NULL;
    }

    ThreadInfo::~ThreadInfo()
    {
        DPRINT("Destroyed thread");
    }

    // ********************************************************
    // FamilyBase class
    // ********************************************************

    // Called at the end of every iteration thread
    extern "C" void FamilyBase::thread_cleanup(void* arg)
    {
        FamilyBase* f = (FamilyBase*)arg;

        DPRINT("Thread Cleanup, phase 1");
        LOCK(&f->m_data_mutex);

        f->m_nThreadsRunning--;
        DPRINT("# Threads running: " << f->m_nThreadsRunning);
        if (f->m_killed)
        {
            // We've entered thread cleanup, wait for notification to continue
            pthread_cond_broadcast(&f->m_thread_cleanup);
            while (f->m_nThreadsRunning > 0)
            {
                CONDWAIT(&f->m_thread_cleanup, &f->m_data_mutex);
                DPRINT("# Threads running: " << f->m_nThreadsRunning);
            }
        }

        DPRINT("Thread Cleanup, phase 2");

        ThreadInfo* ti = GetThreadInfo();
        bool cleanupSelf = ti->m_cleanupSelf;

        // Remove ourselves from the thread list
        f->m_threads.erase(ti->index);

        DPRINT("Thread Cleanup, phase 3");
        delete ti;
        DPRINT("Thread Cleanup, phase 4");
        pthread_setspecific(key_thread_info, NULL);

        // Signal that there are a thread has terminated
        pthread_cond_broadcast(&f->m_termination);

        // Signal that a thread has completed
        pthread_cond_signal(&f->m_thread_completed);

        if (f->m_nThreadsRunning < 0)
        {
            // We are a self-killer thread and we're done
            DPRINT("Killer signing off (" << (g_killers_count - 1) << " left)");
            LOCK(&g_killers_mutex);
            if (--g_killers_count == 0)
            {
                pthread_cond_signal(&g_killers_done);
            }
            UNLOCK(&g_killers_mutex);
        }

        UNLOCK(&f->m_data_mutex);

        if (cleanupSelf)
        {
            delete f;
        }

        DPRINT("Thread Cleanup, phase 5 ... exiting");
#ifdef UTC_CONFIG_NO_CANCELLATION
        // We are not called from the cleanup stack, so exit thread since we're
        // finished.
        pthread_exit(NULL);
#endif
    }

    // This thread function runs an iteration thread
    extern "C" void* FamilyBase::thread_func(void* param)
    {
#ifndef UTC_CONFIG_NO_CANCELLATION
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

// End #ifdef UTC_CONFIG_NO_CANCELLATION
#endif

        ThreadCreateInfo* tci = (ThreadCreateInfo*) param;
        FamilyBase* f = tci->f;

#ifndef UTC_CONFIG_NO_CANCELLATION
        pthread_cleanup_push(thread_cleanup, f);
// End #ifdef UTC_CONFIG_NO_CANCELLATION
#endif

        // Initialize the thread
        DisableCancellation();
        ThreadInfo* ti = f->create_thread( *tci );

        // Copy the readable Family ID to the threadinfo
        DEXEC(ti->m_id = f->m_id);

        DPRINT("Created thread #" << ti->index / f->m_step << " in " << ti->m_id);
        pthread_setspecific(key_thread_info, ti);
        f->set_last_created(ti);


        // Run the thread
        ti->run();

#ifdef UTC_CONFIG_NO_CANCELLATION
        // Run cleanup handler manually
        thread_cleanup(f);
#else
        DisableCancellation();
        // Pop cleanup handler from the cleanup stack
        pthread_cleanup_pop(1);
// End #ifdef UTC_CONFIG_NO_CANCELLATION
#endif

        return 0;
    }

    void FamilyBase::set_last_created(ThreadInfo* ti)
    {
        // Marks the thread as created
        LOCK(&ti->f->m_data_mutex);
        m_lastCreated = ti;
        m_nThreadsRunning++;
        DPRINT("Set last created: " << ti);
        m_threads.insert(make_pair(ti->index,ti));
        pthread_cond_signal(&m_created);
        UNLOCK(&ti->f->m_data_mutex);
    }

    void FamilyBase::create_iteration(int index, bool first, bool last)
    {
        ThreadCreateInfo tci;
        tci.index = index;
        tci.first = first;
        tci.last  = last;
        tci.prev  = m_lastCreated;
        tci.f     = this;

        m_lastCreated = NULL;

        pthread_t tid;
        if (pthread_create(&tid, &g_thread_attr, thread_func, &tci) != 0)
        {
            DPRINT("Unable to create a thread");
            abort();
        }

        // Wait until the thread has been created
        DPRINT("Thread created: " << tid << ": WAITING");
        while (m_lastCreated == NULL)
        {
            CONDWAIT(&m_created, &m_data_mutex);
        }
        DPRINT("Thread created: " << tid << ": CONFIRMED");
    }

    void FamilyBase::create_loop()
    {
        DPRINT(m_id << " Create loop: " << m_start << " to " << m_end << ", step " << m_step << " (block " << m_blockSize << ")");

        // Get data mutex on FamilyBase object
        LOCK(&m_data_mutex);

        // Check if there is a place associated with this family
        // and see if it has been configured for mutual exclusion
        if ((m_place != PLACE_LOCAL) && (m_place != PLACE_GROUP))
        {
            // Place is not local or group, so we have to check for
            // mutual exclusion
            if (m_place->exclusive_flag)
            {
                LOCK(&m_place->exclusion_mutex);

                // Check if place is unlocked, otherwise wait for unlock signal
                while (m_place->exclusion_lock && !m_killed)
                {
                    CONDWAIT(&m_place->exclusion_cond, &m_place->exclusion_mutex);
                }

                // When we're not killed in the meantime, lock the place
                if (!m_killed)
                {
                    // Set locked flag
                    m_place->exclusion_lock = true;

                    // Set this family as locking family
                    m_place->exclusion_f = this;
                }

                UNLOCK(&m_place->exclusion_mutex);
            }
        }

        // Check place, if it is the group place, set to parent's place if
        // we have a parent.
        if(m_place == PLACE_GROUP && m_parent != NULL)
        {
            m_place = m_parent->m_place;
        }

	if (m_step != 0)
	  while (((m_step > 0 && m_index < m_end) || (m_step < 0 && m_index > m_end))
		 && !m_killed)
	    {
	      // Wait until a spot frees up, or until we're killed
	      while (!m_killed && m_threads.size() >= m_blockSize)
		  CONDWAIT(&m_thread_completed, &m_data_mutex);
	      if (!m_killed)
                create_iteration(m_index, 
				 m_index == m_start, 
				 (m_step > 0 && m_index + m_step >= m_end) 
				 || (m_step < 0 && m_index + m_step <= m_end));
	      m_index += m_step;
	    }
	else // step = 0
	  while (!m_killed)
	    {
	      // Wait until a spot frees up, or until we're killed
	      while (!m_killed && m_threads.size() >= m_blockSize)
		  CONDWAIT(&m_thread_completed, &m_data_mutex);
	      if (!m_killed)
                create_iteration(m_index, m_index == m_start, false);
	      m_index += m_end;
	    }


        // Signal that all threads have been created
        m_allCreated = true;
        pthread_cond_broadcast(&m_termination);

        UNLOCK(&m_data_mutex);
    }

    // Static wrapper around create_loop()
    extern "C" void* FamilyBase::thread_creation_func(void* param)
    {
        FamilyBase* f = (FamilyBase*)param;

        // If it is not a continuation family the creator thread
        // will sync on it. The FamilyBase::sync() function calls
        // the FamilyBase::destroy() which in turn "deletes" the family.
        // This can happen before the creation thread running this function
        // checks after calling FamilyBase::create_loop() whether this family
        // is a continuation resulting in some cases "f" pointing to a deleted
        // object and "is_continuation()" returning the wrong value (returning true
        // for a family that is not a continuation one) resulting in segfaults and
        // deadlock (because sync() is called again). For this reason we call
        // the function is_continuation() before we create any threads for this
        // family and cache its value thus getting the correct value always (the 
        // sync() function calls destroy() which deletes the object only
        // after all threads have been created thus at this point we are sure
        // the object is valid). If it is a continuation create we do not
        // have this problem because the global sync() function (the one
        // called from the uTC program) does not call FamilyBase::sync() but 
        // returns immediately. Thus in this case the object is always valid.
        bool is_continuation = f->is_continuation();

        f->create_loop();

        
        if (is_continuation)
        {
            f->sync();
        }
        
        return 0;
    }

    // Create the creation thread
    void FamilyBase::start_creating()
    {
        pthread_t tid;
        if (pthread_create(&tid, &g_thread_attr, thread_creation_func, this) != 0)
        {
            DPRINT("Unable to create family thread");
            abort();
        }
    }

    ExitCode FamilyBase::sync()
    {
        //
        // Wait until:
        // * the creating thread has finished
        // * all threads have completed
        // * all child families have been deleted
        //
        DisableCancellation();
        DPRINT("Synching on " << m_id);

        LOCK(&m_data_mutex);
        while ((m_parent == NULL || !m_parent->m_killed) && (!m_threads.empty() || !m_allCreated || m_firstChild != NULL || m_killbusy))
        {
            CONDWAIT(&m_termination, &m_data_mutex);
            DPRINT("Sync woken up ( Parent killed? " << (m_parent == NULL || !m_parent->m_killed) << ", Threads active? " << !m_threads.empty() << ", Still Creating? " << !m_allCreated << ", Children? " << (m_firstChild != NULL) << " Kill busy? " << m_killbusy << ")");
        }

        ExitCode code = m_exitCode;
        bool killed   = (m_parent != NULL && m_parent->m_killed);

        pthread_cond_broadcast(&m_termination);
        UNLOCK(&m_data_mutex);

        if (killed)
        {
            WaitForCancellation();
        }

        destroy();
        EnableCancellation();
        return code;
    }

    // Squeezes the family
    int FamilyBase::squeeze()
    {
        // Only squeeze if the family hasn't been broken or killed yet
        LOCK(&m_data_mutex);
        if (m_exitCode == EXIT_NORMAL)
        {
            m_exitCode = EXIT_SQUEEZE;
            m_end      = min(m_end, m_index + 1);
        }
        UNLOCK(&m_data_mutex);
        return m_end + 1;
    }

    // Kills the family.
    // The family should be cleaned up (with destroy()) by whatever calls this function.
    void FamilyBase::kill(ExitCode code)
    {
        DPRINT("[KILL " << m_id << "] Killing");

        LOCK(&m_data_mutex);
        m_killed   = true;
        m_killbusy = true;
        m_exitCode = code;

        // Terminate the creating thread and wait for it.
        // Afterwards, no more threads will be created and all those that have been
        // created are registered.
        DPRINT("[KILL " << m_id << "] Waiting for creating thread");
        pthread_cond_signal(&m_thread_completed);
        while (!m_allCreated)
        {
            CONDWAIT(&m_termination, &m_data_mutex);
        }

        // Recurse the kill into child families
        // The killing of a child family will also wake up any synchs in threads of this family.
        DPRINT("[KILL " << m_id << "] Killing child families");
        for (FamilyBase* f = m_firstChild; f != NULL; f = f->m_nextSibling)
        {
            // Kill the child family
            f->kill(code);
        }

        // Terminate all threads in this family.
        // The thread-specific data destructors will clean up the ThreadInfos.
        size_t waitleft = 0;
        for (map<int,ThreadInfo*>::reverse_iterator p = m_threads.rbegin(); p != m_threads.rend(); p++)
        {
            ThreadInfo* ti = p->second;
            if (ti->m_tid == pthread_self())
            {
                DPRINT("[KILL " << m_id << "] Skipping " << ti->m_tid);
                waitleft = 1;

                // Link this family out of the child list from the parent, we will clean it up ourselves
                if (ti->m_cleanupSelf)
                {
                    unlink();
                }

                LOCK(&g_killers_mutex);
                g_killers_count++;
                UNLOCK(&g_killers_mutex);

                // Since the cleanup handler waits for this to drop to zero, we need to adjust it here
                // to force the other threads to actually terminate.
                // Once this thread terminates, it will drop to -1, but that's ok.
                m_nThreadsRunning--;
                pthread_cond_broadcast(&m_thread_cleanup);
            }
#ifndef UTC_CONFIG_NO_CANCELLATION
            else
            {
                pthread_t target = ti->m_tid;
                int retval = 0;
                DPRINT("[KILL " << m_id << "] Canceling " << target);
                retval = pthread_cancel(target);
                if (retval == ESRCH)
                {
                    DPRINT("KILL: Thread " << target << " Not found!");
                    abort();
                }
                else if(retval != 0)
                {
                    DPRINT("KILL: Error in cancellation, return value: " << retval);
                    abort();
                }
            }
#endif
            ti->kill_shareds();
        }

        // Wake up threads waiting in sync()
        pthread_cond_broadcast(&m_termination);

        // Wake up threads waiting in ProtectKill()
        LOCK(&g_kill_mutex);
        pthread_cond_broadcast(&g_kill_done);
        UNLOCK(&g_kill_mutex);

        // Wait until all threads are cleaned up
        while (m_threads.size() > waitleft)
        {
            DPRINT("[KILL " << m_id << "] Waiting for threads: " << m_threads.size() << " left (of " << waitleft << ")");
            CONDWAIT(&m_termination, &m_data_mutex);
        }

        // Clean up all child families
        while (m_firstChild != NULL)
        {
            m_firstChild->destroy(false);
        }

        m_killbusy = false;

        // Wake up threads waiting in sync()
        // to notify that kill is done with this family
        pthread_cond_broadcast(&m_termination);

        DPRINT("[KILL " << m_id << "] Done killing");
        UNLOCK(&m_data_mutex);
        //destroy();
    }


    inline bool FamilyBase::is_root()
    {
        return m_root;
    }

    inline bool FamilyBase::is_continuation()
    {
        return m_continuation;
    }
   
    FamilyBase::FamilyBase(int start, int end, int step, unsigned int blockSize, place place_id, bool root, bool nosync, int* pSqueezeVal)
        : m_start(start), m_end(end), m_step(step), m_index(start), m_root(root), m_continuation(nosync), m_place(place_id), m_pSqueezeValue(pSqueezeVal)
    {
        m_lastCreated     = NULL;
        m_firstChild      = NULL;
        m_prevSibling     = NULL;
        m_parent          = NULL;
        m_nextSibling     = NULL;
        m_nextChildId     = 0;
        m_nThreadsRunning = 0;
        m_allCreated      = false;
        m_exitCode        = EXIT_NORMAL;
        m_killed          = false;
        m_killbusy        = false;

        // Check the blocksize against the defined maximum
        if((blockSize > UTC_CONFIG_BLOCKSIZE_MAX)||(blockSize == 0))
            blockSize = UTC_CONFIG_BLOCKSIZE_MAX;

        // blockSize of 1 won't work with shareds, so we do
        // two threads at the same time.
        m_blockSize = max(2U, blockSize);

        DPRINT("Creating family");

        pthread_cond_init(&m_child_cleanup, NULL);
        pthread_cond_init(&m_thread_cleanup, NULL);
        pthread_cond_init(&m_created, NULL);
        pthread_cond_init(&m_thread_completed, NULL);
        pthread_cond_init(&m_termination, NULL);
        pthread_mutex_init(&m_data_mutex, NULL);

        // Link this family in the list of families
        ThreadInfo* ti = GetThreadInfo();
        if (ti != NULL)
        {
            // Depending on if we are a continuation create or not,
            // we need to select the right parent family
            if (!m_continuation)
            {
                m_parent = ti->f;
            }
            else
            {
                // Find parent to link to, by searching up the family tree
                // until we reach the root or a family marked as 'root'
                FamilyBase* pf = ti->f;

                while (!pf->m_root && pf->m_parent != NULL)
                {
                    pf = pf->m_parent;
                }

                m_parent = pf;
            }

            LOCK(&m_parent->m_data_mutex);

            // Attach to parent by linking child into children list
            m_nextSibling = m_parent->m_firstChild;
            if (m_nextSibling != NULL)
            {
                m_nextSibling->m_prevSibling = this;
            }
            m_parent->m_firstChild = this;

            // Build this family's friendly readable ID
            DEXEC( {
            stringstream id;
            id << m_parent->m_id << "." << m_parent->m_nextChildId++;
            m_id = id.str();
            } );

            UNLOCK(&m_parent->m_data_mutex);

            DPRINT("Created family " << m_id);
        }
    }

    void FamilyBase::unlink()
    {
        if (m_prevSibling == NULL)
        {
            if (m_parent != NULL)
            {
                m_parent->m_firstChild = m_nextSibling;
                if (m_parent->m_firstChild == NULL)
                {
                    // Signal the parent family that it has no more children
                    pthread_cond_signal(&m_parent->m_termination);
                }
            }
        }
        else
        {
            m_prevSibling->m_nextSibling = m_nextSibling;
        }

        if (m_nextSibling != NULL)
        {
            m_nextSibling->m_prevSibling = m_prevSibling;
        }
    }

    void FamilyBase::destroy(bool lock)
    {
        DPRINT("Cleaning up family " << m_id << " (" << lock << ")");

        // Verify that this family has no children
        LOCK(&m_data_mutex);
        if (m_firstChild != NULL)
        {
            fputs("Attempting to destroy a family with children\n", stderr);
            abort();
        }

        UNLOCK(&m_data_mutex);

        if (m_parent != NULL)
        {
            if (lock)
            {
                LOCK(&m_parent->m_data_mutex);
            }

            // Unlink this family from the parent's child list
            unlink();

            if (lock)
            {
                UNLOCK(&m_parent->m_data_mutex);
            }
        }
        DPRINT("- Cleaned up family " << m_id);

        ThreadInfo* ti = GetThreadInfo();
        if (ti == NULL || ti->f != this)
        {
            delete this;
        }
    }

    FamilyBase::~FamilyBase()
    {
        DPRINT("Destroyed family " << m_id);

        // As this family is destroyed, check if it was still holding
        // a place in mutual exclusion and release it if this is the case
        if(m_place != PLACE_LOCAL)
        {
            LOCK(&m_place->exclusion_mutex);

            // Is the place locked, and are we the owner?
            if(m_place->exclusion_lock && (m_place->exclusion_f == this))
            {
                // Release lock
                m_place->exclusion_lock = false;

                // Broadcast conditional to announce release of place
                pthread_cond_broadcast(&m_place->exclusion_cond);
            }

            UNLOCK(&m_place->exclusion_mutex);
        }


        DESTROY(&m_data_mutex);
        pthread_cond_destroy(&m_thread_completed);
        pthread_cond_destroy(&m_termination);
        pthread_cond_destroy(&m_created);
        pthread_cond_destroy(&m_child_cleanup);
        pthread_cond_destroy(&m_thread_cleanup);
    }

    extern "C" void lib_main(void)
    {
        init();

        DPRINT("UTC_PTL: Init complete, creating user program");

        // Create the root family
        family f;

        create(f, PLACE_LOCAL, true, false, 0, 1, 1, 0, NULL, t_main);

        sync(f);

        DPRINT("UTC_PTL: User program complete, cleaning up and exiting");

        cleanup();
    }

}
