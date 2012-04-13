#ifndef UTILITY_H__
#define UTILITY_H__

#define INFINITY_INT    ((int)(~(unsigned int)0 >> 1))

// oop
#define SUPER_MEMBER        __super
#define SUPER_PTR(tp)       (&(tp->__super))
#define SUPER(t)            (t.__super)

#define UNUSED(p) (void)(p)

#endif