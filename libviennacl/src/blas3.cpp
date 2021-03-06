/* =========================================================================
   Copyright (c) 2010-2014, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.
   Portions of this software are copyright by UChicago Argonne, LLC.

                            -----------------
                  ViennaCL - The Vienna Computing Library
                            -----------------

   Project Head:    Karl Rupp                   rupp@iue.tuwien.ac.at

   (A list of authors and contributors can be found in the PDF manual)

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

// include necessary system headers
#include <iostream>

#include "viennacl.hpp"
#include "viennacl_private.hpp"

#include "init_matrix.hpp"

//include basic scalar and vector types of ViennaCL
#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/linalg/direct_solve.hpp"
#include "viennacl/linalg/prod.hpp"

// GEMV

VIENNACL_EXPORTED_FUNCTION ViennaCLStatus ViennaCLgemm(ViennaCLHostScalar alpha, ViennaCLMatrix A, ViennaCLMatrix B, ViennaCLHostScalar beta, ViennaCLMatrix C)
{
  viennacl::backend::mem_handle A_handle;
  viennacl::backend::mem_handle B_handle;
  viennacl::backend::mem_handle C_handle;

  if (init_matrix(A_handle, A) != ViennaCLSuccess)
    return ViennaCLGenericFailure;

  if (init_matrix(B_handle, B) != ViennaCLSuccess)
    return ViennaCLGenericFailure;

  if (init_matrix(C_handle, C) != ViennaCLSuccess)
    return ViennaCLGenericFailure;

  switch (A->precision)
  {
    case ViennaCLFloat:
    {
      viennacl::matrix_base<float> mat_A(A_handle,
                                         A->size1, A->start1, A->stride1, A->internal_size1,
                                         A->size2, A->start2, A->stride2, A->internal_size2, A->order == ViennaCLRowMajor);
      viennacl::matrix_base<float> mat_B(B_handle,
                                         B->size1, B->start1, B->stride1, B->internal_size1,
                                         B->size2, B->start2, B->stride2, B->internal_size2, B->order == ViennaCLRowMajor);
      viennacl::matrix_base<float> mat_C(C_handle,
                                         C->size1, C->start1, C->stride1, C->internal_size1,
                                         C->size2, C->start2, C->stride2, C->internal_size2, C->order == ViennaCLRowMajor);

      if (A->trans == ViennaCLTrans && B->trans == ViennaCLTrans)
        viennacl::linalg::prod_impl(viennacl::trans(mat_A), viennacl::trans(mat_B), mat_C, alpha->value_float, beta->value_float);
      else if (A->trans == ViennaCLTrans && B->trans == ViennaCLNoTrans)
        viennacl::linalg::prod_impl(viennacl::trans(mat_A), mat_B, mat_C, alpha->value_float, beta->value_float);
      else if (A->trans == ViennaCLNoTrans && B->trans == ViennaCLTrans)
        viennacl::linalg::prod_impl(mat_A, viennacl::trans(mat_B), mat_C, alpha->value_float, beta->value_float);
      else if (A->trans == ViennaCLNoTrans && B->trans == ViennaCLNoTrans)
        viennacl::linalg::prod_impl(mat_A, mat_B, mat_C, alpha->value_float, beta->value_float);
      else
        return ViennaCLGenericFailure;

      return ViennaCLSuccess;
    }

    case ViennaCLDouble:
    {
      viennacl::matrix_base<double> mat_A(A_handle,
                                         A->size1, A->start1, A->stride1, A->internal_size1,
                                         A->size2, A->start2, A->stride2, A->internal_size2, A->order == ViennaCLRowMajor);
      viennacl::matrix_base<double> mat_B(B_handle,
                                         B->size1, B->start1, B->stride1, B->internal_size1,
                                         B->size2, B->start2, B->stride2, B->internal_size2, B->order == ViennaCLRowMajor);
      viennacl::matrix_base<double> mat_C(C_handle,
                                         C->size1, C->start1, C->stride1, C->internal_size1,
                                         C->size2, C->start2, C->stride2, C->internal_size2, C->order == ViennaCLRowMajor);

      if (A->trans == ViennaCLTrans && B->trans == ViennaCLTrans)
        viennacl::linalg::prod_impl(viennacl::trans(mat_A), viennacl::trans(mat_B), mat_C, alpha->value_double, beta->value_double);
      else if (A->trans == ViennaCLTrans && B->trans == ViennaCLNoTrans)
        viennacl::linalg::prod_impl(viennacl::trans(mat_A), mat_B, mat_C, alpha->value_double, beta->value_double);
      else if (A->trans == ViennaCLNoTrans && B->trans == ViennaCLTrans)
        viennacl::linalg::prod_impl(mat_A, viennacl::trans(mat_B), mat_C, alpha->value_double, beta->value_double);
      else if (A->trans == ViennaCLNoTrans && B->trans == ViennaCLNoTrans)
        viennacl::linalg::prod_impl(mat_A, mat_B, mat_C, alpha->value_double, beta->value_double);
      else
        return ViennaCLGenericFailure;

      return ViennaCLSuccess;
    }

    default:
      return ViennaCLGenericFailure;
  }
}


// xTRSV

VIENNACL_EXPORTED_FUNCTION ViennaCLStatus ViennaCLtrsm(ViennaCLMatrix A, ViennaCLUplo uplo, ViennaCLDiag diag, ViennaCLMatrix B)
{
  viennacl::backend::mem_handle A_handle;
  viennacl::backend::mem_handle B_handle;

  if (init_matrix(A_handle, A) != ViennaCLSuccess)
    return ViennaCLGenericFailure;

  if (init_matrix(B_handle, B) != ViennaCLSuccess)
    return ViennaCLGenericFailure;

  switch (A->precision)
  {
    case ViennaCLFloat:
    {
      viennacl::matrix_base<float> mat_A(A_handle,
                                         A->size1, A->start1, A->stride1, A->internal_size1,
                                         A->size2, A->start2, A->stride2, A->internal_size2, A->order == ViennaCLRowMajor);
      viennacl::matrix_base<float> mat_B(B_handle,
                                         B->size1, B->start1, B->stride1, B->internal_size1,
                                         B->size2, B->start2, B->stride2, B->internal_size2, B->order == ViennaCLRowMajor);

      if (A->trans == ViennaCLTrans && B->trans == ViennaCLTrans)
      {
        if (uplo == ViennaCLUpper && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::upper_tag());
        else if (uplo == ViennaCLUpper && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::unit_upper_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::lower_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::unit_lower_tag());
        else
          return ViennaCLGenericFailure;
      }
      else if (A->trans == ViennaCLTrans && B->trans == ViennaCLNoTrans)
      {
        if (uplo == ViennaCLUpper && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), mat_B, viennacl::linalg::upper_tag());
        else if (uplo == ViennaCLUpper && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), mat_B, viennacl::linalg::unit_upper_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), mat_B, viennacl::linalg::lower_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), mat_B, viennacl::linalg::unit_lower_tag());
        else
          return ViennaCLGenericFailure;
      }
      else if (A->trans == ViennaCLNoTrans && B->trans == ViennaCLTrans)
      {
        if (uplo == ViennaCLUpper && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::upper_tag());
        else if (uplo == ViennaCLUpper && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::unit_upper_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::lower_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::unit_lower_tag());
        else
          return ViennaCLGenericFailure;
      }
      else if (A->trans == ViennaCLNoTrans && B->trans == ViennaCLNoTrans)
      {
        if (uplo == ViennaCLUpper && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(mat_A, mat_B, viennacl::linalg::upper_tag());
        else if (uplo == ViennaCLUpper && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(mat_A, mat_B, viennacl::linalg::unit_upper_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(mat_A, mat_B, viennacl::linalg::lower_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(mat_A, mat_B, viennacl::linalg::unit_lower_tag());
        else
          return ViennaCLGenericFailure;
      }

      return ViennaCLSuccess;
    }
    case ViennaCLDouble:
    {
      viennacl::matrix_base<double> mat_A(A_handle,
                                         A->size1, A->start1, A->stride1, A->internal_size1,
                                         A->size2, A->start2, A->stride2, A->internal_size2, A->order == ViennaCLRowMajor);
      viennacl::matrix_base<double> mat_B(B_handle,
                                         B->size1, B->start1, B->stride1, B->internal_size1,
                                         B->size2, B->start2, B->stride2, B->internal_size2, B->order == ViennaCLRowMajor);

      if (A->trans == ViennaCLTrans && B->trans == ViennaCLTrans)
      {
        if (uplo == ViennaCLUpper && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::upper_tag());
        else if (uplo == ViennaCLUpper && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::unit_upper_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::lower_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::unit_lower_tag());
        else
          return ViennaCLGenericFailure;
      }
      else if (A->trans == ViennaCLTrans && B->trans == ViennaCLNoTrans)
      {
        if (uplo == ViennaCLUpper && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), mat_B, viennacl::linalg::upper_tag());
        else if (uplo == ViennaCLUpper && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), mat_B, viennacl::linalg::unit_upper_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), mat_B, viennacl::linalg::lower_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), mat_B, viennacl::linalg::unit_lower_tag());
        else
          return ViennaCLGenericFailure;
      }
      else if (A->trans == ViennaCLNoTrans && B->trans == ViennaCLTrans)
      {
        if (uplo == ViennaCLUpper && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::upper_tag());
        else if (uplo == ViennaCLUpper && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::unit_upper_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::lower_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(viennacl::trans(mat_A), viennacl::trans(mat_B), viennacl::linalg::unit_lower_tag());
        else
          return ViennaCLGenericFailure;
      }
      else if (A->trans == ViennaCLNoTrans && B->trans == ViennaCLNoTrans)
      {
        if (uplo == ViennaCLUpper && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(mat_A, mat_B, viennacl::linalg::upper_tag());
        else if (uplo == ViennaCLUpper && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(mat_A, mat_B, viennacl::linalg::unit_upper_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLNonUnit)
          viennacl::linalg::inplace_solve(mat_A, mat_B, viennacl::linalg::lower_tag());
        else if (uplo == ViennaCLLower && diag == ViennaCLUnit)
          viennacl::linalg::inplace_solve(mat_A, mat_B, viennacl::linalg::unit_lower_tag());
        else
          return ViennaCLGenericFailure;
      }

      return ViennaCLSuccess;
    }

    default:
      return  ViennaCLGenericFailure;
  }
}



