// Copyright (c) 2015 Andrew Sutton
// All rights reserved

#include "evaluator.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "stmt.hpp"

#include <iostream>


Value
Evaluator::eval(Expr const* e)
{
  struct Fn
  {
    Evaluator& ev;

    Value operator()(Literal_expr const* e) { return ev.eval(e); }
    Value operator()(Id_expr const* e) { return ev.eval(e); }
    Value operator()(Add_expr const* e) { return ev.eval(e); }
    Value operator()(Sub_expr const* e) { return ev.eval(e); }
    Value operator()(Mul_expr const* e) { return ev.eval(e); }
    Value operator()(Div_expr const* e) { return ev.eval(e); }
    Value operator()(Rem_expr const* e) { return ev.eval(e); }
    Value operator()(Neg_expr const* e) { return ev.eval(e); }
    Value operator()(Pos_expr const* e) { return ev.eval(e); }
    Value operator()(Eq_expr const* e) { return ev.eval(e); }
    Value operator()(Ne_expr const* e) { return ev.eval(e); }
    Value operator()(Lt_expr const* e) { return ev.eval(e); }
    Value operator()(Gt_expr const* e) { return ev.eval(e); }
    Value operator()(Le_expr const* e) { return ev.eval(e); }
    Value operator()(Ge_expr const* e) { return ev.eval(e); }
    Value operator()(And_expr const* e) { return ev.eval(e); }
    Value operator()(Or_expr const* e) { return ev.eval(e); }
    Value operator()(Not_expr const* e) { return ev.eval(e); }
    Value operator()(Call_expr const* e) { return ev.eval(e); }
  };

  return apply(e, Fn {*this});
}


Value 
Evaluator::eval(Literal_expr const* e)
{
  Symbol const* s = e->symbol();
  if (Boolean_sym const* b = as<Boolean_sym>(s))
    return b->value();
  if (Integer_sym const* z = as<Integer_sym>(s))
    return z->value();
  throw std::runtime_error("ill-formed literal");
}


Value
Evaluator::eval(Id_expr const* e)
{
  return stack.lookup(e->symbol())->second;
}


// TODO: Detect overflow.
Value
Evaluator::eval(Add_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  return v1.r.z + v2.r.z;
}


// TODO: Detect overflow.
Value
Evaluator::eval(Sub_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  return v1.r.z - v2.r.z;
}


// TODO: Detect overflow.
Value
Evaluator::eval(Mul_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  return v1.r.z * v2.r.z;
}


Value
Evaluator::eval(Div_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  if (v2.r.z == 0)
    throw std::runtime_error("division by 0");
  return v1.r.z / v2.r.z;
}


Value
Evaluator::eval(Rem_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  if (v2.r.z == 0)
    throw std::runtime_error("division by 0");
  return v1.r.z / v2.r.z;
}


Value
Evaluator::eval(Neg_expr const* e)
{
  Value v = eval(e->operand());
  return -v.r.z;
}


Value
Evaluator::eval(Pos_expr const* e)
{
  return eval(e->operand());
}


// Compare two integer or function values.
template<typename F>
bool 
compare_equal(Value const& v1, Value const& v2, F fn)
{
  if (v1.k == v2.k) {
    if (v1.k == integer_value)
      return fn(v1.r.z, v2.r.z);
    if (v1.k == function_value)
      return fn(v1.r.f, v2.r.f);
    throw std::runtime_error("imcomparable values");
  }
  throw std::runtime_error("invalid operands");
}


Value
Evaluator::eval(Eq_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  return compare_equal(v1, v2, std::equal_to<>());
}


// Compare two integer or function values.
Value
Evaluator::eval(Ne_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  return compare_equal(v1, v2, std::not_equal_to<>());
}


// Compare two integer or function values.
template<typename F>
bool 
compare_less(Value const& v1, Value const& v2, F fn)
{
  if (v1.k == v2.k) {
    if (v1.k == integer_value)
      return fn(v1.r.z, v2.r.z);
    throw std::runtime_error("imcomparable values");
  }
  throw std::runtime_error("invalid operands");
}

// Order two integer values.
Value
Evaluator::eval(Lt_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  return compare_less(v1, v2, std::less<>());
}


Value
Evaluator::eval(Gt_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  return compare_less(v1, v2, std::greater<>());
}


Value
Evaluator::eval(Le_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  return compare_less(v1, v2, std::less_equal<>());
}


Value
Evaluator::eval(Ge_expr const* e)
{
  Value v1 = eval(e->left());
  Value v2 = eval(e->right());
  return compare_less(v1, v2, std::greater_equal<>());
}


Value
Evaluator::eval(And_expr const* e)
{
  Value v = eval(e->left());
  if (!v.r.z)
    return v;
  else
    return eval(e->right());
}


