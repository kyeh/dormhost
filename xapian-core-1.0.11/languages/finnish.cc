/* This file was generated automatically by the Snowball to ISO C++ compiler */

#include <limits.h>
#include "finnish.h"

static int tr_VI(Xapian::Stem::Internal * this_ptr) {
    return (static_cast<Xapian::InternalStemFinnish *>(this_ptr))->r_VI();
}

static int tr_LONG(Xapian::Stem::Internal * this_ptr) {
    return (static_cast<Xapian::InternalStemFinnish *>(this_ptr))->r_LONG();
}

static const among_function af[2] =
{
/*  1 */ tr_VI,
/*  2 */ tr_LONG
};

static const symbol s_0_0[2] = { 'p', 'a' };
static const symbol s_0_1[3] = { 's', 't', 'i' };
static const symbol s_0_2[4] = { 'k', 'a', 'a', 'n' };
static const symbol s_0_3[3] = { 'h', 'a', 'n' };
static const symbol s_0_4[3] = { 'k', 'i', 'n' };
static const symbol s_0_5[4] = { 'h', 0xC3, 0xA4, 'n' };
static const symbol s_0_6[6] = { 'k', 0xC3, 0xA4, 0xC3, 0xA4, 'n' };
static const symbol s_0_7[2] = { 'k', 'o' };
static const symbol s_0_8[3] = { 'p', 0xC3, 0xA4 };
static const symbol s_0_9[3] = { 'k', 0xC3, 0xB6 };

static const struct among a_0[10] =
{
/*  0 */ { 2, s_0_0, -1, 1},
/*  1 */ { 3, s_0_1, -1, 2},
/*  2 */ { 4, s_0_2, -1, 1},
/*  3 */ { 3, s_0_3, -1, 1},
/*  4 */ { 3, s_0_4, -1, 1},
/*  5 */ { 4, s_0_5, -1, 1},
/*  6 */ { 6, s_0_6, -1, 1},
/*  7 */ { 2, s_0_7, -1, 1},
/*  8 */ { 3, s_0_8, -1, 1},
/*  9 */ { 3, s_0_9, -1, 1}
};

static const symbol s_1_0[3] = { 'l', 'l', 'a' };
static const symbol s_1_1[2] = { 'n', 'a' };
static const symbol s_1_2[3] = { 's', 's', 'a' };
static const symbol s_1_3[2] = { 't', 'a' };
static const symbol s_1_4[3] = { 'l', 't', 'a' };
static const symbol s_1_5[3] = { 's', 't', 'a' };

static const struct among a_1[6] =
{
/*  0 */ { 3, s_1_0, -1, -1},
/*  1 */ { 2, s_1_1, -1, -1},
/*  2 */ { 3, s_1_2, -1, -1},
/*  3 */ { 2, s_1_3, -1, -1},
/*  4 */ { 3, s_1_4, 3, -1},
/*  5 */ { 3, s_1_5, 3, -1}
};

static const symbol s_2_0[4] = { 'l', 'l', 0xC3, 0xA4 };
static const symbol s_2_1[3] = { 'n', 0xC3, 0xA4 };
static const symbol s_2_2[4] = { 's', 's', 0xC3, 0xA4 };
static const symbol s_2_3[3] = { 't', 0xC3, 0xA4 };
static const symbol s_2_4[4] = { 'l', 't', 0xC3, 0xA4 };
static const symbol s_2_5[4] = { 's', 't', 0xC3, 0xA4 };

static const struct among a_2[6] =
{
/*  0 */ { 4, s_2_0, -1, -1},
/*  1 */ { 3, s_2_1, -1, -1},
/*  2 */ { 4, s_2_2, -1, -1},
/*  3 */ { 3, s_2_3, -1, -1},
/*  4 */ { 4, s_2_4, 3, -1},
/*  5 */ { 4, s_2_5, 3, -1}
};

static const symbol s_3_0[3] = { 'l', 'l', 'e' };
static const symbol s_3_1[3] = { 'i', 'n', 'e' };

static const struct among a_3[2] =
{
/*  0 */ { 3, s_3_0, -1, -1},
/*  1 */ { 3, s_3_1, -1, -1}
};

