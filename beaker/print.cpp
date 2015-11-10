// Copyright (c) 2015 Andrew Sutton
// All rights reserved

#include "print.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "type.hpp"

#include <iostream>


// -------------------------------------------------------------------------- //
// Types

std::ostream&
operator<<(std::ostream& os, Type const& t)
{
  struct Fn
  {
    std::ostream& os;

    void operator()(Id_type const* t) { os << *t; }
    void operator()(Boolean_type const* t) { os << *t; }
    void operator()(Character_type const* t) { os << *t; }
    void operator()(Integer_type const* t) { os << *t; }
    void operator()(Function_type const* t) { os << *t; }
    void operator()(Block_type const* t) { os << *t; }
    void operator()(Array_type const* t) { os << *t; }
    void operator()(Reference_type const* t) { os << *t; }
    void operator()(Record_type const* t) { os << *t; }
  };

  apply(&t, Fn{os});
  return os;
}


std::ostream&
operator<<(std::ostream& os, Id_type const& t)
{
  return os << "unresolved:" << *t.symbol();
}


std::ostream&
operator<<(std::ostream& os, Boolean_type const&)
{
  return os << "bool";
}


std::ostream&
operator<<(std::ostream& os, Character_type const&)
{
  return os << "char";
}


std::ostream&
operator<<(std::ostream& os, Integer_type const&)
{
  return os << "int";
}


std::ostream&
operator<<(std::ostream& os, Function_type const& t)
{
  os << '(';
  Type_seq const& parms = t.parameter_types();
  for (auto iter = parms.begin(); iter != parms.end(); ++iter) {
    os << **iter;
    if (std::next(iter) != parms.end())
      os << ',';
  }
  os << ')';
  os << " -> " << *t.return_type();
  return os;
}


std::ostream&
operator<<(std::ostream& os, Array_type const& t)
{
  return os << *t.type() << '[' << *t.extent() << ']';
}


std::ostream&
operator<<(std::ostream& os, Block_type const& t)
{
  return os << *t.type() << "[]";
}


std::ostream&
operator<<(std::ostream& os, Reference_type const& t)
{
  return os << "ref " << *t.type();
}


// Just print the name of the type.
std::ostream&
operator<<(std::ostream& os, Record_type const& t)
{
  return os << *t.declaration()->name();
}


// -------------------------------------------------------------------------- //
// Expressions

std::ostream&
operator<<(std::ostream& os, Expr const& e)
{
  struct Fn
  {
    std::ostream& os;

    void operator()(Literal_expr const* e) { os << *e; }
    void operator()(Id_expr const* e) { os << *e; }
    void operator()(Add_expr const* e) { os << *e; }
    void operator()(Sub_expr const* e) { os << *e; }
    void operator()(Mul_expr const* e) { os << *e; }
    void operator()(Div_expr const* e) { os << *e; }
    void operator()(Rem_expr const* e) { os << *e; }
    void operator()(Neg_expr const* e) { os << *e; }
    void operator()(Pos_expr const* e) { os << *e; }
    void operator()(Eq_expr const* e) { os << *e; }
    void operator()(Ne_expr const* e) { os << *e; }
    void operator()(Lt_expr const* e) { os << *e; }
    void operator()(Gt_expr const* e) { os << *e; }
    void operator()(Le_expr const* e) { os << *e; }
    void operator()(Ge_expr const* e) { os << *e; }
    void operator()(And_expr const* e) { os << *e; }
    void operator()(Or_expr const* e) { os << *e; }
    void operator()(Not_expr const* e) { os << *e; }
    void operator()(Call_expr const* e) { os << *e; }
    void operator()(Member_expr const* e) { os << *e; }
    void operator()(Index_expr const* e) { os << *e; }
    void operator()(Value_conv const* e) { os << *e; }
    void operator()(Block_conv const* e) { os << *e; }
    void operator()(Default_init const* e) { os << *e; }
    void operator()(Copy_init const* e) { os << *e; }
  };
  apply(&e, Fn{os});
  return os;
}


std::ostream&
operator<<(std::ostream& os, Literal_expr const& e)
{
  return os << e.value();
}


std::ostream&
operator<<(std::ostream& os, Id_expr const& e)
{
  return os << e.spelling();
}


std::ostream&
operator<<(std::ostream& os, Add_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Sub_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Mul_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Div_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Rem_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Neg_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Pos_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Eq_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Ne_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Lt_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Gt_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Le_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Ge_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, And_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Or_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Not_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Call_expr const&)
{
  return os;
}


std::ostream&
operator<<(std::ostream& os, Member_expr const& e)
{
  return os << *e.scope() << '.' << *e.member();
}


std::ostream&
operator<<(std::ostream& os, Index_expr const& e)
{
  return os << *e.array() << '[' << *e.index() << ']';
}


std::ostream&
operator<<(std::ostream& os, Value_conv const& e)
{
  return os << "__to_value("
            << *e.source() << ','
            << *e.target() << ')';
}


std::ostream&
operator<<(std::ostream& os, Block_conv const& e)
{
  return os << "__to_block("
            << *e.source() << ','
            << *e.target() << ')';
}


std::ostream&
operator<<(std::ostream& os, Default_init const& e)
{
  // return os;
  return os << "__default_init(" << *e.type() << ")";
}


std::ostream&
operator<<(std::ostream& os, Copy_init const& e)
{
  return os << "__copy_init(" << *e.type() << ',' << *e.value() << ")";
  // return os << *e.value();
}
