# Discussion 5 — Copy Constructors, References, and Class Design

**Topics:** C++ copy constructors, references and `const`, inheritance vs
composition.

Work together in your group and put your answers in the class Google Slides
presentation, on your group's page. Make sure everyone understands the code.

## Setup

```bash
git clone https://github.com/msse-chem274A-2026/wk_5_discussion.git
cd wk_5_discussion
make check      # prints your compiler version and does a test compile
make            # builds the two demos and Part 2
```

Nothing is submitted from this repo. Everything is C++17 and the standard
library — nothing to install beyond what you set up in week 1.

---

## Part 1 — References, `const`, and aliasing

Two variables are **aliased** when they refer to the same memory: change one
and the other changes too.

```cpp
double a = 10;
double & b = a;   // b and a are aliased
```

`const` appears in two positions and they protect two different things:

```cpp
const std::string & get_binder_2() const;
//  ^^^^^                          ^^^^^
//  (1) don't modify what           (2) don't modify the object
//      is returned                     this was called on
```

A `const` object may only call functions carrying the second kind.

Here is a CAR design with three ways of getting at the same member:

```cpp
class CarDesign
{
   private:
      std::string binder_;   // which scFv this design uses

   public:
      CarDesign(const std::string & b) : binder_(b) {}

      std::string         get_binder_1() const { return binder_; }  // by value
      const std::string & get_binder_2() const { return binder_; }  // const reference
      std::string &       get_binder_3()       { return binder_; }  // reference
};
```

**Exercise.** Each snippet tries to swap the binder from FMC63 to SJ25C1. Will
it compile? If it does, what is `binder_` at the end, and which variables are
aliased? Predict first, then write a short program and check.

```cpp
CarDesign d("FMC63");                     // 1
std::string s = d.get_binder_1();
s = "SJ25C1";
```
```cpp
CarDesign d("FMC63");                     // 2
const std::string & s = d.get_binder_2();
s = "SJ25C1";
```
```cpp
CarDesign d("FMC63");                     // 3
d.get_binder_3() = "SJ25C1";
```
```cpp
CarDesign d("FMC63");                     // 4
d.get_binder_1() = "SJ25C1";
```
```cpp
const CarDesign d("FMC63");               // 5
std::string s = d.get_binder_2();
```
```cpp
const CarDesign d("FMC63");               // 6
d.get_binder_3() = "SJ25C1";
```

**Questions**

1. Exactly one snippet actually swapped the binder. Which one — and what
   happened in the ones that *looked* like they should have?
2. Explain what the `const` after the parameter list does, and why it is not
   the same thing as the `const` in `const std::string &`.

---

## Part 2 — Copy constructors

**File:** `part2_copies.cpp` · **Build:** `make part2 && ./part2`

### Part A — how often do copies happen?

The `CarDesign` class in this file prints a line every time it is copied.
`main` has six snippets; only the first is uncommented. For each one,
**predict how many `COPY` lines it prints**, then uncomment it, rebuild, and
check.

### Part B — what should a copy contain?

Uncomment the Part B block at the bottom of `main`. A lead design has been
through three tests; you make a variant by copying it and renaming.

Run it and read the table. Then fix the copy constructor — there is a marked
task in the file and three decisions, one per member.

### For reference

Last week you read Psi4's `ShellInfo`, which has **no** copy constructor and
**no** destructor. Compare it with `Matrix` from the same library, which
declares both:

- [`gshell.h`](https://github.com/psi4/psi4/blob/1fb8968fe95e6f8c3c17b94ddd3b277e83965d67/psi4/src/psi4/libmints/gshell.h#L56) — members are `std::vector<double>`
- [`matrix.h`](https://github.com/psi4/psi4/blob/1fb8968fe95e6f8c3c17b94ddd3b277e83965d67/psi4/src/psi4/libmints/matrix.h#L100) — member is `double*** matrix_`

**Questions**

3. Snippet 6 returns a `CarDesign` from a function by value. How many copies
   did you predict, and how many did you get? What does that say about the
   advice "returning by value is expensive"?
4. What was wrong with the compiler-generated copy in Part B? It compiled
   without a single warning — what does that suggest about this kind of bug?
5. `Matrix` needs a copy constructor because it owns memory. `CarDesign` owns
   no memory at all and *still* needed one. So "does it own memory?" is not
   the whole rule — what is the fuller version?

---

## Part 3 — Inheritance or composition?

Discussion only — no code.

### The two relationships

Module 2.3 gave you these:

- **is-a** — *inheritance*. B is a kind of A, so a B can stand in wherever an A
  is expected.
- **has-a** — *composition*. A holds a B and hands part of its job to it.
- …and sometimes **neither**, which is worth being able to spot.

### Worked examples

These three are from module 2.3, so you have met them before. Note the shape of
the justification — that is what we want for each of yours.

| relationship | answer | why |
|---|---|---|
| a SQLAlchemy `User` / `Base` | **is-a** | `User` *is* a SQLAlchemy model. Inheriting from `Base` is what makes it one — here the inheritance *is* the API. |
| a scikit-learn `Pipeline` / a `StandardScaler` | **has-a** | A pipeline is not a kind of scaler. It *holds* one and calls it before the model. Swap the scaler and the pipeline still works. |
| a design / the design it was copied from | **neither** | That is *lineage* — a fact you record about where it came from. Not a part it contains, not a type it specializes. |

**The test:** ask whether the second thing is **what the first thing is**, or
**something the first thing holds**. If it is neither, say so.

### The setup for your four

A screening protocol does two things: it **rejects** any design whose off-target
risk is above a threshold, and it **scores** the ones that survive. Those two
vary *independently*:

- three risk thresholds — strict, standard, permissive
- three potency models — 4-1BB-weighted, CD28-weighted, shape-matched

"Conservative" is strict + 4-1BB. "Aggressive" is standard + CD28. Any threshold
can pair with any model.

**Question**

6. Same job as the worked examples, for these four. One of them is genuinely
   arguable — say which, and give both readings before you pick one.

   | | |
   |---|---|
   | a 2nd-generation design | a design |
   | a design | a booster part |
   | a `ScreeningProtocol` | a `SafetyFilter` |
   | a "conservative" protocol | a `ScreeningProtocol` |

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
