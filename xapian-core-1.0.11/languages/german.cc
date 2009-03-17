/* This file was generated automatically by the Snowball to ISO C++ compiler */

#include <limits.h>
#include "german.h"

static const symbol s_0_1[1] = { 'U' };
static const symbol s_0_2[1] = { 'Y' };
static const symbol s_0_3[2] = { 0xC3, 0xA4 };
static const symbol s_0_4[2] = { 0xC3, 0xB6 };
static const symbol s_0_5[2] = { 0xC3, 0xBC };

static const struct among a_0[6] =
{
/*  0 */ { 0, 0, -1, 6},
/*  1 */ { 1, s_0_1, 0, 2},
/*  2 */ { 1, s_0_2, 0, 1},
/*  3 */ { 2, s_0_3, 0, 3},
/*  4 */ { 2, s_0_4, 0, 4},
/*  5 */ { 2, s_0_5, 0, 5}
};

static const symbol s_1_0[1] = { 'e' };
static const symbol s_1_1[2] = { 'e', 'm' };
static const symbol s_1_2[2] = { 'e', 'n' };
static const symbol s_1_3[3] = { 'e', 'r', 'n' };
static const symbol s_1_4[2] = { 'e', 'r' };
static const symbol s_1_5[1] = { 's' };
static const symbol s_1_6[2] = { 'e', 's' };

static const struct among a_1[7] =
{
/*  0 */ { 1, s_1_0, -1, 1},
/*  1 */ { 2, s_1_1, -1, 1},
/*  2 */ { 2, s_1_2, -1, 1},
/*  3 */ { 3, s_1_3, -1, 1},
/*  4 */ { 2, s_1_4, -1, 1},
/*  5 */ { 1, s_1_5, -1, 2},
/*  6 */ { 2, s_1_6, 5, 1}
};

static const symbol s_2_0[2] = { 'e', 'n' };
static const symbol s_2_1[2] = { 'e', 'r' };
static const symbol s_2_2[2] = { 's', 't' };
static const symbol s_2_3[3] = { 'e', 's', 't' };

static const struct among a_2[4] =
{
/*  0 */ { 2, s_2_0, -1, 1},
/*  1 */ { 2, s_2_1, -1, 1},
/*  2 */ { 2, s_2_2, -1, 2},
/*  3 */ { 3, s_2_3, 2, 1}
};

static const symbol s_3_0[2] = { 'i', 'g' };
static const symbol s_3_1[4] = { 'l', 'i', 'c', 'h' };

static const struct among a_3[2] =
{
/*  0 */ { 2, s_3_0, -1, 1},
/*  1 */ { 4, s_3_1, -1, 1}
};

static const symbol s_4_0[3] = { 'e', 'n', 'd' };
static const symbol s_4_1[2] = { 'i', 'g' };
static const symbol s_4_2[3] = { 'u', 'n', 'g' };
static const symbol s_4_3[4] = { 'l', 'i', 'c', 'h' };
static const symbol s_4_4[4] = { 'i', 's', 'c', 'h' };
static const symbol s_4_5[2] = { 'i', 'k' };
static const symbol s_4_6[4] = { 'h', 'e', 'i', 't' };
static const symbol s_4_7[4] = { 'k', 'e', 'i', 't' };

static const struct among a_4[8] =
{
/*  0 */ { 3, s_4_0, -1, 1},
/*  1 */ { 2, s_4_1, -1, 2},
/*  2 */ { 3, s_4_2, -1, 1},
/*  3 */ { 4, s_4_3, -1, 3},
/*  4 */ { 4, s_4_4, -1, 2},
/*  5 */ { 2, s_4_5, -1, 2},
/*  6 */ { 4, s_4_6, -1, 3},
/*  7 */ { 4, s_4_7, -1, 4}
};

static const unsigned char g_v[] = { 17, 65, 16, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 32, 8 };

static const unsigned char g_s_ending[] = { 117, 30, 5 };

static const unsigned char g_st_ending[] = { 117, 30, 4 };

static const symbol s_0[] = { 0xC3, 0x9F };
static const symbol s_1[] = { 's', 's' };
static const symbol s_2[] = { 'U' };
static const symbol s_3[] = { 'Y' };
static const symbol s_4[] = { 'y' };
static const symbol s_5[] = { 'u' };
static const symbol s_6[] = { 'a' };
static const symbol s_7[] = { 'o' };
static const symbol s_8[] = { 'u' };
static const symbol s_9[] = { 'i', 'g' };
static const symbol s_10[] = { 'e', 'r' };
static const symbol s_11[] = { 'e', 'n' };

