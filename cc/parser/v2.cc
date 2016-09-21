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
struct SExpression;
struct LetExpression;
struct Lambda;

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
                           x3::forward_ast<SExpression>,
                           x3::forward_ast<LetExpression>,
                           x3::forward_ast<Lambda> > {
  using base_type::base_type;
  using base_type::operator=;

  // GCC-Workaround, works in clang without this
  Term() = default;
  Term(const Term &) = default;
  Term& operator=(const Term &) = default;
};

// List (corresponding to an s-expression)
struct SExpression {
  std::vector<Term> terms;
};

// A named expression (can be a constant or a function)
struct LetExpression {
  std::string name;
  Term term;
};

// Wrapper for parameters, boost fusion edge case
struct Parameter {
  std::string value;
};

// An unnamed function
struct Lambda {
  Term term;
  std::vector<Parameter> params;
};

// A wrapper for a term
struct GlobalExpression {
  Term term;
};
}  // namespace AST

// Adapt AST structures
BOOST_FUSION_ADAPT_STRUCT(AST::TypeInt, value);
BOOST_FUSION_ADAPT_STRUCT(AST::TypeSingle, value);
BOOST_FUSION_ADAPT_STRUCT(AST::TypeDouble, value);
BOOST_FUSION_ADAPT_STRUCT(AST::GlobalExpression, term);
BOOST_FUSION_ADAPT_STRUCT(AST::SExpression, terms);
BOOST_FUSION_ADAPT_STRUCT(AST::LetExpression, name, term);
BOOST_FUSION_ADAPT_STRUCT(AST::Lambda, params, term);
BOOST_FUSION_ADAPT_STRUCT(AST::Parameter, value);

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
auto const Special         = lexeme[lit("(") | lit(")") | lit("\"") | lit("let")
                                    | lit("\\") | blank];
auto const single_         = lexeme[float_ >> lit("f")];

// Main rules corresponding to AST nodes
x3::rule<class Term, AST::Term> const Term("Term");
x3::rule<class GlobalExpression, AST::GlobalExpression> const GlobalExpression("GlobalExpression");
x3::rule<class SExpression, AST::SExpression> const SExpression("SExpression");
x3::rule<class LetExpression, AST::LetExpression> const LetExpression("LetExpression");
x3::rule<class Lambda, AST::Lambda> const Lambda("Lambda");
x3::rule<class Identifier, std::string> const Identifier("Identifier");
x3::rule<class TypeInt, AST::TypeInt> const TypeInt("TypeInt");
x3::rule<class TypeSingle, AST::TypeSingle> const TypeSingle("TypeSingle");
x3::rule<class TypeDouble, AST::TypeDouble> const TypeDouble("TypeDouble");
x3::rule<class Parameter, AST::Parameter> const Parameter("Parameter");

// Grammar for AST node rules
auto const GlobalExpression_def = Term;
auto const Term_def             = LetExpression | Lambda | SExpression | Identifier | TypeSingle
                                  | TypeDouble | TypeInt;
auto const SExpression_def      = '(' >> *Term >> ')';
auto const LetExpression_def    = '(' >> lit("let") >> Identifier >> Term >> ")";
auto const Lambda_def           = '(' >> lit("\\") >> lit("(") >> *Parameter >> lit(")") >> Term >> ")";
auto const Identifier_def       = lexeme[(char_ - digit - Special) >> *(char_ - Special)];
auto const TypeInt_def          = x3::long_long;
auto const TypeSingle_def       = single_;
auto const TypeDouble_def       = double_;
auto const Parameter_def        = Identifier;

BOOST_SPIRIT_DEFINE(Term, GlobalExpression, SExpression, LetExpression, Lambda, Identifier,
                    TypeInt, TypeSingle, TypeDouble, Parameter);
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
    void operator()(SExpression const& sexpr) const {
      std::cout << "SEXPRESSION: \n";
      for (auto const& term : sexpr.terms)
        boost::apply_visitor(*this, term);
    }
    void operator()(LetExpression const& named) const {
      std::cout << "NAMED EXPRESSION: " << named.name << "\n";
      boost::apply_visitor(*this, named.term);
    }
    void operator()(Lambda const& lambda) const {
      for (auto const& parameter : lambda.params)
        std::cout << "PARAMETER:" << parameter.value << "\n";
      boost::apply_visitor(*this, lambda.term);
    }
    void operator()(GlobalExpression const& expr) const {
      boost::apply_visitor(*this, expr.term);
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

    AST::GlobalExpression program;
    AST::printer print;
    auto &liliumLang = Grammar::GlobalExpression;
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
