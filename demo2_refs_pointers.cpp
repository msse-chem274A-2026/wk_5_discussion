// demo2_refs_pointers.cpp -- Demo 2: reference or pointer?
//
//   make demo2 && ./demo2
//
// Two points, and a short warning at the end:
//   (a) why the booster part is a pointer and not a reference
//   (b) why copying a design must NOT copy the parts registry
//   (c) 30 seconds on a reference that outlives its object
#include <iostream>
#include <string>

// One protein part from the registry. Prints so you can see copies.
class Domain
{
  private:
    std::string name_;

  public:
    Domain(const std::string & name) : name_(name)
    { std::cout << "    ctor Domain " << name_ << "\n"; }

    Domain(const Domain & other) : name_(other.name_)
    { std::cout << "    COPY Domain " << name_ << "\n"; }

    ~Domain() { std::cout << "    dtor Domain " << name_ << "\n"; }

    const std::string & get_name() const { return name_; }
};

// One CAR design. It does not OWN its parts -- the registry does. It points
// at them. costim_ is a pointer because a 1st-generation design has no
// booster part at all, and a reference cannot be absent.
class CarConstruct
{
  private:
    std::string nickname_;
    const Domain * binder_;
    const Domain * costim_;   // nullptr for a 1st-generation design

  public:
    CarConstruct(const std::string & nickname, const Domain & binder, const Domain * costim)
        : nickname_(nickname), binder_(&binder), costim_(costim)
    {
    }

    const Domain * get_costim() const { return costim_; }
    void set_costim(const Domain * costim) { costim_ = costim; }

    int get_generation() const { return (costim_ == nullptr) ? 1 : 2; }

    void print() const
    {
        std::cout << "    " << nickname_ << "  gen " << get_generation()
                  << "  binder " << binder_->get_name()
                  << "  booster "
                  << (costim_ == nullptr ? std::string("(none)") : costim_->get_name()) << "\n";
    }
};

// Returning a reference to a local. The compiler warns; it still compiles.
static const Domain & make_scratch_BROKEN()
{
    Domain scratch("scratch");
    return scratch;                 // destroyed at this brace
}

// The fix: return by value. Not expensive -- the result is built in place.
static Domain make_scratch_FIXED()
{
    Domain scratch("scratch");
    return scratch;
}

int main()
{
    std::cout << "=== (a) a part that may not exist ===\n\n  parts registry:\n";
    Domain fmc63("FMC63");
    Domain bb("4-1BB");
    Domain cd28("CD28");

    std::cout << "\n  1st generation -- no booster at all:\n";
    CarConstruct gen1("CD19-z", fmc63, nullptr);
    gen1.print();

    std::cout << "\n  2nd generation -- exactly one:\n";
    CarConstruct gen2("CD19-BBz", fmc63, &bb);
    gen2.print();

    std::cout << "\n  binder_ is always present, so it could have been a reference.\n"
              << "  costim_ could not: a reference has no 'absent' value. The biology\n"
              << "  picked the pointer, not style.\n";

    std::cout << "\n=== (b) copying a design does not copy the registry ===\n\n";
    std::cout << "  copying gen2 (watch for COPY Domain lines -- there are none):\n";
    CarConstruct gen2_copy(gen2);
    gen2_copy.print();

    std::cout << "\n  one 4-1BB object, two designs pointing at it:\n";
    std::cout << "    &bb                        = " << &bb << "\n";
    std::cout << "    costim in gen2             = " << gen2.get_costim() << "\n";
    std::cout << "    costim in the copy         = " << gen2_copy.get_costim() << "\n";

    std::cout << "\n  now make it a real variant -- repoint, do not edit:\n";
    gen2_copy.set_costim(&cd28);
    gen2_copy.print();
    std::cout << "    costim in gen2             = " << gen2.get_costim() << "   <- untouched\n";
    std::cout << "\n  'Deep copy' is not a rule. It answers a question: does this object\n"
              << "  own that one? Here it does not, and cloning would be the bug.\n";

    std::cout << "\n=== (c) a reference that outlived its object ===\n\n";
    std::cout.flush();

    Domain good = make_scratch_FIXED();
    std::cout << "  FIXED returned: " << good.get_name() << "\n";

    std::cout << "\n  now the broken one. Plausible answer, garbage, or a crash --\n"
              << "  all three are the same bug.\n";
    std::cout.flush();

    const Domain & ghost = make_scratch_BROKEN();
    std::cout << "  BROKEN returned: " << ghost.get_name() << "\n\n";

    return 0;
}