static const symbol s_4_0[3] = { 'n', 's', 'a' };
static const symbol s_4_1[3] = { 'm', 'm', 'e' };
static const symbol s_4_2[3] = { 'n', 'n', 'e' };
static const symbol s_4_3[2] = { 'n', 'i' };
static const symbol s_4_4[2] = { 's', 'i' };
static const symbol s_4_5[2] = { 'a', 'n' };
static const symbol s_4_6[2] = { 'e', 'n' };
static const symbol s_4_7[3] = { 0xC3, 0xA4, 'n' };
static const symbol s_4_8[4] = { 'n', 's', 0xC3, 0xA4 };

static const struct among a_4[9] =
{
/*  0 */ { 3, s_4_0, -1, 3},
/*  1 */ { 3, s_4_1, -1, 3},
/*  2 */ { 3, s_4_2, -1, 3},
/*  3 */ { 2, s_4_3, -1, 2},
/*  4 */ { 2, s_4_4, -1, 1},
/*  5 */ { 2, s_4_5, -1, 4},
/*  6 */ { 2, s_4_6, -1, 6},
/*  7 */ { 3, s_4_7, -1, 5},
/*  8 */ { 4, s_4_8, -1, 3}
};

static const symbol s_5_0[2] = { 'a', 'a' };
static const symbol s_5_1[2] = { 'e', 'e' };
static const symbol s_5_2[2] = { 'i', 'i' };
static const symbol s_5_3[2] = { 'o', 'o' };
static const symbol s_5_4[2] = { 'u', 'u' };
static const symbol s_5_5[4] = { 0xC3, 0xA4, 0xC3, 0xA4 };
static const symbol s_5_6[4] = { 0xC3, 0xB6, 0xC3, 0xB6 };

static const struct among a_5[7] =
{
/*  0 */ { 2, s_5_0, -1, -1},
/*  1 */ { 2, s_5_1, -1, -1},
/*  2 */ { 2, s_5_2, -1, -1},
/*  3 */ { 2, s_5_3, -1, -1},
/*  4 */ { 2, s_5_4, -1, -1},
/*  5 */ { 4, s_5_5, -1, -1},
/*  6 */ { 4, s_5_6, -1, -1}
};

static const symbol s_6_0[1] = { 'a' };
static const symbol s_6_1[3] = { 'l', 'l', 'a' };
static const symbol s_6_2[2] = { 'n', 'a' };
static const symbol s_6_3[3] = { 's', 's', 'a' };
static const symbol s_6_4[2] = { 't', 'a' };
static const symbol s_6_5[3] = { 'l', 't', 'a' };
static const symbol s_6_6[3] = { 's', 't', 'a' };
static const symbol s_6_7[3] = { 't', 't', 'a' };
static const symbol s_6_8[3] = { 'l', 'l', 'e' };
static const symbol s_6_9[3] = { 'i', 'n', 'e' };
static const symbol s_6_10[3] = { 'k', 's', 'i' };
static const symbol s_6_11[1] = { 'n' };
static const symbol s_6_12[3] = { 'h', 'a', 'n' };
static const symbol s_6_13[3] = { 'd', 'e', 'n' };
static const symbol s_6_14[4] = { 's', 'e', 'e', 'n' };
static const symbol s_6_15[3] = { 'h', 'e', 'n' };
static const symbol s_6_16[4] = { 't', 't', 'e', 'n' };
static const symbol s_6_17[3] = { 'h', 'i', 'n' };
static const symbol s_6_18[4] = { 's', 'i', 'i', 'n' };
static const symbol s_6_19[3] = { 'h', 'o', 'n' };
static const symbol s_6_20[4] = { 'h', 0xC3, 0xA4, 'n' };
static const symbol s_6_21[4] = { 'h', 0xC3, 0xB6, 'n' };
static const symbol s_6_22[2] = { 0xC3, 0xA4 };
static const symbol s_6_23[4] = { 'l', 'l', 0xC3, 0xA4 };
static const symbol s_6_24[3] = { 'n', 0xC3, 0xA4 };
static const symbol s_6_25[4] = { 's', 's', 0xC3, 0xA4 };
static const symbol s_6_26[3] = { 't', 0xC3, 0xA4 };
static const symbol s_6_27[4] = { 'l', 't', 0xC3, 0xA4 };
static const symbol s_6_28[4] = { 's', 't', 0xC3, 0xA4 };
static const symbol s_6_29[4] = { 't', 't', 0xC3, 0xA4 };

