//2.1
int inc_by_value(int val);

void inc_by_pointer(int *val);

void inc_by_reference(int &val);

//2.2
void swap(int *x, int *y);

void swap(int &x, int &y);

int minArrOne(const int arr[], const int size);

int minArrTwo(const int *const *arr, int n, int m);

int my_str_cmp(const char* str1, const char* str2);

int day_of_year(int day, int month, int year, int nDayTab[2][12]);

int day_of_month(int *day, int year, int nDayTab[2][12]);

void print_array(int *arr, int size);

void add_unique(int*& arr, int *size, int newValue);

int recur_summ(int n);

void var_args(int n, ...);

int *my_min(int arr[], int size);

// 7
int encoded32_size(int raw_size);

int decoded32_size(int encode_size);

int encode32(const char *raw_data, int raw_size, char *dst);

int decode32(const char *encoded_data, int encoded_size, char *dst);