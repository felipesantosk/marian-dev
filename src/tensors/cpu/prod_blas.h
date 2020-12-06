#if WASM_COMPATIBLE_BLAS
  #include "3rd_party/onnxjs/src/wasm-ops/gemm.h"
#else
  #include <dnnl.h>
#endif // WASM_COMPATIBLE_BLAS

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
    #if WASM_COMPATIBLE_BLAS
        gemm_f32_imp(transA, transB, rows_a, rows_b, width, alpha, a, b, beta, c);
    #else
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
    #endif
#else
    transA; transB; rows_a; rows_b; width; alpha; a; lda; b; ldb; beta; c; ldc; // make compiler happy
    ABORT("Marian must be compiled with a BLAS library");
#endif
}
