# Discussion 5 — Copy Constructors, References, and Class Design

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

### Part A — when does a copy happen?

`main` has four snippets; only the first is uncommented. For each one,
**predict how many `COPY` lines it prints**, then uncomment it, rebuild, and
check. One at a time.

### Part B — what should a copy contain?

Uncomment the Part B block at the bottom of `main`. A lead design has been
through three tests, and you make a variant by copying it and renaming it.

Run it and look at the table it prints. Then fix the copy constructor — there
is a marked task in the file with three decisions, one per member.

**Questions**

3. Snippets 3 and 4 look different — one uses `(lead)` and one uses `= lead` —
   but both print a `COPY`. Why are they the same thing?
4. In Part B, what was wrong with the copy the compiler wrote for you? It
   compiled without a single warning — what does that tell you about this kind
   of bug?
5. `CarDesign` holds no pointers and allocates no memory, and the compiler's
   copy was *still* wrong. So "write a copy constructor when the class owns
   memory" is not the whole rule. What is the fuller version?

---

## Part 3 — Inheritance or composition?

Discussion only — no code to write.

When you build a class out of another class, there are two ways to do it, and
English tells you which one you want.

### The two relationships

**is-a** — *inheritance*. One thing is a **kind of** another thing.

> A dog **is a** kind of animal.
>
> ```cpp
> class Dog : public Animal { ... };    // Dog inherits from Animal
> ```

**has-a** — *composition*. One thing **contains** another thing.

> A car **has an** engine. A car is not a kind of engine.
>
> ```cpp
> class Car { Engine engine_; ... };    // Engine is a member of Car
> ```

### How to tell them apart

Say the two words out loud with "is a" between them, then with "has a".
Whichever one sounds like English is your answer.

- "A dog is a animal" ✓ &nbsp;&nbsp; "A dog has a animal" ✗ → **is-a**
- "A car is a engine" ✗ &nbsp;&nbsp; "A car has a engine" ✓ → **has-a**

And that tells you what to write:

| | you write |
|---|---|
| **is-a** | a subclass — `class Dog : public Animal` |
| **has-a** | a member variable — `Engine engine_;` inside `Car` |

**Question**

6. For each pair, say whether it is **is-a** or **has-a**, and then say what you
   would write in C++ — a subclass, or a member variable?

   | | |
   |---|---|
   | a student | a person |
   | a house | a door |
   | a 2nd-generation design | a design |
   | a screening protocol | a safety filter |

---

## Troubleshooting

**`g++: No such file or directory`** — on macOS `g++` is usually wired to
clang and works; if not, `make CXX=clang++`. On Linux/WSL install it the same
way you did in week 1. On Windows, work inside WSL.

## What is in this repo

| File | |
|---|---|
| `part2_copies.cpp` | **Part 2** — you edit this |
| `demo1_cost.cpp`, `demo2_refs_pointers.cpp` | the two demos from the front of the session — rerun them any time |
