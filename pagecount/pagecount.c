/*
 * pagecount.c — Homework 5
 *
 * Background
 * ----------
 * Operating systems manage memory in fixed-size chunks called "pages".
 * A common page size is 4 KB (2^12 bytes). Given total addressable memory
 * and a page size, the number of pages is simply:
 *
 *     page_count = memory_size / page_size
 *
 * Your job is to implement pagecount_fast() so it outperforms pagecount()
 * without changing what it computes. Then measure and explain the difference.
 *
 * Build & run
 * -----------
 *   gcc -O2 -o pagecount pagecount.c && ./pagecount
 *
 * Try also: -O0, -O1, -O3. Does it change anything? Why or why not?
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ── Configuration ───────────────────────────────────────────────────────── */

#define TEST_LOOPS 10000000

/*
 * Three representative (memory_size, page_size) pairs.
 * All page sizes are powers of two — this is guaranteed by every real OS.
 */
static const uint64_t MEMORY_SIZES[] = { 1ULL << 32, 1ULL << 40, 1ULL << 52 };
static const uint64_t PAGE_SIZES[]   = { 1ULL << 12, 1ULL << 16, 1ULL << 32 };
#define N_PAIRS 3

/* ── Functions under test ────────────────────────────────────────────────── */

/*
 * pagecount — naive implementation using integer division.
 *
 * Division is one of the slowest integer instructions on x86-64.
 * Compile with -O2 and inspect the output on godbolt.org:
 * does the compiler replace the `div` with something smarter?
 * Why can't it, even at -O3?
 */
uint64_t pagecount(uint64_t memory_size, uint64_t page_size) {
    return memory_size / page_size;
}

/*
 * pagecount_fast — your optimised implementation.
 *
 * Hint: if you know that page_size is always a power of two, and that
 * memory_size is always an exact multiple of page_size, then division
 * can be replaced with a single bitwise operation.
 *
 * Recall: for any power-of-two p,  x / p  ==  x >> log2(p).
 *
 * You may add a parameter or change the signature if it helps — just
 * update the benchmark loop below to match.
 *
 * TODO: implement me.
 */
uint64_t pagecount_fast(uint64_t memory_size, uint64_t page_size) {
    (void)memory_size;
    (void)page_size;
    return 0; /* replace this */
}

/* ── Benchmarking helpers ─────────────────────────────────────────────────── */

/*
 * We measure wall-clock time with clock() and subtract a baseline loop
 * that does the same array indexing and accumulation — but without calling
 * the function under test. This removes loop overhead from the result.
 *
 * The `sink` accumulator is printed at the end so the compiler cannot
 * dead-code-eliminate the loops we are trying to measure.
 */

typedef struct {
    double total_s;   /* net seconds (baseline already subtracted)  */
    double ns_per_op; /* nanoseconds per single call                 */
} BenchResult;

static void check_results(void) {
    int ok = 1;
    for (int i = 0; i < N_PAIRS; i++) {
        uint64_t expected = pagecount(MEMORY_SIZES[i], PAGE_SIZES[i]);
        uint64_t got      = pagecount_fast(MEMORY_SIZES[i], PAGE_SIZES[i]);
        if (expected != got) {
            fprintf(stderr, "  MISMATCH pair %d: expected %llu, got %llu\n",
                    i, (unsigned long long)expected, (unsigned long long)got);
            ok = 0;
        }
    }
    if (ok) puts("  Correctness check passed.\n");
}

static clock_t run_baseline(uint64_t *sink) {
    clock_t start = clock();
    for (int i = 0; i < TEST_LOOPS; i++) {
        uint64_t m = MEMORY_SIZES[i % N_PAIRS];
        uint64_t p = PAGE_SIZES[i % N_PAIRS];
        *sink += 1 + m + p; /* same overhead as the real loops */
    }
    return clock() - start;
}

static clock_t run_pagecount(uint64_t *sink) {
    clock_t start = clock();
    for (int i = 0; i < TEST_LOOPS; i++) {
        uint64_t m = MEMORY_SIZES[i % N_PAIRS];
        uint64_t p = PAGE_SIZES[i % N_PAIRS];
        *sink += pagecount(m, p) + m + p;
    }
    return clock() - start;
}

static clock_t run_pagecount_fast(uint64_t *sink) {
    clock_t start = clock();
    for (int i = 0; i < TEST_LOOPS; i++) {
        uint64_t m = MEMORY_SIZES[i % N_PAIRS];
        uint64_t p = PAGE_SIZES[i % N_PAIRS];
        *sink += pagecount_fast(m, p) + m + p;
    }
    return clock() - start;
}

static BenchResult make_result(clock_t net_clocks) {
    BenchResult r;
    r.total_s   = (double)net_clocks / CLOCKS_PER_SEC;
    r.ns_per_op = r.total_s * 1e9 / TEST_LOOPS;
    return r;
}

/* ── main ────────────────────────────────────────────────────────────────── */

int main(void) {
    uint64_t sink = 0; /* prevents dead-code elimination — do not remove */

    puts("=== pagecount benchmark ===\n");

    /* --- correctness check first --- */
    puts("Correctness:");
    check_results();

    /* --- warm up the cache / branch predictor a little --- */
    for (int i = 0; i < 1000; i++) sink += pagecount(1ULL << 32, 1ULL << 12);

    /* --- baseline: loop overhead without any function call --- */
    clock_t baseline = run_baseline(&sink);

    /* --- naive division --- */
    clock_t clocks_div  = run_pagecount(&sink);
    BenchResult div_res = make_result(clocks_div > baseline ? clocks_div - baseline : 0);

    /* --- your fast version --- */
    clock_t clocks_fast  = run_pagecount_fast(&sink);
    BenchResult fast_res = make_result(clocks_fast > baseline ? clocks_fast - baseline : 0);

    /* --- report --- */
    puts("Results:");
    printf("  pagecount       : %6.2f ns/call  (%d calls, %.4f s total)\n",
           div_res.ns_per_op, TEST_LOOPS, div_res.total_s);
    printf("  pagecount_fast  : %6.2f ns/call  (%d calls, %.4f s total)\n",
           fast_res.ns_per_op, TEST_LOOPS, fast_res.total_s);

    if (fast_res.ns_per_op > 0.0 && div_res.ns_per_op > 0.0) {
        printf("\n  Speedup: %.2fx\n", div_res.ns_per_op / fast_res.ns_per_op);
    } else {
        puts("\n  (Implement pagecount_fast to see the speedup.)");
    }

    /* Prevent the compiler from discarding `sink`. */
    if (sink == 0) puts("(impossible)");

    return 0;
}
