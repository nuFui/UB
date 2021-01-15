# Overview of U language (also plan)

## Variables

### Declaration
`var x int`
### Initialization
`var x int = 3`
or if declared,
`x = 3`
or in general,
`var var_name var_type = identifier_or_literal`

## Injections
### Single injection
`inj "module.u"`

### Multiple injection 
<code> 
inj { 
  "alpha.u",
  "beta.u",
  "gamma.u"
}
</code>

## Procedures
### Syntax
<code>
pro procedure_name(procedure_args) return_type {
  body
}
</code>

## Deject keyword
Dejected variables and function are injected when using "inj".
A.u
<code>
dej var count_dots int = 10
var file_name str = "path/to/file"
</code>

B.u
<code>
inj "A.u"
// Only count_dots is visible here as file_name is local to A.u
</code>