static const struct among a_6[30] =
{
/*  0 */ { 1, s_6_0, -1, 8},
/*  1 */ { 3, s_6_1, 0, -1},
/*  2 */ { 2, s_6_2, 0, -1},
/*  3 */ { 3, s_6_3, 0, -1},
/*  4 */ { 2, s_6_4, 0, -1},
/*  5 */ { 3, s_6_5, 4, -1},
/*  6 */ { 3, s_6_6, 4, -1},
/*  7 */ { 3, s_6_7, 4, 9},
/*  8 */ { 3, s_6_8, -1, -1},
/*  9 */ { 3, s_6_9, -1, -1},
/* 10 */ { 3, s_6_10, -1, -1},
/* 11 */ { 1, s_6_11, -1, 7},
/* 12 */ { 3, s_6_12, 11, 1},
/* 13 */ { 3, s_6_13, 11, -1},
/* 14 */ { 4, s_6_14, 11, -1},
/* 15 */ { 3, s_6_15, 11, 2},
/* 16 */ { 4, s_6_16, 11, -1},
/* 17 */ { 3, s_6_17, 11, 3},
/* 18 */ { 4, s_6_18, 11, -1},
/* 19 */ { 3, s_6_19, 11, 4},
/* 20 */ { 4, s_6_20, 11, 5},
/* 21 */ { 4, s_6_21, 11, 6},
/* 22 */ { 2, s_6_22, -1, 8},
/* 23 */ { 4, s_6_23, 22, -1},
/* 24 */ { 3, s_6_24, 22, -1},
/* 25 */ { 4, s_6_25, 22, -1},
/* 26 */ { 3, s_6_26, 22, -1},
/* 27 */ { 4, s_6_27, 26, -1},
/* 28 */ { 4, s_6_28, 26, -1},
/* 29 */ { 4, s_6_29, 26, 9}
};

static const unsigned char af_6[30] =
{
/*  0 */ 0,
/*  1 */ 0,
/*  2 */ 0,
/*  3 */ 0,
/*  4 */ 0,
/*  5 */ 0,
/*  6 */ 0,
/*  7 */ 0,
/*  8 */ 0,
/*  9 */ 0,
/* 10 */ 0,
/* 11 */ 0,
/* 12 */ 0,
/* 13 */ 1 /* tr_VI */,
/* 14 */ 2 /* tr_LONG */,
/* 15 */ 0,
/* 16 */ 1 /* tr_VI */,
/* 17 */ 0,
/* 18 */ 1 /* tr_VI */,
/* 19 */ 0,
/* 20 */ 0,
/* 21 */ 0,
/* 22 */ 0,
/* 23 */ 0,
/* 24 */ 0,
/* 25 */ 0,
/* 26 */ 0,
/* 27 */ 0,
/* 28 */ 0,
/* 29 */ 0
};

static const symbol s_7_0[3] = { 'e', 'j', 'a' };
static const symbol s_7_1[3] = { 'm', 'm', 'a' };
static const symbol s_7_2[4] = { 'i', 'm', 'm', 'a' };
static const symbol s_7_3[3] = { 'm', 'p', 'a' };
static const symbol s_7_4[4] = { 'i', 'm', 'p', 'a' };
static const symbol s_7_5[3] = { 'm', 'm', 'i' };
static const symbol s_7_6[4] = { 'i', 'm', 'm', 'i' };
static const symbol s_7_7[3] = { 'm', 'p', 'i' };
static const symbol s_7_8[4] = { 'i', 'm', 'p', 'i' };
static const symbol s_7_9[4] = { 'e', 'j', 0xC3, 0xA4 };
static const symbol s_7_10[4] = { 'm', 'm', 0xC3, 0xA4 };
static const symbol s_7_11[5] = { 'i', 'm', 'm', 0xC3, 0xA4 };
static const symbol s_7_12[4] = { 'm', 'p', 0xC3, 0xA4 };
static const symbol s_7_13[5] = { 'i', 'm', 'p', 0xC3, 0xA4 };

