#ifndef __SPBI_BILL_H_
#define __SPBI_BILL_H_

#include <cc.h>

typedef char *identifier_t;
typedef long long amount_t;

typedef struct {
    identifier_t id;
    amount_t amount;
} balance_t;

typedef struct {
    identifier_t from;
    identifier_t to;
    amount_t amount;
} transaction_t;

typedef map(char *, amount_t) balance_map_t;
typedef vec(transaction_t *) transaction_list_t;
typedef map(char *, transaction_list_t *) transaction_map_t;

void get_transactions(transaction_map_t *transaction_map, transaction_list_t *transaction_list, balance_map_t *balances);

#endif // !__SPBI_BILL_H_
