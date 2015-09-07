// Quardrtic and Linear Problem Optimizer
//
// @author: Bingqing Qu
//
// Copyright (C) 2014-2015  Bingqing Qu <sylar.qu@gmail.com>
//
// @license: See LICENSE at root directory
#ifndef OPENLINEAR_SOLVER_H_
#define OPENLINEAR_SOLVER_H_
#include "linear.hpp"
#include "formula.hpp"
namespace oplin{

enum LineSearchCondition
{
    CONSTANT,
    ARMIJO_CONDITION,
    WHOLF_CONDITION
};
/// Base class for solvers
///
///
class SolverBase
{
public:
    virtual ~SolverBase();
    virtual void solve(ProblemPtr, ParamPtr, Eigen::Ref<ColVector>&, std::vector<double>&) = 0;
    virtual double line_search(const ProblemPtr, const Eigen::Ref<ColVector>&, std::vector<double>&,
                               const Eigen::Ref<ColVector>&, const Eigen::Ref<ColVector>&, const double);
};

/// Gradint descent optimizer
///
class GradientDescent: public SolverBase
{
public:
    ~GradientDescent();
    void solve(ProblemPtr, ParamPtr, Eigen::Ref<ColVector>&, std::vector<double>&);
};

/// Stochastic gradient descent optimizer
class StochasticGD: public SolverBase
{
public:
    ~StochasticGD();
};

class L_BFGS: public SolverBase
{
public:
    ~L_BFGS();
    void solve(ProblemPtr, ParamPtr, Eigen::Ref<ColVector>&, std::vector<double>&);
};

/// Newton trust region optimizer
///
/// Reference:
/// Chih-Jen Lin, Ruby C. Weng, and S. Sathiya Keerthi.
/// Trust region Newton method for large-scale logistic regression.
/// Journal of Machine Learning Research, 9:627–650, 2008.
///
class TRON: public SolverBase
{
public:
    ~TRON();
};

} // oplin

#endif// OPENLINEAR_SOLVER_H_
