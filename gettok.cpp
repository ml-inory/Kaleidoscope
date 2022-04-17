// # Compute the x'th fibonacci number.
// def fib(x)
//   if x < 3 then
//     1
//   else
//     fib(x-1)+fib(x-2)

// # This expression will compute the 40th number.
// fib(40)

#include <string>

enum Token {
    tok_eof =   -1,

    // commands
    tok_def =   -2,
    tok_extern  =   -3,

    // primary
    tok_identifier  =   -4,
    tok_number  =   -5,
}