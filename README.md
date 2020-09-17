# PopLua-Disassembler
C++ Disassembler for PopCap's custom Lua format

The purpose of this program is to help reverse-engineer the compiled LUC files that appear in the main.pak files of PopCap's games.

PopCap developed a custom form of Lua, and as such these files can't be run through a standard Lua disassembler/decompiler. Most significantly, the bytecode contains multiple opcodes specific to "PopLua", as it is referred to internally.

This was originally intended simply to display the disassembled operations, but I will now be working towards producing a decompiled .lua file which approximates the original source code.

The operand types for each opcode are not completely correct yet. For opcodes based on standard Lua, I set their operand types to their expected values. For opcodes unique to PopLua, I've tried to approximate what they are based on trial-and-error. At some point it will be possible to disassemble the .exe file to learn more about the game's Lua interpreter, at which point I will know with absolute certainty.
