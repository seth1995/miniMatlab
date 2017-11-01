// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Take the name prefix into account.
#define yylex   Cmdlex

// First part of user declarations.
#line 56 "CmdLineParser.yy" // lalr1.cc:404


#include "Macro_Defs.h"
#include "CmdLineParserUtility.h"  // Needed in addition to the previous #include ...


/*
extern int yylex(Cmd::CmdLineParser::semantic_type *yylval, \
					Cmd::CmdLineParser::location_type *yylloc);
*/					


#line 51 "./CmdLineParser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "CmdLineParser.hpp"

// User implementation prologue.

#line 65 "./CmdLineParser.cpp" // lalr1.cc:412


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace Cmd {
#line 151 "./CmdLineParser.cpp" // lalr1.cc:479

  /// Build a parser object.
  CmdLineParser::CmdLineParser (CmdLineParserUtility *parserUtil_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      parserUtil (parserUtil_yyarg)
  {}

  CmdLineParser::~CmdLineParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  CmdLineParser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  CmdLineParser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  CmdLineParser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  CmdLineParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  CmdLineParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  CmdLineParser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  CmdLineParser::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  CmdLineParser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  CmdLineParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  CmdLineParser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  CmdLineParser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  CmdLineParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  CmdLineParser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  CmdLineParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  CmdLineParser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  CmdLineParser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  CmdLineParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  CmdLineParser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  CmdLineParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  CmdLineParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  CmdLineParser::symbol_number_type
  CmdLineParser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  CmdLineParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  CmdLineParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  CmdLineParser::stack_symbol_type&
  CmdLineParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  CmdLineParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  CmdLineParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  CmdLineParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  CmdLineParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  CmdLineParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  CmdLineParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  CmdLineParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  CmdLineParser::debug_level_type
  CmdLineParser::debug_level () const
  {
    return yydebug_;
  }

  void
  CmdLineParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline CmdLineParser::state_type
  CmdLineParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  CmdLineParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  CmdLineParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  CmdLineParser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, parserUtil));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 140 "CmdLineParser.yy" // lalr1.cc:859
    {
  		int i;
  		char tmp_str[512];
  		
#ifdef TRACE_
		if ((yystack_[0].value.vect).len == 1) {
			cout << LINE_INFO;
			printf("\n[bison] Result %s = %g", (yystack_[2].value.name), (yystack_[0].value.vect).val[0]);  
			fflush(stdout);
		}
#endif

		if ((yystack_[0].value.vect).len > 0) {
			EXPR_ENTRY *entry;
			/* Get the entry once (also do saving) */
			entry = parserUtil->getVariable((yystack_[2].value.name));
			
			entry->len = (yystack_[0].value.vect).len;
			entry->val = (double *) malloc((yystack_[0].value.vect).len*sizeof(double));
			
			for (i = 0; i < (yystack_[0].value.vect).len; i++)
				entry->val[i] = (yystack_[0].value.vect).val[i];
				
			/* If the expr has no name, then release the memory (no need to save) */
			if (!(yystack_[0].value.vect).name)  free((double *) (yystack_[0].value.vect).val);

			/*
			sprintf(tmp_str, "  %s = ", $1);
			parserUtil->dumpResultString(tmp_str);
			parserUtil->printValueList(entry->val, entry->len);
			*/
			parserUtil->dumpVarEntryToResult(entry);
		}
		else {
			sprintf(tmp_str, "  %s = no result", (yystack_[2].value.name));
			parserUtil->dumpResultString(tmp_str);
			//parserUtil->dumpResultString("No result.");
		}
	}
