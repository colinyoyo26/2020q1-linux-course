# [2020q1 quiz2](https://hackmd.io/@colinyoyo26/xs)
Use SSO (small string optimization) and CoW (copy on write) to optimize string, click the link for detail.

## Compile
`$ make test_cpy`

`$ make test_tok`

Append `-DCOW` flag to use COW, e.g. `$ make test_tok -DCOW`

## Result

`$ make test_cpy`

Expected result

```
init

#string
string: gggfoobarbarbarbarbarzzz
&string: 0x7f652945f020
reference count: 1

#cow1
string: 
&string: 0x602080
reference count: 1

#cow2
string: 
&string: 0x6020c0
reference count: 1

after cpy from string to cow1 & cow2

#string
string: gggfoobarbarbarbarbarzzz
&string: 0x7f652945f020
reference count: 3

#cow1
string: gggfoobarbarbarbarbarzzz
&string: 0x7f652945f020
reference count: 3

#cow2
string: gggfoobarbarbarbarbarzzz
&string: 0x7f652945f020
reference count: 3

after concat string

#string
string: (((gggfoobarbarbarbarbarzzz)))
&string: 0x7f652925e020
reference count: 1

#cow1
string: gggfoobarbarbarbarbarzzz
&string: 0x7f652945f020
reference count: 2

#cow2
string: gggfoobarbarbarbarbarzzz
&string: 0x7f652945f020
reference count: 2

after trim cow1

#string
string: (((gggfoobarbarbarbarbarzzz)))
&string: 0x7f652925e020
reference count: 1

#cow1
string: foobarbarbarbarbar
&string: 0x7f652905d020
reference count: 1

#cow2
string: gggfoobarbarbarbarbarzzz
&string: 0x7f652945f020
reference count: 1
```

`$ make test_tok`

Expected result

```
#token
HELLO W

#initial string
HELLOW fooHELLOWbarHbarfooLbarLbarObarW

foo
bar
barfoo
bar
bar
bar
```
