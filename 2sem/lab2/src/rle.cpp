#include <iostream>

void write(const char* input, char* output) {
    int input_len = strlen(input);
    int output_index = 0;
    int count;
    char current_char;

    for (int i = 0; i < input_len; i++) {
        current_char = input[i];
        count = 1;

        while (i + 1 < input_len && input[i] == input[i + 1]) {
            count++;
            i++;
        }

        if (count > 1) {
            int written = snprintf(output + output_index,
                                 (input_len * 3 + 1) - output_index,
                                 "%d%c", count, current_char);
            if (written < 0) break;
            output_index += written;
        } else {
            if (output_index < input_len * 3) {
                output[output_index++] = current_char;
            } else {
                break;
            }
        }
    }
    output[output_index] = '\0';
}

int main()
{
    char* input = "       \\ \n         /\\_/\\  (\n        ( ^.^ ) _)\n          \\\"/  (\n        ( | | )\n       (__d b__)";

    char* output;
    // rle_encode(input, output);
    std::cout << output;
    return 0;
}