static const struct among a_7[14] =
{
/*  0 */ { 3, s_7_0, -1, -1},
/*  1 */ { 3, s_7_1, -1, 1},
/*  2 */ { 4, s_7_2, 1, -1},
/*  3 */ { 3, s_7_3, -1, 1},
/*  4 */ { 4, s_7_4, 3, -1},
/*  5 */ { 3, s_7_5, -1, 1},
/*  6 */ { 4, s_7_6, 5, -1},
/*  7 */ { 3, s_7_7, -1, 1},
/*  8 */ { 4, s_7_8, 7, -1},
/*  9 */ { 4, s_7_9, -1, -1},
/* 10 */ { 4, s_7_10, -1, 1},
/* 11 */ { 5, s_7_11, 10, -1},
/* 12 */ { 4, s_7_12, -1, 1},
/* 13 */ { 5, s_7_13, 12, -1}
};

static const symbol s_8_0[1] = { 'i' };
static const symbol s_8_1[1] = { 'j' };

static const struct among a_8[2] =
{
/*  0 */ { 1, s_8_0, -1, -1},
/*  1 */ { 1, s_8_1, -1, -1}
};

static const symbol s_9_0[3] = { 'm', 'm', 'a' };
static const symbol s_9_1[4] = { 'i', 'm', 'm', 'a' };

static const struct among a_9[2] =
{
/*  0 */ { 3, s_9_0, -1, 1},
/*  1 */ { 4, s_9_1, 0, -1}
};

static const unsigned char g_AEI[] = { 17, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8 };

static const unsigned char g_V1[] = { 17, 65, 16, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 32 };

static const unsigned char g_V2[] = { 17, 65, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 32 };

static const unsigned char g_particle_end[] = { 17, 97, 24, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 32 };

static const symbol s_0[] = { 'k', 's', 'e' };
static const symbol s_1[] = { 'k', 's', 'i' };
static const symbol s_2[] = { 0xC3, 0xA4 };
static const symbol s_3[] = { 0xC3, 0xB6 };
static const symbol s_4[] = { 'i', 'e' };
static const symbol s_5[] = { 'p', 'o' };
static const symbol s_6[] = { 'p', 'o' };

int Xapian::InternalStemFinnish::r_mark_regions() { /* forwardmode */
    I_p1 = l; /* p1 = <integer expression>, line 43 */
    I_p2 = l; /* p2 = <integer expression>, line 44 */
    if (out_grouping_U(g_V1, 97, 246, 1) < 0) return 0; /* goto */ /* grouping V1, line 46 */
    {   int ret = in_grouping_U(g_V1, 97, 246, 1); /* gopast */ /* non V1, line 46 */
        if (ret < 0) return 0;
        c += ret;
    }
    I_p1 = c; /* setmark p1, line 46 */
    if (out_grouping_U(g_V1, 97, 246, 1) < 0) return 0; /* goto */ /* grouping V1, line 47 */
    {   int ret = in_grouping_U(g_V1, 97, 246, 1); /* gopast */ /* non V1, line 47 */
        if (ret < 0) return 0;
        c += ret;
    }
    I_p2 = c; /* setmark p2, line 47 */
    return 1;
}

int Xapian::InternalStemFinnish::r_R2() { /* backwardmode */
    if (!(I_p2 <= c)) return 0; /* p2 <= <integer expression>, line 52 */
    return 1;
}

