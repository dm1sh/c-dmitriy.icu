The idea of creating my own virtual machine came to my mind a year ago. Then I didn't have enough skills and desire. But now, when I have my final exams in a month and a half, I started working on this project.

## Overview

It is a stack-based virtual machine that runs simple byte-code. Currently, it supports basic stack operations (push, pop), arithmetic operations, input-output, and strings. Assembly language is a sequence of commands with/(or without) parameters. Assembly language compiler and runner are combined to one binary with *build* and *run* commands resp.

Every command is represented as 4 bytes number. It can have arguments each of which is a 4 bytes number.

Every string is initialized with zero number in the stack. Currently, the only command using this structure is *OUTS*. It puts characters from stack to console until it reaches zero — end of the string.

## File structure

### Header

Total number of commands - 4 bytes

### Body

A sequence of command structures:

### Command

Command code - 4 bytes

(Optional) Arguments - 4 bytes per each

## Plans

It is just a test stand. After exams, I'm planning to dramatically refactor VM code, change bytecode structure by decreasing command size, and adding string declaration in the header. I'm also going to add conditions and jumps for loop support.

Also, there are plans for separating compiler and VM core. If I will read find information about bootloader development, I will also try to create a bootloader for this VM to run it natively.
