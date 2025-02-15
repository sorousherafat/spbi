#include <assert.h>
#include <cc.h>
#include <stdio.h>
#include <stdlib.h>

#include "bill.h"
#include "log.h"

extern void parse_balances(balance_map_t *balances, char *path);

int main(int argc, char **argv) {
    if (argc != 2) {
        error("did not provide one argument");
    }

    balance_map_t balances;
    init(&balances);
    parse_balances(&balances, argv[1]);

    transaction_map_t transaction_map;
    transaction_list_t transaction_list;
    init(&transaction_map);
    init(&transaction_list);
    get_transactions(&transaction_map, &transaction_list, &balances);
    cleanup(&balances);

    FILE *out = popen("column -t", "w");
    if (!out) {
        error("cannot pipe ro column\n");
    }
    for_each(&transaction_map, id, transaction_list) {
        fprintf(out, "%s:", *id);
        for_each(*transaction_list, transaction) {
            if (*id == (*transaction)->from) {
                fprintf(out, "\t%lld->%s", (*transaction)->amount, (*transaction)->to);
                continue;
            }
            if (*id == (*transaction)->to) {
                fprintf(out, "\t%lld<-%s", (*transaction)->amount, (*transaction)->from);
                continue;
            }
            assert(false);
        }
        fprintf(out, "\n");
    }
    pclose(out);

    for_each(&transaction_list, transaction) { free(*transaction); }
    cleanup(&transaction_list);
    for_each(&transaction_map, id, transaction_list) {
        cleanup(*transaction_list);
        free(*transaction_list);
        free(*id);
    }
    cleanup(&transaction_map);
}
