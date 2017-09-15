class CSRMatrix {
    public:
        class Builder;

    private:
        const double* nonzero_elements;
        const int* count_nonzero;
        const int* column_index;
        CSRMatrix(const double* nonzero_elements, const int* count_nonzero, const int* column_index) : nonzero_elements(nonzero_elements), count_nonzero(count_nonzero), column_index(column_index){}

    public:
	void print();
	double* multiply(double* vector);
};

class CSRMatrix::Builder {
    private:
        double* nonzero_elements;
        int* count_nonzero;
        int* column_index;

    public:
        Builder& setNonzeroElements(double* nonzero_elements){
            this->nonzero_elements = nonzero_elements;
            return *this;
        }

        Builder& setCountNonzero(int* count_nonzero){
            this->count_nonzero = count_nonzero;
            return *this;
        }

        Builder& setColumnIndex(int* column_index){
            this->column_index = column_index;
            return *this;
        }

        CSRMatrix build(){
            return CSRMatrix(this->nonzero_elements, this->count_nonzero, this->column_index);
        }
};
