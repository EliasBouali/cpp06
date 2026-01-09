#include "ScalarConverter.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cctype>
#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <limits>

ScalarConverter::ScalarConverter()
{}

ScalarConverter::ScalarConverter(const ScalarConverter &other)
{
  (void)other;
}

ScalarConverter& ScalarConverter::operator=(const ScalarConverter &other)
{
  (void)other;
  return *this;
}

ScalarConverter::~ScalarConverter(){}

enum eType
{
  T_CHAR,
  T_INT,
  T_FLOAT,
  T_DOUBLE,
  T_PSEUDO,
  T_INVALID
};

static bool is_nan(double d)
{
  return (d != d);
}

static bool is_pos_inf(double d)
{
  if (!std::numeric_limits<double>::has_infinity)
    return false;
  return d == std::numeric_limits<double>::infinity();
}

static bool is_neg_inf(double d)
{
  if (!std::numeric_limits<double>::has_infinity)
    return false;
  return d == -std::numeric_limits<double>::infinity();
}

static bool is_inf(double d)
{
  return is_pos_inf(d) || is_neg_inf(d);
}

static bool is_pseudo(const std::string &s)
{
  return (s == "nan" || s == "+inf" || s == "-inf" || s == "nanf" || s == "+inff" || s == "-inff");
}

static bool is_single_quoted_char(const std::string &s)
{
  return (s.size() == 3 && s[0] == '\'' && s[2] == '\'');
}

static bool is_single_char(const std::string &s)
{
  return (s.size() == 1 && std::isalpha(static_cast<unsigned char>(s[0])));
}

static bool parse_double_strict(const std::string& s, double& out)
{
  errno = 0;
  char* first_invalid = 0;
  const char* cstr = s.c_str();
  out = std::strtod(cstr, &first_invalid);

  if (first_invalid == cstr)
      return false;
  if (*first_invalid != '\0')
      return false;
  if (errno == ERANGE)
      std::cerr << "Warning: overflow or underflow during parsing\n";
  return true;
}

static eType detect_number_type(const std::string& s)
{
  if (s.empty())
    return T_INVALID;
  std::size_t i = 0;
  if (s[i] == '+' || s[i] == '-')
    i++;
  if (i >= s.size())
    return T_INVALID;
  bool has_digits_before = false;
  bool has_digits_after = false;
  bool has_dot = false;

  while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i])))
  {
    has_digits_before = true;
    i++;
  }

  if (i < s.size() && s[i] == '.')
  {
    has_dot = true;
    i++;
    while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i])))
    {
      has_digits_after = true;
      i++;
    }
  }
  if (!has_digits_before && !has_digits_after)
    return T_INVALID;
  if (i == s.size())
  {
    if (has_dot)
      return T_DOUBLE;
    return T_INT;
  }
  if (i == s.size() - 1 && s[i] == 'f' && has_dot)
    return T_FLOAT;
  return T_INVALID;
}

static bool is_integral_value(double d)
{
  if (is_nan(d) || is_inf(d))
      return false;
  return d == std::floor(d); // retourne le plus grand entier sous forme de double
}

static void print_char(double d)
{
  std::cout << "char: ";
  if (is_nan(d) || is_inf(d))
  {
    std::cout << "impossible\n";
    return ;
  }
  if (d < static_cast<double>(std::numeric_limits<char>::min()) || d > static_cast<double>(std::numeric_limits<char>::max()))
  {
    std::cout << "impossible\n";
    return ;
  }
  char c = static_cast<char>(d);
  if (!std::isprint(static_cast<unsigned char>(c)))
  {
    std::cout << "non displayable\n";
    return ;
  }
  std::cout << "'" << c << "'" << "\n";
}

static void print_int(double d)
{
  std::cout << "int: ";
  if (is_nan(d) || is_inf(d))
  {
    std::cout << "impossible\n";
    return;
  }

  if (d < static_cast<double>(std::numeric_limits<int>::min()) ||
      d > static_cast<double>(std::numeric_limits<int>::max()))
  {
    std::cout << "impossible\n";
    return;
  }
  int n = static_cast<int>(d);
  std::cout << n << "\n";
}


