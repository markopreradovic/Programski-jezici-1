#pragma once

#include "VectorSetN.h"
#include "OrderedVectorSetN.h"
#include "Eigen/Dense"
#include <iostream>

class MyMatrix {
    Eigen::MatrixXd elements;

public:
	MyMatrix(const Vector::VectorSetN& vector) {
        size_t i, j;
        elements.resize(vector.elemNum, vector.elemDimension);
		for (i = 0; i < vector.elemNum; ++i) {
			for (j = 0; j < vector.elemDimension; ++j)
				elements(i, j) = (vector[i])[j];
		}
	}

	size_t rank() const {
		Eigen::FullPivLU<Eigen::MatrixXd> luDecomp(elements);
		size_t rank = luDecomp.rank();
		return rank;
	}
};
