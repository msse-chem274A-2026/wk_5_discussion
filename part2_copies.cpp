// part2_copies.cpp -- Part 2: copy constructors
//
//   make part2 && ./part2
//
// One class, two jobs:
//   (A) predict how OFTEN copies happen
//   (B) decide what a copy should CONTAIN
#include <iostream>
#include <string>

// Stand-in for a registry handing out barcodes.
static int g_next_id = 1;
static int next_id() { int id = g_next_id; g_next_id++; return id; }

class CarDesign
{
  private:
    std::string name_;
    int id_;
    int n_tests_run_;

  public:
    CarDesign(const std::string & name)
        : name_(name), id_(next_id()), n_tests_run_(0)
    {
    }

    // ===================================================================
    // PART B TASK: this copy constructor is exactly what the compiler
    // would have generated -- every member copied straight across. It
    // prints so you can count copies in Part A.
    //
    // Three decisions, one per member. Two of them are wrong:
    //   name_          the same design, so copying it is fine
    //   id_            is a copy the same design, or a new one?
    //   n_tests_run_   has this copy been to the bench?
    //
    // next_id() gives you a fresh barcode.
    // ===================================================================
    CarDesign(const CarDesign & other)
        : name_(other.name_), id_(other.id_), n_tests_run_(other.n_tests_run_)
    {
        std::cout << "  COPY " << name_ << "\n";
    }

    void rename(const std::string & name) { name_ = name; }
    void run_test() { n_tests_run_++; }

    void print() const
    {
        std::cout << "    barcode " << id_ << " | " << name_
                  << " | tests run: " << n_tests_run_ << "\n";
    }
};

static void screen_by_value(CarDesign d) { d.print(); }
void screen_by_const_ref(const CarDesign & d) { d.print(); }

int main()
{
    // ---------------------------------------------------------- PART A
    // Four snippets. For each one, predict how many COPY lines it prints,
    // THEN uncomment it, rebuild, and check. One at a time.

    std::cout << "--- 1: pass a design to a function BY VALUE ---\n";
    {
        CarDesign lead("CD19-BBz");
        screen_by_value(lead);
    }

    // std::cout << "--- 2: the same function, BY CONST REFERENCE ---\n";
    // {
    //     CarDesign lead("CD19-BBz");
    //     screen_by_const_ref(lead);
    // }

    // std::cout << "--- 3: make a second design from the first ---\n";
    // {
    //     CarDesign lead("CD19-BBz");
    //     CarDesign variant(lead);
    // }

    // std::cout << "--- 4: the same thing, written with = ---\n";
    // {
    //     CarDesign lead("CD19-BBz");
    //     CarDesign variant = lead;
    // }

    // ---------------------------------------------------------- PART B
    // Uncomment this block once you get to Part B.

    // std::cout << "\n--- Part B: the variant library ---\n";
    // {
    //     CarDesign lead("CD19-BBz");
    //     lead.run_test();
    //     lead.run_test();
    //     lead.run_test();
    //
    //     CarDesign variant(lead);
    //     variant.rename("CD19-28z");
    //
    //     lead.print();
    //     variant.print();
    // }

    return 0;
}
