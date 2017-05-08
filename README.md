# OS-Principle-Experiment
对于ex5，size_t变量需注意
// PR_SIZET on Win64 = "I"
// PR_SIZET on Win32 = ""
// PR_SIZET on Linux64 = "l"
// ...
size_t u;
scanf("%" PR_SIZET "u", &u);