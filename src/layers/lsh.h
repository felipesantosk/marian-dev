#include "graph/expression_graph.h"
#include <memory>

namespace faiss {
  struct IndexLSH;
}

namespace marian {

class LSH {
public:
  LSH(int k, int nbits) : k_{k} {
#if !BLAS_FOUND
    ABORT("LSH-based output approximation requires BLAS library");
#endif
  }

  Expr apply(Expr query, Expr values, Expr bias);

private:
  int k_{100};

  Expr search(Expr query, Expr values);
  Expr affine(Expr idx, Expr query, Expr values, Expr bias);
};

}