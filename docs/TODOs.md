- [ ] C++ try-catch blocks only handle C++ exceptions. Errors like segmentation faults are lower-level, 
    and try-catch ignores these events and behaves the same as if there was no try-catch block. If master 
    process encounters segmentation fault, the server will not be able to restart itself again.
