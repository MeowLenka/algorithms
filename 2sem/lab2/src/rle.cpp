#include <iostream>

size_t rle_encode(const void *src, size_t n_bytes, void *dst)
{                         // write записать со сжатием
    int output_index = 0; // buf = AAAAABBBCCDNJK
    int count;
    char current_char;
    const char *buf = (char *)src;
    char *encoded_str = (char *)dst;

    for (int i = 0; i < n_bytes; i++)
    {
        current_char = buf[i];
        count = 1;

        while (i + 1 < n_bytes && buf[i] == buf[i + 1])
        {
            count++;
            i++;
        }

        if (count > 1)
        {
            int written = snprintf(encoded_str + output_index,
                                   (n_bytes * 3 + 1) - output_index,
                                   "%d%c", count, current_char);
            if (written < 0)
                break;
            output_index += written;
        }
        else
        {
            if (output_index < n_bytes * 3)
            {
                encoded_str[output_index++] = current_char;
            }
            else
            {
                break;
            }
        }
    }
    encoded_str[output_index] = '\0';
    return output_index;
}

// size_t rle_encodeee(const void *src, size_t n_bytes, char *dst)
// {
//     const char *buf = (char *)src;
//     char *encoded_str = (char *)dst;
//     size_t n_result = 0, count;

//     for (int i = 0; i < n_bytes; i++)
//     {
//         count = 1;
//         while (i + 1 < n_bytes && buf[i] == buf[i + 1])
//         {
//             count++;
//             i++;
//         }
//         if (count > 1)
//         {
//             int written = snprintf(encoded_str + n_result, (n_bytes * 3), "%d", count);
//             n_result += written;
//         }
//         encoded_str[n_result++] = buf[i];
//     }
//     return n_result;
// }

size_t rle_decode(FILE *_file, const void *dst, size_t max_bytes)
{
    char *buf = (char *)dst; // output
    char let;
    // char *output = malloc((max_bytes * 10) + 1);
    if (!buf)
        return NULL;

    int output_index = 0;
    size_t i = 0, n_result = 0;

    while (!feof(_file) && n_result < max_bytes - 1)
    {
        let = static_cast<char>(getc(_file));

        int count = 0;
        if (let >= '0' && let <= '9')
        {
            while (!feof(_file) && let >= '0' && let <= '9')
            {
                count = (count) * 10 + (let - '0');
                let = static_cast<char>(getc(_file));
            }
        }
        if (count < max_bytes - n_result)
        {
            if (count == 0)
            {
                buf[n_result++] = let;
            }
            else
            {
                for (int j = 0; j < count; j++)
                {
                    buf[n_result++] = let;
                }
            }
        }
        else
        {
            buf[n_result] = '\0';
            return n_result;
        }
    }
    buf[n_result] = '\0';
    return n_result;
}
// size_t decode(FILE *file, void *buffer, size_t max_bytes)
// {
//     char *ptr = (char *)buffer;
//     char count, c;
//     size_t i = 0;
//     while (!feof(file) && i < max_bytes - 1)
//     {
//         count = getc(file);
//         if (i + count > max_bytes)
//         {
//             ungetc(count, file);
//             return i;
//         }
//         char c = getc(file);
//         for (int j = 0; j < count; j++)
//             ptr[i++] = c;
//     }

//     return i;
// }

// read() rle
// {
//     char *buf = (char *)dst; // output
//     char let;

//     int output_index = 0;
//     size_t i = 0, n_result = 0;

//     while (!feof(_file) && n_result < max_bytes)
//     {
//         let = static_cast<char>(getc(_file));

//         int count = 0;
//         if (let >= '0' && let <= '9')
//         {
//             while (!feof(_file) && let >= '0' && let <= '9')
//             {
//                 count = (count) * 10 + (let - '0');
//                 let = static_cast<char>(getc(_file));
//             }
//         }
//         if (count < max_bytes - n_result)
//         {
//             if (count == 0)
//             {
//                 buf[n_result++] = let;
//             }
//             else
//             {
//                 for (int j = 0; j < count; j++)
//                 {
//                     buf[n_result++] = let;
//                 }
//             }
//         }
//         if else ()
//         else
//         {
//             buf[n_result] = '\0';
//             return n_result;
//         }
//     }
//     buf[n_result] = '\0';
//     return n_result;
// }

int main()
{
    // char *input = "       \\ \n         /\\_/\\  (\n        ( ^.^ ) _)\n          \\\"/  (\n        ( | | )\n       (__d b__)";
    char *input = "AANBBBNALOOOOOOOOOOAAAAANBBBNCMMALOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";

    // char *output1;
    // rle_encode(input, strlen(input), output1);
    // std::cout << output1;
    // std::cout << "\n----------\n";

    // input = "AAAAANBBBNCMMALOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOAAAAANBBBNCMMALOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
    // char *output2 = new char[strlen(input) * 2];
    // rle_encodeee(input, strlen(input), output2);
    // std::cout << output2;

    FILE *file = fopen("test.txt", "r");
    char *output = new char[13];
    rle_decode(file, output, 12);
    std::cout << output;
    return 0;
}