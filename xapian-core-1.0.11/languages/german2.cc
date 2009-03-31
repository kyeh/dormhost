/* This file was generated automatically by the Snowball to ISO C++ compiler */

#include <limits.h>
#include "german2.h"

static const symbol s_0_1[2] = { 'a', 'e' };
static const symbol s_0_2[2] = { 'o', 'e' };
static const symbol s_0_3[2] = { 'q', 'u' };
static const symbol s_0_4[2] = { 'u', 'e' };
static const symbol s_0_5[2] = { 0xC3, 0x9F };

static const struct among a_0[6] =
{
/*  0 */ { 0, 0, -1, 6},
/*  1 */ { 2, s_0_1, 0, 2},
/*  2 */ { 2, s_0_2, 0, 3},
/*  3 */ { 2, s_0_3, 0, 5},
/*  4 */ { 2, s_0_4, 0, 4},
/*  5 */ { 2, s_0_5, 0, 1}
};

static const symbol s_1_1[1] = { 'U' };
static const symbol s_1_2[1] = { 'Y' };
static const symbol s_1_3[2] = { 0xC3, 0xA4 };
static const symbol s_1_4[2] = { 0xC3, 0xB6 };
static const symbol s_1_5[2] = { 0xC3, 0xBC };

static const struct among a_1[6] =
{
/*  0 */ { 0, 0, -1, 6},
/*  1 */ { 1, s_1_1, 0, 2},
/*  2 */ { 1, s_1_2, 0, 1},
/*  3 */ { 2, s_1_3, 0, 3},
/*  4 */ { 2, s_1_4, 0, 4},
/*  5 */ { 2, s_1_5, 0, 5}
};

static const symbol s_2_0[1] = { 'e' };
static const symbol s_2_1[2] = { 'e', 'm' };
static const symbol s_2_2[2] = { 'e', 'n' };
static const symbol s_2_3[3] = { 'e', 'r', 'n' };
static const symbol s_2_4[2] = { 'e', 'r' };
static const symbol s_2_5[1] = { 's' };
static const symbol s_2_6[2] = { 'e', 's' };

static const struct among a_2[7] =
{
/*  0 */ { 1, s_2_0, -1, 1},
/*  1 */ { 2, s_2_1, -1, 1},
/*  2 */ { 2, s_2_2, -1, 1},
/*  3 */ { 3, s_2_3, -1, 1},
/*  4 */ { 2, s_2_4, -1, 1},
/*  5 */ { 1, s_2_5, -1, 2},
/*  6 */ { 2, s_2_6, 5, 1}
};

static const symbol s_3_0[2] = { 'e', 'n' };
static const symbol s_3_1[2] = { 'e', 'r' };
static const symbol s_3_2[2] = { 's', 't' };
static const symbol s_3_3[3] = { 'e', 's', 't' };

static const struct among a_3[4] =
{
/*  0 */ { 2, s_3_0, -1, 1},
/*  1 */ { 2, s_3_1, -1, 1},
/*  2 */ { 2, s_3_2, -1, 2},
/*  3 */ { 3, s_3_3, 2, 1}
};

static const symbol s_4_0[2] = { 'i', 'g' };
static const symbol s_4_1[4] = { 'l', 'i', 'c', 'h' };

static const struct among a_4[2] =
{
/*  0 */ { 2, s_4_0, -1, 1},
/*  1 */ { 4, s_4_1, -1, 1}
};

static const symbol s_5_0[3] = { 'e', 'n', 'd' };
static const symbol s_5_1[2] = { 'i', 'g' };
static const symbol s_5_2[3] = { 'u', 'n', 'g' };
static const symbol s_5_3[4] = { 'l', 'i', 'c', 'h' };
static const symbol s_5_4[4] = { 'i', 's', 'c', 'h' };
static const symbol s_5_5[2] = { 'i', 'k' };
static const symbol s_5_6[4] = { 'h', 'e', 'i', 't' };
static const symbol s_5_7[4] = { 'k', 'e', 'i', 't' };

