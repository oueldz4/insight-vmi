/** \file
 *  This C header file was generated by $ANTLR version 3.0.1
 *
 *     -  From the grammar source file : src/C.g
 *     -                            On : 2012-03-20 00:14:59
 *     -                 for the lexer : CLexerLexer *
 * Editing it, at least manually, is not wise. 
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 * View this file with tabs set to 8 (:set ts=8 in gvim) and indent at 4 (:set sw=4 in gvim)
 *
 * The lexer CLexerhas the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 * 
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pCLexer, which is returned from a call to CLexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the entry points are provided anyway.
 * * The entry points for CLexer are  as follows:
 *
 *  - void      pCLexer->T25(pCLexer)
 *  - void      pCLexer->T26(pCLexer)
 *  - void      pCLexer->T27(pCLexer)
 *  - void      pCLexer->T28(pCLexer)
 *  - void      pCLexer->T29(pCLexer)
 *  - void      pCLexer->T30(pCLexer)
 *  - void      pCLexer->T31(pCLexer)
 *  - void      pCLexer->T32(pCLexer)
 *  - void      pCLexer->T33(pCLexer)
 *  - void      pCLexer->T34(pCLexer)
 *  - void      pCLexer->T35(pCLexer)
 *  - void      pCLexer->T36(pCLexer)
 *  - void      pCLexer->T37(pCLexer)
 *  - void      pCLexer->T38(pCLexer)
 *  - void      pCLexer->T39(pCLexer)
 *  - void      pCLexer->T40(pCLexer)
 *  - void      pCLexer->T41(pCLexer)
 *  - void      pCLexer->T42(pCLexer)
 *  - void      pCLexer->T43(pCLexer)
 *  - void      pCLexer->T44(pCLexer)
 *  - void      pCLexer->T45(pCLexer)
 *  - void      pCLexer->T46(pCLexer)
 *  - void      pCLexer->T47(pCLexer)
 *  - void      pCLexer->T48(pCLexer)
 *  - void      pCLexer->T49(pCLexer)
 *  - void      pCLexer->T50(pCLexer)
 *  - void      pCLexer->T51(pCLexer)
 *  - void      pCLexer->T52(pCLexer)
 *  - void      pCLexer->T53(pCLexer)
 *  - void      pCLexer->T54(pCLexer)
 *  - void      pCLexer->T55(pCLexer)
 *  - void      pCLexer->T56(pCLexer)
 *  - void      pCLexer->T57(pCLexer)
 *  - void      pCLexer->T58(pCLexer)
 *  - void      pCLexer->T59(pCLexer)
 *  - void      pCLexer->T60(pCLexer)
 *  - void      pCLexer->T61(pCLexer)
 *  - void      pCLexer->T62(pCLexer)
 *  - void      pCLexer->T63(pCLexer)
 *  - void      pCLexer->T64(pCLexer)
 *  - void      pCLexer->T65(pCLexer)
 *  - void      pCLexer->T66(pCLexer)
 *  - void      pCLexer->T67(pCLexer)
 *  - void      pCLexer->T68(pCLexer)
 *  - void      pCLexer->T69(pCLexer)
 *  - void      pCLexer->T70(pCLexer)
 *  - void      pCLexer->T71(pCLexer)
 *  - void      pCLexer->T72(pCLexer)
 *  - void      pCLexer->T73(pCLexer)
 *  - void      pCLexer->T74(pCLexer)
 *  - void      pCLexer->T75(pCLexer)
 *  - void      pCLexer->T76(pCLexer)
 *  - void      pCLexer->T77(pCLexer)
 *  - void      pCLexer->T78(pCLexer)
 *  - void      pCLexer->T79(pCLexer)
 *  - void      pCLexer->T80(pCLexer)
 *  - void      pCLexer->T81(pCLexer)
 *  - void      pCLexer->T82(pCLexer)
 *  - void      pCLexer->T83(pCLexer)
 *  - void      pCLexer->T84(pCLexer)
 *  - void      pCLexer->T85(pCLexer)
 *  - void      pCLexer->T86(pCLexer)
 *  - void      pCLexer->T87(pCLexer)
 *  - void      pCLexer->T88(pCLexer)
 *  - void      pCLexer->T89(pCLexer)
 *  - void      pCLexer->T90(pCLexer)
 *  - void      pCLexer->T91(pCLexer)
 *  - void      pCLexer->T92(pCLexer)
 *  - void      pCLexer->T93(pCLexer)
 *  - void      pCLexer->T94(pCLexer)
 *  - void      pCLexer->T95(pCLexer)
 *  - void      pCLexer->T96(pCLexer)
 *  - void      pCLexer->T97(pCLexer)
 *  - void      pCLexer->T98(pCLexer)
 *  - void      pCLexer->T99(pCLexer)
 *  - void      pCLexer->T100(pCLexer)
 *  - void      pCLexer->T101(pCLexer)
 *  - void      pCLexer->T102(pCLexer)
 *  - void      pCLexer->T103(pCLexer)
 *  - void      pCLexer->T104(pCLexer)
 *  - void      pCLexer->T105(pCLexer)
 *  - void      pCLexer->T106(pCLexer)
 *  - void      pCLexer->T107(pCLexer)
 *  - void      pCLexer->T108(pCLexer)
 *  - void      pCLexer->T109(pCLexer)
 *  - void      pCLexer->T110(pCLexer)
 *  - void      pCLexer->T111(pCLexer)
 *  - void      pCLexer->T112(pCLexer)
 *  - void      pCLexer->T113(pCLexer)
 *  - void      pCLexer->T114(pCLexer)
 *  - void      pCLexer->T115(pCLexer)
 *  - void      pCLexer->T116(pCLexer)
 *  - void      pCLexer->T117(pCLexer)
 *  - void      pCLexer->T118(pCLexer)
 *  - void      pCLexer->T119(pCLexer)
 *  - void      pCLexer->T120(pCLexer)
 *  - void      pCLexer->T121(pCLexer)
 *  - void      pCLexer->T122(pCLexer)
 *  - void      pCLexer->T123(pCLexer)
 *  - void      pCLexer->T124(pCLexer)
 *  - void      pCLexer->T125(pCLexer)
 *  - void      pCLexer->T126(pCLexer)
 *  - void      pCLexer->T127(pCLexer)
 *  - void      pCLexer->T128(pCLexer)
 *  - void      pCLexer->T129(pCLexer)
 *  - void      pCLexer->T130(pCLexer)
 *  - void      pCLexer->T131(pCLexer)
 *  - void      pCLexer->T132(pCLexer)
 *  - void      pCLexer->T133(pCLexer)
 *  - void      pCLexer->IDENTIFIER(pCLexer)
 *  - void      pCLexer->LETTER(pCLexer)
 *  - void      pCLexer->CHARACTER_LITERAL(pCLexer)
 *  - void      pCLexer->STRING_LITERAL(pCLexer)
 *  - void      pCLexer->STRING_GUTS(pCLexer)
 *  - void      pCLexer->HEX_LITERAL(pCLexer)
 *  - void      pCLexer->DECIMAL_LITERAL(pCLexer)
 *  - void      pCLexer->OCTAL_LITERAL(pCLexer)
 *  - void      pCLexer->HexDigit(pCLexer)
 *  - void      pCLexer->IntegerTypeSuffix(pCLexer)
 *  - void      pCLexer->FLOATING_POINT_LITERAL(pCLexer)
 *  - void      pCLexer->Exponent(pCLexer)
 *  - void      pCLexer->FloatTypeSuffix(pCLexer)
 *  - void      pCLexer->EscapeSequence(pCLexer)
 *  - void      pCLexer->OctalEscape(pCLexer)
 *  - void      pCLexer->HexEscape(pCLexer)
 *  - void      pCLexer->UnicodeEscape(pCLexer)
 *  - void      pCLexer->WS(pCLexer)
 *  - void      pCLexer->COMMENT(pCLexer)
 *  - void      pCLexer->LINE_COMMENT(pCLexer)
 *  - void      pCLexer->LINE_COMMAND(pCLexer)
 *  - void      pCLexer->Tokens(pCLexer)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
#ifndef	_CLexer_H
#define _CLexer_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

     #if !defined(__cplusplus) && ! defined(debugmsg)
         #define debugmsg(m, ...) printf(m, __VA_ARGS__)
     #endif


#ifdef	WIN32
// Disable: Unreferenced parameter,                - Rules with parameters that are not used
//          constant conditional,                  - ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable        - tree rewrite vairables declared but not needed
//          Unreferenced local variable            - lexer rulle decalres but does not always use _type
//          potentially unitialized variable used  - retval always returned from a rule 
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4701 )
#endif



/** Context tracking structure for CLexer
 */
