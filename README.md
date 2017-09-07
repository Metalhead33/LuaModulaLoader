# Lua Extension Loader

This allows you to create libraries in C or C++ (functions you want to register must have C linkage, no name-mangling), dynamically load them and register their functions in Lua.

Needless to say, this means that the libraries themselves must depend on Lua, and must be functions that return an integer (the number of values the function will return to Lua), and take a pointer to a Lua state as their one and only argument.

So far, it only works on UNIX-based systems, and I only tested it on Gentoo Linux in an x86_64 environment. In the future, I will extend it to also support Windows. 

The newly added Lua functions:
* ```extend.new( <path to library, absolute or relative>, <table containing all the function names> )``` - loads the library and prepares the functions for regstering. Does not register them yet. It returns a table whose metatable is userdata: to be specific, a pointer to a pointer: a pointer to a pointer to module struct.
* ```<library>:debugInfo()``` - prints some debug information
* ```<library>:register()``` - registers the functions stored within the struct that is being pointed at by the pointer that the userdata is pointing at. Returns a table with all the functions, therefore it is imperative that you use it in the format ```<new library name> = <library>:register()```
* ```<library>:metaRegister( <new metatable's name> )``` - similar to the previous one, except that it stores the functions within a metatable, whose name is given in an argument. Does not return anything.
