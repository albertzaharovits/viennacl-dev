#ifndef VIENNACL_GENERATOR_UTILS_HPP
#define VIENNACL_GENERATOR_UTILS_HPP

/* =========================================================================
   Copyright (c) 2010-2013, Institute for Microelectronics,
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


/** @file viennacl/generator/utils.hpp
    @brief Internal utils
*/

#include <sstream>

#include "viennacl/generator/forwards.h"
#include "viennacl/ocl/forwards.h"

#include "viennacl/scheduler/forwards.h"

#include "viennacl/traits/size.hpp"
#include "viennacl/traits/row_major.hpp"

namespace viennacl{

  namespace generator{

    namespace utils{

    static std::string numeric_type_to_string(scheduler::statement_node_numeric_type const & type){
        switch(type){
            case scheduler::FLOAT_TYPE :
                return "float";
            case scheduler::DOUBLE_TYPE :
                return  "double";
            default :
                throw generator_not_supported_exception("Unsupported Scalartype");
        }
    }

    template<class Fun>
    static typename Fun::result_type call_on_host_scalar(scheduler::lhs_rhs_element element, Fun const & fun){
        assert(element.type_family == scheduler::SCALAR_TYPE_FAMILY && bool("Must be called on a host scalar"));
        switch(element.numeric_type){
        case scheduler::FLOAT_TYPE :
            return fun(element.host_float);
        case scheduler::DOUBLE_TYPE :
            return fun(element.host_double);
        default :
            throw generator_not_supported_exception("Unsupported Scalartype");
        }
    }

    template<class Fun>
    static typename Fun::result_type call_on_scalar(scheduler::lhs_rhs_element element, Fun const & fun){
        assert(element.type_family == scheduler::SCALAR_TYPE_FAMILY && bool("Must be called on a scalar"));
        switch(element.numeric_type){
        case scheduler::FLOAT_TYPE :
            return fun(*element.scalar_float);
        case scheduler::DOUBLE_TYPE :
            return fun(*element.scalar_double);
        default :
            throw generator_not_supported_exception("Unsupported Scalartype");
        }
    }

    template<class Fun>
    static typename Fun::result_type call_on_vector(scheduler::lhs_rhs_element element, Fun const & fun){
        assert(element.type_family == scheduler::VECTOR_TYPE_FAMILY && bool("Must be called on a vector"));
        switch(element.numeric_type){
        case scheduler::FLOAT_TYPE :
            return fun(*element.vector_float);
        case scheduler::DOUBLE_TYPE :
            return fun(*element.vector_double);
        default :
            throw generator_not_supported_exception("Unsupported Scalartype");
        }
    }

    template<class Fun>
    static typename Fun::result_type call_on_implicit_vector(scheduler::lhs_rhs_element element, Fun const & fun){
        assert(element.type_family == scheduler::VECTOR_TYPE_FAMILY   && bool("Must be called on a implicit_vector"));
        assert(element.subtype     == scheduler::IMPLICIT_VECTOR_TYPE && bool("Must be called on a implicit_vector"));
        switch(element.numeric_type){
        case scheduler::FLOAT_TYPE :
            return fun(*element.implicit_vector_float);
        case scheduler::DOUBLE_TYPE :
            return fun(*element.implicit_vector_double);
        default :
            throw generator_not_supported_exception("Unsupported Scalartype");
        }
    }

    template<class Fun>
    static typename Fun::result_type call_on_matrix(scheduler::lhs_rhs_element element, Fun const & fun){
        assert(element.type_family == scheduler::MATRIX_TYPE_FAMILY && bool("Must be called on a matrix"));
        switch(element.numeric_type){
        case scheduler::FLOAT_TYPE :
            return fun(*element.matrix_float);
        case scheduler::DOUBLE_TYPE :
            return fun(*element.matrix_double);
        default :
            throw generator_not_supported_exception("Unsupported Scalartype");
        }
    }


    template<class Fun>
    static typename Fun::result_type call_on_implicit_matrix(scheduler::lhs_rhs_element element, Fun const & fun){
        assert(element.type_family == scheduler::MATRIX_TYPE_FAMILY   && bool("Must be called on a matrix_vector"));
        assert(element.subtype     == scheduler::IMPLICIT_MATRIX_TYPE && bool("Must be called on a matrix_vector"));
        switch(element.numeric_type){
        case scheduler::FLOAT_TYPE :
            return fun(*element.implicit_matrix_float);
        case scheduler::DOUBLE_TYPE :
            return fun(*element.implicit_matrix_double);
        default :
            throw generator_not_supported_exception("Unsupported Scalartype");
        }
    }

