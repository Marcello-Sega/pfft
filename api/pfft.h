/*
 * Copyright (c) 2003, 2007-8 Matteo Frigo
 * Copyright (c) 2003, 2007-8 Massachusetts Institute of Technology
 * Copyright (c) 2002, 2009 Jens Keiner, Stefan Kunis, Daniel Potts
 * Copyright (c) 2010-2013 Michael Pippig
 *
 * This file is part of PFFT.
 *
 * PFFT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PFFT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PFFT.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PFFT_H
#define PFFT_H 1

#include <stdarg.h>     /* va_list */
#include <mpi.h>
#include <fftw3-mpi.h>

#ifdef __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else /* !__cplusplus */
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif /* __cplusplus */

BEGIN_C_DECLS

#define PFFT_EXTERN extern
#define PFFT_CONCAT(prefix, name) prefix ## name

/* create PFFT r2r kinds as links to appropiate FFTW r2r kinds */
#define PFFT_R2HC      FFTW_R2HC
#define PFFT_HC2R      FFTW_HC2R
#define PFFT_DHT       FFTW_DHT
#define PFFT_REDFT00   FFTW_REDFT00
#define PFFT_REDFT01   FFTW_REDFT01
#define PFFT_REDFT10   FFTW_REDFT10
#define PFFT_REDFT11   FFTW_REDFT11
#define PFFT_RODFT00   FFTW_RODFT00
#define PFFT_RODFT01   FFTW_RODFT01
#define PFFT_RODFT10   FFTW_RODFT10
#define PFFT_RODFT11   FFTW_RODFT11

#define PFFT_FORWARD   FFTW_FORWARD
#define PFFT_BACKWARD  FFTW_BACKWARD

