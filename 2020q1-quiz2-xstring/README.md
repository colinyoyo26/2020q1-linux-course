# [2020q1 quiz2](https://hackmd.io/@colinyoyo26/xs)
Use SSO (small string optimization) and CoW (copy on write) to optimize string, click the link for detail.

## Compile
`$ gcc -o test_cow -std=gnu11 test_output_cow.c xs.c`

`$ gcc -o test_tok -std=gnu11 test_output_tok.c xs.c`

## Result

`$ ./test_cow`

Expected result

```
after cpy from string to cow1 & cow2

#string
string: gggfoobarbarbarbarbarzzz
&string: 0x1735010
reference count: 3

#cow1
string: gggfoobarbarbarbarbarzzz
&string: 0x1735010
reference count: 1

#cow2
string: gggfoobarbarbarbarbarzzz
&string: 0x1735010
reference count: 1

after concat cow1

#string
string: gggfoobarbarbarbarbarzzz
&string: 0x1735010
reference count: 2

#cow1
string: (((gggfoobarbarbarbarbarzzz)))
&string: 0x1735450
reference count: 1

#cow2
string: gggfoobarbarbarbarbarzzz
&string: 0x1735010
reference count: 1

after cpy from cow1 to cow2

#string
string: gggfoobarbarbarbarbarzzz
&string: 0x1735010
reference count: 1

#cow1
string: (((gggfoobarbarbarbarbarzzz)))
&string: 0x1735450
reference count: 2

#cow2
string: (((gggfoobarbarbarbarbarzzz)))
&string: 0x1735450
reference count: 1

after trim cow1

#string
string: gggfoobarbarbarbarbarzzz
&string: 0x1735010
reference count: 1

#cow1
string: foobarbarbarbarbar
&string: 0x1735480
reference count: 1

#cow2
string: (((gggfoobarbarbarbarbarzzz)))
&string: 0x1735450
reference count: 1
```

`$ ./test_tok`

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
