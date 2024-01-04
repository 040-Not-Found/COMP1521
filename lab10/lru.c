// Simulate LRU replacement of page frames

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// represent an entry in a simple inverted page table

typedef struct ipt_entry {
    int virtual_page;        // == -1 if physical page free
    int last_access_time;
} ipt_entry_t;


void lru(int n_physical_pages, int n_virtual_pages);
void access_page(int virtual_page, int access_time, int n_physical_pages, struct ipt_entry *ipt);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n-physical-pages> <n-virtual-pages>\n", argv[0]);
        return 1;
    }
    lru(atoi(argv[1]), atoi(argv[2]));
    return 0;
}


void lru(int n_physical_pages, int n_virtual_pages) {
    printf("Simulating %d pages of physical memory, %d pages of virtual memory\n",
          n_physical_pages, n_virtual_pages);
    struct ipt_entry *ipt = malloc(n_physical_pages * sizeof *ipt);
    assert(ipt);

    for (int i = 0; i < n_physical_pages; i++) {
        ipt[i].virtual_page = -1;
        ipt[i].last_access_time = -1;
    }

    int virtual_page;
    for (int access_time = 0; scanf("%d", &virtual_page) == 1; access_time++) {
        assert(virtual_page >= 0 && virtual_page < n_virtual_pages);
        access_page(virtual_page, access_time, n_physical_pages, ipt);
    }
}


// if virtual_page is not in ipt, the first free page is used
// if there is no free page, the least-recently-used page is evicted
//
// a single line of output describing the page access is always printed
// the last_access_time in ipt is always updated

void access_page(int virtual_page, int access_time, int n_physical_pages, struct ipt_entry *ipt) {
    printf("Time %d: virtual page %d ", access_time, virtual_page);

    int in_ipt = 0;
    int empty = 0;
    int i = 0;
    int min_access = ipt[0].last_access_time;
    int page = 0;
    while (i < n_physical_pages) {
        if (ipt[i].virtual_page == virtual_page) {
            in_ipt = 1;
            break;
        } else if (ipt[i].last_access_time == -1) {
            empty = 1;
            break;
        }
        if (ipt[i].last_access_time < min_access) {
            min_access = ipt[i].last_access_time;
            page = i;
        }
        i++;
    }

    if (in_ipt == 1) {
        ipt[i].last_access_time = access_time;
        printf("-> physical page %d\n", i);
    } else if (empty == 1) {
        ipt[i].virtual_page = virtual_page;
        ipt[i].last_access_time = access_time;
        printf("loaded to physical page %d\n", i);
    } else {        
        printf(" - virtual page %d evicted - loaded to physical page %d\n", 
     ipt[page].virtual_page, page);
        ipt[page].last_access_time = access_time;
        ipt[page].virtual_page = virtual_page;
    }
}
