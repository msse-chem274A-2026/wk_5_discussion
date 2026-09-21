// part2_copies.cpp -- Part 2: copy constructors
//
//   make part2 && ./part2
//
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

  public:
    CarDesign(const std::string & name)
        : name_(name), id_(next_id())
    {}

    // Copy constructos -- prints a message every time it's called
    CarDesign(const CarDesign & other)
        : name_(other.name_), id_(other.id_)
    {
        std::cout << "    I COPIED " << name_ << "!!" << "\n";
    }

    void print() const
    {
        std::cout << "    barcode " << id_ << " | " << name_ << "\n";
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

    std::cout << "\n--- snippet 1: screen a design, by value ---\n";
    { CarDesign lead("CD19-BBz"); screen_by_value(lead); }

    // std::cout << "\n--- snippet 2: the same, by const reference ---\n";
    // { CarDesign lead("CD19-BBz"); screen_by_const_ref(lead); }

    // std::cout << "\n--- snippet 3: make a variant from the lead ---\n";
    // { CarDesign lead("CD19-BBz"); CarDesign variant(lead); lead.print(); variant.print(); }

    // std::cout << "\n--- snippet 4: written with = instead ---\n";
    // { CarDesign lead("CD19-BBz"); CarDesign variant = lead; lead.print(); variant.print(); }

    // std::cout << "\n--- snippet 5: just another name for the lead ---\n";
    // { CarDesign lead("CD19-BBz"); const CarDesign & same = lead; same.print(); }

    // std::cout << "\n--- snippet 6: a factory function ---\n";
    // { CarDesign variant = make_variant(); variant.print();}

    return 0;
}
