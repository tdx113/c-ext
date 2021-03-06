#ifndef CONTEXT_H
#define CONTEXT_H

#include "asm_context.h"

typedef fcontext_t coroutine_context_t;
typedef void (*coroutine_func_t)(void*);

namespace lib
{
    class Context
    {
    public:
        Context(size_t stack_size,coroutine_func_t fn,void *private_data);
        ~Context();
        static void context_func(void *arg);
        inline bool is_end()
        {
            return end_;
        }
        //让出携程上下文
        bool swap_out();
        bool swap_in();

        coroutine_context_t ctx;
        coroutine_context_t swap_ctx_;
        coroutine_func_t fn_;
        char* stack_;
        uint32_t stack_size_;
        void *private_data_;
        coroutine_context_t ctx_;
    protected:
        bool end_ = false;

    };
}




#endif // !1CONTEXT