static const struct among a_5[8] =
{
/*  0 */ { 3, s_5_0, -1, 1},
/*  1 */ { 2, s_5_1, -1, 2},
/*  2 */ { 3, s_5_2, -1, 1},
/*  3 */ { 4, s_5_3, -1, 3},
/*  4 */ { 4, s_5_4, -1, 2},
/*  5 */ { 2, s_5_5, -1, 2},
/*  6 */ { 4, s_5_6, -1, 3},
/*  7 */ { 4, s_5_7, -1, 4}
};

static const unsigned char g_v[] = { 17, 65, 16, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 32, 8 };

static const unsigned char g_s_ending[] = { 117, 30, 5 };

static const unsigned char g_st_ending[] = { 117, 30, 4 };

static const symbol s_0[] = { 'U' };
static const symbol s_1[] = { 'Y' };
static const symbol s_2[] = { 's', 's' };
static const symbol s_3[] = { 0xC3, 0xA4 };
static const symbol s_4[] = { 0xC3, 0xB6 };
static const symbol s_5[] = { 0xC3, 0xBC };
static const symbol s_6[] = { 'y' };
static const symbol s_7[] = { 'u' };
static const symbol s_8[] = { 'a' };
static const symbol s_9[] = { 'o' };
static const symbol s_10[] = { 'u' };
static const symbol s_11[] = { 'i', 'g' };
static const symbol s_12[] = { 'e', 'r' };
static const symbol s_13[] = { 'e', 'n' };

int Xapian::InternalStemGerman2::r_prelude() { /* forwardmode */
    int among_var;
    {   int c_test1 = c; /* test, line 30 */
        while(1) { /* repeat, line 30 */
            int c2 = c;
            while(1) { /* goto, line 30 */
                int c3 = c;
                if (in_grouping_U(g_v, 97, 252, 0)) goto lab1; /* grouping v, line 31 */
                bra = c; /* [, line 31 */
                {   int c4 = c; /* or, line 31 */
                    if (c == l || p[c] != 'u') goto lab3;
                    c++;
                    ket = c; /* ], line 31 */
                    if (in_grouping_U(g_v, 97, 252, 0)) goto lab3; /* grouping v, line 31 */
                    {   int ret = slice_from_s(1, s_0); /* <-, line 31 */
                        if (ret < 0) return ret;
                    }
                    goto lab2;
                lab3:
                    c = c4;
                    if (c == l || p[c] != 'y') goto lab1;
                    c++;
                    ket = c; /* ], line 32 */
                    if (in_grouping_U(g_v, 97, 252, 0)) goto lab1; /* grouping v, line 32 */
                    {   int ret = slice_from_s(1, s_1); /* <-, line 32 */
                        if (ret < 0) return ret;
                    }
                }
            lab2:
                c = c3;
                break;
            lab1:
                c = c3;
                {   int ret = skip_utf8(p, c, 0, l, 1);
                    if (ret < 0) goto lab0;
                    c = ret; /* goto, line 30 */
                }
            }
            continue;
        lab0:
            c = c2;
            break;
        }
        c = c_test1;
    }
    while(1) { /* repeat, line 35 */
        int c5 = c;
        bra = c; /* [, line 36 */
        among_var = find_among(a_0, 6, 0, 0); /* substring, line 36 */
        if (!(among_var)) goto lab4;
        ket = c; /* ], line 36 */
        switch(among_var) { /* among, line 36 */
            case 0: goto lab4;
            case 1:
                {   int ret = slice_from_s(2, s_2); /* <-, line 37 */
                    if (ret < 0) return ret;
                }
                break;
            case 2:
                {   int ret = slice_from_s(2, s_3); /* <-, line 38 */
                    if (ret < 0) return ret;
                }
                break;
            case 3:
                {   int ret = slice_from_s(2, s_4); /* <-, line 39 */
                    if (ret < 0) return ret;
                }
                break;
            case 4:
                {   int ret = slice_from_s(2, s_5); /* <-, line 40 */
                    if (ret < 0) return ret;
                }
                break;
            case 5:
                {   int ret = skip_utf8(p, c, 0, l, + 2); /* hop, line 41 */
                    if (ret < 0) goto lab4;
                    c = ret;
                }
                break;
            case 6:
                {   int ret = skip_utf8(p, c, 0, l, 1);
                    if (ret < 0) goto lab4;
                    c = ret; /* next, line 42 */
                }
                break;
        }
        continue;
    lab4:
        c = c5;
        break;
    }
    return 1;
}

