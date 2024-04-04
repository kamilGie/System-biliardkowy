#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NUMBER_SIZE 5

typedef struct { unsigned long digits[MAX_NUMBER_SIZE]; } billiard_system_number;


billiard_system_number add_billiard_system_numbers(const billiard_system_number m, const billiard_system_number n, bool *carry_out)
{
    billiard_system_number res = {};
    *carry_out = false;

    for (int i = 0; i < MAX_NUMBER_SIZE; i++)
    {
        res.digits[i] = m.digits[i] + n.digits[i] + *carry_out;

        *carry_out = (n.digits[i] > res.digits[i] - *carry_out) ||
                     (n.digits[i] > res.digits[i]) ||
                     (m.digits[i] > res.digits[i] - *carry_out) ||
                     (m.digits[i] > res.digits[i]);
    }

    return res;
}


billiard_system_number multiply_by_2(billiard_system_number l, bool *carry_out)
{
    return add_billiard_system_numbers(l, l, &*carry_out);
}


billiard_system_number multiply_by_10(billiard_system_number l, bool *carry_out)
{
    bool tmp_carry[4];
    l = add_billiard_system_numbers(multiply_by_2(multiply_by_2(multiply_by_2(l, &tmp_carry[0]), &tmp_carry[1]), &tmp_carry[2]), multiply_by_2(l, &tmp_carry[0]), &tmp_carry[3]);
    if (tmp_carry[0] || tmp_carry[1] || tmp_carry[2] || tmp_carry[3]){
        *carry_out = true;
    }

    return l;
}


billiard_system_number string_to_billiard_system_number(char s[], bool *carry_out)
{
    billiard_system_number result = {}, unit = {};

    for (int i = 0; i < strlen(s); i++)
    {
        result = multiply_by_10(result, &*carry_out);
        if (*carry_out) { break; }

        unit.digits[0] = s[i] - '0';
        result = add_billiard_system_numbers(unit, result, &*carry_out);
        if (*carry_out ) { break; }
    }

    return result;
}


void print_billiard_system_number(billiard_system_number l)
{
    int significant_indices = MAX_NUMBER_SIZE - 1;
    while (significant_indices > 0 && l.digits[significant_indices] == 0)
    {
        significant_indices--;
    }

    while(significant_indices >= 0)
    {
        printf("%lu ", l.digits[significant_indices]);
        significant_indices--;
    }

    printf("\n");
}


bool are_different(unsigned long number_1[], unsigned long number_2[])
{
    for (int i = 0; i < MAX_NUMBER_SIZE; i++)
    {
        if (number_1[i] != number_2[i]) { return true; }
    }

    return false;
}


bool number_greater_or_equal(unsigned long greater[], unsigned long smaller[])
{
    for (int i = MAX_NUMBER_SIZE - 1; i >= 0; i--)
    {
        if (greater[i] > smaller[i]) {
            return true;
        }else if(greater[i] < smaller[i]){
            return false;
        } 
    }
    
    return true;
}


billiard_system_number multiply_billiard_system_numbers(billiard_system_number m, billiard_system_number n, bool *carry_out)
{
    billiard_system_number counter = {}, result = {};
    bool temp_carry_index;
    *carry_out = false;

    while (are_different(counter.digits, n.digits) && !*carry_out)
    {
        billiard_system_number temp_m = m;
        billiard_system_number index = {1};

        billiard_system_number next_index = add_billiard_system_numbers(counter, multiply_by_2(index, &temp_carry_index), &*carry_out);
            while (number_greater_or_equal(n.digits, next_index.digits) && !*carry_out && !temp_carry_index)
            {
                index = multiply_by_2(index, &temp_carry_index);
                temp_m = multiply_by_2(temp_m, &*carry_out);
                if (*carry_out) { return result; }

                next_index = add_billiard_system_numbers(counter, multiply_by_2(index, &temp_carry_index), &*carry_out);
            }

        counter = add_billiard_system_numbers(counter, index, &*carry_out);
        result = add_billiard_system_numbers(temp_m, result, &*carry_out);
    };

    return result;
}



int main()
{
    billiard_system_number m, n;
    char sm[MAX_NUMBER_SIZE * 20 + 1], sn[MAX_NUMBER_SIZE * 20 + 1];
    scanf("%s %s", sm, sn);

    bool carry = false;
    m = string_to_billiard_system_number(sm, &carry);
    if (carry)
    {
        printf("m number too large!\n");
        return 0;
    }

    n = string_to_billiard_system_number(sn, &carry);
    if (carry)
    {
        printf("n number too large!\n");
        return 0;
    }

    billiard_system_number sum = {};
    sum = add_billiard_system_numbers(m, n, &carry);
    if (carry){
        printf("Carry in sum!\n");
    }
    print_billiard_system_number(sum);


    billiard_system_number product = {};
    product = multiply_billiard_system_numbers(m, n, &carry);
    if (carry){
        printf("Carry in product!\n");
    } else{
        print_billiard_system_number(product);
    }

    return 0;
}