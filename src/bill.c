#include <assert.h>
#include <cc.h>
#include <stdbool.h>

#include "bill.h"

int balance_cmp_desc(const void *a, const void *b);
void get_sorted_balances_from_map(balance_t sorted_balances[], map(char *, amount_t) * balance_map, int count);
void sort_balances(balance_t balances[], int count);
void remove_perfect_matches(transaction_map_t *transaction_map, transaction_list_t *transaction_list, balance_t balances[], int count);
void remove_imperfect_matches(transaction_map_t *transaction_map, transaction_list_t *transaction_list, balance_t balances[], int count);

void get_transactions(transaction_map_t *transaction_map, transaction_list_t *transaction_list, balance_map_t *balances) {
    int balance_count = size(balances);
    balance_t *sorted_balances = malloc(balance_count * sizeof(balance_t));
    get_sorted_balances_from_map(sorted_balances, balances, balance_count);
    for_each(balances, id, _) {
        transaction_list_t *id_transactions = malloc(sizeof(transaction_list_t));
        init(id_transactions);
        insert(transaction_map, *id, id_transactions);
    }
    remove_perfect_matches(transaction_map, transaction_list, sorted_balances, balance_count);
    sort_balances(sorted_balances, balance_count);
    remove_imperfect_matches(transaction_map, transaction_list, sorted_balances, balance_count);
    for (int index = 0; index < balance_count; index++) {
        amount_t final_balance = sorted_balances[index].amount;
        assert(final_balance >= 0 && final_balance < balance_count);
    }
    free(sorted_balances);
}

int balance_cmp_desc(const void *a, const void *b) { return ((balance_t *)b)->amount - ((balance_t *)a)->amount; }

void get_sorted_balances_from_map(balance_t sorted_balances[], map(char *, amount_t) * balance_map, int count) {
    uint index = 0;
    for_each(balance_map, id, amount) {
        balance_t *balance = &sorted_balances[index];
        balance->id = *id;
        balance->amount = *amount;
        index++;
    }

    sort_balances(sorted_balances, count);
}

void sort_balances(balance_t balances[], int count) { qsort(balances, count, sizeof(balance_t), balance_cmp_desc); }

void remove_perfect_matches(transaction_map_t *transaction_map, transaction_list_t *transaction_list, balance_t balances[], int count) {
    int positive_index = 0;
    int negative_index = count - 1;
    balance_t *positive_balance = &balances[positive_index];
    balance_t *negative_balance = &balances[negative_index];
    while (positive_balance->amount > 0 && negative_balance->amount < 0) {
        amount_t sum = positive_balance->amount + negative_balance->amount;
        if (sum > 0) {
            positive_index++;
            break;
        }
        if (sum < 0) {
            negative_index--;
            break;
        }
        transaction_t *transaction = malloc(sizeof(transaction_t));
        transaction->from = negative_balance->id;
        transaction->to = positive_balance->id;
        transaction->amount = positive_balance->amount;
        positive_balance->amount = 0;
        negative_balance->amount = 0;
        push(transaction_list, transaction);
        transaction_list_t **from_list = get(transaction_map, negative_balance->id);
        push(*from_list, transaction);
        transaction_list_t **to_list = get(transaction_map, positive_balance->id);
        push(*to_list, transaction);
        positive_index++;
        negative_index--;
        assert(positive_index < count && negative_index > -1);
        positive_balance = &balances[positive_index];
        negative_balance = &balances[negative_index];
    }
}

void remove_imperfect_matches(transaction_map_t *transaction_map, transaction_list_t *transaction_list, balance_t balances[], int count) {
    int positive_index = 0;
    int negative_index = count - 1;
    balance_t *positive_balance = &balances[positive_index];
    balance_t *negative_balance = &balances[negative_index];
    while (positive_balance->amount > 0 && negative_balance->amount < 0) {
        transaction_t *transaction = malloc(sizeof(transaction_t));
        amount_t sum = positive_balance->amount + negative_balance->amount;
        if (sum > 0) {
            transaction->amount = -negative_balance->amount;
            positive_balance->amount -= transaction->amount;
            negative_balance->amount = 0;
            negative_index--;
        } else if (sum < 0) {
            transaction->amount = positive_balance->amount;
            negative_balance->amount += transaction->amount;
            positive_balance->amount = 0;
            positive_index++;
        } else {
            transaction->amount = positive_balance->amount;
            positive_balance->amount = 0;
            negative_balance->amount = 0;
            positive_index++;
            negative_index--;
        }
        transaction->from = negative_balance->id;
        transaction->to = positive_balance->id;
        push(transaction_list, transaction);
        transaction_list_t **from_list = get(transaction_map, negative_balance->id);
        push(*from_list, transaction);
        transaction_list_t **to_list = get(transaction_map, positive_balance->id);
        push(*to_list, transaction);
        assert(positive_index < count && negative_index > -1);
        positive_balance = &balances[positive_index];
        negative_balance = &balances[negative_index];
    }
}
