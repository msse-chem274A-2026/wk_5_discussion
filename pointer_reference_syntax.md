---

## Warm-up — what `&` and `*` mean

`*` and `&` each do **two different jobs**, and *where you write them* tells you
which one.

| you write | where | read it as |
|---|---|---|
| `int * p` | in a declaration, after a type | "`p` is a **pointer to** an `int`" |
| `*p` | in an expression, before a pointer | "the `int` **at** `p`" (this is called *dereferencing*) |
| `int & r = x` | in a declaration, after a type | "`r` is a **reference to** an `int` — another name for `x`" |
| `&x` | in an expression, before a variable | "the **address of** `x`" |



```cpp
int x = 5;
int * p = &x;    // "p is a pointer to an integer, set to the address of x"
int & r = x;     // "r is a reference to int: another name for x"
*p = 4;          // "the int at p becomes 4"
r = 4;           // r is just x, so no symbol needed. does the same thing as *p = 4
```


### Exercise

For each snippet: **does it compile?** If it does, what is `a` at the end? If it
does not, **is there anywhere you could add a single `*` or `&` to make it
compile?** (Sometimes the answer is no.) Predict first, then write a short
program and check.

1. ```cpp
   int a = 1;
   int * p = a;
   ```

1. ```cpp
   void bump(int n) { n = n + 1; }

   int a = 1;
   bump(a); // how can we modify this so a is increased by 1?
   ```

1. ```cpp
   int a = 1;
   int * p = &a;
   p = 4; 
   ```

1. ```cpp
   int a = 1;
   double * p = &a;
   ```

1. ```cpp
   void bump(int * n) { *n = *n + 1; }

   int a = 1;
   bump(a);
   ```

1. ```cpp
   int a = 1;
   int * p = &a;
   a = p; 
   ```

**Questions**

W1. Snippet 2 compiles, but `bump` was meant to add one to `a`. Where does the
    one `&` go to fix it? Is it the same kind of `&` as the fix for snippet 1?

W2. Snippets 3 and 6 both need a `*` in front of `p`. In one, the `*` *writes*
    through the pointer; in the other it *reads* through it. Which is which?

W3. Snippet 4 has no fix. What is wrong with it that no `*` or `&` can change?

---
