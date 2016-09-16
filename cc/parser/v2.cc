#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>

#include <iostream>
#include <fstream>
#include <string>

namespace x3 = boost::spirit::x3;

/*
 * LILIUM AST base structure
 */
namespace AST {
struct List;

// Wrapper for an integer type value
struct TypeInt {
  int64_t value;
};

// Wrapper for a single type value
struct TypeSingle {
  float value;
};

// Wrapper for a double type value
struct TypeDouble {
  double value;
};

/*
 * NOTE: Using wrappers for literals with specific number type, compile
 *       errors otherwise. Possibly better solution?
 */
struct Term : x3::variant< std::string,
                           TypeSingle,
                           TypeDouble,
                           TypeInt,
                           x3::forward_ast<List> > {
  using base_type::base_type;
  using base_type::operator=;

  // GCC-Workaround, works in clang without this
  Term() = default;
  Term(const Term &) = default;
  Term& operator=(const Term &) = default;
};

// List (corresponding to an s-expression)
struct List {
  std::vector<Term> terms;
};
}  // namespace AST

// Adapt AST structures
BOOST_FUSION_ADAPT_STRUCT(AST::TypeInt, value);
BOOST_FUSION_ADAPT_STRUCT(AST::TypeSingle, value);
BOOST_FUSION_ADAPT_STRUCT(AST::TypeDouble, value);
BOOST_FUSION_ADAPT_STRUCT(AST::List, terms)

/*
 * LILIUM Grammar definition
 */
namespace Grammar {
using x3::char_;
using x3::blank;
using x3::digit;
using x3::lexeme;
using x3::lit;

// Helper rules for special chars and reals
x3::real_parser<double, x3::strict_real_policies<double> > const double_ = {};
x3::real_parser<float, x3::strict_real_policies<float> > const float_ = {};
auto const Special         = lexeme[lit("(") | lit(")") | lit("\"") | blank];
auto const single_         = lexeme[float_ >> lit("f")];

// Main rules corresponding to AST nodes
x3::rule<class SExpression, AST::List> const SExpression("SExpression");
x3::rule<class Term, AST::Term> const Term("Term");
x3::rule<class Identifier, std::string> const Identifier("Identifier");
x3::rule<class TypeInt, AST::TypeInt> const TypeInt("TypeInt");
x3::rule<class TypeSingle, AST::TypeSingle> const TypeSingle("TypeSingle");
x3::rule<class TypeDouble, AST::TypeDouble> const TypeDouble("TypeDouble");

// Grammar for AST node rules
auto const SExpression_def = '(' >> *Term >> ')';
auto const Identifier_def  = lexeme[(char_ - digit - Special) >> *(char_ - Special)];
auto const Term_def        = Identifier | TypeSingle | TypeDouble | TypeInt | SExpression;
auto const TypeInt_def     = x3::long_long;
auto const TypeSingle_def  = single_;
auto const TypeDouble_def  = double_;

BOOST_SPIRIT_DEFINE(SExpression, Term, Identifier, TypeInt, TypeSingle, TypeDouble);
}  // namespace Grammar

/*
 * LILIUM AST Evaluation
 */
namespace AST {
  struct printer {
    void operator()(std::string const& id) const {
      std::cout << "IDENTIF: " << id << "\n";
    }
    void operator()(TypeInt const& num) const {
      std::cout << "INTEGER: " << num.value << "\n";
    }
    void operator()(TypeSingle const& num) const {
      std::cout << "SINGLEP: " << num.value << "\n";
    }
    void operator()(TypeDouble const& num) const {
      std::cout << "DOUBLEP: " << num.value << "\n";
    }
    void operator()(List const& sexpr) const {
      std::cout << "SEXPRESSION: \n";
      /*for (auto const& term : sExpression.terms)
        boost::apply_visitor(*this, term);*/
      std::for_each(sexpr.terms.begin(), sexpr.terms.end(), [this](auto const& term) { boost::apply_visitor(*this, term); });
    }
  };
}  // namespace AST

// PARSER
int main() {
  typedef std::string::const_iterator iterator_type;

  std::string str;
  while (std::getline(std::cin, str)) {
    if (str.empty() || str[0] == 'q' || str[0] == 'Q')
      break;

    AST::List program;
    AST::printer print;
    auto &liliumLang = Grammar::SExpression;
    iterator_type iter = str.begin();
    iterator_type end = str.end();
    x3::ascii::space_type space;
    bool r = x3::phrase_parse(iter, end, liliumLang, space, program);

    if (r && iter == end) {
      std::cout << "Valid!\n";
      print(program);
    } else {
      std::cout << "Invalid!\n";
    }
  }
  return 0;
}