/*
  huge second-order macro that defines prototypes for all API
  functions.  We expand this macro for each supported precision

  PX: PFFT name-mangling macro (parallel)
  X : FFTW name-mangling macro (serial)
  R : real data type
  C : complex data type
*/
#define PFFT_DEFINE_API(PX, X, R, C, INT)                                               \
                                                                                        \
  typedef struct PX(plan_s) *PX(plan);                                                  \
  typedef struct PX(gcplan_s) *PX(gcplan);                                              \
                                                                                        \
  typedef struct {                                                                      \
    int rnk_pm;                                                                         \
    int rnk_trafo;                                                                      \
    int rnk_remap;                                                                      \
    int iter;                                                                           \
    double whole;                                                                       \
    double *trafo;                                                                      \
    double *remap;                                                                      \
    double remap_3dto2d[2];                                                             \
    double itwiddle;                                                                    \
    double otwiddle;                                                                    \
  } PX(timer_s);                                                                        \
  typedef PX(timer_s) *PX(timer);                                                       \
                                                                                        \
  typedef struct {                                                                      \
    int iter;                                                                           \
    double whole;                                                                       \
    double pad_zeros;                                                                   \
    double exchange;                                                                    \
  } PX(gctimer_s);                                                                      \
  typedef PX(gctimer_s) *PX(gctimer);                                                   \
                                                                                        \
                                                                                        \
  PFFT_EXTERN void PX(init)(void);                                                      \
  PFFT_EXTERN void PX(cleanup)(void);                                                   \
                                                                                        \
  PFFT_EXTERN void *PX(malloc)(size_t n);					        \
  PFFT_EXTERN R *PX(alloc_real)(size_t n);					        \
  PFFT_EXTERN C *PX(alloc_complex)(size_t n);				                \
  PFFT_EXTERN void PX(free)(void *p);					                \
                                                                                        \
  PFFT_EXTERN void PX(execute)(PX(plan) fftplan);                                       \
  PFFT_EXTERN void PX(destroy_plan)(PX(plan) fftplan);                                  \
                                                                                        \
  PFFT_EXTERN void PX(init_input_complex_3d)(                                           \
    const INT *n, const INT *local_n, const INT *local_n_start,                         \
    C *data);                                                                           \
  PFFT_EXTERN void PX(init_input_complex)(                                              \
      int rnk_n, const INT *n, const INT *local_n, const INT *local_start,              \
      C *data);                                                                         \
                                                                                        \
  PFFT_EXTERN void PX(init_input_real_3d)(                                              \
    const INT *n, const INT *local_n, const INT *local_n_start,                         \
    R *data);                                                                           \
  PFFT_EXTERN void PX(init_input_real)(                                                 \
      int rnk_n, const INT *n, const INT *local_n, const INT *local_start,              \
      R *data);                                                                         \
                                                                                        \
  PFFT_EXTERN R PX(check_output_complex_3d)(                                            \
    const INT *n, const INT *local_n, const INT *local_n_start,                         \
    const C *data, MPI_Comm comm);                                                      \
  PFFT_EXTERN R PX(check_output_complex)(                                               \
      int rnk_n, const INT *n, const INT *local_n, const INT *local_start,              \
      const C *data, MPI_Comm comm);                                                    \
                                                                                        \
  PFFT_EXTERN R PX(check_output_real_3d)(                                               \
    const INT *n, const INT *local_n, const INT *local_n_start,                         \
    const R *data, MPI_Comm comm);                                                      \
  PFFT_EXTERN R PX(check_output_real)(                                                  \
    int rnk_n, const INT *n, const INT *local_n, const INT *local_start,                \
    const R *data, MPI_Comm comm);                                                      \
                                                                                        \
  PFFT_EXTERN void PX(local_block_3d)(                                                  \
    const INT *n, int *which_block, MPI_Comm comm_cart,                                 \
    INT *local_n, INT *local_n_start);                                                  \
                                                                                        \
  PFFT_EXTERN INT PX(local_size_dft_3d)(                                                \
      const INT *n, MPI_Comm comm_cart, unsigned pfft_flags,                            \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
  PFFT_EXTERN INT PX(local_size_dft_r2c_3d)(                                            \
      const INT *n, MPI_Comm comm_cart, unsigned pfft_flags,                            \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
  PFFT_EXTERN INT PX(local_size_dft_c2r_3d)(                                            \
      const INT *n, MPI_Comm comm_cart, unsigned pfft_flags,                            \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
  PFFT_EXTERN INT PX(local_size_r2r_3d)(                                                \
      const INT *n, MPI_Comm comm_cart, unsigned pfft_flags,                            \
      INT *local_ni, INT *local_i_start, INT *local_no, INT *local_o_start);            \
                                                                                        \
  PFFT_EXTERN INT PX(local_size_dft)(                                                   \
      int rnk, const INT *n,                                                            \
      MPI_Comm comm_cart, unsigned pfft_flags,                                          \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
  PFFT_EXTERN INT PX(local_size_dft_r2c)(                                               \
      int rnk, const INT *n,                                                            \
      MPI_Comm comm_cart, unsigned pfft_flags,                                          \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
  PFFT_EXTERN INT PX(local_size_dft_c2r)(                                               \
      int rnk, const INT *n,                                                            \
      MPI_Comm comm_cart, unsigned pfft_flags,                                          \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
  PFFT_EXTERN INT PX(local_size_r2r)(                                                   \
      int rnk_n, const INT *n,                                                          \
      MPI_Comm comm_cart, unsigned pfft_flags,                                          \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
                                                                                        \
  PFFT_EXTERN INT PX(local_size_many_dft)(                                              \
      int rnk, const INT *n, const INT *ni, const INT *no,                              \
      INT howmany, const INT *iblock, const INT *oblock,                                \
      MPI_Comm comm_cart, unsigned pfft_flags,                                          \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
  PFFT_EXTERN INT PX(local_size_many_dft_r2c)(                                          \
      int rnk_n, const INT *n, const INT *ni, const INT *no,                            \
      INT howmany, const INT *iblock, const INT *oblock,                                \
      MPI_Comm comm_cart, unsigned pfft_flags,                                          \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
  PFFT_EXTERN INT PX(local_size_many_dft_c2r)(                                          \
      int rnk_n, const INT *n, const INT *ni, const INT *no,                            \
      INT howmany, const INT *iblock, const INT *oblock,                                \
      MPI_Comm comm_cart, unsigned pfft_flags,                                          \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
  PFFT_EXTERN INT PX(local_size_many_r2r)(                                              \
      int rnk_n, const INT *n, const INT *ni, const INT *no,                            \
      INT howmany, const INT *iblock, const INT *oblock,                                \
      MPI_Comm comm_cart, unsigned pfft_flags,                                          \
      INT *local_ni, INT *local_i_start,                                                \
      INT *local_no, INT *local_o_start);                                               \
                                                                                        \
                                                                                        \
  PFFT_EXTERN PX(plan) PX(plan_dft_3d)(                                                 \
      const INT *n, C *in, C *out, MPI_Comm comm_cart,                                  \
      int sign, unsigned pfft_flags);                                                   \
  PFFT_EXTERN PX(plan) PX(plan_dft_r2c_3d)(                                             \
      const INT *n, R *in, C *out, MPI_Comm comm_cart,                                  \
      int sign, unsigned pfft_flags);                                                   \
  PFFT_EXTERN PX(plan) PX(plan_dft_c2r_3d)(                                             \
      const INT *n, C *in, R *out, MPI_Comm comm_cart,                                  \
      int sign, unsigned pfft_flags);                                                   \
  PFFT_EXTERN PX(plan) PX(plan_r2r_3d)(                                                 \
      const INT *n, R *in, R *out, MPI_Comm comm_cart,                                  \
      const PX(r2r_kind) *kinds, unsigned pfft_flags);                                  \
                                                                                        \
  PFFT_EXTERN PX(plan) PX(plan_dft)(                                                    \
      int rnk, const INT *n, C *in, C *out, MPI_Comm comm_cart,                         \
      int sign, unsigned pfft_flags);                                                   \
  PFFT_EXTERN PX(plan) PX(plan_dft_r2c)(                                                \
      int rnk_n, const INT *n, R *in, C *out, MPI_Comm comm_cart,                       \
      int sign, unsigned pfft_flags);                                                   \
  PFFT_EXTERN PX(plan) PX(plan_dft_c2r)(                                                \
      int rnk_n, const INT *n, C *in, R *out, MPI_Comm comm_cart,                       \
      int sign, unsigned pfft_flags);                                                   \
  PX(plan) PX(plan_r2r)(                                                                \
    int rnk_n, const INT *n, R *in, R *out, MPI_Comm comm_cart,                         \
    const PX(r2r_kind) *kinds, unsigned pfft_flags);                                    \
                                                                                        \
  PFFT_EXTERN PX(plan) PX(plan_many_dft)(                                               \
      int rnk, const INT *n, const INT *ni, const INT *no,                              \
      INT howmany, const INT *iblock, const INT *oblock,                                \
      C *in, C *out, MPI_Comm comm_cart,                                                \
      int sign, unsigned pfft_flags);                                                   \
  PFFT_EXTERN PX(plan) PX(plan_many_dft_r2c)(                                           \
      int rnk_n, const INT *n, const INT *ni, const INT *no,                            \
      INT howmany, const INT *iblock, const INT *oblock,                                \
      R *in, C *out, MPI_Comm comm_cart,                                                \
      int sign, unsigned pfft_flags);                                                   \
  PFFT_EXTERN PX(plan) PX(plan_many_dft_c2r)(                                           \
      int rnk_n, const INT *n, const INT *ni, const INT *no,                            \
      INT howmany, const INT *iblock, const INT *oblock,                                \
      C *in, R *out, MPI_Comm comm_cart,                                                \
      int sign, unsigned pfft_flags);                                                   \
  PFFT_EXTERN PX(plan) PX(plan_many_r2r)(                                               \
      int rnk_n, const INT *n, const INT *ni, const INT *no,                            \
      INT howmany, const INT *iblock, const INT *oblock,                                \
      R *in, R *out, MPI_Comm comm_cart,                                                \
      const PX(r2r_kind) *kinds, unsigned pfft_flags);                                  \
                                                                                        \
  PFFT_EXTERN PX(plan) PX(plan_many_dft_skipped)(                                       \
      int rnk, const INT *n, const INT *ni, const INT *no,                              \
      INT howmany, const INT *iblock, const INT *oblock,                                \
      const int *skip_trafos, C *in, C *out, MPI_Comm comm_cart,                        \
      int sign, unsigned pfft_flags);                                                   \
                                                                                        \
                                                                                        \
  PFFT_EXTERN INT PX(prod_INT)(                                                         \
      int d, const INT *vec);                                                           \
  PFFT_EXTERN INT PX(sum_INT)(                                                          \
      int d, const INT *vec);                                                           \
  PFFT_EXTERN int PX(equal_INT)(                                                        \
      int d, const INT *vec1, const INT *vec2);                                         \
  PFFT_EXTERN void PX(vcopy_INT)(                                                       \
      int d, const INT *vec1,                                                           \
      INT *vec2);                                                                       \
  PFFT_EXTERN void PX(vadd_INT)(                                                        \
      int d, const INT *vec1, const INT *vec2,                                          \
      INT *sum);                                                                        \
  PFFT_EXTERN void PX(vsub_INT)(                                                        \
      int d, const INT *vec1, const INT *vec2,                                          \
      INT *sum);                                                                        \
                                                                                        \
  void PX(apr_complex_3d)(                                                              \
      const C *data, const INT *local_n,                                    		\
      const INT *local_start, const char *name, MPI_Comm comm);                         \
  void PX(apr_complex_permuted_3d)(                                                     \
      const C *data, const INT *local_n,                                     		\
      const INT *local_start,                                                           \
      int perm0, int perm1, int perm2, const char *name, MPI_Comm comm);                \
                                                                                        \
  PFFT_EXTERN void PX(get_args)(                                                        \
      int argc, char **argv, const char *name,                                          \
      int neededArgs, unsigned type,                                                    \
      void *parameter);                                                                 \
                                                                                        \
                                                                                        \
  PFFT_EXTERN void PX(reset_timer)(                                                     \
      PX(plan) ths);                                                                    \
  PFFT_EXTERN PX(timer) PX(get_timer)(                                                  \
      const PX(plan) ths);                                                              \
  PFFT_EXTERN void PX(print_average_timer)(                                             \
      const PX(plan) ths, MPI_Comm comm);                                               \
  PFFT_EXTERN void PX(print_average_timer_adv)(                                         \
      const PX(plan) ths, MPI_Comm comm);                                               \
  PFFT_EXTERN void PX(write_average_timer)(                                             \
      const PX(plan) ths, const char *name, MPI_Comm comm);                             \
  PFFT_EXTERN void PX(write_average_timer_adv)(                                         \
      const PX(plan) ths, const char *name, MPI_Comm comm);                             \
                                                                                        \
  PFFT_EXTERN PX(timer) PX(copy_timer)(                                                 \
      const PX(timer) orig);                                                            \
  PFFT_EXTERN void PX(average_timer)(                                                   \
      PX(timer) ths);                                                                   \
  PFFT_EXTERN PX(timer) PX(add_timers)(                                                 \
      const PX(timer) sum1, const PX(timer) sum2);                                      \
  PFFT_EXTERN PX(timer) PX(reduce_max_timer)(                                           \
      const PX(timer) ths, MPI_Comm comm);                                              \
  PFFT_EXTERN double* PX(convert_timer2vec)(                                            \
      const PX(timer) ths);                                                             \
  PFFT_EXTERN PX(timer) PX(convert_vec2timer)(                                          \
      const double *times);                                                             \
  PFFT_EXTERN void PX(destroy_timer)(                                                   \
      PX(timer) ths);                                                                   \
                                                                                        \
                                                                                        \
                                                                                        \
  PFFT_EXTERN void PX(vfprintf)(                                                        \
      MPI_Comm comm, FILE *stream, const char *format, va_list ap);                     \
  PFFT_EXTERN void PX(fprintf)(                                                         \
      MPI_Comm comm, FILE *stream, const char *format, ...);                            \
  PFFT_EXTERN void PX(printf)(                                                          \
      MPI_Comm comm, const char *format, ...);                                          \
                                                                                        \
  PFFT_EXTERN int PX(create_procmesh)(                                                  \
      int rnk, MPI_Comm comm, const int *np,                                            \
      MPI_Comm *comm_cart);                                                             \
  PFFT_EXTERN int PX(create_procmesh_2d)(                                               \
      MPI_Comm comm, int np0, int np1,                                                  \
      MPI_Comm *comm_cart_2d);                                                          \
                                                                                        \
                                                                                        \
                                                                                        \
  PFFT_EXTERN INT PX(local_size_gc_3d)(                                                 \
    const INT *local_n, const INT *local_start,                                         \
    INT alloc_local, const INT *gc_below, const INT *gc_above,                          \
    INT *local_ngc, INT *local_gc_start);                                               \
  PFFT_EXTERN INT PX(local_size_gc)(                                                    \
    int rnk_n, const INT *local_n, const INT *local_start,                              \
    INT alloc_local, const INT *gc_below, const INT *gc_above,                          \
    INT *local_ngc, INT *local_gc_start);                                               \
  PFFT_EXTERN INT PX(local_size_many_gc)(                                               \
    int rnk_n, const INT *local_n, const INT *local_start, INT alloc_local,             \
    INT howmany, const INT *gc_below, const INT *gc_above,                              \
    INT *local_ngc, INT *local_gc_start);                                               \
                                                                                        \
  PFFT_EXTERN PX(gcplan) PX(plan_rgc_3d)(                                               \
      const INT *n, const INT *gc_below, const INT *gc_above,                           \
      R *data, MPI_Comm comm_cart, unsigned gc_flags);                                  \
  PFFT_EXTERN PX(gcplan) PX(plan_cgc_3d)(                                               \
    const INT *n, const INT *gc_below, const INT *gc_above,                             \
    C *data, MPI_Comm comm_cart, unsigned gc_flags);                                    \
  PFFT_EXTERN PX(gcplan) PX(plan_rgc)(                                                  \
      int rnk_n, const INT *n, const INT *gc_below, const INT *gc_above,                \
      R *data, MPI_Comm comm_cart, unsigned gc_flags);                                  \
  PFFT_EXTERN PX(gcplan) PX(plan_cgc)(                                                  \
      int rnk_n, const INT *n, const INT *gc_below, const INT *gc_above,                \
      C *data, MPI_Comm comm_cart, unsigned gc_flags);                                  \
  PFFT_EXTERN PX(gcplan) PX(plan_many_rgc)(                                             \
      int rnk_n, const INT *n, INT howmany, const INT *block,                           \
      const INT *gc_below, const INT *gc_above, R *data, MPI_Comm comm_cart,            \
      unsigned gc_flags);                                                               \
  PFFT_EXTERN PX(gcplan) PX(plan_many_cgc)(                                             \
      int rnk_n, const INT *n, INT howmany, const INT *block,                           \
      const INT *gc_below, const INT *gc_above, C *data, MPI_Comm comm_cart,            \
      unsigned gc_flags);                                                               \
                                                                                        \
  PFFT_EXTERN void PX(exchange)(                                                        \
      PX(gcplan) ths);                                                                  \
  PFFT_EXTERN void PX(reduce)(                                                          \
      PX(gcplan) ths);                                                                  \
  PFFT_EXTERN void PX(destroy_gcplan)(                                                  \
      PX(gcplan) ths);                                                                  \
                                                                                        \
  PFFT_EXTERN void PX(reset_gctimers)(                                                  \
      PX(gcplan) ths);                                                                  \
  PFFT_EXTERN PX(gctimer) PX(get_gctimer_exg)(                                          \
      const PX(gcplan) ths);                                                            \
  PFFT_EXTERN PX(gctimer) PX(get_gctimer_red)(                                          \
      const PX(gcplan) ths);                                                            \
  PFFT_EXTERN void PX(print_average_gctimer)(                                           \
      const PX(gcplan) ths, MPI_Comm comm);                                             \
  PFFT_EXTERN void PX(print_average_gctimer_adv)(                                       \
      const PX(gcplan) ths, MPI_Comm comm);                                             \
  PFFT_EXTERN void PX(write_average_gctimer)(                                           \
      const PX(gcplan) ths, const char *name, MPI_Comm comm);                           \
  PFFT_EXTERN void PX(write_average_gctimer_adv)(                                       \
      const PX(gcplan) ths, const char *name, MPI_Comm comm);                           \
                                                                                        \
  PFFT_EXTERN PX(gctimer) PX(copy_gctimer)(                                             \
      const PX(gctimer) orig);                                                          \
  PFFT_EXTERN void PX(average_gctimer)(                                                 \
      PX(gctimer) ths);                                                                 \
  PFFT_EXTERN PX(gctimer) PX(add_gctimers)(                                             \
      const PX(gctimer) sum1, const PX(gctimer) sum2);                                  \
  PFFT_EXTERN PX(gctimer) PX(reduce_max_gctimer)(                                       \
      const PX(gctimer) ths, MPI_Comm comm);                                            \
  PFFT_EXTERN void PX(convert_gctimer2vec)(                                             \
      const PX(gctimer) ths, double *times);                                            \
  PFFT_EXTERN PX(gctimer) PX(convert_vec2gctimer)(                                      \
      const double *times);                                                             \
  PFFT_EXTERN void PX(destroy_gctimer)(                                                 \
      PX(gctimer) ths);



#define PFFT_MANGLE_DOUBLE(name) PFFT_CONCAT(pfft_, name)
#define PFFT_MANGLE_FLOAT(name) PFFT_CONCAT(pfftf_, name)
#define PFFT_MANGLE_LONG_DOUBLE(name) PFFT_CONCAT(pfftl_, name)

/* provide FFTW_MANGLE_PREFIX macro */
#ifndef FFTW_MANGLE_PREFIX
# define FFTW_MANGLE_PREFIX(name)  name
#endif

typedef FFTW_MANGLE_PREFIX(fftw_complex) pfft_complex;
typedef FFTW_MANGLE_PREFIX(fftwf_complex) pfftf_complex;
typedef FFTW_MANGLE_PREFIX(fftwl_complex) pfftl_complex;
typedef FFTW_MANGLE_PREFIX(fftw_r2r_kind) pfft_r2r_kind;
typedef FFTW_MANGLE_PREFIX(fftwf_r2r_kind) pfftf_r2r_kind;
typedef FFTW_MANGLE_PREFIX(fftwl_r2r_kind) pfftl_r2r_kind;

PFFT_DEFINE_API(PFFT_MANGLE_DOUBLE, FFTW_MANGLE_DOUBLE, double, pfft_complex, ptrdiff_t)
PFFT_DEFINE_API(PFFT_MANGLE_FLOAT, FFTW_MANGLE_FLOAT, float, pfftf_complex, ptrdiff_t)
PFFT_DEFINE_API(PFFT_MANGLE_LONG_DOUBLE, FFTW_MANGLE_LONG_DOUBLE, long double, pfftl_complex, ptrdiff_t)

//#define FFTW(name) FFTW_MANGLE_DOUBLE(name)

#define PFFT_TRANSPOSED_NONE      (0U)
#define PFFT_TRANSPOSED_IN        (1U<< 0)
#define PFFT_TRANSPOSED_OUT       (1U<< 1)
#define PFFT_SHIFTED_NONE         (0U)
#define PFFT_SHIFTED_IN           (1U<< 2)
#define PFFT_SHIFTED_OUT          (1U<< 3)
#define PFFT_MEASURE              (0U)     /* default: use FFTW_MEASURE for fftw planer */
#define PFFT_ESTIMATE             (1U<< 4) /* use FFTW_ESTIMATE for fftw planer */
#define PFFT_PATIENT              (1U<< 5) /* use FFTW_PATIENT for fftw planer */
#define PFFT_EXHAUSTIVE           (1U<< 6) /* use FFTW_EXHAUSTIVE for fftw planer */
#define PFFT_NO_TUNE              (0U)     /* default: disable tuning of local FFTs and transpositions */
#define PFFT_TUNE                 (1U<< 7) /* enable tuning of local FFTs and transpositions */
#define PFFT_PRESERVE_INPUT       (1U<< 8) /* out-of-place plans do not overwrite the input */ 
#define PFFT_DESTROY_INPUT        (1U<< 9) /* default for out-of-place plans is: PRESERVE_INPUT*/
#define PFFT_BUFFERED_INPLACE     (1U<<10) /* use second array of same size, similar to out-of-place but results end up in input array */

#define PFFT_DEFAULT_BLOCK        FFTW_MPI_DEFAULT_BLOCK

#define PFFT_DEFAULT_BLOCKS       NULL
#define PFFT_NO_GCELLS            NULL

#define FPFFT_DEFAULT_BLOCKS      (-1)
#define FPFFT_NO_GCELLS           (-1)

#define PFFT_INT                  (1U)
#define PFFT_PTRDIFF_T            (2U)
#define PFFT_FLOAT                (3U)
#define PFFT_DOUBLE               (4U)
#define PFFT_UNSIGNED             (5U)

#define PFFT_GC_NONTRANSPOSED     (0U)
#define PFFT_GC_TRANSPOSED        (1U<< 0)
#define PFFT_GC_SENDRECV          (1U<< 1)
#define PFFT_GC_RMA               (1U<< 2)

END_C_DECLS

#endif /* !PFFT_H */