int Xapian::InternalStemGerman2::r_mark_regions() { /* forwardmode */
    I_p1 = l; /* p1 = <integer expression>, line 50 */
    I_p2 = l; /* p2 = <integer expression>, line 51 */
    {   int c_test1 = c; /* test, line 53 */
        {   int ret = skip_utf8(p, c, 0, l, + 3); /* hop, line 53 */
            if (ret < 0) return 0;
            c = ret;
        }
        I_x = c; /* setmark x, line 53 */
        c = c_test1;
    }
    {   int ret = out_grouping_U(g_v, 97, 252, 1); /* gopast */ /* grouping v, line 55 */
        if (ret < 0) return 0;
        c += ret;
    }
    {   int ret = in_grouping_U(g_v, 97, 252, 1); /* gopast */ /* non v, line 55 */
        if (ret < 0) return 0;
        c += ret;
    }
    I_p1 = c; /* setmark p1, line 55 */
    /* try, line 56 */
    if (!(I_p1 < I_x)) goto lab0; /* p1 < <integer expression>, line 56 */
    I_p1 = I_x; /* p1 = <integer expression>, line 56 */
lab0:
    {   int ret = out_grouping_U(g_v, 97, 252, 1); /* gopast */ /* grouping v, line 57 */
        if (ret < 0) return 0;
        c += ret;
    }
    {   int ret = in_grouping_U(g_v, 97, 252, 1); /* gopast */ /* non v, line 57 */
        if (ret < 0) return 0;
        c += ret;
    }
    I_p2 = c; /* setmark p2, line 57 */
    return 1;
}

int Xapian::InternalStemGerman2::r_postlude() { /* forwardmode */
    int among_var;
    while(1) { /* repeat, line 61 */
        int c1 = c;
        bra = c; /* [, line 63 */
        among_var = find_among(a_1, 6, 0, 0); /* substring, line 63 */
        if (!(among_var)) goto lab0;
        ket = c; /* ], line 63 */
        switch(among_var) { /* among, line 63 */
            case 0: goto lab0;
            case 1:
                {   int ret = slice_from_s(1, s_6); /* <-, line 64 */
                    if (ret < 0) return ret;
                }
                break;
            case 2:
                {   int ret = slice_from_s(1, s_7); /* <-, line 65 */
                    if (ret < 0) return ret;
                }
                break;
            case 3:
                {   int ret = slice_from_s(1, s_8); /* <-, line 66 */
                    if (ret < 0) return ret;
                }
                break;
            case 4:
                {   int ret = slice_from_s(1, s_9); /* <-, line 67 */
                    if (ret < 0) return ret;
                }
                break;
            case 5:
                {   int ret = slice_from_s(1, s_10); /* <-, line 68 */
                    if (ret < 0) return ret;
                }
                break;
            case 6:
                {   int ret = skip_utf8(p, c, 0, l, 1);
                    if (ret < 0) goto lab0;
                    c = ret; /* next, line 69 */
                }
                break;
        }
        continue;
    lab0:
        c = c1;
        break;
    }
    return 1;
}

int Xapian::InternalStemGerman2::r_R1() { /* backwardmode */
    if (!(I_p1 <= c)) return 0; /* p1 <= <integer expression>, line 76 */
    return 1;
}