#line 631 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 180 "CmdLineParser.yy" // lalr1.cc:859
    {
		int i;
		char tmp_str[512];

		if ((yystack_[0].value.vect).len > 0) 
		{
			if ((yystack_[0].value.vect).name) {  /* the expr has a name */
				/* such as typing "<ID>;" */
               	sprintf(tmp_str, "  %s = ", (yystack_[0].value.vect).name);
				parserUtil->dumpResultString(tmp_str);
				parserUtil->dumpValueListToResult((yystack_[0].value.vect).val, (yystack_[0].value.vect).len);
			} 
			else { 	/* the expr does not have a name */
				/* Assign it to the var "ans" */
				parserUtil->ansEntry = parserUtil->getVariable("ans");
				/* Release the old memory */
				if (parserUtil->ansEntry->val) 	
					free((double *) parserUtil->ansEntry->val);
				parserUtil->ansEntry->len = (yystack_[0].value.vect).len;
				/* Allocate new memory */
				parserUtil->ansEntry->val = (double *) malloc((yystack_[0].value.vect).len*sizeof(double));
				for (i = 0; i < (yystack_[0].value.vect).len; i++) 	
					parserUtil->ansEntry->val[i] = (yystack_[0].value.vect).val[i];
				/* Release the memory */
				free((double *) (yystack_[0].value.vect).val);

				/*
               	sprintf(tmp_str, "  ans = ");
				parserUtil->dumpResultString(tmp_str);
				parserUtil->printValueList(parserUtil->ansEntry->val, 
						parserUtil->ansEntry->len);
				*/
				parserUtil->dumpVarEntryToResult(parserUtil->ansEntry);
			}
		}
	}
#line 672 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 217 "CmdLineParser.yy" // lalr1.cc:859
    {
			//printf("\n<plot> command");  fflush(stdout);
            //sprintf(tmp_str, "\nPlotting done");
            //sprintf(tmp_str, " "); dumpResultString(tmp_str);
	}
#line 682 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 223 "CmdLineParser.yy" // lalr1.cc:859
    {
	}
#line 689 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 226 "CmdLineParser.yy" // lalr1.cc:859
    {
	}
