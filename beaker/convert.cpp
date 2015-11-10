
#include "convert.hpp"
#include "type.hpp"
#include "expr.hpp"
#include "decl.hpp"

#include <iostream>


// If e has reference type T&, return a conversion
// to the value type T. Otherwise, no conversions
// are required and e is returned.
Expr*
convert_to_value(Expr* e)
{
  if (Reference_type const* t = as<Reference_type>(e->type()))
    return new Value_conv(t->nonref(), e);
  else
    return e;
}


// If e has type T[N], return a conversion to
// the chunk T[]. Otherwise, just return e.
Expr*
convert_to_block(Expr* e)
{
  if (Array_type const* a = as<Array_type>(e->type()))
    return new Block_conv(get_block_type(a->type()), e);
  else
    return e;
}


// Find a conversion from e to t. If no such
// conversion exists, return nullptr. Diagnostics
// a better handled in the calling context.
Expr*
convert(Expr* e, Type const* t)
{
  // If e has type t, no conversions are needed.
  if (e->type() == t)
    return e;
  Expr* c = e;

  // Ojbect/value transformations

  // If t is a non-reference type, try an
  // object-to-value conversion:
  //
  //    A& -> B
  if (!is<Reference_type>(t)) {
    c = convert_to_value(e);
    if (c->type() == t)
      return c;
  }

  // Type conversions

  // Determine if we can apply an array-to-chunk
  // conversion. This is the case when we have
  //
  //    A[N] -> B[]
  if (is<Block_type>(t)) {
    c = convert_to_block(c);
    if (c->type() == t)
      return c;
  }

  // If we've exhaused all possible conversions
  // without matching the type, then just return
  // nullptr.

  return nullptr;
}