int Xapian::InternalStemGerman2::r_R2() { /* backwardmode */
    if (!(I_p2 <= c)) return 0; /* p2 <= <integer expression>, line 77 */
    return 1;
}

int Xapian::InternalStemGerman2::r_standard_suffix() { /* backwardmode */
    int among_var;
    {   int m1 = l - c; (void)m1; /* do, line 80 */
        ket = c; /* [, line 81 */
        if (c <= lb || p[c - 1] >> 5 != 3 || !((811040 >> (p[c - 1] & 0x1f)) & 1)) goto lab0; /* substring, line 81 */
        among_var = find_among_b(a_2, 7, 0, 0);
        if (!(among_var)) goto lab0;
        bra = c; /* ], line 81 */
        {   int ret = r_R1(); /* call R1, line 81 */
            if (ret == 0) goto lab0;
            if (ret < 0) return ret;
        }
        switch(among_var) { /* among, line 81 */
            case 0: goto lab0;
            case 1:
                if (slice_del() == -1) return -1; /* delete, line 83 */
                break;
            case 2:
                if (in_grouping_b_U(g_s_ending, 98, 116, 0)) goto lab0; /* grouping s_ending, line 86 */
                if (slice_del() == -1) return -1; /* delete, line 86 */
                break;
        }
    lab0:
        c = l - m1;
    }
    {   int m2 = l - c; (void)m2; /* do, line 90 */
        ket = c; /* [, line 91 */
        if (c - 1 <= lb || p[c - 1] >> 5 != 3 || !((1327104 >> (p[c - 1] & 0x1f)) & 1)) goto lab1; /* substring, line 91 */
        among_var = find_among_b(a_3, 4, 0, 0);
        if (!(among_var)) goto lab1;
        bra = c; /* ], line 91 */
        {   int ret = r_R1(); /* call R1, line 91 */
            if (ret == 0) goto lab1;
            if (ret < 0) return ret;
        }
        switch(among_var) { /* among, line 91 */
            case 0: goto lab1;
            case 1:
                if (slice_del() == -1) return -1; /* delete, line 93 */
                break;
            case 2:
                if (in_grouping_b_U(g_st_ending, 98, 116, 0)) goto lab1; /* grouping st_ending, line 96 */
                {   int ret = skip_utf8(p, c, lb, l, - 3); /* hop, line 96 */
                    if (ret < 0) goto lab1;
                    c = ret;
                }
                if (slice_del() == -1) return -1; /* delete, line 96 */
                break;
        }
    lab1:
        c = l - m2;
    }
    {   int m3 = l - c; (void)m3; /* do, line 100 */
        ket = c; /* [, line 101 */
        if (c - 1 <= lb || p[c - 1] >> 5 != 3 || !((1051024 >> (p[c - 1] & 0x1f)) & 1)) goto lab2; /* substring, line 101 */
        among_var = find_among_b(a_5, 8, 0, 0);
        if (!(among_var)) goto lab2;
        bra = c; /* ], line 101 */
        {   int ret = r_R2(); /* call R2, line 101 */
            if (ret == 0) goto lab2;
            if (ret < 0) return ret;
        }
        switch(among_var) { /* among, line 101 */
            case 0: goto lab2;
            case 1:
                if (slice_del() == -1) return -1; /* delete, line 103 */
                {   int m4 = l - c; (void)m4; /* try, line 104 */
                    ket = c; /* [, line 104 */
                    if (!(eq_s_b(2, s_11))) { c = l - m4; goto lab3; } /* literal, line 104 */
                    bra = c; /* ], line 104 */
                    {   int m5 = l - c; (void)m5; /* not, line 104 */
                        if (c <= lb || p[c - 1] != 'e') goto lab4;
                        c--;
                        { c = l - m4; goto lab3; }
                    lab4:
                        c = l - m5;
                    }
                    {   int ret = r_R2(); /* call R2, line 104 */
                        if (ret == 0) { c = l - m4; goto lab3; }
                        if (ret < 0) return ret;
                    }
                    if (slice_del() == -1) return -1; /* delete, line 104 */
                lab3:
                    ;
                }
                break;
            case 2:
                {   int m6 = l - c; (void)m6; /* not, line 107 */
                    if (c <= lb || p[c - 1] != 'e') goto lab5;
                    c--;
                    goto lab2;
                lab5:
                    c = l - m6;
                }
                if (slice_del() == -1) return -1; /* delete, line 107 */
                break;
            case 3:
                if (slice_del() == -1) return -1; /* delete, line 110 */
                {   int m7 = l - c; (void)m7; /* try, line 111 */
                    ket = c; /* [, line 112 */
                    {   int m8 = l - c; (void)m8; /* or, line 112 */
                        if (!(eq_s_b(2, s_12))) goto lab8; /* literal, line 112 */
                        goto lab7;
                    lab8:
                        c = l - m8;
                        if (!(eq_s_b(2, s_13))) { c = l - m7; goto lab6; } /* literal, line 112 */
                    }
                lab7:
                    bra = c; /* ], line 112 */
                    {   int ret = r_R1(); /* call R1, line 112 */
                        if (ret == 0) { c = l - m7; goto lab6; }
                        if (ret < 0) return ret;
                    }
                    if (slice_del() == -1) return -1; /* delete, line 112 */
                lab6:
                    ;
                }
                break;
            case 4:
                if (slice_del() == -1) return -1; /* delete, line 116 */
                {   int m9 = l - c; (void)m9; /* try, line 117 */
                    ket = c; /* [, line 118 */
                    if (c - 1 <= lb || (p[c - 1] != 103 && p[c - 1] != 104)) { c = l - m9; goto lab9; } /* substring, line 118 */
                    among_var = find_among_b(a_4, 2, 0, 0);
                    if (!(among_var)) { c = l - m9; goto lab9; }
                    bra = c; /* ], line 118 */
                    {   int ret = r_R2(); /* call R2, line 118 */
                        if (ret == 0) { c = l - m9; goto lab9; }
                        if (ret < 0) return ret;
                    }
                    switch(among_var) { /* among, line 118 */
                        case 0: { c = l - m9; goto lab9; }
                        case 1:
                            if (slice_del() == -1) return -1; /* delete, line 120 */
                            break;
                    }
                lab9:
                    ;
                }
                break;
        }
    lab2:
        c = l - m3;
    }
    return 1;
}

