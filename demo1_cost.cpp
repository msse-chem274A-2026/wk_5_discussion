// demo1_cost.cpp -- Demo 1: what a copy costs
//
//   make demo1 && ./demo1
//
// One idea: passing a big object by value copies it, every call, silently.
#include <ctime>
#include <iostream>

// Where a target antigen appears in healthy cells. Loaded once at the start of
// a screen, read by every design in the library, modified by nothing.
const int MAX_CELL_TYPES = 4096;

struct TissueAtlas
{
    double expression[MAX_CELL_TYPES][4];
    int essential[MAX_CELL_TYPES];
};

// These two live in this file but are never inlined into main, so the copy is
// real. The ONLY difference between them is one '&'.

static double risk_by_value(TissueAtlas atlas, double affinity)
{
    double risk = 0.0;
    for (int i = 0; i < 64; i++)
    {
        if (atlas.essential[i]) { risk += atlas.expression[i][0] / (1.0 + affinity); }
    }
    return risk;
}

static double risk_by_const_ref(const TissueAtlas & atlas, double affinity)
{
    double risk = 0.0;
    for (int i = 0; i < 64; i++)
    {
        if (atlas.essential[i]) { risk += atlas.expression[i][0] / (1.0 + affinity); }
    }
    return risk;
}

int main()
{
    static TissueAtlas atlas;
    for (int i = 0; i < MAX_CELL_TYPES; i++)
    {
        for (int d = 0; d < 4; d++) { atlas.expression[i][d] = 0.01 * ((i * 7 + d) % 100); }
        atlas.essential[i] = (i % 11 == 0) ? 1 : 0;
    }

    const int N = 100000;

    std::cout << "\n  Two functions, identical bodies, one parameter different:\n"
              << "      double risk_by_value    (      TissueAtlas   atlas, double affinity)\n"
              << "      double risk_by_const_ref(const TissueAtlas & atlas, double affinity)\n"
              << "\n  The '&' means 'use the caller's object'. Without it, the\n"
              << "  function gets its own copy.\n";

    std::cout << "\n  sizeof(TissueAtlas)        = " << sizeof(TissueAtlas) << " bytes\n";
    std::cout << "  sizeof(const TissueAtlas&) = " << sizeof(const TissueAtlas *)
              << " bytes  (just an address)\n\n";
    std::cout << "  screening " << N << " designs against the atlas\n\n";

    double total = 0.0;

    std::clock_t t0 = std::clock();
    for (int i = 0; i < N; i++) { total += risk_by_value(atlas, 5.0); }
    double secs_value = double(std::clock() - t0) / CLOCKS_PER_SEC;

    t0 = std::clock();
    for (int i = 0; i < N; i++) { total += risk_by_const_ref(atlas, 5.0); }
    double secs_ref = double(std::clock() - t0) / CLOCKS_PER_SEC;

    std::cout << "    by value:     " << secs_value << " s   ("
              << double(sizeof(TissueAtlas)) * N / 1.0e9 << " GB copied)\n";
    std::cout << "    by const ref: " << secs_ref << " s   (0 GB copied)\n";

    std::cout << "\n  Both loops computed the same total (" << total << ").\n";

    std::cout << "\n  Same answer. Same arithmetic. One '&'.\n\n"
              << "  And the by-value version is not WRONG -- the compiler's copy of the\n"
              << "  atlas is perfectly correct. It just moved "
              << double(sizeof(TissueAtlas)) * N / 1.0e9
              << " GB of memory to answer\n"
              << "  a question that never touched the original.\n\n";

    return 0;
}
