# UB

Beta implementation of toy U lang. The agony.

## Milestones

* [x] Create a lexer
* [ ] Create a parser
  * [x] Create expression parser
    * [x] Numerical
    * [x] Stringical?
    * [x] Expectational syntax checking
    * TODO: Leading 0 or tailing zero trim.
    * TODO: Definitions of "()" and other special cases.
    * TODO: Multiple expression evaluation.
  * [ ] Create variable expression parser
    * [x] Variable eval
    * [ ] Persistent variable register

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

### Examples
Evaluation
```
> U/bin/U run -s "4^(1+1)-10"
4^(1+1)-10 = 6

> U/bin/U run -s "10^10^10"
10^10^10 = inf

> U/bin/U run -s "'hello ' * 3"
'hello ' * 3 = hello hello hello

> U/bin/U run -s "'cat' + 'dog'"
'cat' + 'dog' = catdog
```

```
> U/bin/U toks -f /path/to/file/containing/"-1+2^(9/3)*22".u
[type: 'subtract' line: 1 column: 0 file: <stdin> value: (null)]
[type: 'integer literal' line: 1 column: 1 file: <stdin> value: 1]
[type: 'add' line: 1 column: 2 file: <stdin> value: (null)]
[type: 'integer literal' line: 1 column: 3 file: <stdin> value: 2]
[type: 'exponentiate' line: 1 column: 4 file: <stdin> value: (null)]
[type: 'left parentheses' line: 1 column: 5 file: <stdin> value: (null)]
[type: 'integer literal' line: 1 column: 6 file: <stdin> value: 9]
[type: 'divide' line: 1 column: 7 file: <stdin> value: (null)]
[type: 'integer literal' line: 1 column: 8 file: <stdin> value: 3]
[type: 'right parentheses' line: 1 column: 9 file: <stdin> value: (null)]
[type: 'multiply' line: 1 column: 10 file: <stdin> value: (null)]
[type: 'integer literal' line: 1 column: 11 file: <stdin> value: 22]
```