int Xapian::InternalStemFinnish::r_particle_etc() { /* backwardmode */
    int among_var;
    {   int m1 = l - c; (void)m1; /* setlimit, line 55 */
        int mlimit1;
        if (c < I_p1) return 0;
        c = I_p1; /* tomark, line 55 */
        mlimit1 = lb; lb = c;
        c = l - m1;
        ket = c; /* [, line 55 */
        among_var = find_among_b(a_0, 10, 0, 0); /* substring, line 55 */
        if (!(among_var)) { lb = mlimit1; return 0; }
        bra = c; /* ], line 55 */
        lb = mlimit1;
    }
    switch(among_var) { /* among, line 56 */
        case 0: return 0;
        case 1:
            if (in_grouping_b_U(g_particle_end, 97, 246, 0)) return 0; /* grouping particle_end, line 62 */
            break;
        case 2:
            {   int ret = r_R2(); /* call R2, line 64 */
                if (ret <= 0) return ret;
            }
            break;
    }
    if (slice_del() == -1) return -1; /* delete, line 66 */
    return 1;
}

int Xapian::InternalStemFinnish::r_possessive() { /* backwardmode */
    int among_var;
    {   int m1 = l - c; (void)m1; /* setlimit, line 69 */
        int mlimit1;
        if (c < I_p1) return 0;
        c = I_p1; /* tomark, line 69 */
        mlimit1 = lb; lb = c;
        c = l - m1;
        ket = c; /* [, line 69 */
        among_var = find_among_b(a_4, 9, 0, 0); /* substring, line 69 */
        if (!(among_var)) { lb = mlimit1; return 0; }
        bra = c; /* ], line 69 */
        lb = mlimit1;
    }
    switch(among_var) { /* among, line 70 */
        case 0: return 0;
        case 1:
            {   int m2 = l - c; (void)m2; /* not, line 72 */
                if (c <= lb || p[c - 1] != 'k') goto lab0;
                c--;
                return 0;
            lab0:
                c = l - m2;
            }
            if (slice_del() == -1) return -1; /* delete, line 72 */
            break;
        case 2:
            if (slice_del() == -1) return -1; /* delete, line 74 */
            ket = c; /* [, line 74 */
            if (!(eq_s_b(3, s_0))) return 0; /* literal, line 74 */
            bra = c; /* ], line 74 */
            {   int ret = slice_from_s(3, s_1); /* <-, line 74 */
                if (ret < 0) return ret;
            }
            break;
        case 3:
            if (slice_del() == -1) return -1; /* delete, line 78 */
            break;
        case 4:
            if (c - 1 <= lb || p[c - 1] != 97) return 0; /* among, line 81 */
            if (!(find_among_b(a_1, 6, 0, 0))) return 0;
            if (slice_del() == -1) return -1; /* delete, line 81 */
            break;
        case 5:
            if (c - 2 <= lb || p[c - 1] != 164) return 0; /* among, line 83 */
            if (!(find_among_b(a_2, 6, 0, 0))) return 0;
            if (slice_del() == -1) return -1; /* delete, line 84 */
            break;
        case 6:
            if (c - 2 <= lb || p[c - 1] != 101) return 0; /* among, line 86 */
            if (!(find_among_b(a_3, 2, 0, 0))) return 0;
            if (slice_del() == -1) return -1; /* delete, line 86 */
            break;
    }
    return 1;
}

int Xapian::InternalStemFinnish::r_LONG() { /* backwardmode */
    if (!(find_among_b(a_5, 7, 0, 0))) return 0; /* among, line 91 */
    return 1;
}

int Xapian::InternalStemFinnish::r_VI() { /* backwardmode */
    if (c <= lb || p[c - 1] != 'i') return 0;
    c--;
    if (in_grouping_b_U(g_V2, 97, 246, 0)) return 0; /* grouping V2, line 93 */
    return 1;
}

