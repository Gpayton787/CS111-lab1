## UID: 306032767

## Pipe Up

This program chains together programs using the standard output of one as the standard output of the next.

## Building

`make`

Run make in the terminal to build.

## Running

Ex:

`./pipe ls cat wc`

Calling pipe with these arguments will run them each sequentially. Thus you should expect "ls" to list the directories which is then passed to "cat" who's output is then passed to "wc". The final output should the standard output of wc. 

## Cleaning up

`make clean`

Run make clean to get rid of executable files.