int Xapian::InternalStemGerman::r_prelude() { /* forwardmode */
    {   int c_test1 = c; /* test, line 30 */
        while(1) { /* repeat, line 30 */
            int c2 = c;
            {   int c3 = c; /* or, line 33 */
                bra = c; /* [, line 32 */
                if (!(eq_s(2, s_0))) goto lab2; /* literal, line 32 */
                ket = c; /* ], line 32 */
                {   int ret = slice_from_s(2, s_1); /* <-, line 32 */
                    if (ret < 0) return ret;
                }
                goto lab1;
            lab2:
                c = c3;
                {   int ret = skip_utf8(p, c, 0, l, 1);
                    if (ret < 0) goto lab0;
                    c = ret; /* next, line 33 */
                }
            }
        lab1:
            continue;
        lab0:
            c = c2;
            break;
        }
        c = c_test1;
    }
    while(1) { /* repeat, line 36 */
        int c4 = c;
        while(1) { /* goto, line 36 */
            int c5 = c;
            if (in_grouping_U(g_v, 97, 252, 0)) goto lab4; /* grouping v, line 37 */
            bra = c; /* [, line 37 */
            {   int c6 = c; /* or, line 37 */
                if (c == l || p[c] != 'u') goto lab6;
                c++;
                ket = c; /* ], line 37 */
                if (in_grouping_U(g_v, 97, 252, 0)) goto lab6; /* grouping v, line 37 */
                {   int ret = slice_from_s(1, s_2); /* <-, line 37 */
                    if (ret < 0) return ret;
                }
                goto lab5;
            lab6:
                c = c6;
                if (c == l || p[c] != 'y') goto lab4;
                c++;
                ket = c; /* ], line 38 */
                if (in_grouping_U(g_v, 97, 252, 0)) goto lab4; /* grouping v, line 38 */
                {   int ret = slice_from_s(1, s_3); /* <-, line 38 */
                    if (ret < 0) return ret;
                }
            }
        lab5:
            c = c5;
            break;
        lab4:
            c = c5;
            {   int ret = skip_utf8(p, c, 0, l, 1);
                if (ret < 0) goto lab3;
                c = ret; /* goto, line 36 */
            }
        }
        continue;
    lab3:
        c = c4;
        break;
    }
    return 1;
}

int Xapian::InternalStemGerman::r_mark_regions() { /* forwardmode */
    I_p1 = l; /* p1 = <integer expression>, line 44 */
    I_p2 = l; /* p2 = <integer expression>, line 45 */
    {   int c_test1 = c; /* test, line 47 */
        {   int ret = skip_utf8(p, c, 0, l, + 3); /* hop, line 47 */
            if (ret < 0) return 0;
            c = ret;
        }
        I_x = c; /* setmark x, line 47 */
        c = c_test1;
    }
    {   int ret = out_grouping_U(g_v, 97, 252, 1); /* gopast */ /* grouping v, line 49 */
        if (ret < 0) return 0;
        c += ret;
    }
    {   int ret = in_grouping_U(g_v, 97, 252, 1); /* gopast */ /* non v, line 49 */
        if (ret < 0) return 0;
        c += ret;
    }
    I_p1 = c; /* setmark p1, line 49 */
    /* try, line 50 */
    if (!(I_p1 < I_x)) goto lab0; /* p1 < <integer expression>, line 50 */
    I_p1 = I_x; /* p1 = <integer expression>, line 50 */
lab0:
    {   int ret = out_grouping_U(g_v, 97, 252, 1); /* gopast */ /* grouping v, line 51 */
        if (ret < 0) return 0;
        c += ret;
    }
    {   int ret = in_grouping_U(g_v, 97, 252, 1); /* gopast */ /* non v, line 51 */
        if (ret < 0) return 0;
        c += ret;
    }
    I_p2 = c; /* setmark p2, line 51 */
    return 1;
}