typedef struct CLexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;

    void (*mT25)	(struct CLexer_Ctx_struct * ctx);
    void (*mT26)	(struct CLexer_Ctx_struct * ctx);
    void (*mT27)	(struct CLexer_Ctx_struct * ctx);
    void (*mT28)	(struct CLexer_Ctx_struct * ctx);
    void (*mT29)	(struct CLexer_Ctx_struct * ctx);
    void (*mT30)	(struct CLexer_Ctx_struct * ctx);
    void (*mT31)	(struct CLexer_Ctx_struct * ctx);
    void (*mT32)	(struct CLexer_Ctx_struct * ctx);
    void (*mT33)	(struct CLexer_Ctx_struct * ctx);
    void (*mT34)	(struct CLexer_Ctx_struct * ctx);
    void (*mT35)	(struct CLexer_Ctx_struct * ctx);
    void (*mT36)	(struct CLexer_Ctx_struct * ctx);
    void (*mT37)	(struct CLexer_Ctx_struct * ctx);
    void (*mT38)	(struct CLexer_Ctx_struct * ctx);
    void (*mT39)	(struct CLexer_Ctx_struct * ctx);
    void (*mT40)	(struct CLexer_Ctx_struct * ctx);
    void (*mT41)	(struct CLexer_Ctx_struct * ctx);
    void (*mT42)	(struct CLexer_Ctx_struct * ctx);
    void (*mT43)	(struct CLexer_Ctx_struct * ctx);
    void (*mT44)	(struct CLexer_Ctx_struct * ctx);
    void (*mT45)	(struct CLexer_Ctx_struct * ctx);
    void (*mT46)	(struct CLexer_Ctx_struct * ctx);
    void (*mT47)	(struct CLexer_Ctx_struct * ctx);
    void (*mT48)	(struct CLexer_Ctx_struct * ctx);
    void (*mT49)	(struct CLexer_Ctx_struct * ctx);
    void (*mT50)	(struct CLexer_Ctx_struct * ctx);
    void (*mT51)	(struct CLexer_Ctx_struct * ctx);
    void (*mT52)	(struct CLexer_Ctx_struct * ctx);
    void (*mT53)	(struct CLexer_Ctx_struct * ctx);
    void (*mT54)	(struct CLexer_Ctx_struct * ctx);
    void (*mT55)	(struct CLexer_Ctx_struct * ctx);
    void (*mT56)	(struct CLexer_Ctx_struct * ctx);
    void (*mT57)	(struct CLexer_Ctx_struct * ctx);
    void (*mT58)	(struct CLexer_Ctx_struct * ctx);
    void (*mT59)	(struct CLexer_Ctx_struct * ctx);
    void (*mT60)	(struct CLexer_Ctx_struct * ctx);
    void (*mT61)	(struct CLexer_Ctx_struct * ctx);
    void (*mT62)	(struct CLexer_Ctx_struct * ctx);
    void (*mT63)	(struct CLexer_Ctx_struct * ctx);
    void (*mT64)	(struct CLexer_Ctx_struct * ctx);
    void (*mT65)	(struct CLexer_Ctx_struct * ctx);
    void (*mT66)	(struct CLexer_Ctx_struct * ctx);
    void (*mT67)	(struct CLexer_Ctx_struct * ctx);
    void (*mT68)	(struct CLexer_Ctx_struct * ctx);
    void (*mT69)	(struct CLexer_Ctx_struct * ctx);
    void (*mT70)	(struct CLexer_Ctx_struct * ctx);
    void (*mT71)	(struct CLexer_Ctx_struct * ctx);
    void (*mT72)	(struct CLexer_Ctx_struct * ctx);
    void (*mT73)	(struct CLexer_Ctx_struct * ctx);
    void (*mT74)	(struct CLexer_Ctx_struct * ctx);
    void (*mT75)	(struct CLexer_Ctx_struct * ctx);
    void (*mT76)	(struct CLexer_Ctx_struct * ctx);
    void (*mT77)	(struct CLexer_Ctx_struct * ctx);
    void (*mT78)	(struct CLexer_Ctx_struct * ctx);
    void (*mT79)	(struct CLexer_Ctx_struct * ctx);
    void (*mT80)	(struct CLexer_Ctx_struct * ctx);
    void (*mT81)	(struct CLexer_Ctx_struct * ctx);
    void (*mT82)	(struct CLexer_Ctx_struct * ctx);
    void (*mT83)	(struct CLexer_Ctx_struct * ctx);
    void (*mT84)	(struct CLexer_Ctx_struct * ctx);
    void (*mT85)	(struct CLexer_Ctx_struct * ctx);
    void (*mT86)	(struct CLexer_Ctx_struct * ctx);
    void (*mT87)	(struct CLexer_Ctx_struct * ctx);
    void (*mT88)	(struct CLexer_Ctx_struct * ctx);
    void (*mT89)	(struct CLexer_Ctx_struct * ctx);
    void (*mT90)	(struct CLexer_Ctx_struct * ctx);
    void (*mT91)	(struct CLexer_Ctx_struct * ctx);
    void (*mT92)	(struct CLexer_Ctx_struct * ctx);
    void (*mT93)	(struct CLexer_Ctx_struct * ctx);
    void (*mT94)	(struct CLexer_Ctx_struct * ctx);
    void (*mT95)	(struct CLexer_Ctx_struct * ctx);
    void (*mT96)	(struct CLexer_Ctx_struct * ctx);
    void (*mT97)	(struct CLexer_Ctx_struct * ctx);
    void (*mT98)	(struct CLexer_Ctx_struct * ctx);
    void (*mT99)	(struct CLexer_Ctx_struct * ctx);
    void (*mT100)	(struct CLexer_Ctx_struct * ctx);
    void (*mT101)	(struct CLexer_Ctx_struct * ctx);
    void (*mT102)	(struct CLexer_Ctx_struct * ctx);
    void (*mT103)	(struct CLexer_Ctx_struct * ctx);
    void (*mT104)	(struct CLexer_Ctx_struct * ctx);
    void (*mT105)	(struct CLexer_Ctx_struct * ctx);
    void (*mT106)	(struct CLexer_Ctx_struct * ctx);
    void (*mT107)	(struct CLexer_Ctx_struct * ctx);
    void (*mT108)	(struct CLexer_Ctx_struct * ctx);
    void (*mT109)	(struct CLexer_Ctx_struct * ctx);
    void (*mT110)	(struct CLexer_Ctx_struct * ctx);
    void (*mT111)	(struct CLexer_Ctx_struct * ctx);
    void (*mT112)	(struct CLexer_Ctx_struct * ctx);
    void (*mT113)	(struct CLexer_Ctx_struct * ctx);
    void (*mT114)	(struct CLexer_Ctx_struct * ctx);
    void (*mT115)	(struct CLexer_Ctx_struct * ctx);
    void (*mT116)	(struct CLexer_Ctx_struct * ctx);
    void (*mT117)	(struct CLexer_Ctx_struct * ctx);
    void (*mT118)	(struct CLexer_Ctx_struct * ctx);
    void (*mT119)	(struct CLexer_Ctx_struct * ctx);
    void (*mT120)	(struct CLexer_Ctx_struct * ctx);
    void (*mT121)	(struct CLexer_Ctx_struct * ctx);
    void (*mT122)	(struct CLexer_Ctx_struct * ctx);
    void (*mT123)	(struct CLexer_Ctx_struct * ctx);
    void (*mT124)	(struct CLexer_Ctx_struct * ctx);
    void (*mT125)	(struct CLexer_Ctx_struct * ctx);
    void (*mT126)	(struct CLexer_Ctx_struct * ctx);
    void (*mT127)	(struct CLexer_Ctx_struct * ctx);
    void (*mT128)	(struct CLexer_Ctx_struct * ctx);
    void (*mT129)	(struct CLexer_Ctx_struct * ctx);
    void (*mT130)	(struct CLexer_Ctx_struct * ctx);
    void (*mT131)	(struct CLexer_Ctx_struct * ctx);
    void (*mT132)	(struct CLexer_Ctx_struct * ctx);
    void (*mT133)	(struct CLexer_Ctx_struct * ctx);
    void (*mIDENTIFIER)	(struct CLexer_Ctx_struct * ctx);
    void (*mLETTER)	(struct CLexer_Ctx_struct * ctx);
    void (*mCHARACTER_LITERAL)	(struct CLexer_Ctx_struct * ctx);
    void (*mSTRING_LITERAL)	(struct CLexer_Ctx_struct * ctx);
    void (*mSTRING_GUTS)	(struct CLexer_Ctx_struct * ctx);
    void (*mHEX_LITERAL)	(struct CLexer_Ctx_struct * ctx);
    void (*mDECIMAL_LITERAL)	(struct CLexer_Ctx_struct * ctx);
    void (*mOCTAL_LITERAL)	(struct CLexer_Ctx_struct * ctx);
    void (*mHexDigit)	(struct CLexer_Ctx_struct * ctx);
    void (*mIntegerTypeSuffix)	(struct CLexer_Ctx_struct * ctx);
    void (*mFLOATING_POINT_LITERAL)	(struct CLexer_Ctx_struct * ctx);
    void (*mExponent)	(struct CLexer_Ctx_struct * ctx);
    void (*mFloatTypeSuffix)	(struct CLexer_Ctx_struct * ctx);
    void (*mEscapeSequence)	(struct CLexer_Ctx_struct * ctx);
    void (*mOctalEscape)	(struct CLexer_Ctx_struct * ctx);
    void (*mHexEscape)	(struct CLexer_Ctx_struct * ctx);
    void (*mUnicodeEscape)	(struct CLexer_Ctx_struct * ctx);
    void (*mWS)	(struct CLexer_Ctx_struct * ctx);
    void (*mCOMMENT)	(struct CLexer_Ctx_struct * ctx);
    void (*mLINE_COMMENT)	(struct CLexer_Ctx_struct * ctx);
    void (*mLINE_COMMAND)	(struct CLexer_Ctx_struct * ctx);
    void (*mTokens)	(struct CLexer_Ctx_struct * ctx);    unsigned char * (*getGrammarFileName)();
    void	    (*free)   (struct CLexer_Ctx_struct * ctx);
        
}
    CLexer, * pCLexer;