int Xapian::InternalStemFinnish::r_case_ending() { /* backwardmode */
    int among_var;
    {   int m1 = l - c; (void)m1; /* setlimit, line 96 */
        int mlimit1;
        if (c < I_p1) return 0;
        c = I_p1; /* tomark, line 96 */
        mlimit1 = lb; lb = c;
        c = l - m1;
        ket = c; /* [, line 96 */
        among_var = find_among_b(a_6, 30, af_6, af); /* substring, line 96 */
        if (!(among_var)) { lb = mlimit1; return 0; }
        bra = c; /* ], line 96 */
        lb = mlimit1;
    }
    switch(among_var) { /* among, line 97 */
        case 0: return 0;
        case 1:
            if (c <= lb || p[c - 1] != 'a') return 0;
            c--;
            break;
        case 2:
            if (c <= lb || p[c - 1] != 'e') return 0;
            c--;
            break;
        case 3:
            if (c <= lb || p[c - 1] != 'i') return 0;
            c--;
            break;
        case 4:
            if (c <= lb || p[c - 1] != 'o') return 0;
            c--;
            break;
        case 5:
            if (!(eq_s_b(2, s_2))) return 0; /* literal, line 102 */
            break;
        case 6:
            if (!(eq_s_b(2, s_3))) return 0; /* literal, line 103 */
            break;
        case 7:
            {   int m2 = l - c; (void)m2; /* try, line 111 */
                {   int m3 = l - c; (void)m3; /* and, line 113 */
                    {   int m4 = l - c; (void)m4; /* or, line 112 */
                        {   int ret = r_LONG(); /* call LONG, line 111 */
                            if (ret == 0) goto lab2;
                            if (ret < 0) return ret;
                        }
                        goto lab1;
                    lab2:
                        c = l - m4;
                        if (!(eq_s_b(2, s_4))) { c = l - m2; goto lab0; } /* literal, line 112 */
                    }
                lab1:
                    c = l - m3;
                    {   int ret = skip_utf8(p, c, lb, 0, -1);
                        if (ret < 0) { c = l - m2; goto lab0; }
                        c = ret; /* next, line 113 */
                    }
                }
                bra = c; /* ], line 113 */
            lab0:
                ;
            }
            break;
        case 8:
            if (in_grouping_b_U(g_V1, 97, 246, 0)) return 0; /* grouping V1, line 119 */
            if (out_grouping_b_U(g_V1, 97, 246, 0)) return 0; /* non V1, line 119 */
            break;
        case 9:
            if (c <= lb || p[c - 1] != 'e') return 0;
            c--;
            break;
    }
    if (slice_del() == -1) return -1; /* delete, line 138 */
    B_ending_removed = 1; /* set ending_removed, line 139 */
    return 1;
}

int Xapian::InternalStemFinnish::r_other_endings() { /* backwardmode */
    int among_var;
    {   int m1 = l - c; (void)m1; /* setlimit, line 142 */
        int mlimit1;
        if (c < I_p2) return 0;
        c = I_p2; /* tomark, line 142 */
        mlimit1 = lb; lb = c;
        c = l - m1;
        ket = c; /* [, line 142 */
        among_var = find_among_b(a_7, 14, 0, 0); /* substring, line 142 */
        if (!(among_var)) { lb = mlimit1; return 0; }
        bra = c; /* ], line 142 */
        lb = mlimit1;
    }
    switch(among_var) { /* among, line 143 */
        case 0: return 0;
        case 1:
            {   int m2 = l - c; (void)m2; /* not, line 146 */
                if (!(eq_s_b(2, s_5))) goto lab0; /* literal, line 146 */
                return 0;
            lab0:
                c = l - m2;
            }
            break;
    }
    if (slice_del() == -1) return -1; /* delete, line 151 */
    return 1;
}

int Xapian::InternalStemFinnish::r_i_plural() { /* backwardmode */
    {   int m1 = l - c; (void)m1; /* setlimit, line 154 */
        int mlimit1;
        if (c < I_p1) return 0;
        c = I_p1; /* tomark, line 154 */
        mlimit1 = lb; lb = c;
        c = l - m1;
        ket = c; /* [, line 154 */
        if (c <= lb || (p[c - 1] != 105 && p[c - 1] != 106)) { lb = mlimit1; return 0; } /* substring, line 154 */
        if (!(find_among_b(a_8, 2, 0, 0))) { lb = mlimit1; return 0; }
        bra = c; /* ], line 154 */
        lb = mlimit1;
    }
    if (slice_del() == -1) return -1; /* delete, line 158 */
    return 1;
}