#line 696 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 232 "CmdLineParser.yy" // lalr1.cc:859
    {
		int i, len=0;
		char tmp_str[512];
		
#ifdef TRACE_
		if ((yystack_[2].value.vect).len && (yystack_[0].value.vect).len) {
			cout << LINE_INFO;
			printf("Adding <%g> and <%g>", (yystack_[2].value.vect).val[0], (yystack_[0].value.vect).val[0]);
			printf("\n$1.len = %d and $3.len = %d", (yystack_[2].value.vect).len, (yystack_[0].value.vect).len);
		}
#endif		
		if ((yystack_[2].value.vect).len == 0 || (yystack_[0].value.vect).len == 0) {
				sprintf(tmp_str, "  Invalid operand\n");
				parserUtil->dumpResultString(tmp_str);
				(yylhs.value.vect).len = 0;
				(yylhs.value.vect).val = NULL;
		} 
		else {
  			if ((yystack_[1].value.ch) == '+') {
				//cout << LINE_INFO << "Adding two numbers" << endl;
				
				if ((yystack_[2].value.vect).len == (yystack_[0].value.vect).len) {
					len = (yystack_[2].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++) {
						(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[i] + (yystack_[0].value.vect).val[i]; 
						/*
						cout << LINE_INFO;
						printf("i = %d; Sum = %g", i, $$.val[i]);
						*/
					}
				} else if ((yystack_[2].value.vect).len == 1 && (yystack_[0].value.vect).len > 1) {
					len = (yystack_[0].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[0] + (yystack_[0].value.vect).val[i]; 
				} else if ((yystack_[2].value.vect).len > 1 && (yystack_[0].value.vect).len == 1) {
					len = (yystack_[2].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[i] + (yystack_[0].value.vect).val[0]; 
				} else {
					//cout << LINE_INFO << "Code is here" << endl;
					sprintf(tmp_str, "  Unmatched dimensions\n");
					parserUtil->dumpResultString(tmp_str); 
					(yylhs.value.vect).name = NULL;
					(yylhs.value.vect).len = 0;
					(yylhs.value.vect).val = NULL;
					//yyerrok;
				}
			} 
			else if ((yystack_[1].value.ch) == '-') {          
				if ((yystack_[2].value.vect).len == (yystack_[0].value.vect).len) {
					len = (yystack_[2].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[i] - (yystack_[0].value.vect).val[i]; 
				} else if ((yystack_[2].value.vect).len == 1) {
					len = (yystack_[0].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[0] - (yystack_[0].value.vect).val[i]; 
				} else if ((yystack_[0].value.vect).len == 1) {
					len = (yystack_[2].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[i] - (yystack_[0].value.vect).val[0]; 
				} else {
					sprintf(tmp_str, "  Unmatched dimensions\n");
					parserUtil->dumpResultString(tmp_str); 
					(yylhs.value.vect).name = NULL;
					(yylhs.value.vect).len = 0;
					(yylhs.value.vect).val = NULL;
					//yyerrok;
				}
			}
			(yylhs.value.vect).len = len;
		}
		(yylhs.value.vect).name = NULL;

		/* Release the memory if needed */
		if (!(yystack_[2].value.vect).name && (yystack_[2].value.vect).val)  free((double *) (yystack_[2].value.vect).val);
		if (!(yystack_[0].value.vect).name && (yystack_[0].value.vect).val)  free((double *) (yystack_[0].value.vect).val);
		}
#line 785 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 316 "CmdLineParser.yy" // lalr1.cc:859
    { 
			//cout << LINE_INFO << "Passing T --> E" << endl;
			(yylhs.value.vect) = (yystack_[0].value.vect); 
		}
#line 794 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 322 "CmdLineParser.yy" // lalr1.cc:859
    {
		int i, len=0;
		char tmp_str[512];
		/* Check the validity of the operands */
		if ((yystack_[2].value.vect).len == 0 || (yystack_[0].value.vect).len == 0) {
			cout << LINE_INFO << endl;
			sprintf(tmp_str, "Invalid operand");
			parserUtil->dumpResultString(tmp_str);
			(yylhs.value.vect).len = 0;
			(yylhs.value.vect).val = NULL;
		} 
		else {
  			if ((yystack_[1].value.ch) == '*') { 
				if ((yystack_[2].value.vect).len == (yystack_[0].value.vect).len) {
					len = (yystack_[2].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[i] * (yystack_[0].value.vect).val[i]; 
				} else if ((yystack_[2].value.vect).len == 1) {
					len = (yystack_[0].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[0] * (yystack_[0].value.vect).val[i]; 
				} else if ((yystack_[0].value.vect).len == 1) {
					len = (yystack_[2].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++)
						(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[i] * (yystack_[0].value.vect).val[0]; 
				} else {
					sprintf(tmp_str, "\nUnmatched dimensions");
					parserUtil->dumpResultString(tmp_str); 
				}
  			} 
  			else if ((yystack_[1].value.ch) == '/') { /* for division '/' */
#ifdef TRACE_
				cout << LINE_INFO;
				printf("Dividing <%g> and <%g>", (yystack_[2].value.vect).val[0], (yystack_[0].value.vect).val[0]);
#endif				
				if ((yystack_[2].value.vect).len == (yystack_[0].value.vect).len) { /* vec / vec */
					len = (yystack_[2].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++) {
						if ((yystack_[0].value.vect).val[i] == 0) {
							sprintf(tmp_str, "\nDivided by zero."); 
							parserUtil->dumpResultString(tmp_str); 
							(yylhs.value.vect).val[i] = MAXFLOAT;  /* defined in <math.h>*/
						} else {
							(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[i] / (yystack_[0].value.vect).val[i]; 
						}
					}
				} else if ((yystack_[2].value.vect).len == 1) { /* scalr / vec */
					len = (yystack_[0].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					for (i = 0; i < len; i++) {
						if ((yystack_[0].value.vect).val[i] == 0) {
							sprintf(tmp_str, "\nDivided by zero."); 
							parserUtil->dumpResultString(tmp_str); 
							(yylhs.value.vect).val[i] = MAXFLOAT;  /* defined in <math.h>*/
						} else {
							(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[0] / (yystack_[0].value.vect).val[i]; 
						}
					}
				} else if ((yystack_[0].value.vect).len == 1) { /* vec / scalar */
					len = (yystack_[2].value.vect).len;
					(yylhs.value.vect).val = (double *) malloc(len*sizeof(double));
					if ((yystack_[0].value.vect).val[0] == 0) {
						sprintf(tmp_str, "\nDivided by zero."); 
						parserUtil->dumpResultString(tmp_str); 
						for (i = 0; i < len; i++) 
							(yylhs.value.vect).val[i] = MAXFLOAT;  /* defined in <math.h>*/
					} else {
						for (i = 0; i < len; i++) {
							(yylhs.value.vect).val[i] = (yystack_[2].value.vect).val[i] / (yystack_[0].value.vect).val[0]; 
						}
					}
				} else {
					sprintf(tmp_str, "\nUnmatched dimensions");
					parserUtil->dumpResultString(tmp_str); 
				}
			}
			else {
				sprintf(tmp_str, "\nUnknown mul operation"); 
				parserUtil->dumpResultString(tmp_str); 
			}
			(yylhs.value.vect).len = len;
		}
		(yylhs.value.vect).name = NULL;

		/* release memory if needed */
		if (!(yystack_[2].value.vect).name && (yystack_[2].value.vect).val)  free((double *) (yystack_[2].value.vect).val);
		if (!(yystack_[0].value.vect).name && (yystack_[0].value.vect).val)  free((double *) (yystack_[0].value.vect).val);
		}
#line 891 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 414 "CmdLineParser.yy" // lalr1.cc:859
    { 	
			//cout << LINE_INFO << "Passing G --> T" << endl;
			(yylhs.value.vect) = (yystack_[0].value.vect); 
		}
#line 900 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 420 "CmdLineParser.yy" // lalr1.cc:859
    {	
			//cout << LINE_INFO << "Passing F --> G" << endl;
 			(yylhs.value.vect) = (yystack_[0].value.vect); 
		}
#line 909 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 424 "CmdLineParser.yy" // lalr1.cc:859
    { 
			(yylhs.value.vect) = (yystack_[0].value.vect); 
			/*printf("\n---step to G"); fflush(stdout);*/
		}
#line 918 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 430 "CmdLineParser.yy" // lalr1.cc:859
    {
			int i;
			double x, x1, x2, dx;
			char tmp_str[512];
			
			/* printf("\nProcessing expression: F : F : F"); fflush(stdout);*/

			x1 = (yystack_[4].value.vect).val[0]; dx = (yystack_[2].value.vect).val[0]; x2 = (yystack_[0].value.vect).val[0];

			/* Ill cases */
			if ( (dx==0) || (dx>0 && x1>x2) || (dx<0 && x1<x2) ) { 
				(yylhs.value.vect).len = 0; 
				(yylhs.value.vect).val = NULL; 
				sprintf(tmp_str, "  Invalid vector: %g : %g : %g \n", \
					(yystack_[4].value.vect).val[0], (yystack_[2].value.vect).val[0], (yystack_[0].value.vect).val[0]);
				parserUtil->dumpResultString(tmp_str);
			} 
			else {
				x = x1;
				/* Count the vector length */
				if (dx > 0)
					for (i = 0; x < x2+dx*0.5; x += dx, ++i);
					// +dx*0.5 to correct floating point error
				else
					for (i = 0; x > x2+dx*0.5; x += dx, ++i);
					
				//printf("\ni = %d; dx = %f; x = %f", i, dx, x); fflush(stdout);
				
				(yylhs.value.vect).len = i;
				(yylhs.value.vect).val = (double *)malloc(i*sizeof(double));
				for (i = 0; i < (yylhs.value.vect).len; i++)  
					(yylhs.value.vect).val[i] = x1+i*dx;
			}
			(yylhs.value.vect).name = NULL;

			/* Release the memory if needed */
			if (!(yystack_[4].value.vect).name)  free((double *) (yystack_[4].value.vect).val);
			if (!(yystack_[2].value.vect).name)  free((double *) (yystack_[2].value.vect).val);
			if (!(yystack_[0].value.vect).name)  free((double *) (yystack_[0].value.vect).val);
		}
#line 963 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 470 "CmdLineParser.yy" // lalr1.cc:859
    {
			int i;
			double x, x1, x2, dx = 1;
			char tmp_str[512];
			/* printf("\nProcessing pattern E : E"); fflush(stdout);*/
			x1 = (yystack_[2].value.vect).val[0]; x2 = (yystack_[0].value.vect).val[0];
			if (x1 > x2) { 
				sprintf(tmp_str, "  Invalid vector: %g : %g \n", \
					(yystack_[2].value.vect).val[0], (yystack_[0].value.vect).val[0]);
				parserUtil->dumpResultString(tmp_str);
				(yylhs.value.vect).len = 0; 
				(yylhs.value.vect).val = NULL; 
				/*return;*/ 
				/* don't return directly; it could cause malfunctioning */
			} else {
				x = x1;
				for (i = 0; x <= x2; x += dx, ++i);
				/*printf("\ni = %d", i); fflush(stdout);*/
				(yylhs.value.vect).len = i;
				(yylhs.value.vect).val = (double *)malloc(i*sizeof(double));
				for (i = 0; i < (yylhs.value.vect).len; i++)  
					(yylhs.value.vect).val[i] = x1+i*dx;
			}
			(yylhs.value.vect).name = NULL;

			/* release the memory if needed */
			if (!(yystack_[2].value.vect).name)  free((double *) (yystack_[2].value.vect).val);
			if (!(yystack_[0].value.vect).name)  free((double *) (yystack_[0].value.vect).val);
		}
#line 997 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 501 "CmdLineParser.yy" // lalr1.cc:859
    {
  			/* <NUM> is a double */
			(yylhs.value.vect).name = NULL;
  			(yylhs.value.vect).val = (double *) malloc(sizeof(double));
  			(yylhs.value.vect).val[0] = (yystack_[0].value.dval); 
			(yylhs.value.vect).len = 1;		/* scalar */
#ifdef TRACE_
			cout << LINE_INFO;
			printf("Parsed a number <%g>", (yystack_[0].value.dval));
#endif
		}
#line 1013 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 512 "CmdLineParser.yy" // lalr1.cc:859
    {
			char tmp_str[512];
			if (strcmp((yystack_[0].value.name), "pi") == 0) {
				(yylhs.value.vect).name = NULL;
  				(yylhs.value.vect).val = (double *) malloc(sizeof(double));
				(yylhs.value.vect).val[0] = PI;
				(yylhs.value.vect).len = 1;		/* scalar */
			} else {
				sprintf(tmp_str, "\nUnknown constant: %s ", (yystack_[0].value.name));
				parserUtil->dumpResultString(tmp_str); 
			}
		}
#line 1030 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 524 "CmdLineParser.yy" // lalr1.cc:859
    {
  			char tmp_str[512];
			sprintf(tmp_str, "\nUnknown function name: %s ", (yystack_[3].value.name));
			parserUtil->dumpResultString(tmp_str); 
			(yylhs.value.vect).name = NULL;
			(yylhs.value.vect).len = 0;
			(yylhs.value.vect).val = NULL;
		}
#line 1043 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 533 "CmdLineParser.yy" // lalr1.cc:859
    {
        	char tmp_str[512];
#ifdef TRACE_
        	cout << LINE_INFO;
        	printf("Parsed var \"%s\"", (yystack_[0].value.name));
#endif
			EXPR_ENTRY *expr_entry = parserUtil->lookupVariable((yystack_[0].value.name));
			if (! expr_entry) {
				sprintf(tmp_str, "  Undefined variable: %s\n", (yystack_[0].value.name));
				parserUtil->dumpResultString(tmp_str);
				//printf("\nUndefined variable: %s\n", $1);
				(yylhs.value.vect).name = NULL;
				(yylhs.value.vect).val = NULL;
				(yylhs.value.vect).len = 0;
			} 
			else {
#ifdef TRACE_
				cout << LINE_INFO;
				printf("Found variable %s = %g", expr_entry->name,
						expr_entry->val[0]);
#endif
				(yylhs.value.vect).name = expr_entry->name; 
				(yylhs.value.vect).val = expr_entry->val; 
				(yylhs.value.vect).len = expr_entry->len; 
			}
		}
#line 1074 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 559 "CmdLineParser.yy" // lalr1.cc:859
    {
        	/*
        	cout << LINE_INFO << "E = " << $2.val[0] << endl;
        	*/
			(yylhs.value.vect) = (yystack_[1].value.vect);
		}
#line 1085 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 565 "CmdLineParser.yy" // lalr1.cc:859
    {
  			char tmp_str[512];
			if ((yystack_[1].value.vect).len == 0) {
				sprintf(tmp_str, "\nInvalid argument");
				parserUtil->dumpResultString(tmp_str); 
				(yylhs.value.vect).name = NULL;
				(yylhs.value.vect).len = 0;
				(yylhs.value.vect).val = NULL;
			} else {
				(yylhs.value.vect) = parserUtil->evaluateFunction((yystack_[3].value.name), (yystack_[1].value.vect));
				/* Release memory if needed */
				if (!(yystack_[1].value.vect).name)  free((double *) (yystack_[1].value.vect).val);
			}
		}
#line 1104 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 579 "CmdLineParser.yy" // lalr1.cc:859
    {
			/* this ADDOP is treated as a SIGN (+/-) */
			int i;
			(yylhs.value.vect).val = (double *) malloc((yystack_[0].value.vect).len*sizeof(double));
			if ((yystack_[1].value.ch) == '+') { 
				for (i = 0; i < (yystack_[0].value.vect).len; i++) (yylhs.value.vect).val[i] =  (yystack_[0].value.vect).val[i];
			} else {          
				for (i = 0; i < (yystack_[0].value.vect).len; i++) (yylhs.value.vect).val[i] = -(yystack_[0].value.vect).val[i];
			}
			(yylhs.value.vect).name = NULL;
			(yylhs.value.vect).len = (yystack_[0].value.vect).len;

			if (!(yystack_[0].value.vect).name)  free((double *) (yystack_[0].value.vect).val);
		}
#line 1123 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 595 "CmdLineParser.yy" // lalr1.cc:859
    {
			char tmp_str[512];
  			/*printf("\nE1 has length %d; E2 has length %d", $3.len, $5.len);
			fflush(stdout);*/
			if ((yystack_[3].value.vect).len > 0 && (yystack_[3].value.vect).len == (yystack_[1].value.vect).len) {
				parserUtil->plotVector2D((yystack_[3].value.vect), (yystack_[1].value.vect));
				/* The plot command only plots to the latest active
				   figure window. If no figure exists, it creates one. */
			} 
			else if ((yystack_[3].value.vect).len != (yystack_[1].value.vect).len) {
				sprintf(tmp_str, "  The two vectors do not have equal length.\n");
				parserUtil->dumpResultString(tmp_str); 
			}
  		}
#line 1142 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 611 "CmdLineParser.yy" // lalr1.cc:859
    {
			char tmp_str[512];
  			/*printf("\n[bison] Command name: %s", $1);
			fflush(stdout);*/

			if ( (yystack_[1].value.dval) > 0 && floor((yystack_[1].value.dval)) == (yystack_[1].value.dval) ) {
				parserUtil->request_figure((yystack_[1].value.dval));
				/* If the figure is not there, make a new figure;
				   otherwise, make it active. */
			} 
			else {
				sprintf(tmp_str, "\nFigure handle must be 1, 2, ...");
				parserUtil->dumpResultString(tmp_str); 
			}
  		}
#line 1162 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 626 "CmdLineParser.yy" // lalr1.cc:859
    {
  			/*printf("\n[bison] Command name: %s", $1);
			fflush(stdout);*/

			/* Start a new figure */
			parserUtil->new_plotting_window( NULL );
			/*	sprintf(tmp_str, "\n[bison] Figure handle must be 1, 2, ...");
				dumpResultString(tmp_str); */
  		}
#line 1176 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 637 "CmdLineParser.yy" // lalr1.cc:859
    {  /* command "clear/print all" */
			if (strcmp((yystack_[1].value.name), "clear") == 0 && strcmp((yystack_[0].value.name), "all") == 0)
				parserUtil->clearAllVariables();
			else if (strcmp((yystack_[1].value.name), "print") == 0 && strcmp((yystack_[0].value.name), "all") == 0)
				parserUtil->dumpAllVariables();
		}
#line 1187 "./CmdLineParser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 645 "CmdLineParser.yy" // lalr1.cc:859
    {
			(yylhs.value.name) = strdup((yystack_[0].value.name));
		}
#line 1195 "./CmdLineParser.cpp" // lalr1.cc:859
    break;


#line 1199 "./CmdLineParser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  CmdLineParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  CmdLineParser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char CmdLineParser::yypact_ninf_ = -43;

  const signed char CmdLineParser::yytable_ninf_ = -1;

  const signed char
  CmdLineParser::yypact_[] =
  {
      17,   -43,    29,    -7,   -14,   -43,   -43,    -6,    -5,    29,
      10,    18,    34,   -43,   -43,    24,   -43,   -43,   -43,    32,
      25,   -43,    29,    29,    29,    29,    41,     4,   -43,    29,
      42,    29,    29,    44,    45,    46,   -43,    18,    27,    30,
      -1,    37,   -43,    34,    17,   -43,    40,    17,    17,    17,
      47,   -43,   -43,    29,   -43,   -43,    29,   -43,   -43,   -43,
      17,    36,   -43,   -43,   -43
  };

  const unsigned char
  CmdLineParser::yydefact_[] =
  {
      12,    21,     0,    24,     0,    22,    32,     0,    30,     0,
       0,     4,    14,    16,    18,    17,     6,     8,    10,     0,
      24,    27,     0,     0,     0,     0,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,    31,     2,     0,     0,
       0,     0,    25,    13,    12,    15,    20,    12,    12,    12,
       0,    23,    26,     0,    29,     5,     0,     7,     9,    11,
      12,     0,    19,     3,    28
  };

  const signed char
  CmdLineParser::yypgoto_[] =
  {
     -43,   -42,   -43,   -43,   -43,   -43,   -43,    -8,    31,    28,
     -43,    -2,   -43,   -43,   -43,    39
  };

  const signed char
  CmdLineParser::yydefgoto_[] =
  {
      -1,    10,    50,    30,    33,    34,    35,    11,    12,    13,
      14,    15,    16,    17,    18,    19
  };

  const unsigned char
  CmdLineParser::yytable_[] =
  {
      21,    27,    55,    29,    24,    57,    58,    59,    29,    22,
      28,    23,    25,    26,    37,    38,    39,    40,    63,    53,
       1,     2,    29,    42,     3,     4,     5,     6,     7,     8,
      46,    29,     1,     2,    29,     9,    20,     4,     5,    31,
      29,    32,     6,    23,    41,    61,    51,     9,    44,    52,
      47,    48,    49,    60,    62,    64,    54,    56,    36,    45,
      43
  };

  const unsigned char
  CmdLineParser::yycheck_[] =
  {
       2,     9,    44,     4,    18,    47,    48,    49,     4,    16,
       0,    18,    18,    18,    22,    23,    24,    25,    60,    20,
       3,     4,     4,    19,     7,     8,     9,    10,    11,    12,
      32,     4,     3,     4,     4,    18,     7,     8,     9,     5,
       4,    17,    10,    18,     3,    53,    19,    18,     6,    19,
       6,     6,     6,     6,    56,    19,    19,    17,    19,    31,
      29
  };

  const unsigned char
  CmdLineParser::yystos_[] =
  {
       0,     3,     4,     7,     8,     9,    10,    11,    12,    18,
      22,    28,    29,    30,    31,    32,    33,    34,    35,    36,
       7,    32,    16,    18,    18,    18,    18,    28,     0,     4,
      24,     5,    17,    25,    26,    27,    36,    28,    28,    28,
      28,     3,    19,    29,     6,    30,    32,     6,     6,     6,
      23,    19,    19,    20,    19,    22,    17,    22,    22,    22,
       6,    28,    32,    22,    19
  };

  const unsigned char
  CmdLineParser::yyr1_[] =
  {
       0,    21,    23,    22,    24,    22,    25,    22,    26,    22,
      27,    22,    22,    28,    28,    29,    29,    30,    30,    31,
      31,    32,    32,    32,    32,    32,    32,    32,    33,    34,
      34,    35,    36
  };

  const unsigned char
  CmdLineParser::yyr2_[] =
  {
       0,     2,     0,     6,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     3,     1,     3,     1,     1,     1,     5,
       3,     1,     1,     4,     1,     3,     4,     2,     6,     4,
       1,     2,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const CmdLineParser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "NUM", "ADDOP", "MULTOP",
  "EOL", "ID", "FUNC", "CONST", "STRING_S", "PLOTOP", "FIGUREOP",
  "CLEAR_STR", "PRINT_STR", "ALL_STR", "'='", "':'", "'('", "')'", "','",
  "$accept", "L", "$@1", "$@2", "$@3", "$@4", "$@5", "E", "T", "G", "VEC",
  "F", "PLOT", "FIGURE", "COMMAND_ALL", "token_str", YY_NULLPTR
  };


  const unsigned short int
  CmdLineParser::yyrline_[] =
  {
       0,   140,   140,   140,   180,   180,   217,   217,   223,   223,
     226,   226,   229,   232,   316,   322,   414,   420,   424,   430,
     470,   501,   512,   524,   533,   559,   565,   579,   595,   611,
     626,   637,   645
  };

  // Print the state stack on the debug stream.
  void
  CmdLineParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  CmdLineParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  CmdLineParser::token_number_type
  CmdLineParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      18,    19,     2,     2,    20,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    17,     2,
       2,    16,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15
    };
    const unsigned int user_token_number_max_ = 270;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // Cmd
#line 1565 "./CmdLineParser.cpp" // lalr1.cc:1167
#line 650 "CmdLineParser.yy" // lalr1.cc:1168



/*
	User defined (overloading) error messaging.
	This function is called when the parser encounters error.
*/
void Cmd::CmdLineParser::error(const Cmd::CmdLineParser::location_type &loc, 
								const std::string &msg)
{
	stringstream ss;
	ss << loc << ": " << msg << endl;
	cout << LINE_INFO << "\n\t" << ss.str() << endl;

	parserUtil->dumpResultString(ss.str().c_str());

	//parserUtil->printParseError(loc, msg);
}


//#include "CmdLineParserUtility.h"


#ifdef NO_BISON_MAIN

main()
{
	yyparse();
}
#endif

