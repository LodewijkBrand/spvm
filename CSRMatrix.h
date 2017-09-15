class CSRMatrix {
    public:
        class Builder;

    private:
        const double* nonzero_elements;
        const int* count_nonzero;
        const int* column_index;
        const int num_rows, num_cols, num_nz;
        CSRMatrix(const int num_nz, const int num_rows, const int num_cols, const double* nonzero_elements, const int* count_nonzero, const int* column_index) : num_nz(num_nz), num_rows(num_rows), num_cols(num_cols), nonzero_elements(nonzero_elements), count_nonzero(count_nonzero), column_index(column_index){}

    public:
        void print();
        double* multiply(double* vector);
        double* parallel_multiply(double* vector);
        int getNumRows();
        int getNumCols();
};

class CSRMatrix::Builder {
    private:
        double* nonzero_elements;
        int* count_nonzero;
        int* column_index;
        int num_rows, num_cols, num_nz;

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

        Builder& setNumRows(int num_rows){
            this->num_rows = num_rows;
            return *this;
        }

        Builder& setNumCols(int num_cols){
            this->num_cols = num_cols;
            return *this;
        }

        Builder& setNumNonzero(int num_nz){
            this->num_nz = num_nz;
            return *this;
        }

        CSRMatrix build(){
            return CSRMatrix(this->num_nz, this->num_rows, this->num_cols, this->nonzero_elements, this->count_nonzero, this->column_index);
        }
};