Value
Evaluator::eval(Or_expr const* e)
{
  Value v = eval(e->left());
  if (v.r.z)
    return v;
  else
    return eval(e->right());
}


Value
Evaluator::eval(Not_expr const* e)
{
  Value v = eval(e->operand());
  return !v.r.z;
}


Value
Evaluator::eval(Call_expr const* e)
{
  // Evaluate the function expression.
  Value v = eval(e->target());
  Function_decl const* f = v.r.f;

  // Evaluate each argument in turn.
  Value_seq args;
  args.reserve(e->arguments().size());
  for (Expr const* a : e->arguments())
    args.push_back(eval(a));

  // Build the new call frame by pushing bindings
  // from each parameter to the corresponding argument.
  //
  // FIXME: Since everything type-checked, these *must*
  // happen to magically line up. However, it would be
  // a good idea to verify.
  Store_sentinel frame(*this);
  for (std::size_t i = 0; i < args.size(); ++i) {
    Decl* p = f->parameters()[i];
    Value& v = args[i];
    stack.top().bind(p->name(), v);
  }

  // Evaluate the function definition.
  //
  // TODO: Check result in case we've thrown
  // an exception (for example).
  Value result;
  Control ctl = eval(f->body(), result);
  if (ctl != return_ctl)
    throw std::runtime_error("function evaluation failed");

  return result;
}


// -------------------------------------------------------------------------- //
// Evaluation of declarations

void
Evaluator::eval(Decl const* d)
{
  struct Fn
  {
    Evaluator& ev;

    void operator()(Variable_decl const* d) { ev.eval(d); }
    void operator()(Function_decl const* d) { ev.eval(d); }
    void operator()(Parameter_decl const* d) { ev.eval(d); }
    void operator()(Module_decl const* d) { ev.eval(d); }
  };

  return apply(d, Fn{*this});
}


void
Evaluator::eval(Variable_decl const* d)
{
  Value v = eval(d->init());
  stack.top().bind(d->name(), v);
}


// Bind the symbol to a function value.
void
Evaluator::eval(Function_decl const* d)
{
  stack.top().bind(d->name(), d);
}


// This function should never be called.
void
Evaluator::eval(Parameter_decl const*)
{
  return;
}

void
Evaluator::eval(Module_decl const* d)
{
  Store_sentinel store(*this);
  for (Decl const* d1 : d->declarations())
    eval(d1);
}


// -------------------------------------------------------------------------- //
// Evaluation of statements

// Evaluate the given statement, returning a
// control instruction, which determines how
// the evaluation proceeds. Storage is provided
// for a return value as an output argument.
Control
Evaluator::eval(Stmt const* s, Value& r)
{
  struct Fn
  {
    Evaluator& ev;
    Value& r;

    Control operator()(Empty_stmt const* s) { return ev.eval(s, r); }
    Control operator()(Block_stmt const* s) { return ev.eval(s, r); }
    Control operator()(Return_stmt const* s) { return ev.eval(s, r); }
    Control operator()(Expression_stmt const* s) { return ev.eval(s, r); }
    Control operator()(Declaration_stmt const* s) { return ev.eval(s, r); }
  };

  return apply(s, Fn{*this, r});
}


Control
Evaluator::eval(Empty_stmt const* s, Value& r)
{
  return next_ctl;
}


Control
Evaluator::eval(Block_stmt const* s, Value& r)
{
  Store_sentinel store(*this);
  for(Stmt const* s1 : s->statements()) {
    Control ctl = eval(s1, r);
    if (ctl == return_ctl)
      return ctl;

    // TODO: Handle other control mechanisms.
  }
  return next_ctl;
}


Control
Evaluator::eval(Return_stmt const* s, Value& r)
{
  r = eval(s->value());
  return return_ctl;
}


Control
Evaluator::eval(Expression_stmt const* s, Value& r)
{
  eval(s->expression());
  return next_ctl;
}


Control
Evaluator::eval(Declaration_stmt const* s, Value& r)
{
  eval(s->declaration());
  return next_ctl;
}


// -------------------------------------------------------------------------- //
// Program execution

// Execute the given function.
//
// TODO: What if there are operands?
Value
Evaluator::exec(Function_decl const* fn)
{
  // Evaluate all of the top-level declarations in
  // order to re-establish the evaluation context.
  Store_sentinel store(*this);
  Module_decl const* m = cast<Module_decl>(fn->context());
  for (Decl const* d : m->declarations())
    eval(d);

  // TODO: Check the result code.
  Value result;
  Control ctl = eval(fn->body(), result);
  if (ctl != return_ctl)
    throw std::runtime_error("function error");

  return result;
}
