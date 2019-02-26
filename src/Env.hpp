#ifndef _Env_HPP
#define _Env_HPP

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include "Exp.hpp"
#include "Error.hpp"

class Env : Exp
{
private:
  Env* top;
  Env* parent;
  // Lifetime storage
  std::vector<std::unique_ptr<Exp>> perm;
  // Garbage Collected Storage
  std::vector<std::unique_ptr<Exp>> heap;
  std::map<std::string, Exp*> vars;
public:
  Env() = default;
  Env(Env& base);
  ~Env() = default;
  Env(const Env& exp) = default;
  Env& operator=(const Env& exp) = default;

  // Virtual Methods of Exps
  Exp* clone() override;
  void mark() override;
  ExpType getType() override;
  void applyProduction(Lexer& lexer, Env* env) override;
  Exp& eval(Env& env) override;
  Exp& select(Env& env) override;
  void print(std::ostream& stream) override;
  void accept(Visitor& vis) override;

  // Unique Methods
  void setParent(Env* env);

  void addPerm(Exp* exp);
  void addHeap(Exp* exp);

  void define(std::string var, Exp* val);
  Exp& lookup(std::string var);
  void mutate(std::string var, Exp* val);

  void extend(std::vector<std::string> vars, std::vector<Exp*> vals);
};


#endif
