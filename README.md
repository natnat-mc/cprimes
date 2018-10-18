# CPrimes
A prime calculator, in C.

## Its goals
- None, really
- Have a variable amount of threads running at the same time (not yet)
- Eventually run with CUDA

## Its problems
- It indexes files with `int` instead of `off_t` for now, so it's limited to 2G
- It uses POSIX functions, so it needs POSIX compatibility (but honestly, you should be ok unless you run Windows)