int Xapian::InternalStemFinnish::r_t_plural() { /* backwardmode */
    int among_var;
    {   int m1 = l - c; (void)m1; /* setlimit, line 161 */
        int mlimit1;
        if (c < I_p1) return 0;
        c = I_p1; /* tomark, line 161 */
        mlimit1 = lb; lb = c;
        c = l - m1;
        ket = c; /* [, line 162 */
        if (c <= lb || p[c - 1] != 't') { lb = mlimit1; return 0; }
        c--;
        bra = c; /* ], line 162 */
        {   int m_test2 = l - c; /* test, line 162 */
            if (in_grouping_b_U(g_V1, 97, 246, 0)) { lb = mlimit1; return 0; } /* grouping V1, line 162 */
            c = l - m_test2;
        }
        if (slice_del() == -1) return -1; /* delete, line 163 */
        lb = mlimit1;
    }
    {   int m3 = l - c; (void)m3; /* setlimit, line 165 */
        int mlimit3;
        if (c < I_p2) return 0;
        c = I_p2; /* tomark, line 165 */
        mlimit3 = lb; lb = c;
        c = l - m3;
        ket = c; /* [, line 165 */
        if (c - 2 <= lb || p[c - 1] != 97) { lb = mlimit3; return 0; } /* substring, line 165 */
        among_var = find_among_b(a_9, 2, 0, 0);
        if (!(among_var)) { lb = mlimit3; return 0; }
        bra = c; /* ], line 165 */
        lb = mlimit3;
    }
    switch(among_var) { /* among, line 166 */
        case 0: return 0;
        case 1:
            {   int m4 = l - c; (void)m4; /* not, line 167 */
                if (!(eq_s_b(2, s_6))) goto lab0; /* literal, line 167 */
                return 0;
            lab0:
                c = l - m4;
            }
            break;
    }
    if (slice_del() == -1) return -1; /* delete, line 170 */
    return 1;
}

int Xapian::InternalStemFinnish::r_tidy() { /* backwardmode */
    {   int m1 = l - c; (void)m1; /* setlimit, line 173 */
        int mlimit1;
        if (c < I_p1) return 0;
        c = I_p1; /* tomark, line 173 */
        mlimit1 = lb; lb = c;
        c = l - m1;
        {   int m2 = l - c; (void)m2; /* do, line 174 */
            {   int m3 = l - c; (void)m3; /* and, line 174 */
                {   int ret = r_LONG(); /* call LONG, line 174 */
                    if (ret == 0) goto lab0;
                    if (ret < 0) return ret;
                }
                c = l - m3;
                ket = c; /* [, line 174 */
                {   int ret = skip_utf8(p, c, lb, 0, -1);
                    if (ret < 0) goto lab0;
                    c = ret; /* next, line 174 */
                }
                bra = c; /* ], line 174 */
                if (slice_del() == -1) return -1; /* delete, line 174 */
            }
        lab0:
            c = l - m2;
        }
        {   int m4 = l - c; (void)m4; /* do, line 175 */
            ket = c; /* [, line 175 */
            if (in_grouping_b_U(g_AEI, 97, 228, 0)) goto lab1; /* grouping AEI, line 175 */
            bra = c; /* ], line 175 */
            if (out_grouping_b_U(g_V1, 97, 246, 0)) goto lab1; /* non V1, line 175 */
            if (slice_del() == -1) return -1; /* delete, line 175 */
        lab1:
            c = l - m4;
        }
        {   int m5 = l - c; (void)m5; /* do, line 176 */
            ket = c; /* [, line 176 */
            if (c <= lb || p[c - 1] != 'j') goto lab2;
            c--;
            bra = c; /* ], line 176 */
            {   int m6 = l - c; (void)m6; /* or, line 176 */
                if (c <= lb || p[c - 1] != 'o') goto lab4;
                c--;
                goto lab3;
            lab4:
                c = l - m6;
                if (c <= lb || p[c - 1] != 'u') goto lab2;
                c--;
            }
        lab3:
            if (slice_del() == -1) return -1; /* delete, line 176 */
        lab2:
            c = l - m5;
        }
        {   int m7 = l - c; (void)m7; /* do, line 177 */
            ket = c; /* [, line 177 */
            if (c <= lb || p[c - 1] != 'o') goto lab5;
            c--;
            bra = c; /* ], line 177 */
            if (c <= lb || p[c - 1] != 'j') goto lab5;
            c--;
            if (slice_del() == -1) return -1; /* delete, line 177 */
        lab5:
            c = l - m7;
        }
        lb = mlimit1;
    }
    if (in_grouping_b_U(g_V1, 97, 246, 1) < 0) return 0; /* goto */ /* non V1, line 179 */
    ket = c; /* [, line 179 */
    {   int ret = skip_utf8(p, c, lb, 0, -1);
        if (ret < 0) return 0;
        c = ret; /* next, line 179 */
    }
    bra = c; /* ], line 179 */
    {   symbol * ret = slice_to(S_x); /* -> x, line 179 */
        if (ret == 0) return -1;
        S_x = ret;
    }
    if (!(eq_v_b(S_x))) return 0; /* name x, line 179 */
    if (slice_del() == -1) return -1; /* delete, line 179 */
    return 1;
}

