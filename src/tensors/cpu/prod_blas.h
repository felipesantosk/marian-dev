#if BLAS_FOUND
#include <dnnl.h>
#endif


inline void sgemm(bool transA,
                  bool transB,
                  int rows_a,
                  int rows_b,
                  int width,
                  float alpha,
                  float* a,
                  int lda,
                  float* b,
                  int ldb,
                  float beta,
                  float* c,
                  int ldc) {
#if BLAS_FOUND
  dnnl_sgemm(transA ? 'T' : 'N', 
             transB ? 'T' : 'N', 
             (dnnl_dim_t)rows_a,
             (dnnl_dim_t)rows_b, 
             (dnnl_dim_t)width, 
             alpha, 
             a, 
             (dnnl_dim_t)lda,
             b, 
             (dnnl_dim_t)ldb, 
             beta, 
             c, 
             (dnnl_dim_t)ldc);
#else
    transA; transB; rows_a; rows_b; width; alpha; a; lda; b; ldb; beta; c; ldc; // make compiler happy
    ABORT("Marian must be compiled with a BLAS library");
#endif
}