      template<class Fun>
      static typename Fun::result_type call_on_element(scheduler::lhs_rhs_element const & element, Fun const & fun){
        switch(element.type_family){
          case scheduler::SCALAR_TYPE_FAMILY:
            if (element.subtype == scheduler::HOST_SCALAR_TYPE)
              return call_on_host_scalar(element, fun);
            else
              return call_on_scalar(element, fun);
          case scheduler::VECTOR_TYPE_FAMILY :
            if (element.subtype == scheduler::IMPLICIT_VECTOR_TYPE)
              return call_on_implicit_vector(element, fun);
            else
              return call_on_vector(element, fun);
          case scheduler::MATRIX_TYPE_FAMILY:
            if (element.subtype == scheduler::IMPLICIT_MATRIX_TYPE)
              return call_on_implicit_matrix(element, fun);
            else
              return call_on_matrix(element,fun);
          default:
            throw generator_not_supported_exception("Unsupported datastructure type : Not among {Scalar, Vector, Matrix}");
        }
      }

      struct scalartype_size_fun{
          typedef std::size_t result_type;
          result_type operator()(float const &) const { return sizeof(float); }
          result_type operator()(double const &) const { return sizeof(double); }
          template<class T> result_type operator()(T const &) const { return sizeof(typename viennacl::result_of::cpu_value_type<T>::type); }
      };

      struct internal_size_fun{
          typedef std::size_t result_type;
          template<class T>
          result_type operator()(T const &t) const { return viennacl::traits::internal_size(t); }
      };

      struct handle_fun{
          typedef cl_mem result_type;
          template<class T>
          result_type operator()(T const &t) const { return t.handle().opencl_handle(); }
      };

      struct internal_size1_fun{
          typedef std::size_t result_type;
          template<class T>
          result_type operator()(T const &t) const { return viennacl::traits::internal_size1(t); }
      };

      struct internal_size2_fun{
          typedef std::size_t result_type;
          template<class T>
          result_type operator()(T const &t) const { return viennacl::traits::internal_size2(t); }
      };

      template<class T, class U>
      struct is_same_type { enum { value = 0 }; };

      template<class T>
      struct is_same_type<T,T> { enum { value = 1 }; };

      template <class T>
      inline std::string to_string ( T const t )
      {
        std::stringstream ss;
        ss << t;
        return ss.str();
      }

      inline bool is_row_major_matrix(scheduler::lhs_rhs_element const & element){
        return  (element.subtype==scheduler::DENSE_MATRIX_TYPE && element.numeric_type==scheduler::FLOAT_TYPE && viennacl::traits::row_major(*(matrix_base<float>*)element.matrix_float))
             || (element.subtype==scheduler::DENSE_MATRIX_TYPE && element.numeric_type==scheduler::DOUBLE_TYPE && viennacl::traits::row_major(*(matrix_base<double>*)element.matrix_double));
      }

      inline bool is_reduction(scheduler::op_element const & op){
        return op.type_family==scheduler::OPERATION_VECTOR_REDUCTION_TYPE_FAMILY
            || op.type_family==scheduler::OPERATION_COLUMNS_REDUCTION_TYPE_FAMILY
            || op.type_family==scheduler::OPERATION_ROWS_REDUCTION_TYPE_FAMILY;
      }

      inline bool implemented_as_kernel(scheduler::op_element const & op){
        return op.type_subfamily==scheduler::OPERATION_FUNCTION_TYPE_SUBFAMILY || is_reduction(op);
      }

      inline bool interpret_as_transposed(std::vector<scheduler::statement_node> const & array, scheduler::lhs_rhs_element const & element){
        return  is_row_major_matrix(element)

            || (element.type_family==scheduler::COMPOSITE_OPERATION_FAMILY
               && array[element.node_index].op.type==scheduler::OPERATION_UNARY_TRANS_TYPE
               && !is_row_major_matrix(array[element.node_index].lhs));
      }

      template<class T>
      struct type_to_string;
      template<> struct type_to_string<float> { static const char * value() { return "float"; } };
      template<> struct type_to_string<double> { static const char * value() { return "double"; } };


      template<class T>
      struct first_letter_of_type;
      template<> struct first_letter_of_type<float> { static char value() { return 'f'; } };
      template<> struct first_letter_of_type<double> { static char value() { return 'd'; } };

      class kernel_generation_stream : public std::ostream{
        private:

          class kgenstream : public std::stringbuf{
            public:
              kgenstream(std::ostringstream& oss,unsigned int const & tab_count) : oss_(oss), tab_count_(tab_count){ }
              int sync() {
                for(unsigned int i=0 ; i<tab_count_;++i)
                  oss_ << "    ";
                oss_ << str();
                str("");
                return !oss_;
              }
              ~kgenstream() {  pubsync(); }
            private:
              std::ostream& oss_;
              unsigned int const & tab_count_;
          };

        public:
          kernel_generation_stream() : std::ostream(new kgenstream(oss,tab_count_)), tab_count_(0){ }

          std::string str(){ return oss.str(); }

          void inc_tab(){ ++tab_count_; }

          void dec_tab(){ --tab_count_; }

          ~kernel_generation_stream(){ delete rdbuf(); }

        private:
          unsigned int tab_count_;
          std::ostringstream oss;
      };


    }

  }

}
#endif
