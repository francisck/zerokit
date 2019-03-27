#ifndef __JUCE_THREAD_JUCEHEADER__
#define __JUCE_THREAD_JUCEHEADER__

namespace zgui
{

/**
    Encapsulates a thread.

    Subclasses derive from Thread and implement the run() method, in which they
    do their business. The thread can then be started with the startThread() method
    and controlled with various other methods.

    This class also contains some thread-related static methods, such
    as sleep(), yield(), getCurrentThreadId() etc.

    @see CriticalSection, WaitableEvent, Process, ThreadWithProgressWindow,
         MessageManagerLock
*/
class Thread
{
public:
    /**
        Creates a thread.

        When first created, the thread is not running. Use the startThread()
        method to start it.
    */
    Thread();

    /** Destructor.

        Deleting a Thread object that is running will only give the thread a
        brief opportunity to stop itself cleanly, so it's recommended that you
        should always call stopThread() with a decent timeout before deleting,
        to avoid the thread being forcibly killed (which is a Bad Thing).
    */
    virtual ~Thread();

    /** Must be implemented to perform the thread's actual code.

        Remember that the thread must regularly check the threadShouldExit()
        method whilst running, and if this returns true it should return from
        the run() method as soon as possible to avoid being forcibly killed.

        @see threadShouldExit, startThread
    */
    virtual void run() = 0;

    // Thread control functions..

    /** Starts the thread running.

        This will start the thread's run() method.
        (if it's already started, startThread() won't do anything).

        @see stopThread
    */
    void startThread();

    /** Starts the thread with a given priority.

        Launches the thread with a given priority, where 0 = lowest, 10 = highest.
        If the thread is already running, its priority will be changed.

        @see startThread, setPriority
    */
    void startThread(int priority);

    /** Attempts to stop the thread running.

        This method will cause the threadShouldExit() method to return true
        and call notify() in case the thread is currently waiting.

        Hopefully the thread will then respond to this by exiting cleanly, and
        the stopThread method will wait for a given time-period for this to
        happen.

        If the thread is stuck and fails to respond after the time-out, it gets
        forcibly killed, which is a very bad thing to happen, as it could still
        be holding locks, etc. which are needed by other parts of your program.

        @param timeOutMilliseconds  The number of milliseconds to wait for the
                                    thread to finish before killing it by force. A negative
                                    value in here will wait forever.
        @see signalThreadShouldExit, threadShouldExit, waitForThreadToExit, isThreadRunning
    */
    void stopThread(int timeOutMilliseconds);

    /** Returns true if the thread is currently active */
    bool isThreadRunning() const;

    /** Sets a flag to tell the thread it should stop.

        Calling this means that the threadShouldExit() method will then return true.
        The thread should be regularly checking this to see whether it should exit.

        If your thread makes use of wait(), you might want to call notify() after calling
        this method, to interrupt any waits that might be in progress, and allow it
        to reach a point where it can exit.

        @see threadShouldExit
        @see waitForThreadToExit
    */
    void signalThreadShouldExit();

    /** Checks whether the thread has been told to stop running.

        Threads need to check this regularly, and if it returns true, they should
        return from their run() method at the first possible opportunity.

        @see signalThreadShouldExit
    */
    inline bool threadShouldExit() const { return shouldExit; }

    /** Waits for the thread to stop.

        This will waits until isThreadRunning() is false or until a timeout expires.

        @param timeOutMilliseconds  the time to wait, in milliseconds. If this value
                                    is less than zero, it will wait forever.
        @returns    true if the thread exits, or false if the timeout expires first.
    */
    bool waitForThreadToExit(int timeOutMilliseconds) const;

    /** Changes the thread's priority.
        May return false if for some reason the priority can't be changed.

        @param priority     the new priority, in the range 0 (lowest) to 10 (highest). A priority
                            of 5 is normal.
    */
    bool setPriority(int priority);

    /** Changes the priority of the caller thread.

        Similar to setPriority(), but this static method acts on the caller thread.
        May return false if for some reason the priority can't be changed.

        @see setPriority
    */
    static bool setCurrentThreadPriority(int priority);

    /** Sets the affinity mask for the thread.

        This will only have an effect next time the thread is started - i.e. if the
        thread is already running when called, it'll have no effect.

        @see setCurrentThreadAffinityMask
    */
    void setAffinityMask(uint32_t affinityMask);

    /** Changes the affinity mask for the caller thread.

        This will change the affinity mask for the thread that calls this static method.

        @see setAffinityMask
    */
    static void setCurrentThreadAffinityMask(uint32_t affinityMask);

    // this can be called from any thread that needs to pause..
    static void __stdcall sleep(int milliseconds);

    /** Yields the calling thread's current time-slot. */
    static void __stdcall yield();

    /** Makes the thread wait for a notification.

        This puts the thread to sleep until either the timeout period expires, or
        another thread calls the notify() method to wake it up.

        A negative time-out value means that the method will wait indefinitely.

        @returns    true if the event has been signalled, false if the timeout expires.
    */
    bool wait(int timeOutMilliseconds) const;

    /** Wakes up the thread.

        If the thread has called the wait() method, this will wake it up.

        @see wait
    */
    void notifyThreads() const;

    /** Returns an id that identifies the caller thread.

        To find the ID of a particular thread object, use getThreadId().

        @returns    a unique identifier that identifies the calling thread.
        @see getThreadId
    */
    static uint32_t getCurrentThreadId();

    /** Finds the thread object that is currently running.

        Note that the main UI thread (or other non-Juce threads) don't have a Thread
        object associated with them, so this will return 0.
    */
    static Thread* getCurrentThread();

    /** Returns the ID of this thread.

        That means the ID of this thread object - not of the thread that's calling the method.

        This can change when the thread is started and stopped, and will be invalid if the
        thread's not actually running.

        @see getCurrentThreadId
    */
    uint32_t getThreadId() const throw() { return _threadId; }

private:
    void* volatile _threadHandle;
    uint32_t _threadId;
    CriticalSection startStopLock;
    WaitableEvent startSuspensionEvent, _defaultEvent;
    int threadPriority;
    uint32_t affinityMask;
    bool volatile shouldExit;

    friend void zgui_threadEntryPoint(void*);

    void launchThread();
    void closeThreadHandle();
    void killThread();
    void threadEntryPoint();
    static bool setThreadPriority(void*, int);

    ZGUI_DECLARE_NON_COPYABLE(Thread);
};

}

#endif   // __JUCE_THREAD_JUCEHEADER__
