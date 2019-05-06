#ifndef CALLBACK_H
#define CALLBACK_H



/* reg_callback.h */
typedef void (*callback)(void);
void register_callback(callback ptr_reg_callback);
#endif // CALLBACK_H
