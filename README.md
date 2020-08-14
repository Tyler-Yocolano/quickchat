# quickchat
Have you ever wanted to trash talk people in Rocket League, but didn't want to type on your keyboard?  Is a simple quick chat option not enough?  Well now you can, using only a cryptic series of quick chat options! It might take a while, and they might never get what you were doing, but you'll know.  And that's all that matters.

## Getting Started
Either download the 'quickchat.exe' from the releases or build from source using CMake.  Then, run ```.\quickchat <source_file>.qc``` in the command-line.  There is an REPL, but loops don't work yet.

## Language
This is based on brainfuck so all the same commands are here, plus a few extra. In quickchat, multiple tapes can exist. Therefore, all commands require the name of the tape to act on.

### Commands
| Command | brainfuck | Description |
| ------- | --------- | ----------- |
| \<NAME>: I got it! | > | Increment tape pointer. |
| \<NAME>: Defending... | < | Decrement tape pointer. |
| \<NAME>: Nice shot! | + | Increment value at pointer. |
| \<NAME>: No problem. | - | Decrement value at pointer. |
| \<NAME>: Take the shot! | \[ | Begin loop if value at tape != 0. |
| \<NAME>: What a save! | ] | End loop, jump back to beginning. |
| \<NAME>: Incoming! | , | Accept one byte of input. |
| \<NAME>: Calculated. | . | Output one byte. |
| \<NAME>: Great pass! |   | Copy the current value from the tape indexed at the current value's position. |
| \<Name> joined the match |   | Create a new tape with the name <NAME>. |
| \<Name> left the match |   | Delete the tape with the name <NAME>. |

### Syntax
Each line must start with an identifier, followed by either a join command, a leave command, or a command. Commands are seperated by newlines.

1. \<Name><SINGLE_SPACE><JOINED\|\|LEFT>\\n
2. \<NAME>:<SINGLE_SPACE>\<COMMAND>\\n

There's no way to comment the code because I believe code should be self commenting.  See the 'Hello World' example below for instance.

## Hello World!
```
Punctuation joined the match
Capitals joined the match
Lowercase joined the match
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
CAPITALS: Great pass!
CAPITALS: Nice shot!
LOWERCASE: Great pass!
LOWERCASE: Nice shot!
PUNCTUATION: Take the shot!
PUNCTUATION: No problem.
PUNCTUATION: I got it!
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
CAPITALS: Take the shot!
CAPITALS: No problem.
CAPITALS: I got it!
CAPITALS: Great pass!
CAPITALS: Take the shot!
CAPITALS: No problem.
CAPITALS: I got it!
CAPITALS: Nice shot!
CAPITALS: Nice shot!
CAPITALS: Defending...
CAPITALS: What a save!
CAPITALS: Defending...
CAPITALS: What a save!
LOWERCASE: Take the shot!
LOWERCASE: No problem.
LOWERCASE: I got it!
LOWERCASE: Great pass!
LOWERCASE: Take the shot!
LOWERCASE: No problem.
LOWERCASE: I got it!
LOWERCASE: Nice shot!
LOWERCASE: Nice shot!
LOWERCASE: Nice shot!
LOWERCASE: Defending...
LOWERCASE: What a save!
LOWERCASE: Defending...
LOWERCASE: What a save!
PUNCTUATION: Defending...
PUNCTUATION: What a save!
PUNCTUATION: I got it!
CAPITALS: I got it!
CAPITALS: I got it!
LOWERCASE: I got it!
LOWERCASE: I got it!
CAPITALS: Nice shot!
CAPITALS: Nice shot!
CAPITALS: Calculated.
helper joined the match
HELPER: Nice shot!
HELPER: Nice shot!
HELPER: Great pass!
LOWERCASE: No problem.
LOWERCASE: No problem.
LOWERCASE: No problem.
LOWERCASE: No problem.
LOWERCASE: Calculated.
HELPER: Nice shot!
HELPER: Nice shot!
HELPER: Nice shot!
HELPER: Calculated.
HELPER: Calculated.
The_L joined the match
THE_L: Nice shot!
THE_L: Nice shot!
THE_L: Nice shot!
THE_L: Great pass!
HELPER: Nice shot!
HELPER: Nice shot!
HELPER: Nice shot!
HELPER: Calculated.
PUNCTUATION: Nice shot!
PUNCTUATION: Nice shot!
PUNCTUATION: Calculated.
CAPITALS: I got it!
CAPITALS: Great pass!
CAPITALS: Take the shot!
CAPITALS: No problem.
CAPITALS: No problem.
CAPITALS: Defending...
CAPITALS: Nice shot!
CAPITALS: I got it!
CAPITALS: What a save!
CAPITALS: Defending...
CAPITALS: No problem.
CAPITALS: Calculated.
HELPER: Calculated.
HELPER: Nice shot!
HELPER: Nice shot!
HELPER: Nice shot!
HELPER: Calculated.
helper left the match
THE_L: Calculated.
The_L left the match
LOWERCASE: No problem.
LOWERCASE: Calculated.
PUNCTUATION: Nice shot!
PUNCTUATION: Calculated.
Capitals left the match
Lowercase left the match
Punctuation left the match
```
Output: Hello World!

## References
Crafting Interpreters: https://craftinginterpreters.com/

C++ clox implementation: https://github.com/hashemi/cloxpp/tree/master/cloxpp

I built this to learn about interpreters/compilers and C++.
