// This is vxl/vnl/vnl_least_squares_cost_function.cxx

//-*- c++ -*-------------------------------------------------------------------
//
// vnl_least_squares_cost_function
// Author: Andrew W. Fitzgibbon, Oxford RRG
// Created: 20 Aug 99
//
//-----------------------------------------------------------------------------

#include "vnl_least_squares_cost_function.h"

vnl_least_squares_cost_function::vnl_least_squares_cost_function(vnl_least_squares_function* f):
  vnl_cost_function(f->get_number_of_unknowns()),
  storage_(f->get_number_of_residuals()),
  jacobian_(f->get_number_of_residuals(), f->get_number_of_unknowns()),
  f_(f)
{
}

double vnl_least_squares_cost_function::f(const vnl_vector<double>& x)
{
  f_->f(x, storage_);
  return storage_.squared_magnitude();
}

void vnl_least_squares_cost_function::gradf(const vnl_vector<double>& x, vnl_vector<double>& gradient)
{
  // residuals = a, b, c, ...
  // params = x, y, z, ...
  // f = a^2 + b^2 + c^2 + ...
  // df/dx = 2a*da/dx + 2b*db/dx + ...

  if (f_->has_gradient()) {
    f_->f(x,storage_);
    f_->gradf(x, jacobian_);
    for (unsigned int c=0; c<jacobian_.columns(); ++c) {
      gradient[c] = 0.0;
      for (unsigned int r=0; r<jacobian_.rows(); ++r)
        gradient[c] += storage_[r] * jacobian_(r,c);
      gradient[c] *= 2;
    }
  }
}