int Xapian::InternalStemGerman::r_postlude() { /* forwardmode */
    int among_var;
    while(1) { /* repeat, line 55 */
        int c1 = c;
        bra = c; /* [, line 57 */
        among_var = find_among(a_0, 6, 0, 0); /* substring, line 57 */
        if (!(among_var)) goto lab0;
        ket = c; /* ], line 57 */
        switch(among_var) { /* among, line 57 */
            case 0: goto lab0;
            case 1:
                {   int ret = slice_from_s(1, s_4); /* <-, line 58 */
                    if (ret < 0) return ret;
                }
                break;
            case 2:
                {   int ret = slice_from_s(1, s_5); /* <-, line 59 */
                    if (ret < 0) return ret;
                }
                break;
            case 3:
                {   int ret = slice_from_s(1, s_6); /* <-, line 60 */
                    if (ret < 0) return ret;
                }
                break;
            case 4:
                {   int ret = slice_from_s(1, s_7); /* <-, line 61 */
                    if (ret < 0) return ret;
                }
                break;
            case 5:
                {   int ret = slice_from_s(1, s_8); /* <-, line 62 */
                    if (ret < 0) return ret;
                }
                break;
            case 6:
                {   int ret = skip_utf8(p, c, 0, l, 1);
                    if (ret < 0) goto lab0;
                    c = ret; /* next, line 63 */
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

int Xapian::InternalStemGerman::r_R1() { /* backwardmode */
    if (!(I_p1 <= c)) return 0; /* p1 <= <integer expression>, line 70 */
    return 1;
}

int Xapian::InternalStemGerman::r_R2() { /* backwardmode */
    if (!(I_p2 <= c)) return 0; /* p2 <= <integer expression>, line 71 */
    return 1;
}

int Xapian::InternalStemGerman::r_standard_suffix() { /* backwardmode */
    int among_var;
    {   int m1 = l - c; (void)m1; /* do, line 74 */
        ket = c; /* [, line 75 */
        if (c <= lb || p[c - 1] >> 5 != 3 || !((811040 >> (p[c - 1] & 0x1f)) & 1)) goto lab0; /* substring, line 75 */
        among_var = find_among_b(a_1, 7, 0, 0);
        if (!(among_var)) goto lab0;
        bra = c; /* ], line 75 */
        {   int ret = r_R1(); /* call R1, line 75 */
            if (ret == 0) goto lab0;
            if (ret < 0) return ret;
        }
        switch(among_var) { /* among, line 75 */
            case 0: goto lab0;
            case 1:
                if (slice_del() == -1) return -1; /* delete, line 77 */
                break;
            case 2:
                if (in_grouping_b_U(g_s_ending, 98, 116, 0)) goto lab0; /* grouping s_ending, line 80 */
                if (slice_del() == -1) return -1; /* delete, line 80 */
                break;
        }
    lab0:
        c = l - m1;
    }
    {   int m2 = l - c; (void)m2; /* do, line 84 */
        ket = c; /* [, line 85 */
        if (c - 1 <= lb || p[c - 1] >> 5 != 3 || !((1327104 >> (p[c - 1] & 0x1f)) & 1)) goto lab1; /* substring, line 85 */
        among_var = find_among_b(a_2, 4, 0, 0);
        if (!(among_var)) goto lab1;
        bra = c; /* ], line 85 */
        {   int ret = r_R1(); /* call R1, line 85 */
            if (ret == 0) goto lab1;
            if (ret < 0) return ret;
        }
        switch(among_var) { /* among, line 85 */
            case 0: goto lab1;
            case 1:
                if (slice_del() == -1) return -1; /* delete, line 87 */
                break;
            case 2:
                if (in_grouping_b_U(g_st_ending, 98, 116, 0)) goto lab1; /* grouping st_ending, line 90 */
                {   int ret = skip_utf8(p, c, lb, l, - 3); /* hop, line 90 */
                    if (ret < 0) goto lab1;
                    c = ret;
                }
                if (slice_del() == -1) return -1; /* delete, line 90 */
                break;
        }
    lab1:
        c = l - m2;
    }
    {   int m3 = l - c; (void)m3; /* do, line 94 */
        ket = c; /* [, line 95 */
        if (c - 1 <= lb || p[c - 1] >> 5 != 3 || !((1051024 >> (p[c - 1] & 0x1f)) & 1)) goto lab2; /* substring, line 95 */
        among_var = find_among_b(a_4, 8, 0, 0);
        if (!(among_var)) goto lab2;
        bra = c; /* ], line 95 */
        {   int ret = r_R2(); /* call R2, line 95 */
            if (ret == 0) goto lab2;
            if (ret < 0) return ret;
        }
        switch(among_var) { /* among, line 95 */
            case 0: goto lab2;
            case 1:
                if (slice_del() == -1) return -1; /* delete, line 97 */
                {   int m4 = l - c; (void)m4; /* try, line 98 */
                    ket = c; /* [, line 98 */
                    if (!(eq_s_b(2, s_9))) { c = l - m4; goto lab3; } /* literal, line 98 */
                    bra = c; /* ], line 98 */
                    {   int m5 = l - c; (void)m5; /* not, line 98 */
                        if (c <= lb || p[c - 1] != 'e') goto lab4;
                        c--;
                        { c = l - m4; goto lab3; }
                    lab4:
                        c = l - m5;
                    }
                    {   int ret = r_R2(); /* call R2, line 98 */
                        if (ret == 0) { c = l - m4; goto lab3; }
                        if (ret < 0) return ret;
                    }
                    if (slice_del() == -1) return -1; /* delete, line 98 */
                lab3:
                    ;
                }
                break;
            case 2:
                {   int m6 = l - c; (void)m6; /* not, line 101 */
                    if (c <= lb || p[c - 1] != 'e') goto lab5;
                    c--;
                    goto lab2;
                lab5:
                    c = l - m6;
                }
                if (slice_del() == -1) return -1; /* delete, line 101 */
                break;
            case 3:
                if (slice_del() == -1) return -1; /* delete, line 104 */
                {   int m7 = l - c; (void)m7; /* try, line 105 */
                    ket = c; /* [, line 106 */
                    {   int m8 = l - c; (void)m8; /* or, line 106 */
                        if (!(eq_s_b(2, s_10))) goto lab8; /* literal, line 106 */
                        goto lab7;
                    lab8:
                        c = l - m8;
                        if (!(eq_s_b(2, s_11))) { c = l - m7; goto lab6; } /* literal, line 106 */
                    }
                lab7:
                    bra = c; /* ], line 106 */
                    {   int ret = r_R1(); /* call R1, line 106 */
                        if (ret == 0) { c = l - m7; goto lab6; }
                        if (ret < 0) return ret;
                    }
                    if (slice_del() == -1) return -1; /* delete, line 106 */
                lab6:
                    ;
                }
                break;
            case 4:
                if (slice_del() == -1) return -1; /* delete, line 110 */
                {   int m9 = l - c; (void)m9; /* try, line 111 */
                    ket = c; /* [, line 112 */
                    if (c - 1 <= lb || (p[c - 1] != 103 && p[c - 1] != 104)) { c = l - m9; goto lab9; } /* substring, line 112 */
                    among_var = find_among_b(a_3, 2, 0, 0);
                    if (!(among_var)) { c = l - m9; goto lab9; }
                    bra = c; /* ], line 112 */
                    {   int ret = r_R2(); /* call R2, line 112 */
                        if (ret == 0) { c = l - m9; goto lab9; }
                        if (ret < 0) return ret;
                    }
                    switch(among_var) { /* among, line 112 */
                        case 0: { c = l - m9; goto lab9; }
                        case 1:
                            if (slice_del() == -1) return -1; /* delete, line 114 */
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

int Xapian::InternalStemGerman::stem() { /* forwardmode */
    {   int c1 = c; /* do, line 125 */
        {   int ret = r_prelude(); /* call prelude, line 125 */
            if (ret == 0) goto lab0;
            if (ret < 0) return ret;
        }
    lab0:
        c = c1;
    }
    {   int c2 = c; /* do, line 126 */
        {   int ret = r_mark_regions(); /* call mark_regions, line 126 */
            if (ret == 0) goto lab1;
            if (ret < 0) return ret;
        }
    lab1:
        c = c2;
    }
    lb = c; c = l; /* backwards, line 127 */

    {   int m3 = l - c; (void)m3; /* do, line 128 */
        {   int ret = r_standard_suffix(); /* call standard_suffix, line 128 */
            if (ret == 0) goto lab2;
            if (ret < 0) return ret;
        }
    lab2:
        c = l - m3;
    }
    c = lb;
    {   int c4 = c; /* do, line 129 */
        {   int ret = r_postlude(); /* call postlude, line 129 */
            if (ret == 0) goto lab3;
            if (ret < 0) return ret;
        }
    lab3:
        c = c4;
    }
    return 1;
}

Xapian::InternalStemGerman::InternalStemGerman()
    : I_x(0), I_p2(0), I_p1(0)
{
}

Xapian::InternalStemGerman::~InternalStemGerman()
{
}

const char *
Xapian::InternalStemGerman::get_description() const
{
    return "german";
}