/* Function protoypes for the lexer functions that external translation units
 * may wish to call.
 */
ANTLR3_API pCLexer CLexerNew         (pANTLR3_INPUT_STREAM     instream);/** Symbolic definitions of all the tokens that the lexer will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif 
#define T114      114
#define T115      115
#define T116      116
#define T117      117
#define T118      118
#define T119      119
#define FloatTypeSuffix      17
#define LETTER      11
#define T29      29
#define T28      28
#define T27      27
#define T26      26
#define T25      25
#define EOF      -1
#define T120      120
#define T122      122
#define T121      121
#define T124      124
#define T123      123
#define T127      127
#define T128      128
#define T125      125
#define T126      126
#define STRING_LITERAL      10
#define T129      129
#define FLOATING_POINT_LITERAL      9
#define T38      38
#define T37      37
#define T39      39
#define T131      131
#define T34      34
#define COMMENT      22
#define T130      130
#define T33      33
#define T36      36
#define T35      35
#define T30      30
#define T133      133
#define T32      32
#define T132      132
#define T31      31
#define LINE_COMMENT      23
#define IntegerTypeSuffix      15
#define CHARACTER_LITERAL      8
#define T49      49
#define T48      48
#define T100      100
#define T43      43
#define T42      42
#define T102      102
#define T41      41
#define T101      101
#define T40      40
#define T47      47
#define T46      46
#define T45      45
#define T44      44
#define T109      109
#define T107      107
#define T108      108
#define T105      105
#define WS      21
#define T106      106
#define T103      103
#define T104      104
#define T50      50
#define LINE_COMMAND      24
#define T59      59
#define T113      113
#define T52      52
#define T112      112
#define T51      51
#define T111      111
#define T54      54
#define T110      110
#define EscapeSequence      12
#define DECIMAL_LITERAL      7
#define T53      53
#define T56      56
#define T55      55
#define T58      58
#define T57      57
#define T75      75
#define T76      76
#define T73      73
#define T74      74
#define T79      79
#define T77      77
#define T78      78
#define STRING_GUTS      13
#define HexEscape      19
#define Exponent      16
#define HexDigit      14
#define T72      72
#define T71      71
#define T70      70
#define T62      62
#define T63      63
#define T64      64
#define T65      65
#define T66      66
#define T67      67
#define T68      68
#define T69      69
#define IDENTIFIER      4
#define HEX_LITERAL      5
#define T61      61
#define T60      60
#define T99      99
#define T97      97
#define T98      98
#define T95      95
#define T96      96
#define OCTAL_LITERAL      6
#define T94      94
#define Tokens      134
#define T93      93
#define T92      92
#define T91      91
#define T90      90
#define T88      88
#define T89      89
#define T84      84
#define T85      85
#define T86      86
#define T87      87
#define UnicodeEscape      20
#define T81      81
#define T80      80
#define T83      83
#define OctalEscape      18
#define T82      82
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

/* End of token definitions for CLexer
 * =============================================================================
 */
/** \} */

#endif
/* END - Note:Keep extra linefeed to satisfy UNIX systems */
