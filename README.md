# Discussion 3: Copy Constructors, References, and Class Design

**Topics:** C++ copy constructors, references and `const`, inheritance vs
composition.

Work together in your group and put your answers in the class Google Slides
presentation, on your group's page. 



## Setup

```bash
git clone https://github.com/msse-chem274A-2026/wk_5_discussion.git
cd wk_5_discussion
make check      # prints your compiler version and does a test compile
make            # builds the two demos and Part 2
```


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
compile?** (Sometimes the answer is no.) 

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

W2. Snippet 4 has no fix. What is wrong with it that no `*` or `&` can change?


## Part 1 — References and `const`

When you ask an object for one of its members, you can get back **a copy** of
it, or **the member itself**. Which one you get changes whether writing to it
changes the object.

```cpp
class CarDesign
{
   private:
      std::string binder_;            // which binder this design uses

   public:
      CarDesign(const std::string & b) : binder_(b) {}

      std::string        get_binder_1()       { return binder_; }  // a COPY
      std::string &      get_binder_2()       { return binder_; }  // the MEMBER ITSELF
      const std::string & get_binder_3() const { return binder_; } // the member, READ-ONLY
};
```

The `&` is what makes the difference. `get_binder_1` hands you a copy; change
it and the design is untouched. `get_binder_2` hands you the member itself;
change it and you have changed the design. `get_binder_3` hands you the member
but marks it `const`, so you may read it and not write it.

### Two places `const` shows up

```cpp
const std::string & get_binder_3() const { return binder_; }
//  ^^^^^ (1)                      ^^^^^ (2)
```

- **Before the type** — you may not change *what is handed back*.
- **After the parentheses** — this function does not change *the object it was
  called on*. Only functions marked this way can be called on a `const` object.

### Exercise

Each snippet tries to change the design's binder from FMC63 to SJ25C1.

**For each one: does it compile? And is `binder_` FMC63 or SJ25C1 at the end?**
Predict first, then write a short program and check.

```cpp
CarDesign d("FMC63");                    // 1
std::string s = d.get_binder_1();
s = "SJ25C1";
```
```cpp
CarDesign d("FMC63");                    // 2
d.get_binder_2() = "SJ25C1";
```
```cpp
CarDesign d("FMC63");                    // 3
d.get_binder_3() = "SJ25C1";
```
```cpp
const CarDesign d("FMC63");              // 4
d.get_binder_2() = "SJ25C1";
```

**Questions**

1. Only one of the four actually changed the binder. Which one, and what was
   different about it?
2. Snippets 3 and 4 both fail, but for different reasons. What is each one's
   reason?

---

## Part 2 — Copy constructors

**File:** `part2_copies.cpp` · **Build:** `make part2 && ./part2`

A **copy constructor** runs whenever a new object is made from an existing one.
The `CarDesign` class in this file prints `COPY` every time it runs, so you can
see exactly when that happens.

### When does a copy happen?

`main` has four snippets; only the first is uncommented. For each one,
**predict how many `COPY` lines it prints**, then uncomment it, rebuild, and
check. One at a time.

**Questions**

3. Snippets 3 and 4 look different — one uses `(lead)` and one uses `= lead` —
   but both print a `COPY`. Why are they the same thing?
4. `CarDesign` holds no pointers and allocates no memory, and the compiler's
   copy was *still* wrong. So "write a copy constructor when the class owns
   memory" is not the whole rule. What is the fuller version?

---
