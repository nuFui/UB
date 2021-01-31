# UB

Beta implementation of toy U lang. The agony.

Based on: https://github.com/davidcallanan/py-myopl-code

## Milestones

* [x] Create a lexer
* [ ] Create a parser
  * [x] Create expression parser & evaluation
  * [ ] Create variable expression parser & evalation

## Usage

First navigate to root directory UB.
Type `make`.
Then:
```
U <command> <option> ...
```
or
```
U/bin/U <command> <option> ...
```

Commands supported: 
* `help` for help.
* `run` to run/evaluate program.
* `toks` to view tokens produced by program.
* `repl`  interactive shell.

Options supported:
* `-s` followed by string (program).
* `-f` followed by path to program.