static void print_float(double d)
{
  std::cout << "float: ";
  if (is_nan(d))
  {
      std::cout << "nanf\n";
      return;
  }
  if (is_pos_inf(d))
  {
      std::cout << "+inff\n";
      return;
  }
  if (is_neg_inf(d))
  {
      std::cout << "-inff\n";
      return;
  }

  const double fmax = static_cast<double>(std::numeric_limits<float>::max());
  if (d > fmax)
  {
      std::cout << "+inff\n";
      return;
  }
  if (d < -fmax)
  {
      std::cout << "-inff\n";
      return;
  }

  float f = static_cast<float>(d);
  std::ostringstream oss;

  double ad = ::fabs(d);

  if (is_integral_value(d))
  {
    if (ad >= 1e6)
      oss << std::scientific << std::setprecision(1) << f; 
    else
      oss << std::fixed << std::setprecision(1) << f;
  }
  else
      oss << std::setprecision(std::numeric_limits<float>::digits10 + 1) << f;

  std::cout << oss.str() << "f\n";
}

static void print_double(double d)
{
    std::cout << "double: ";

    if (is_nan(d))
    {
        std::cout << "nan\n";
        return;
    }
    if (is_pos_inf(d))
    {
        std::cout << "+inf\n";
        return;
    }
    if (is_neg_inf(d))
    {
        std::cout << "-inf\n";
        return;
    }

    std::ostringstream oss;

    double ad = ::fabs(d);

    if (is_integral_value(d))
    {
        if (ad >= 1e6)
            oss << std::scientific << std::setprecision(1) << d;
        else
            oss << std::fixed << std::setprecision(1) << d;
    }
    else
    {
        oss << std::setprecision(std::numeric_limits<double>::digits10 + 1) << d;
    }

    std::cout << oss.str() << "\n";
}


static void print_all_from_double(double d)
{
  print_char(d);
  print_int(d);
  print_float(d);
  print_double(d);
}


void ScalarConverter::convert(const std::string& literal)
{
  if (literal.empty())
  {
      std::cout << "char: impossible\n";
      std::cout << "int: impossible\n";
      std::cout << "float: impossible\n";
      std::cout << "double: impossible\n";
      return;
  }

  if (is_pseudo(literal))
  {
    std::cout << "char: impossible\n";
    std::cout << "int: impossible\n";

    if (literal == "nan")
    {
      std::cout << "float: nanf\n";
      std::cout << "double: nan\n";
    }
    else if (literal == "nanf")
    {
      std::cout << "float: nanf\n";
      std::cout << "double: nan\n";
    }
    else if (literal == "+inf")
    {
      std::cout << "float: +inff\n";
      std::cout << "double: +inf\n";
    }
    else if (literal == "-inf")
    {
        std::cout << "float: -inff\n";
        std::cout << "double: -inf\n";
    }
    else if (literal == "+inff")
    {
        std::cout << "float: +inff\n";
        std::cout << "double: +inf\n";
    }
    else if (literal == "-inff")
    {
        std::cout << "float: -inff\n";
        std::cout << "double: -inf\n";
    }
    return;
  }

  if (is_single_quoted_char(literal))
  {
      double d = static_cast<double>(literal[1]);
      print_all_from_double(d);
      return;
  }
  if (is_single_char(literal))
  {
      double d = static_cast<double>(literal[0]);
      print_all_from_double(d);
      return;
  }

  eType t = detect_number_type(literal);
  if (t == T_INVALID)
  {
      std::cout << "char: impossible\n";
      std::cout << "int: impossible\n";
      std::cout << "float: impossible\n";
      std::cout << "double: impossible\n";
      return;
  }

  std::string to_parse = literal;
  if (t == T_FLOAT)
      to_parse = literal.substr(0, literal.size() - 1);

  double d;
  if (!parse_double_strict(to_parse, d))
  {
      std::cout << "char: impossible\n";
      std::cout << "int: impossible\n";
      std::cout << "float: impossible\n";
      std::cout << "double: impossible\n";
      return;
  }

  print_all_from_double(d);
}
