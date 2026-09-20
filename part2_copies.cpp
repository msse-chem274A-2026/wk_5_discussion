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
CarDesign make_variant() { CarDesign d("CD19-28z"); return d; }

int main()
{
    // ---------------------------------------------------------- PART A
    // Predict how many COPY lines each snippet prints, THEN uncomment it,
    // rebuild, and check. One at a time.

    std::cout << "--- snippet 1: screen a design, by value ---\n";
    { CarDesign lead("CD19-BBz"); screen_by_value(lead); }

    // std::cout << "--- snippet 2: the same, by const reference ---\n";
    // { CarDesign lead("CD19-BBz"); screen_by_const_ref(lead); }

    // std::cout << "--- snippet 3: make a variant from the lead ---\n";
    // { CarDesign lead("CD19-BBz"); CarDesign variant(lead); }

    // std::cout << "--- snippet 4: written with = instead ---\n";
    // { CarDesign lead("CD19-BBz"); CarDesign variant = lead; }

    // std::cout << "--- snippet 5: just another name for the lead ---\n";
    // { CarDesign lead("CD19-BBz"); const CarDesign & same = lead; same.print(); }

    // std::cout << "--- snippet 6: a factory function ---\n";
    // { CarDesign variant = make_variant(); }

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
