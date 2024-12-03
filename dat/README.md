# DAT and Lua files

This folder contains the DAT and Lua files used in the plugin.
The Lua files are extracted from the DAT so that they can be shown with GitHub's Lua syntax highlighting.

## cori's_message_box.lua

This Lua file triggers an in-game message box with the specified LTEXT file contents. It is based on a Lua 
file provided by CorinaMarie at Simtropolis.

## lua_extensions_example.lua

This file contains the function stubs that the DLL will replace with a native C++ function. The functions
are in a Lua table whose name is known to the DLL.