int Xapian::InternalStemFinnish::stem() { /* forwardmode */
    {   int c1 = c; /* do, line 185 */
        {   int ret = r_mark_regions(); /* call mark_regions, line 185 */
            if (ret == 0) goto lab0;
            if (ret < 0) return ret;
        }
    lab0:
        c = c1;
    }
    B_ending_removed = 0; /* unset ending_removed, line 186 */
    lb = c; c = l; /* backwards, line 187 */

    {   int m2 = l - c; (void)m2; /* do, line 188 */
        {   int ret = r_particle_etc(); /* call particle_etc, line 188 */
            if (ret == 0) goto lab1;
            if (ret < 0) return ret;
        }
    lab1:
        c = l - m2;
    }
    {   int m3 = l - c; (void)m3; /* do, line 189 */
        {   int ret = r_possessive(); /* call possessive, line 189 */
            if (ret == 0) goto lab2;
            if (ret < 0) return ret;
        }
    lab2:
        c = l - m3;
    }
    {   int m4 = l - c; (void)m4; /* do, line 190 */
        {   int ret = r_case_ending(); /* call case_ending, line 190 */
            if (ret == 0) goto lab3;
            if (ret < 0) return ret;
        }
    lab3:
        c = l - m4;
    }
    {   int m5 = l - c; (void)m5; /* do, line 191 */
        {   int ret = r_other_endings(); /* call other_endings, line 191 */
            if (ret == 0) goto lab4;
            if (ret < 0) return ret;
        }
    lab4:
        c = l - m5;
    }
    {   int m6 = l - c; (void)m6; /* or, line 192 */
        if (!(B_ending_removed)) goto lab6; /* Boolean test ending_removed, line 192 */
        {   int m7 = l - c; (void)m7; /* do, line 192 */
            {   int ret = r_i_plural(); /* call i_plural, line 192 */
                if (ret == 0) goto lab7;
                if (ret < 0) return ret;
            }
        lab7:
            c = l - m7;
        }
        goto lab5;
    lab6:
        c = l - m6;
        {   int m8 = l - c; (void)m8; /* do, line 192 */
            {   int ret = r_t_plural(); /* call t_plural, line 192 */
                if (ret == 0) goto lab8;
                if (ret < 0) return ret;
            }
        lab8:
            c = l - m8;
        }
    }
lab5:
    {   int m9 = l - c; (void)m9; /* do, line 193 */
        {   int ret = r_tidy(); /* call tidy, line 193 */
            if (ret == 0) goto lab9;
            if (ret < 0) return ret;
        }
    lab9:
        c = l - m9;
    }
    c = lb;
    return 1;
}

Xapian::InternalStemFinnish::InternalStemFinnish()
    : B_ending_removed(0), S_x(0), I_p2(0), I_p1(0)
{
    S_x = create_s();
}

Xapian::InternalStemFinnish::~InternalStemFinnish()
{
    lose_s(S_x);
}

const char *
Xapian::InternalStemFinnish::get_description() const
{
    return "finnish";
}
