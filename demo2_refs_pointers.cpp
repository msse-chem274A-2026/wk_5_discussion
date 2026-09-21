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
// at them.
//
// Look at the CONSTRUCTOR, not the members. Both members are pointers (a
// reference member would make this class awkward to copy), but the parameters
// differ on purpose:
//
//     const Domain & binder    always present -> a reference. You CANNOT
//                              pass nothing: there is no null reference, so
//                              the compiler refuses a design with no binder.
//     const Domain * costim    may be absent  -> a pointer, and nullptr is
//                              a legitimate value meaning 1st generation.
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

    // Uncomment to see the compiler refuse a design with no binder:
    // CarConstruct bad("no-binder", nullptr, &bb);

    std::cout << "\n  2nd generation -- exactly one:\n";
    CarConstruct gen2("CD19-BBz", fmc63, &bb);
    gen2.print();

    std::cout << "\n  The constructor takes them differently:\n\n"
              << "      const Domain & binder    a REFERENCE -- always there\n"
              << "      const Domain * costim    a POINTER   -- may be nullptr\n\n"
              << "  A pointer can be nullptr. A reference always refers to something.\n"
              << "  So the compiler will not let you build a design with no binder:\n"
              << "      CarConstruct bad(\"oops\", nullptr, &bb);   // will not compile\n";

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
    std::cout << "    &cd28                      = " << &cd28 << "\n";
    std::cout << "    costim in the copy         = " << gen2_copy.get_costim()
              << "   <- now points at CD28\n";
    std::cout << "    costim in gen2             = " << gen2.get_costim()
              << "   <- untouched\n";
    std::cout << "\n  'Deep copy' is not a rule. It answers a question: does this object\n"
              << "  own that one? Here it does not, and cloning would be the bug.\n";

    std::cout << "\n=== (c) a reference that outlived its object ===\n\n";
    std::cout.flush();

    Domain good = make_scratch_FIXED();
    std::cout << "  FIXED returned: " << good.get_name() << "\n";

    std::cout << "\n  Now the broken one -- it returns a reference to a local, which is\n"
              << "  gone by the time we read it.\n"
              << "  >>> If nothing prints after this line, the program crashed.\n"
              << "  >>> A crash is the LUCKY outcome. The dangerous one is when it\n"
              << "  >>> prints something that looks fine.\n";
    std::cout.flush();

    const Domain & ghost = make_scratch_BROKEN();
    std::cout << "  BROKEN returned: " << ghost.get_name() << "\n\n";

    return 0;
}