int Xapian::InternalStemGerman2::stem() { /* forwardmode */
    {   int c1 = c; /* do, line 131 */
        {   int ret = r_prelude(); /* call prelude, line 131 */
            if (ret == 0) goto lab0;
            if (ret < 0) return ret;
        }
    lab0:
        c = c1;
    }
    {   int c2 = c; /* do, line 132 */
        {   int ret = r_mark_regions(); /* call mark_regions, line 132 */
            if (ret == 0) goto lab1;
            if (ret < 0) return ret;
        }
    lab1:
        c = c2;
    }
    lb = c; c = l; /* backwards, line 133 */

    {   int m3 = l - c; (void)m3; /* do, line 134 */
        {   int ret = r_standard_suffix(); /* call standard_suffix, line 134 */
            if (ret == 0) goto lab2;
            if (ret < 0) return ret;
        }
    lab2:
        c = l - m3;
    }
    c = lb;
    {   int c4 = c; /* do, line 135 */
        {   int ret = r_postlude(); /* call postlude, line 135 */
            if (ret == 0) goto lab3;
            if (ret < 0) return ret;
        }
    lab3:
        c = c4;
    }
    return 1;
}

Xapian::InternalStemGerman2::InternalStemGerman2()
    : I_x(0), I_p2(0), I_p1(0)
{
}

Xapian::InternalStemGerman2::~InternalStemGerman2()
{
}

const char *
Xapian::InternalStemGerman2::get_description() const
{
    return "german2";
}
