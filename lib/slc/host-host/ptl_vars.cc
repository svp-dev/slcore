// ptl_vars.cc: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//


#include "ptl_vars.h"

using namespace std;

namespace uTC
{

#ifdef UTC_CONFIG_PTHREAD_CONDITIONAL


// pthread_cond_t states
enum
{
    STATE_INVALID,      // Conditional is invalid
    STATE_IDLE,         // Conditional has not been signaled or broadcasted
    STATE_SIGNALED,     // Conditional has been signaled
    STATE_BROADCASTED,  // Conditional has been broadcasted
};

// This structure is the thread's 'link' in a conditional's waiting list.
// It's placed on the thread stack during pthread_cond_wait and holds a pointer
// to the next thread's link.
struct pthread_link_t
{
    pthread_link_t* next;
};

int pthread_cond_init(pthread_cond_t* cond, void* unused)
{
    // Initialize our conditional to idle, and no waiting threads
    cond->state = STATE_IDLE;
    cond->first = NULL;
    return 0;
}

int pthread_cond_destroy(pthread_cond_t* cond)
{
    // Check if there are no more threads waiting on the conditional
    if(cond->first == NULL)
    {
        // Some value to indicate that it is invalid now
        cond->state = STATE_INVALID;
    }
    else    
        return EBUSY;

    return 0;
}

int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex)
{
    DPRINT ("COND WAIT: " << cond );
    
    if(cond->state == STATE_INVALID)
    {
        return EINVAL;
    }
    
    // Check if a broadcast is in progress
    if(cond->state == STATE_BROADCASTED)
    {
        UNLOCK(mutex);
        UTC_YIELD();
        LOCK(mutex);
        // Return as we act as if we received the broadcast
        return 0;
    }

    // Add this thread to the back of the linked list of waiting threads
    pthread_link_t link = { NULL };
    if (cond->first != NULL)
    {
        // There were already threads waiting on this conditional,
        // point the last one's 'next' to us
        cond->last->next = &link;
    }
    else
    {
        // We're the first thread waiting on this conditional
        cond->first = &link;
    }
    cond->last = &link; // We're the last thread now

    // Busy-waiting loop that waits for a conditional signal
    while(1)
    {
        if (cond->state == STATE_INVALID)
        {
            // How did this happen? The conditional became invalid
            // while were waiting on it!
            return EINVAL;
        }
    
        if (cond->state != STATE_IDLE)
        {
            // Broadcast or signal has happened, check if we're the first thread
            if (cond->first == &link)
            {
                // Yes, set the next thread as first and exit the wait loop
                cond->first = link.next;
                break;
            }
        }
        
        // Release the lock and yield to scheduler
        UNLOCK(mutex);
        UTC_YIELD();
        
        // Try to aquire the lock again
        LOCK(mutex);
        DPRINT ("COND - BUSY WAIT: " << cond);
    }
    
    if (cond->state == STATE_SIGNALED || cond->first == NULL)
    {
        // Reset conditional state when it's been signaled, or (when
        // broadcasted) when there are no more waiting threads
        cond->state = STATE_IDLE;
    }
    
    return 0;
}

int pthread_cond_signal(pthread_cond_t* cond)
{
    // We assume we have the mutex lock already, as done in the lib

    // Mark the conditional as broadcasted if there are threads
    // waiting on it
    if(cond->first != NULL)
        cond->state = STATE_SIGNALED;
    DPRINT ("COND ==> SIGNAL: " << cond );
    return 0;
}

int pthread_cond_broadcast(pthread_cond_t* cond)
{
    // We assume we have the mutex lock already, as done in the lib

    // Mark the conditional as signaled if there are threads
    // waiting on it
    if(cond->first != NULL)
        cond->state = STATE_BROADCASTED;
    DPRINT ("COND ==> BCAST: " << cond );
    return 0;
}
// End #ifdef UTC_CONFIG_PTHREAD_CONDITIONAL
#endif

#ifdef UTC_CONFIG_PTHREAD_TSD

// Global mutex to protect map
static pthread_mutex_t pthread_key_mutex = PTHREAD_MUTEX_INITIALIZER;

int pthread_key_create(pthread_key_t *key, void (*destr_function) (void *))
{
     // Key and destructor function are ignored, we only support one key
     *key = new pthread_key_t_map;
     return 0;
}

int pthread_key_delete(pthread_key_t key)
{
    delete key;
    return 0;
}

void* pthread_getspecific(pthread_key_t key)
{
    void* data = NULL;
    DPRINT("pthread_getspecific enter");
    LOCK(&pthread_key_mutex);
    pthread_key_t_map::iterator iter = key->find(pthread_self());
    if(iter != key->end())
    {
        data = iter->second;
    }    
    UNLOCK(&pthread_key_mutex);
    DPRINT("pthread_getspecific exit");
    return data;
}

int pthread_setspecific(pthread_key_t key, void *pointer)
{
    LOCK(&pthread_key_mutex);
    (*key)[pthread_self()] = pointer;
    UNLOCK(&pthread_key_mutex);
    return 0;
}

// End #ifdef UTC_CONFIG_PTHREAD_TSD
#endif

// End namespace uTC
}
