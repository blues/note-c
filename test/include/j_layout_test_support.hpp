/*!
 * @file j_layout_test_support.hpp
 *
 * Shared machinery for validating the packed `J` layout against the historical
 * unpacked one.
 *
 * The two layouts cannot coexist in one binary -- `J` is a single type, selected
 * at compile time by NOTE_C_STORAGE_OPTIMIZATION -- so the A/B comparison is done by
 * building this suite twice and comparing the two runs:
 *
 *   1. Every test asserts layout-INDEPENDENT invariants, so each build stands on
 *      its own: parse/print round trips, structural equality, ownership, and no
 *      leaked allocations.
 *   2. Every test also records, per corpus document, the exact printed output
 *      and the exact heap cost. Those records are written to a report file, and
 *      scripts/run_ab_layout_comparison.sh diffs the two builds' reports. The
 *      output columns must be byte-identical; the memory columns are the
 *      measurement.
 *   3. A digest over every printed output is asserted in-test against a single
 *      constant, so a build that silently changes rendering fails immediately
 *      rather than waiting for the cross-build diff.
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2025 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#pragma once

#include <catch2/catch_test_macros.hpp>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

#include "n_lib.h"

// The corpus and render modes live in their own header so the golden
// generator can compile them against a pristine historical build; see
// scripts/generate_corpus_golden.sh.
#include "j_corpus.hpp"

namespace jlayout
{

// Defined below; declared here because the chunk models need it.
inline bool isPackedLayout();

// ---------------------------------------------------------------------------
// Allocator instrumentation
// ---------------------------------------------------------------------------

struct Stats {
    size_t mallocCalls;
    size_t freeCalls;
    size_t bytesRequested;
    size_t liveBytes;
    size_t liveBlocks;
};

inline Stats &stats()
{
    static Stats s = { 0, 0, 0, 0, 0 };
    return s;
}

// Live-block registry, so leaks and net heap cost can both be measured exactly.
inline std::vector<std::pair<void *, size_t> > &blocks()
{
    static std::vector<std::pair<void *, size_t> > b;
    return b;
}

// Allocation-failure injection.
//
// `failAtCall` is the 1-based ordinal of the malloc that should return NULL;
// zero disables injection. Counting ordinals rather than sizes lets a test walk
// every allocation an operation makes and assert that failing at each one in
// turn leaves nothing leaked and nothing dangling -- the paths that are
// otherwise never taken, and where a half-built node is most likely to escape.
struct FailInjector {
    size_t failAtCall;   // 1-based; 0 = never fail
    size_t callsSeen;    // attempts, including the one that was refused
    bool   fired;
};

inline FailInjector &failInjector()
{
    static FailInjector f = { 0, 0, false };
    return f;
}

inline void failAllocationAt(size_t oneBasedOrdinal)
{
    FailInjector &f = failInjector();
    f.failAtCall = oneBasedOrdinal;
    f.callsSeen = 0;
    f.fired = false;
}

inline void stopFailingAllocations()
{
    failAllocationAt(0);
}

inline void *trackedMalloc(size_t size)
{
    FailInjector &f = failInjector();
    ++f.callsSeen;
    if ((f.failAtCall != 0) && (f.callsSeen == f.failAtCall)) {
        f.fired = true;
        return NULL;
    }

    void *p = malloc(size);
    if (p != NULL) {
        Stats &s = stats();
        ++s.mallocCalls;
        s.bytesRequested += size;
        s.liveBytes += size;
        ++s.liveBlocks;
        blocks().push_back(std::make_pair(p, size));
    }
    return p;
}

inline void trackedFree(void *ptr)
{
    if (ptr == NULL) {
        return;
    }
    Stats &s = stats();
    ++s.freeCalls;
    std::vector<std::pair<void *, size_t> > &b = blocks();
    for (size_t i = b.size(); i-- > 0;) {
        if (b[i].first == ptr) {
            s.liveBytes -= b[i].second;
            --s.liveBlocks;
            b.erase(b.begin() + static_cast<long>(i));
            break;
        }
    }
    free(ptr);
}

// Install the instrumented allocator and start from a clean slate.
//
// Catch2 re-enters a SCENARIO body once per leaf section, and a REQUIRE that
// fires aborts the case without running its cleanup, so the live-block registry
// must be reset per entry. Otherwise one failure cascades into spurious leak
// reports in every later scenario.
inline void installTrackedAllocator()
{
    NoteSetFn(trackedMalloc, trackedFree, NULL, NULL);
    stopFailingAllocations();
    Stats &s = stats();
    s.mallocCalls = 0;
    s.freeCalls = 0;
    s.bytesRequested = 0;
    s.liveBytes = 0;
    s.liveBlocks = 0;
    blocks().clear();
}

inline void resetStats()
{
    Stats &s = stats();
    s.mallocCalls = 0;
    s.freeCalls = 0;
    s.bytesRequested = 0;
    // liveBytes/liveBlocks intentionally NOT reset: they track outstanding
    // allocations across a whole test so leaks are visible.
}

// True if `p` lies inside the block that starts at `base` and is `len` long.
inline bool within(const void *p, const void *base, size_t len)
{
    const char *cp = static_cast<const char *>(p);
    const char *cb = static_cast<const char *>(base);
    return (cp >= cb) && (cp < cb + len);
}

// Size of the tracked allocation containing `p`, or 0 if `p` is not inside one.
inline size_t owningBlockSize(const void *p)
{
    const std::vector<std::pair<void *, size_t> > &b = blocks();
    for (size_t i = b.size(); i-- > 0;) {
        if (within(p, b[i].first, b[i].second)) {
            return b[i].second;
        }
    }
    return 0;
}

// True if `p` points inside the same allocation that `node` starts.
inline bool insideNodeAllocation(const J *node, const void *p)
{
    if (p == NULL) {
        return false;
    }
    const std::vector<std::pair<void *, size_t> > &b = blocks();
    for (size_t i = b.size(); i-- > 0;) {
        if (b[i].first == static_cast<const void *>(node)) {
            return within(p, b[i].first, b[i].second);
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// Real-allocator chunk models
//
// Bytes REQUESTED understates what a document actually costs. Every allocation
// on a real target also carries a per-chunk header and is rounded up to the
// allocator's alignment, and small requests are floored at a minimum chunk. The
// models below are the two allocators a Cortex-M Notecard host is most likely to
// be running. Both are 32-bit.
// ---------------------------------------------------------------------------

// sizeof(J) on the 32-bit target, per layout. The host build is 64-bit, so
// every node allocation is shrunk by the difference before the chunk model is
// applied. Nothing else in a node's allocation is word-size dependent: the
// trailing key/value bytes are plain byte counts.
// The target sizes depend on JNUMBER's width as well as the layout. With a
// double, the historical node is 48 and the packed one 40. With
// NOTE_C_SINGLE_PRECISION, JNUMBER is a float, which removes the historical
// layout's tail padding on its own: BOTH layouts are 40 there, so the member
// reorder contributes nothing and the entire saving comes from inline packing.
//
// Hardcoding 48/40 unconditionally would overstate the single-precision win by
// attributing 8 bytes per node to a change that did not happen. Verified by
// cross-compiling for Cortex-M4: 48 / 40 / 40 / 40 across the four cells.
enum {
    J_SIZE_HOST = sizeof(J),
#ifdef NOTE_C_SINGLE_PRECISION
    J_SIZE_TARGET_PACKED = 40,
    J_SIZE_TARGET_UNPACKED = 40
#else
    J_SIZE_TARGET_PACKED = 40,
    J_SIZE_TARGET_UNPACKED = 48
#endif
};

// newlib / dlmalloc: 4-byte in-use chunk header, 8-byte alignment, 16-byte
// minimum chunk. This is what arm-none-eabi-newlib's full malloc does.
inline size_t chunkDlmalloc(size_t n)
{
    size_t c = (n + 4 + 7) & ~(size_t)7;
    return c < 16 ? 16 : c;
}

// FreeRTOS heap_4: an 8-byte BlockLink_t is prepended and the whole thing is
// rounded to portBYTE_ALIGNMENT (8).
inline size_t chunkFreeRtosHeap4(size_t n)
{
    size_t c = (n + 8 + 7) & ~(size_t)7;
    return c < 16 ? 16 : c;
}

// newlib nano-malloc (--specs=nano.specs), common on small Cortex-M parts:
// 4-byte size field, 8-byte alignment, 8-byte minimum.
inline size_t chunkNanoMalloc(size_t n)
{
    size_t c = (n + 4 + 7) & ~(size_t)7;
    return c < 8 ? 8 : c;
}

struct ChunkTotals {
    size_t raw;        // bytes as requested, 32-bit sizes
    size_t dlmalloc;
    size_t freertos;
    size_t nano;
};

// Walk the live blocks, translate node allocations to their 32-bit size, and
// total them under each model. `nodePtrs` identifies which blocks are nodes.
inline ChunkTotals chunkTotals(const std::vector<const void *> &nodePtrs)
{
    const size_t shrink = (size_t)J_SIZE_HOST -
                          (isPackedLayout() ? (size_t)J_SIZE_TARGET_PACKED
                           : (size_t)J_SIZE_TARGET_UNPACKED);
    ChunkTotals t = { 0, 0, 0, 0 };
    const std::vector<std::pair<void *, size_t> > &b = blocks();
    for (size_t i = 0; i < b.size(); ++i) {
        size_t sz = b[i].second;
        bool isNode = false;
        for (size_t k = 0; k < nodePtrs.size(); ++k) {
            if (nodePtrs[k] == b[i].first) {
                isNode = true;
                break;
            }
        }
        if (isNode) {
            sz -= shrink;
        }
        t.raw += sz;
        t.dlmalloc += chunkDlmalloc(sz);
        t.freertos += chunkFreeRtosHeap4(sz);
        t.nano += chunkNanoMalloc(sz);
    }
    return t;
}

// ---------------------------------------------------------------------------
// Which layout is this build?
// ---------------------------------------------------------------------------

inline const char *layoutName()
{
#ifndef NOTE_C_STORAGE_OPTIMIZATION
    return "unpacked";
#else
    return "packed";
#endif
}

inline bool isPacked()
{
#ifndef NOTE_C_STORAGE_OPTIMIZATION
    return false;
#else
    return true;
#endif
}

inline bool isPackedLayout()
{
    return isPacked();
}

// Total bytes allocated for a node. The packed layout records this in objlen;
// the unpacked layout has no such member and every node is exactly sizeof(J).
// Wrapping it keeps the tests free of #ifdefs -- isPacked() is a runtime test,
// so an unguarded item->objlen would still have to COMPILE in both builds.
inline size_t nodeAllocSize(const J *item)
{
#ifndef NOTE_C_STORAGE_OPTIMIZATION
    (void)item;
    return sizeof(J);
#else
    return item->objlen;
#endif
}


// ---------------------------------------------------------------------------
// Historical goldens
//
// What the SHIPPED implementation renders for each (document, mode), recorded
// by scripts/generate_corpus_golden.sh from a pristine `git archive` of the
// base revision and checked in under test/data/.
//
// This is the only check in the suite that looks outside the working tree. The
// cross-layout diff and the digest below both compare the rewrite against
// itself: they catch a change that affects ONE layout, and are blind to one
// that affects both. Comparing against a recording of the released behavior is
// what closes that gap.
// ---------------------------------------------------------------------------

typedef std::vector<std::pair<std::string, std::string> > GoldenRows;

// Undo the escaping the generator applies so each row stays one line.
inline std::string goldenUnescape(const std::string &in)
{
    std::string out;
    out.reserve(in.size());
    for (size_t i = 0; i < in.size(); ++i) {
        if ((in[i] == '\\') && ((i + 1) < in.size())) {
            switch (in[i + 1]) {
            case 'n':
                out += '\n';
                ++i;
                continue;
            case 't':
                out += '\t';
                ++i;
                continue;
            case '\\':
                out += '\\';
                ++i;
                continue;
            default:
                break;
            }
        }
        out += in[i];
    }
    return out;
}

// Keyed by "doc\tmode". Empty if the file is missing, which the caller must
// treat as a failure rather than a pass -- a golden that silently disappears
// would otherwise turn this check into a no-op.
inline const GoldenRows &goldens()
{
    static GoldenRows rows;
    static bool loaded = false;
    if (loaded) {
        return rows;
    }
    loaded = true;

#ifdef NOTE_C_SINGLE_PRECISION
    const char *file = J_TEST_DATA_DIR "/corpus_golden_single.tsv";
#else
    const char *file = J_TEST_DATA_DIR "/corpus_golden.tsv";
#endif
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        return rows;
    }

    std::string line;
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (c != '\n') {
            line += (char)c;
            continue;
        }
        if (!line.empty() && (line[0] != '#')) {
            size_t t1 = line.find('\t');
            size_t t2 = (t1 == std::string::npos) ? std::string::npos : line.find('\t', t1 + 1);
            if (t2 != std::string::npos) {
                rows.push_back(std::make_pair(line.substr(0, t2),
                                              goldenUnescape(line.substr(t2 + 1))));
            }
        }
        line.clear();
    }
    fclose(f);
    return rows;
}

// The recorded rendering for a (document, mode), or NULL if there is none.
inline const std::string *goldenFor(const char *doc, const char *mode)
{
    const std::string key = std::string(doc) + "\t" + mode;
    const GoldenRows &rows = goldens();
    for (size_t i = 0; i < rows.size(); ++i) {
        if (rows[i].first == key) {
            return &rows[i].second;
        }
    }
    return NULL;
}

// ---------------------------------------------------------------------------
// Deliberate deviations from the historical output
//
// Two printer bugs were fixed alongside the layout work, so a handful of
// documents legitimately render differently than the recorded goldens. Each one
// is listed here with its reason, and the suite asserts that the row STILL
// differs -- so if a fix is ever reverted, the exception fails rather than
// quietly starting to match again.
//
// Nothing gets added to this table to make a test pass. A new entry is a claim
// that the shipped implementation was wrong, and needs the evidence to match.
// ---------------------------------------------------------------------------

struct GoldenException {
    const char *doc;
    const char *mode;    // NULL matches every mode
    bool historicalOutputIsInvalidJson;
    const char *why;
};

inline const std::vector<GoldenException> &goldenExceptions()
{
    static const std::vector<GoldenException> e = {
        // _print_string_ptr advanced the output pointer by 4 after writing the
        // 'u' and four hex digits of a \uXXXX escape, i.e. one byte short of
        // the five it had emitted. Every escape after the first therefore
        // overwrote the one before it and the string was left unterminated.
        // The historical output for these is not merely different, it is not
        // JSON: it fails to reparse.
        {
            "escapes-control", NULL, true,
            "historical _print_string_ptr truncated after the first \\uXXXX escape"
        },
        {
            "escapes-mixed", NULL, true,
            "historical _print_string_ptr truncated after the first \\uXXXX escape"
        },
        {
            "escaped-key", NULL, true,
            "historical _print_string_ptr truncated after the first \\uXXXX escape"
        },

        // _print_object emitted a member's indentation before deciding whether
        // omitempty elided that member, leaving orphaned whitespace and a
        // rendering that was not a fixed point under reparse-and-reprint. Only
        // the formatted+omitempty mode indents, so only it is affected.
        {
            "all-falsy", "formatted-omitempty", false,
            "historical _print_object indented members that omitempty then elided"
        },
        {
            "falsy-around", "formatted-omitempty", false,
            "historical _print_object indented members that omitempty then elided"
        },
        {
            "nested-falsy", "formatted-omitempty", false,
            "historical _print_object indented members that omitempty then elided"
        },
        {
            "many-members", "formatted-omitempty", false,
            "historical _print_object indented members that omitempty then elided"
        },
    };
    return e;
}

inline const GoldenException *goldenExceptionFor(const char *doc, const char *mode)
{
    const std::vector<GoldenException> &e = goldenExceptions();
    for (size_t i = 0; i < e.size(); ++i) {
        if ((strcmp(e[i].doc, doc) == 0)
                && ((e[i].mode == NULL) || (strcmp(e[i].mode, mode) == 0))) {
            return &e[i];
        }
    }
    return NULL;
}

// ---------------------------------------------------------------------------
// Output digest
//
// FNV-1a over every byte every corpus document renders to, in a fixed order.
// One constant that both builds must agree on, so a rendering change fails the
// build that caused it instead of only showing up in a cross-build diff.
// ---------------------------------------------------------------------------

inline uint64_t fnv1a(uint64_t h, const char *s)
{
    while (*s) {
        h ^= (unsigned char)*s++;
        h *= 1099511628211ULL;
    }
    return h;
}

inline uint64_t fnv1aSeed()
{
    return 1469598103934665603ULL;
}

// The digest both layouts must produce. Overridable from the build so the value
// can be regenerated deliberately when the corpus changes.
#ifndef J_LAYOUT_EXPECTED_DIGEST
#ifdef NOTE_C_SINGLE_PRECISION
#define J_LAYOUT_EXPECTED_DIGEST 13861798192606499111ULL
#else
#define J_LAYOUT_EXPECTED_DIGEST 17055881978828097657ULL
#endif
#endif

// ---------------------------------------------------------------------------
// Report file
//
// One tab-separated row per (document, mode). The A/B script diffs the output
// column across builds and tabulates the memory columns.
// ---------------------------------------------------------------------------

class Report
{
public:
    Report()
    {
        const char *path = getenv("J_LAYOUT_REPORT");
        if (path != NULL) {
            f = fopen(path, "w");
        }
        if (f != NULL) {
            fprintf(f, "# layout=%s sizeof_J=%zu\n", layoutName(), sizeof(J));
            fprintf(f, "#doc\tmode\tallocs\tbytes\tnodes\ttextlen\traw32\tdlmalloc\tfreertos\tnano\toutput\n");
        }
    }
    ~Report()
    {
        if (f != NULL) {
            fclose(f);
        }
    }

    void row(const char *doc, const char *mode, size_t allocs, size_t bytes,
             size_t nodes, const ChunkTotals &t, const char *output)
    {
        if (f == NULL) {
            return;
        }
        fprintf(f, "%s\t%s\t%zu\t%zu\t%zu\t%zu\t%zu\t%zu\t%zu\t%zu\t",
                doc, mode, allocs, bytes, nodes, strlen(output),
                t.raw, t.dlmalloc, t.freertos, t.nano);
        for (const char *p = output; *p; ++p) {
            /* keep the row on one line and byte-exact */
            if (*p == '\n') {
                fputs("\\n", f);
            } else if (*p == '\t') {
                fputs("\\t", f);
            } else if (*p == '\\') {
                fputs("\\\\", f);
            } else {
                fputc(*p, f);
            }
        }
        fputc('\n', f);
        fflush(f);   /* so a crash still leaves a usable partial report */
    }

    void summary(const char *key, size_t value)
    {
        if (f != NULL) {
            fprintf(f, "@%s\t%zu\n", key, value);
        }
    }

private:
    FILE *f = NULL;
};

inline Report &report()
{
    static Report r;
    return r;
}

// ---------------------------------------------------------------------------
// Tree walking
// ---------------------------------------------------------------------------

inline void collectNodes(const J *j, std::vector<const void *> &out)
{
    for (; j != NULL; j = j->next) {
        out.push_back((const void *)j);
        if (j->child != NULL) {
            collectNodes(j->child, out);
        }
    }
}

inline size_t countNodes(const J *j)
{
    size_t n = 0;
    for (; j != NULL; j = j->next) {
        ++n;
        if (j->child != NULL) {
            n += countNodes(j->child);
        }
    }
    return n;
}

} // namespace jlayout
