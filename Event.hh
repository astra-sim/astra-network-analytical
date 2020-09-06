#ifndef __EVENT_HH__
#define __EVENT_HH__

struct Event {
public:
    /**
     * Construct new event.
     * @param fun_ptr pointer to event handler
     * @param fun_arg pointer to event handler argument
     */
    Event(void (*fun_ptr)(void *), void *fun_arg) noexcept : fun_ptr(fun_ptr), fun_arg(fun_arg) { };

    /**
     * Run the event handler.
     */
    void run() const noexcept;

private:
    /**
     * function pointer directing the event handler
     */
    void (*fun_ptr)(void *);

    /**
     * pointer to the event handler argument.
     */
    void *fun_arg;
};

#endif
